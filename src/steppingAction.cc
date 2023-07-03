#include "steppingAction.hh"

SteppingAction::SteppingAction() {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step *step) {
    G4Track *track = step->GetTrack();

    file << "\"" << (int)track->GetVertexPosition().x()
                 << (int)track->GetVertexPosition().y()
                 << (int)track->GetVertexPosition().z() << "\"" << ",";

    // position
    file << track->GetPosition().x() << "," <<
            track->GetPosition().y() << "," <<
            track->GetPosition().z() << ",";

    // momentum
    file << track->GetMomentum().x() << "," <<
            track->GetMomentum().y() << "," <<
            track->GetMomentum().z() << std::endl;
}
