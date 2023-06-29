#include "iostream"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "src/construction.hh"
#include "src/physics.hh"
#include "src/action.hh"

int main(int argc, char** argv) {
    auto *runManager = new G4RunManager();

    auto *detector = new DetectorConstruction();
    if (argc > 2) detector->voxelFile = argv[2];
    else detector->voxelFile = "voxels.txt";

    runManager->SetUserInitialization(detector);

    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());

    runManager->Initialize();

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if (argc < 1 || *argv[1] == '0') {
        auto *ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
    } else if (*argv[1] == '1') {
        UImanager->ApplyCommand("/control/execute run.mac");
    }
}
