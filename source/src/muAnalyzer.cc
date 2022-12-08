
#include "muAnalyzer.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Types.hh"

#include <vector>
#include "TTree.h"
#include "TFile.h"

muAnalyzer::muAnalyzer() {
}

muAnalyzer::~muAnalyzer() {
}

muAnalyzer::muAnalyzer(const muAnalyzer &obj) {
    tree = obj.tree;
}

void muAnalyzer::SetInit(G4bool isRootIn, TString filenameIn) {
    isRoot = isRootIn;
    filename = filenameIn;
}

void muAnalyzer::Init() {
    //if(isRoot == true){
    tree = new TTree("tree", "mc output");
    tree->Branch("event", &event, "event/I");  // [yy] add
    tree->Branch("nHit", &nHit, "nHit/I");
    tree->Branch("x", &x, "x/D");
    tree->Branch("y", &y, "y/D");
    tree->Branch("z", &z, "z/D");
    tree->Branch("px", &px, "px/D");
    tree->Branch("py", &py, "py/D");
    tree->Branch("pz", &pz, "pz/D");
    tree->Branch("time", &time, "time/D");
    tree->Branch("eIn", &eIn, "eIn/D");
    tree->Branch("eDep", &eDep, "eDep/D");
    tree->Branch("TrackID", &trackID, "trackID/I");
    tree->Branch("copyNo", &copyNo, "copyNo/I");
    tree->Branch("particle", &particleID, "particle/I");
    //} else {
    // ascii file
    //outFile.open(filename, std::ios::out);
    outFile.open(filename + ".txt", std::ios::out); // [yy]
    outFile.setf(std::ios::scientific, std::ios::floatfield);
    outFile << std::setprecision(8);
    //}
}

void muAnalyzer::Fill(int buf0,                      // nHit [yy] modify
                      std::vector <G4int> buf1,      // event [yy] add
                      std::vector <G4double> buf2,   // x
                      std::vector <G4double> buf3,   // y
                      std::vector <G4double> buf4,   // z
                      std::vector <G4double> buf5,   // time
                      std::vector <G4double> buf6,   // eIn
                      std::vector <G4double> buf7,   // eDep
                      std::vector <G4double> buf8,   // time
                      std::vector <G4double> buf9,   // eIn
                      std::vector <G4double> buf10,  // eDep
                      std::vector <G4int> buf11,     // TrackID
                      std::vector <G4int> buf12,     // copyNo
                      std::vector <G4int> buf13      // particleID
) {
    /*
     * Fill(int nHit,                     //nHit [yy]
                      std::vector <G4int> eventbuf,      //event [yy]
                      std::vector <G4double> xbuf,   //x
                      std::vector <G4double> ybuf,   //y
                      std::vector <G4double> zbuf,   //z
                      std::vector <G4double> pxbuf,   //x
                      std::vector <G4double> pybuf,   //y
                      std::vector <G4double> pzbuf,   //z
                      std::vector <G4double> timebuf,   //time
                      std::vector <G4double> eInbuf,   //eIn
                      std::vector <G4double> eDepbuf,   //eDep
                      std::vector <G4int> trackIDbuf,   //TrackID
                      std::vector <G4int> copyNobuf,   //copyNo
                      std::vector <G4int> particleIDbuf   //particleID
     */

    // Assign inputs to class properties
    nHit = buf0;
    eventbuf = buf1;
    xbuf = buf2;
    ybuf = buf3;
    zbuf = buf4;
    pxbuf = buf5;
    pybuf = buf6;
    pzbuf = buf7;
    timebuf = buf8;
    eInbuf = buf9;
    eDepbuf = buf10;
    trackIDbuf = buf11;
    copyNobuf = buf12;
    particleIDbuf = buf13;

    //if (isRoot == true){
    for (int i = 0; i < nHit; ++i) {
        event = eventbuf.at(i);
        x = xbuf.at(i);
        y = ybuf.at(i);
        z = zbuf.at(i);
        px = pxbuf.at(i);
        py = pybuf.at(i);
        pz = pzbuf.at(i);
        time = timebuf.at(i);
        eIn = eInbuf.at(i);
        eDep = eDepbuf.at(i);
        trackID = trackIDbuf.at(i);
        copyNo = copyNobuf.at(i);
        particleID = particleIDbuf.at(i);
        tree->Fill();
    }

    //} else {
    //outFile << nHit << std::endl; //[yy] comment
    for (int i = 0; i < nHit; ++i) {
        outFile << eventbuf.at(i) // [yy]
                << " " << nHit  // [yy]
                << " " << xbuf.at(i)
                << " " << ybuf.at(i)
                << " " << zbuf.at(i)
                << " " << pxbuf.at(i)
                << " " << pybuf.at(i)
                << " " << pzbuf.at(i)
                << " " << timebuf.at(i)
                << " " << eInbuf.at(i)
                << " " << eDepbuf.at(i)
                << " " << trackIDbuf.at(i)
                << " " << copyNobuf.at(i)
                << " " << particleIDbuf.at(i)
                << std::endl;
    }
    //}

    eventbuf.clear();
    xbuf.clear();
    ybuf.clear();
    zbuf.clear();
    pxbuf.clear();
    pybuf.clear();
    pzbuf.clear();
    timebuf.clear();
    eInbuf.clear();
    eDepbuf.clear();
    trackIDbuf.clear();
    copyNobuf.clear();
    particleIDbuf.clear();
}

void muAnalyzer::Terminate() {
    if (isRoot == true) {
        //auto fout = new TFile(filename,"recreate");
        auto fout = new TFile("out.root", "recreate"); // [yy]
        tree->Write();
        fout->Close();
        delete tree;
    } else {
        outFile.close();
    }

}

void muAnalyzer::SetFileName(TString name) {
    filename = name;
}
