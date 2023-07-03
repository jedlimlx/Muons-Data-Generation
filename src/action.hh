#include "iostream"

#include "G4VUserActionInitialization.hh"

#include "generator.hh"

class ActionInitialization : public G4VUserActionInitialization {
public:
    ActionInitialization();
    ~ActionInitialization();

    const char* file;

    virtual void Build() const;
};