#include "PrimaryGeneratorAction.hh"

#include <cmath>
#include <random>

#include "G4ParticleTable.hh"
#include "G4ThreeVector.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(int nParticles,
                                               double particleEnergyMin,
                                               double particleEnergyMax,
                                               double sigmaTheta,
                                               double sigmaPhi)
    : m_nParticles(nParticles),
      m_particleEnergyMin(particleEnergyMin),
      m_particleEnergyMax(particleEnergyMax),
      m_sigmaTheta(sigmaTheta),
      m_sigmaPhi(sigmaPhi),
      G4VUserPrimaryGeneratorAction() {
  m_particleGun = new G4ParticleGun(m_nParticles);

  m_particle = G4ParticleTable::GetParticleTable()->FindParticle(11);

  m_particleGun->SetParticleDefinition(m_particle);

  m_rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
  auto normal = std::normal_distribution<>(0, 1);
  auto uniform = std::uniform_real_distribution<>(0, 1);

  m_particleGun->SetParticlePosition(G4ThreeVector());

  for (std::size_t i = 0; i < m_particleGun->GetNumberOfParticles(); i++) {
    double E = m_particleEnergyMin +
               (m_particleEnergyMax - m_particleEnergyMin) * uniform(m_rng);

    // double phi = 0 + 0 * normal(m_rng);
    // double theta = 0 + 0 * normal(m_rng);
    double phi = 0 + m_sigmaPhi * normal(m_rng);
    double theta = 0 + m_sigmaTheta * normal(m_rng);

    G4ThreeVector dir(std::sin(theta) * std::cos(phi),
                      std::sin(theta) * std::sin(phi), std::cos(theta));

    m_particleGun->SetParticleEnergy(E);
    m_particleGun->SetParticleMomentumDirection(dir);
    m_particleGun->GeneratePrimaryVertex(event);
  }
}
