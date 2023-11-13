#include "string"

#include "construction.hh"
#include "CONSTANTS.hh"

using namespace std;

DetectorConstruction::DetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
    // Defining the primary volume
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *worldMaterial = nist->FindOrBuildMaterial("G4_AIR");

    G4double padding = PADDING*m;
    G4double targetSize = TARGET_SIZE*m;
    G4double detectorDistance = DETECTOR_DISTANCE*m;

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
    int numVoxels = RESOLUTION;
    G4double voxelSize = targetSize/numVoxels;

    G4String materials[] = {
            "G4_BENZENE",
            "G4_METHANOL",
            "G4_WATER",
            "G4_Mg",
            "G4_CONCRETE",
            "G4_GYPSUM",
            "G4_Ca",
            "G4_S",
            "G4_Si",
            "G4_Al",
            "G4_Cs",
            "G4_Mn",
            "G4_Fe",
            "G4_I",
            "G4_Ni",
            "G4_Mo",
            "G4_Ag",
            "G4_Po",
            "G4_Pb",
            "G4_Au",
            "G4_U"
    };

    auto *solidVoxel = new G4Box("voxel", voxelSize/2, voxelSize/2, voxelSize/2);

    G4LogicalVolume *logicalVoxels[21];
    for (int i = 0; i < 21; i++) {
        logicalVoxels[i] = new G4LogicalVolume(
                solidVoxel,
                nist->FindOrBuildMaterial(materials[i]),
                "logicalDetector" + to_string(i)
        );
    }

    ifstream file(voxelFile);  // opening the file

    int count = 0;
    string text;
    while (getline(file, text)) {
        if (text[0] != '0') {
            int z = count % numVoxels - numVoxels/2;
            int y = (count / numVoxels) % numVoxels - numVoxels/2;
            int x = count / (numVoxels*numVoxels) - numVoxels/2;

            G4VPhysicalVolume *physicalVoxel = new G4PVPlacement(
                    nullptr,
                    G4ThreeVector(x*voxelSize, y*voxelSize, z*voxelSize),
                    logicalVoxels[stoi(text) - 1],
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
