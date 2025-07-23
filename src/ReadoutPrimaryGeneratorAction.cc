#include "ReadoutPrimaryGeneratorAction.hh"

#include <cmath>

#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ThreeVector.hh"

ReadoutPrimaryGeneratorAction::ReadoutPrimaryGeneratorAction(
    const std::string& path)
    : G4VUserPrimaryGeneratorAction() {
  m_particleGun = new G4ParticleGun(1);

  m_particle = G4ParticleTable::GetParticleTable()->FindParticle(11);

  m_particleGun->SetParticleDefinition(m_particle);

  m_file.open(path);

  m_rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

void ReadoutPrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
  m_particleGun->SetParticlePosition(G4ThreeVector(0, 0, 0));

  std::string s;
  char del = ',';
  if (!std::getline(m_file, s)) {
    return;
  };

  std::stringstream stream(s);
  std::string res;

  std::getline(stream, res, del);
  double px = std::stod(res);

  std::getline(stream, res, del);
  double py = std::stod(res);

  std::getline(stream, res, del);
  double pz = std::stod(res);

  double gamma = std::sqrt(1 + px * px + py * py + pz * pz);
  double E = gamma * 0.511 * MeV;

  double P = std::sqrt(px * px + py * py + pz * pz);
  G4ThreeVector dir(px, py, pz);
  dir /= P;

  m_particleGun->SetParticleEnergy(E);
  m_particleGun->SetParticleMomentumDirection(dir);
  m_particleGun->GeneratePrimaryVertex(event);
}
