#ifndef GeneratorAction_h
#define GeneratorAction_h

#include <random>

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
 public:
  PrimaryGeneratorAction(int nParticles, double particleEnergyMin,
                         double particleEnergyMax, double sigmaTheta,
                         double sigmaPhi);
  ~PrimaryGeneratorAction() override = default;

  void GeneratePrimaries(G4Event* event) override;

 private:
  int m_nParticles;
  double m_particleEnergyMin;
  double m_particleEnergyMax;
  double m_sigmaTheta;
  double m_sigmaPhi;

  std::mt19937 m_rng;

  G4ParticleDefinition* m_particle = nullptr;
  G4ParticleGun* m_particleGun = nullptr;
};

#endif
