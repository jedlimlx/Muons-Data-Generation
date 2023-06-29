#include "G4VUserActionInitialization.hh"

#include "generator.hh"

class ActionInitialization : public G4VUserActionInitialization {
public:
    ActionInitialization();
    ~ActionInitialization();

    virtual void Build() const;
    virtual void BuildForMaster() const;
};