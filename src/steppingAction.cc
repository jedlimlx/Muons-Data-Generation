#include "steppingAction.hh"

SteppingAction::SteppingAction() {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step *step) {
    G4Track *track = step->GetTrack();

    // only want muons
    if (track->GetParticleDefinition()->GetParticleName() != "mu-") return;
    if ((int)track->GetVertexPosition().z() != 900) return;

    if (track->GetVertexPosition().z() > 890) {
        double t = -1800 / track->GetVertexMomentumDirection().z();

        G4ThreeVector vec = t * track->GetVertexMomentumDirection() + track->GetVertexPosition();
        if (vec.x() > 1000 || vec.x() < -1000 || vec.y() > 1000 || vec.y() < -1000) {
            track->SetTrackStatus(fStopAndKill);
        }
    }

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

    file << step->GetPreStepPoint()->GetTouchable()->GetCopyNumber() << std::endl;
}
