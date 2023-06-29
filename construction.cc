#include "construction.hh"

DetectorConstruction::DetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
    // Defining the primary volume
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

    auto *solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m);

    auto *logicalWorld = new G4LogicalVolume(solidWorld, worldMat, "logicalWorld");

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

    // todo build voxels

    // Building the sensitive detector (only one because I'm lazy)
    auto *solidDetector = new G4Box("solidDetector", 0.5*m, 0.5*m, 0.01*m);
    logicalDetector = new G4LogicalVolume(solidDetector, worldMat, "logicalDetector");

    G4VPhysicalVolume *physicalDetector = new G4PVPlacement(
            nullptr,
            G4ThreeVector(0*m, 0*m, 0.49*m),
            logicalDetector,
            "physicalDetector",
            logicalWorld,
            false,
            0,
            true
    );

    constructSensitiveDetector();

    return physicalWorld;
}

void DetectorConstruction::constructSensitiveDetector() {
    auto* sensitiveDetector = new SensitiveDetector("sensitiveDetector");
    logicalDetector->SetSensitiveDetector(sensitiveDetector);
}