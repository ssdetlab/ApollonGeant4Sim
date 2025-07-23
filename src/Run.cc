#include "Run.hh"

#include <cstddef>

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "SamplingHit.hh"

Run::Run(const std::string& filePath, const std::string& treeName) {
  m_file = new TFile(filePath.c_str(), "RECREATE");
  m_tree = new TTree(treeName.c_str(), treeName.c_str());

  m_tree->Branch("geoId", &m_geoId);

  m_tree->Branch("trackId", &m_trackId);
  m_tree->Branch("pdgId", &m_pdgId);

  m_tree->Branch("pos", &m_pos);
  m_tree->Branch("vertex", &m_vertex);

  m_tree->Branch("momDir", &m_momDir);
  m_tree->Branch("momDirIP", &m_momDirIP);

  m_tree->Branch("eDep", &m_eDep);
  m_tree->Branch("eTot", &m_eTot);
  m_tree->Branch("eIP", &m_eIP);
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

  m_trackId.clear();
  m_pdgId.clear();

  m_pos.clear();
  m_vertex.clear();

  m_momDir.clear();
  m_momDirIP.clear();

  m_eDep.clear();
  m_eTot.clear();
  m_eIP.clear();

  auto* hcOfThisEvent = event->GetHCofThisEvent();
  if (hcOfThisEvent == nullptr) {
    return;
  }
  std::size_t nCollections = hcOfThisEvent->GetNumberOfCollections();
  for (std::size_t i = 0; i < nCollections; i++) {
    auto* hitCollection = hcOfThisEvent->GetHC(i);
    std::size_t hcSize = hitCollection->GetSize();

    m_geoId.reserve(hcSize);

    m_trackId.reserve(hcSize);
    m_pdgId.reserve(hcSize);

    m_pos.reserve(hcSize);
    m_vertex.reserve(hcSize);

    m_momDir.reserve(hcSize);
    m_momDirIP.reserve(hcSize);

    m_eDep.reserve(hcSize);
    m_eTot.reserve(hcSize);
    m_eIP.reserve(hcSize);

    for (std::size_t j = 0; j < hcSize; j++) {
      const auto* hit = dynamic_cast<SamplingHit*>(hitCollection->GetHit(j));

      m_geoId.push_back(hit->GetGeometryId());

      m_trackId.push_back(hit->GetTrackId());
      m_pdgId.push_back(hit->GetPdgId());

      m_pos.push_back(hit->GetPos());
      m_vertex.push_back(hit->GetVertex());

      m_momDir.push_back(hit->GetMomDir());
      m_momDirIP.push_back(hit->GetMomDirIP());

      m_eDep.push_back(hit->GetEDep());
      m_eTot.push_back(hit->GetETot());
      m_eIP.push_back(hit->GetEIP());
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
