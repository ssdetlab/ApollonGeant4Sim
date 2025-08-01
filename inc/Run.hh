#ifndef Run_h
#define Run_h

#include "G4Run.hh"
#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"

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

  std::vector<int> m_geoId;

  std::vector<int> m_trackId;
  std::vector<int> m_pdgId;

  std::vector<TVector3> m_pos;
  std::vector<TVector3> m_vertex;

  std::vector<TVector3> m_momDir;
  std::vector<TVector3> m_momDirIP;

  std::vector<double> m_eDep;
  std::vector<double> m_eTot;
  std::vector<double> m_eIP;
};

#endif
