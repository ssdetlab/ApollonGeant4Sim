#ifndef Run_h
#define Run_h

#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "TFile.h"
#include "TTree.h"
#include "TVector2.h"
#include "TVector3.h"

class Run : public G4Run {
 public:
  Run(const std::string& filePath, const std::string& treeName,
      double pixelThreshold);
  ~Run() override;

  void RecordEvent(const G4Event*) override;
  void Merge(const G4Run*) override;

 private:
  TFile* m_file = nullptr;
  TTree* m_tree = nullptr;

  int m_geoId;
  int m_pixIdX;
  int m_pixIdY;

  int m_isSignal;

  TVector2 m_geoCenterLocal;
  TVector3 m_geoCenterGlobal;

  double m_totEDep;

  std::vector<int> m_parentTrackId;
  std::vector<int> m_trackId;
  int m_eventId;
  int m_runId;

  std::vector<TVector3> m_hitPosGlobal;
  std::vector<TVector2> m_hitPosLocal;

  std::vector<TVector3> m_hitMomDir;
  std::vector<double> m_hitE;
  std::vector<double> m_hitP;

  std::vector<TVector3> m_ipMomDir;
  std::vector<double> m_ipE;
  std::vector<double> m_ipP;
  std::vector<TVector3> m_vertex;

  std::vector<double> m_eDep;
  std::vector<int> m_pdgId;

  double m_pairProductionE = 3.62 * eV;
  double m_pixelThreshold;
};

#endif
