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

    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());

    runManager->Initialize();

    auto *ui = new G4UIExecutive(argc, argv);

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if (argc > 0 && *argv[0] == '0') {
        UImanager->ApplyCommand("/vis.mac/execute vis.mac");
    } else if (*argv[0] == '1') {
        UImanager->ApplyCommand("/vis.mac/execute run.mac");
    }

    ui->SessionStart();
}
