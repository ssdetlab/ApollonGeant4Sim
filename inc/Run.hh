#ifndef Run_h
#define Run_h

#include "G4Run.hh"
#include "G4ThreeVector.hh"
#include "TFile.h"
#include "TTree.h"

class Run : public G4Run {
 public:
  Run(const std::string& filePath, const std::string& treeName);
  ~Run() override;

  void RecordEvent(const G4Event*) override;
  void Merge(const G4Run*) override;

 public:
 private:
  TFile* m_file = nullptr;
  TTree* m_tree = nullptr;

  std::vector<G4int> m_geoId;

  std::vector<G4int> m_trackId;
  std::vector<G4int> m_pdgId;

  std::vector<G4ThreeVector> m_pos;
  std::vector<G4ThreeVector> m_vertex;

  std::vector<G4ThreeVector> m_momDir;
  std::vector<G4ThreeVector> m_momDirIP;

  std::vector<G4double> m_eDep;
  std::vector<G4double> m_eTot;
  std::vector<G4double> m_eIP;
};

#endif
