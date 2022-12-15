
#ifndef muTrackAction_h
#define muTrackAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class muRunAction;


class muTrackAction : public G4UserTrackingAction {
public:
    muTrackAction(muRunAction *);

    ~muTrackAction();

public:
    void PreUserTrackingAction(const G4Track *);

    void PostUserTrackingAction(const G4Track *);


private:
    muRunAction *runAction;

};


#endif


