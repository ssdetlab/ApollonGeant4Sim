#ifndef ReadoutGeneratorAction_h
#define ReadoutGeneratorAction_h

#include <fstream>
#include <random>

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class ReadoutPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
 public:
  ReadoutPrimaryGeneratorAction(const std::string& path);
  ~ReadoutPrimaryGeneratorAction() override = default;

  void GeneratePrimaries(G4Event* event) override;

 private:
  std::ifstream m_file;

  std::mt19937 m_rng;

  G4ParticleDefinition* m_particle = nullptr;
  G4ParticleGun* m_particleGun = nullptr;
};

#endif
