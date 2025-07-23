#include "SamplingHit.hh"

#include <iomanip>

#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"

G4ThreadLocal G4Allocator<SamplingHit>* TrackerHitAllocator = nullptr;

G4bool SamplingHit::operator==(const SamplingHit& right) const {
  return (this == &right) ? true : false;
}

void SamplingHit::Draw() {
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if (pVVisManager) {
    G4Circle circle(m_pos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1., 0., 0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

void SamplingHit::Print() {
  G4cout << "  trackID: " << m_trackId << " PDG ID: " << m_pdgId
         << "Edep: " << std::setw(7) << G4BestUnit(m_eDep, "Energy")
         << " Position: " << std::setw(7) << G4BestUnit(m_pos, "Length")
         << G4endl;
}
