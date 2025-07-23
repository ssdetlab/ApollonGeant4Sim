#ifndef SamplingPlane_h
#define SamplingPlane_h

#include <G4String.hh>
#include "G4VSensitiveDetector.hh"
#include "SamplingHit.hh"

class G4Step;
class G4HCofThisEvent;

class SamplingVolume : public G4VSensitiveDetector {
 public:
  SamplingVolume(const G4String& name, const G4String& hitsCollectionName,
                 const G4String& indexedVolumeName);
  ~SamplingVolume() override = default;

  void Initialize(G4HCofThisEvent* hitCollection) override;
  G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
  void EndOfEvent(G4HCofThisEvent* hitCollection) override;

 private:
  G4String m_indexedVolumeName;
  TrackerHitsCollection* m_hitsCollection = nullptr;
};

#endif
