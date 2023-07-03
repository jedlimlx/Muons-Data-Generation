#include "iostream"

#include "G4Track.hh"
#include "G4UserSteppingAction.hh"

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction();
    ~SteppingAction();

    std::ofstream file;

    virtual void UserSteppingAction(const G4Step* step);
};