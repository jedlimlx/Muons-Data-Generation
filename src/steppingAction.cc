#include "steppingAction.hh"
#include "CONSTANTS.hh"

int detector_distance = (int) ((DETECTOR_DISTANCE + TARGET_SIZE / 2)*1000);
int detector_size = (int) ((TARGET_SIZE / 2 + PADDING)*1000);

SteppingAction::SteppingAction() {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step *step) {
    G4Track *track = step->GetTrack();

    // only want muons
    if (track->GetParticleDefinition()->GetParticleName() != "mu-") return;
    if (not TRAJECTORY && (int)track->GetVertexPosition().z() != detector_distance) return;

    if (track->GetVertexPosition().z() > detector_distance-10) {
        double t = -2*detector_distance / track->GetVertexMomentumDirection().z();

        G4ThreeVector vec = t * track->GetVertexMomentumDirection() + track->GetVertexPosition();
        if (vec.x() > detector_size || vec.x() < -detector_size || vec.y() > detector_size || vec.y() < -detector_size) {
            track->SetTrackStatus(fStopAndKill);
        }
    }

    if (TRAJECTORY) {
        file << "\"" << (int)track->GetVertexPosition().x()
             << (int)track->GetVertexPosition().y()
             << (int)track->GetVertexPosition().z() << "\"" << ",";

        file << track->GetVertexPosition().x() << "," <<
             track->GetVertexPosition().y() << "," <<
             track->GetVertexPosition().z() << ",";

        file << track->GetVertexMomentumDirection().x() << "," <<
             track->GetVertexMomentumDirection().y() << "," <<
             track->GetVertexMomentumDirection().z() << ",";

        // position
        file << track->GetPosition().x() << "," <<
             track->GetPosition().y() << "," <<
             track->GetPosition().z() << ",";

        // momentum direction
        file << track->GetMomentumDirection().x() << "," <<
             track->GetMomentumDirection().y() << "," <<
             track->GetMomentumDirection().z() << ",";

        // energy of muon
        file << track->GetKineticEnergy() << ",";

        file << step->GetPreStepPoint()->GetTouchable()->GetCopyNumber() << std::endl;
    }
}
