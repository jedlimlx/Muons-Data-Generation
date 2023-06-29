#include "string"
#include "iostream"

#include "construction.hh"

using namespace std;

DetectorConstruction::DetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
    // Defining the primary volume
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *worldMaterial = nist->FindOrBuildMaterial("G4_AIR");

    G4double padding = 0.2*m;
    G4double targetSize = 1*m;
    G4double detectorDistance = 0.4*m;

    auto *solidWorld = new G4Box(
            "solidWorld",
            targetSize/2+padding,
            targetSize/2+padding,
            targetSize/2+detectorDistance
    );

    auto *logicalWorld = new G4LogicalVolume(solidWorld, worldMaterial, "logicalWorld");

    auto *physicalWorld = new G4PVPlacement(
            nullptr,
            G4ThreeVector(0, 0, 0),
            logicalWorld,
            "physicalWorld",
            nullptr,
            false,
            0,
            true
    );

    // Building the voxels
    int numVoxels = 128;
    G4double voxelSize = targetSize/numVoxels;
    G4Material *voxelMaterial = nist->FindOrBuildMaterial("G4_U");

    auto *solidVoxel = new G4Box("voxel", voxelSize/2, voxelSize/2, voxelSize/2);
    auto *logicalVoxel = new G4LogicalVolume(solidVoxel, voxelMaterial, "logicalDetector");

    ifstream file(voxelFile);  // opening the file

    int count = 0;
    string text;
    while (getline(file, text)) {
        if (text[0] == '1') {
            int x = count % numVoxels - numVoxels/2;
            int y = (count / numVoxels) % numVoxels - numVoxels/2;
            int z = count / (numVoxels*numVoxels) - numVoxels/2;

            G4VPhysicalVolume *physicalVoxel = new G4PVPlacement(
                    nullptr,
                    G4ThreeVector(x*voxelSize, y*voxelSize, z*voxelSize),
                    logicalVoxel,
                    "physicalVoxel",
                    logicalWorld,
                    false,
                    count,
                    false
            );
        }

        count++;
    }

    // Building the sensitive detector (only one because I'm lazy)
    auto *solidDetector = new G4Box("solidDetector", targetSize/2+padding, targetSize/2+padding, 0.01*m);
    auto *logicalDetector = new G4LogicalVolume(solidDetector, worldMaterial, "logicalDetector");

    G4VPhysicalVolume *physicalDetector = new G4PVPlacement(
            nullptr,
            G4ThreeVector(0*m, 0*m, -(targetSize/2+detectorDistance-0.01*m)),
            logicalDetector,
            "physicalDetector",
            logicalWorld,
            false,
            0,
            true
    );

    // Construct the sensitive detector
    auto* sensitiveDetector = new SensitiveDetector("sensitiveDetector");
    sensitiveDetector->file = ofstream(outputFile);
    logicalDetector->SetSensitiveDetector(sensitiveDetector);

    return physicalWorld;
}
