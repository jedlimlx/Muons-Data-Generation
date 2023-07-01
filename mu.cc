#include "iostream"
#include "bits/stdc++.h"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "src/construction.hh"
#include "src/physics.hh"
#include "src/action.hh"

int main(int argc, char** argv) {
    // because the simulation is somehow IO bound
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    if (argc < 4) {
        std::cout << "The input format is ./mu <mode> <voxels file> <output file>" << std::endl;
        std::cout << "0 for interactive mode\n1 for batch mode\n";
    }

    auto *runManager = new G4RunManager();

    auto *detector = new DetectorConstruction();
    detector->voxelFile = argv[2];
    detector->outputFile = argv[3];

    runManager->SetUserInitialization(detector);
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());

    runManager->Initialize();

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if (*argv[1] == '0') {
        auto *ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
    } else if (*argv[1] == '1') {
        UImanager->ApplyCommand("/control/execute run.mac");
    }
}
