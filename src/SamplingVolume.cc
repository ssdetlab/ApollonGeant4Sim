#include "SamplingVolume.hh"

#include <G4String.hh>

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"

SamplingVolume::SamplingVolume(const G4String& name,
                               const G4String& hitsCollectionName,
                               const G4String& indexedVolumeName)
    : m_indexedVolumeName(indexedVolumeName), G4VSensitiveDetector(name) {
  collectionName.insert(hitsCollectionName);
}

void SamplingVolume::Initialize(G4HCofThisEvent* hce) {
  // Create hits collection
  m_hitsCollection =
      new TrackerHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, m_hitsCollection);
}

G4bool SamplingVolume::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
  auto newHit = new SamplingHit();

  int id = -1;
  const auto& touchable = aStep->GetTrack()->GetTouchableHandle();
  const auto* history = touchable->GetHistory();
  for (std::size_t i = 0; i < touchable->GetHistoryDepth(); i++) {
    if (history->GetVolume(i)->GetName().find("TrackingChamber1") != std::string::npos) {
      id = 10;
    }
    if (history->GetVolume(i)->GetName().find("TrackingChamber2") != std::string::npos) {
      id = 20;
    }
    if (history->GetVolume(i)->GetName() == m_indexedVolumeName) {
      id += history->GetVolume(i)->GetCopyNo();
      break;
    }
  }
  newHit->SetGeometryId(id);

  newHit->SetTrackId(aStep->GetTrack()->GetTrackID());
  newHit->SetPdgId(
      aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());

  newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
  newHit->SetVertex(aStep->GetTrack()->GetVertexPosition());

  newHit->SetMomDir(aStep->GetTrack()->GetMomentumDirection());
  newHit->SetMomDirIP(aStep->GetTrack()->GetVertexMomentumDirection());

  newHit->SetEDep(aStep->GetTotalEnergyDeposit());
  newHit->SetETot(aStep->GetTrack()->GetTotalEnergy());
  newHit->SetEIP(aStep->GetTrack()->GetVertexKineticEnergy());

  m_hitsCollection->insert(newHit);

  return true;
}

void SamplingVolume::EndOfEvent(G4HCofThisEvent* hitCollection) {
  if (verboseLevel > 1) {
    G4int nofHits = m_hitsCollection->entries();
    G4cout << G4endl << "-------->Hits Collection: in this event they are "
           << nofHits << " hits in the tracker chambers: " << G4endl;
    for (G4int i = 0; i < nofHits; i++)
      (*m_hitsCollection)[i]->Print();
  }

  hitCollection->AddHitsCollection(hitCollection->GetNumberOfCollections(),
                                   m_hitsCollection);
}
