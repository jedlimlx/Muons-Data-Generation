#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "QGSP_BERT.hh"
#include "G4PhysListFactory.hh"

class PhysicsList : public G4VModularPhysicsList {
public:
    PhysicsList();
    ~PhysicsList();
};