#include "iostream"

#include "G4VSensitiveDetector.hh"

class SensitiveDetector : public G4VSensitiveDetector {
public:
    SensitiveDetector(G4String);
    ~SensitiveDetector();

    std::ofstream file;

private:
    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
};