#include "detector.hh"
#include "CONSTANTS.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name) {}

SensitiveDetector::~SensitiveDetector() {}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history) {
    G4Track *track = step->GetTrack();

    // only want muons
    if (track->GetParticleDefinition()->GetParticleName() != "mu-") return true;
    if ((int)track->GetVertexPosition().z() != (int)((DETECTOR_DISTANCE + TARGET_SIZE / 2)*1000)) return true;

    // position
    file << track->GetPosition().x() << "," <<
            track->GetPosition().y() << "," <<
            track->GetPosition().z() << ",";

    // momentum
    file << track->GetMomentum().x() << "," <<
            track->GetMomentum().y() << "," <<
            track->GetMomentum().z() << ",";

    // starting position
    file << track->GetVertexPosition().x() << "," <<
            track->GetVertexPosition().y() << "," <<
            track->GetVertexPosition().z() << ",";

    // starting momentum direction
    file << track->GetVertexMomentumDirection().x() << "," <<
            track->GetVertexMomentumDirection().y() << "," <<
            track->GetVertexMomentumDirection().z() << ",";

    // starting energy
    file << track->GetVertexKineticEnergy() << std::endl;

    track->SetTrackStatus(fStopAndKill);

    return true;
}
