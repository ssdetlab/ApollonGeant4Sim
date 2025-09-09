#include "SamplingVolume.hh"

#include <G4TwoVector.hh>

#include "G4HCofThisEvent.hh"
#include "G4RotationMatrix.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4String.hh"
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
  int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, m_hitsCollection);
}

bool SamplingVolume::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
  auto newHit = new SamplingHit();

  int id = 100;
  const auto& touchable = aStep->GetTrack()->GetTouchableHandle();
  const auto* history = touchable->GetHistory();
  for (std::size_t i = 0; i < touchable->GetHistoryDepth(); i++) {
    if (history->GetVolume(i)->GetName().find("TrackingChamber1") !=
        std::string::npos) {
      id = 10;
    }
    if (history->GetVolume(i)->GetName().find("TrackingChamber2") !=
        std::string::npos) {
      id = 20;
    }
    if (history->GetVolume(i)->GetName() == m_indexedVolumeName) {
      id += history->GetVolume(i)->GetCopyNo();
      break;
    }
  }
  newHit->SetGeometryId(id);

  G4Track* track = aStep->GetTrack();

  newHit->SetParentTrackId(track->GetParentID());
  newHit->SetTrackId(track->GetTrackID());
  newHit->SetPdgId(track->GetParticleDefinition()->GetPDGEncoding());

  G4ThreeVector origin = touchable->GetTranslation();
  G4RotationMatrix rotation = *touchable->GetRotation();
  G4ThreeVector hitGlobal = aStep->GetPostStepPoint()->GetPosition();
  G4ThreeVector hitLocal = rotation.inverse() * (hitGlobal - origin);
  newHit->SetHitPosGlobal(hitGlobal);
  newHit->SetHitPosLocal({hitLocal.x(), hitLocal.y()});

  int pixIdX = (hitLocal.x() + m_chipX / 2.0) / m_pixelX;
  int pixIdY = (hitLocal.y() + m_chipY / 2.0) / m_pixelY;

  G4TwoVector pixCenterLocal((pixIdX + 0.5) * m_pixelX - m_chipX / 2.0,
                             (pixIdY + 0.5) * m_pixelY - m_chipY / 2.0);
  G4ThreeVector pixCenterGlobal = rotation * pixCenterLocal + origin;

  newHit->SetPixCenterLocal(pixCenterLocal);
  newHit->SetPixCenterGlobal(pixCenterGlobal);
  newHit->SetPixelId(pixIdX, pixIdY);

  newHit->SetMomDir(track->GetMomentumDirection());
  newHit->SetMomDirIP(track->GetVertexMomentumDirection());
  newHit->SetVertex(track->GetVertexPosition());

  newHit->SetEDep(aStep->GetTotalEnergyDeposit());
  newHit->SetETot(track->GetTotalEnergy());
  newHit->SetPTot(track->GetKineticEnergy());

  double vertexP = track->GetVertexKineticEnergy();
  double mass = track->GetParticleDefinition()->GetPDGMass();
  newHit->SetEIP(std::hypot(vertexP, mass));
  newHit->SetPIP(vertexP);

  m_hitsCollection->insert(newHit);

  return true;
}

void SamplingVolume::EndOfEvent(G4HCofThisEvent* hitCollection) {
  if (verboseLevel > 1) {
    int nofHits = m_hitsCollection->entries();
    G4cout << G4endl << "-------->Hits Collection: in this event they are "
           << nofHits << " hits in the tracker chambers: " << G4endl;
    for (int i = 0; i < nofHits; i++)
      (*m_hitsCollection)[i]->Print();
  }

  hitCollection->AddHitsCollection(hitCollection->GetNumberOfCollections(),
                                   m_hitsCollection);
}
