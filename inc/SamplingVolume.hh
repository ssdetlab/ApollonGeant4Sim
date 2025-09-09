#ifndef SamplingPlane_h
#define SamplingPlane_h

#include "G4SystemOfUnits.hh"
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
  bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
  void EndOfEvent(G4HCofThisEvent* hitCollection) override;

 private:
  std::string m_indexedVolumeName;

  const double m_pixelX = 29.24 * um;
  const double m_pixelY = 26.88 * um;

  const double m_chipX = 29.94176 * mm;
  const double m_chipY = 13.762560 * mm;

  TrackerHitsCollection* m_hitsCollection = nullptr;
};

#endif
