#include "iostream"

#include "generator.hh"


PrimaryGenerator::PrimaryGenerator() {
    fParticleGun = new G4GeneralParticleSource();
}

PrimaryGenerator::~PrimaryGenerator() {
    delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event *event) {
    fParticleGun->GeneratePrimaryVertex(event);
}
