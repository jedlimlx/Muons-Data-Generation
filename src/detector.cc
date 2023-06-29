#include "detector.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name) {}

SensitiveDetector::~SensitiveDetector() {}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history) {
    std::cout << "muon detected" << std::endl;

    G4Track *track = step->GetTrack();

    std::cout << "muon detected at " << track->GetPosition() << " with " << track->GetMomentumDirection() << std::endl;
    std::cout << "muon started from " << track->GetVertexPosition() << " with " << track->GetVertexMomentumDirection() << std::endl;

    return true;
}
