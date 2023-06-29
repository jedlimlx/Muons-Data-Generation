#include "iostream"

#include "generator.hh"


PrimaryGenerator::PrimaryGenerator() {
    fParticleGun = new G4GeneralParticleSource();
    //new G4ParticleGun(1);
}

PrimaryGenerator::~PrimaryGenerator() {
    delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event *event) {
    /*
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "mu-";
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName);

    G4ThreeVector pos(0, 0, 0);
    G4ThreeVector mon(0, 0, 1);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mon);
    fParticleGun->SetParticleMomentum(100*GeV);
    fParticleGun->SetParticleDefinition(particle);
    */

    fParticleGun->GeneratePrimaryVertex(event);
}
