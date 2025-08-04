#ifndef Run_h
#define Run_h

#include "G4Run.hh"
#include "TFile.h"
#include "TTree.h"
#include "TVector2.h"
#include "TVector3.h"

class Run : public G4Run {
 public:
  Run(const std::string& filePath, const std::string& treeName);
  ~Run() override;

  void RecordEvent(const G4Event*) override;
  void Merge(const G4Run*) override;

 private:
  TFile* m_file = nullptr;
  TTree* m_tree = nullptr;

  std::vector<int> m_geoId;
  std::vector<int> m_isSignal;

  std::vector<int> m_parentTrackId;
  std::vector<int> m_trackId;
  std::vector<int> m_runId;

  std::vector<TVector3> m_hitPosGlobal;
  std::vector<TVector2> m_hitPosLocal;

  std::vector<TVector3> m_hitMomDir;
  std::vector<double> m_hitE;

  std::vector<TVector3> m_ipMomDir;
  std::vector<double> m_ipE;
  std::vector<TVector3> m_vertex;

  std::vector<double> m_eDep;
  std::vector<int> m_pdgId;
};

#endif
