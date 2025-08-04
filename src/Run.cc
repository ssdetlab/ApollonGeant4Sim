#include "Run.hh"

#include <cstddef>

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "SamplingHit.hh"

Run::Run(const std::string& filePath, const std::string& treeName) {
  m_file = new TFile(filePath.c_str(), "RECREATE");
  m_tree = new TTree(treeName.c_str(), treeName.c_str());

  int bufSize = 32000;
  int splitLvl = 0;

  m_tree->Branch("geoId", &m_geoId, bufSize, splitLvl);
  m_tree->Branch("isSignal", &m_isSignal, bufSize, splitLvl);

  m_tree->Branch("parentTrackId", &m_parentTrackId, bufSize, splitLvl);
  m_tree->Branch("trackId", &m_trackId, bufSize, splitLvl);
  m_tree->Branch("runId", &m_runId, bufSize, splitLvl);

  m_tree->Branch("hitPosGlobal", &m_hitPosGlobal, bufSize, splitLvl);
  m_tree->Branch("hitPosLocal", &m_hitPosLocal, bufSize, splitLvl);

  m_tree->Branch("hitMomDir", &m_hitMomDir, bufSize, splitLvl);
  m_tree->Branch("hitE", &m_hitE, bufSize, splitLvl);

  m_tree->Branch("ipMomDir", &m_ipMomDir, bufSize, splitLvl);
  m_tree->Branch("ipE", &m_ipE, bufSize, splitLvl);
  m_tree->Branch("vertex", &m_vertex, bufSize, splitLvl);

  m_tree->Branch("eDep", &m_eDep, bufSize, splitLvl);
  m_tree->Branch("pdgId", &m_pdgId, bufSize, splitLvl);
}

Run::~Run() {
  m_tree->Write();
  m_file->Close();
}

void Run::RecordEvent(const G4Event* event) {
  if (event->GetEventID() % 100000 == 0) {
    std::cout << event->GetEventID() << "\n";
  }
  m_geoId.clear();
  m_isSignal.clear();

  m_parentTrackId.clear();
  m_trackId.clear();
  m_runId.clear();

  m_hitPosGlobal.clear();
  m_hitPosLocal.clear();

  m_hitMomDir.clear();
  m_hitE.clear();

  m_ipMomDir.clear();
  m_ipE.clear();
  m_vertex.clear();

  m_eDep.clear();
  m_pdgId.clear();

  auto* hcOfThisEvent = event->GetHCofThisEvent();
  if (hcOfThisEvent == nullptr) {
    return;
  }
  std::size_t nCollections = hcOfThisEvent->GetNumberOfCollections();
  for (std::size_t i = 0; i < nCollections; i++) {
    auto* hitCollection = hcOfThisEvent->GetHC(i);
    std::size_t hcSize = hitCollection->GetSize();

    m_geoId.reserve(hcSize);
    m_isSignal.reserve(hcSize);

    m_parentTrackId.reserve(hcSize);
    m_trackId.reserve(hcSize);
    m_runId.reserve(hcSize);

    m_hitPosGlobal.reserve(hcSize);
    m_hitPosLocal.reserve(hcSize);

    m_hitMomDir.reserve(hcSize);
    m_hitE.reserve(hcSize);

    m_ipMomDir.reserve(hcSize);
    m_ipE.reserve(hcSize);
    m_vertex.reserve(hcSize);

    m_eDep.reserve(hcSize);
    m_pdgId.reserve(hcSize);

    for (std::size_t j = 0; j < hcSize; j++) {
      const auto* hit = dynamic_cast<SamplingHit*>(hitCollection->GetHit(j));

      m_geoId.push_back(hit->GetGeometryId());

      bool isSignal = (hit->GetPdgId() == 11) && (hit->GetTrackId() == 1) &&
                      (hit->GetParentTrackId() == 0);

      m_isSignal.push_back(isSignal);

      m_parentTrackId.push_back(hit->GetParentTrackId());
      m_trackId.push_back(hit->GetTrackId());
      m_runId.push_back(event->GetEventID());

      m_hitPosGlobal.push_back(TVector3(hit->GetHitPosGlobal().x(),
                                        hit->GetHitPosGlobal().y(),
                                        hit->GetHitPosGlobal().z()));
      m_hitPosLocal.push_back(
          TVector2(hit->GetHitPosLocal().x(), hit->GetHitPosLocal().y()));

      m_hitMomDir.push_back(TVector3(hit->GetMomDir().x(), hit->GetMomDir().y(),
                                     hit->GetMomDir().z()));
      m_hitE.push_back(hit->GetETot());

      m_ipMomDir.push_back(TVector3(hit->GetMomDirIP().x(),
                                    hit->GetMomDirIP().y(),
                                    hit->GetMomDirIP().z()));
      m_ipE.push_back(hit->GetEIP());
      m_vertex.push_back(TVector3(hit->GetVertex().x(), hit->GetVertex().y(),
                                  hit->GetVertex().z()));

      m_eDep.push_back(hit->GetEDep());
      m_pdgId.push_back(hit->GetPdgId());
    }
    if (m_geoId.empty()) {
      continue;
    }
    m_tree->Fill();
  }
}

void Run::Merge(const G4Run* aRun) {
  const Run* localRun = static_cast<const Run*>(aRun);
  G4Run::Merge(aRun);
}
