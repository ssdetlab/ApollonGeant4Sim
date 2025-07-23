#ifndef SamplingHit_h
#define SamplingHit_h 1

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"
#include "tls.hh"

class SamplingHit : public G4VHit {
 public:
  SamplingHit() = default;
  SamplingHit(const SamplingHit&) = default;
  ~SamplingHit() override = default;

  /// operators
  SamplingHit& operator=(const SamplingHit&) = default;
  G4bool operator==(const SamplingHit&) const;

  inline void* operator new(size_t);
  inline void operator delete(void*);

  /// methods from base class
  void Draw() override;
  void Print() override;

  /// Set methods
  void SetGeometryId(G4int id) { m_geoId = id; };

  void SetTrackId(G4int track) { m_trackId = track; };
  void SetPdgId(G4int id) { m_pdgId = id; };

  void SetPos(G4ThreeVector xyz) { m_pos = xyz; };
  void SetVertex(G4ThreeVector xyz) { m_vertex = xyz; };

  void SetMomDir(G4ThreeVector xyz) { m_momDir = xyz; };
  void SetMomDirIP(G4ThreeVector xyz) { m_momDirIP = xyz; };

  void SetEDep(G4double de) { m_eDep = de; };
  void SetETot(G4double e) { m_eTot = e; };
  void SetEIP(G4double e) { m_eIP = e; };

  /// Get methods
  G4int GetGeometryId() const { return m_geoId; };

  G4int GetTrackId() const { return m_trackId; };
  G4int GetPdgId() const { return m_pdgId; };

  G4ThreeVector GetPos() const { return m_pos; };
  G4ThreeVector GetVertex() const { return m_vertex; };

  G4ThreeVector GetMomDir() const { return m_momDir; };
  G4ThreeVector GetMomDirIP() const { return m_momDirIP; };

  G4double GetEDep() const { return m_eDep; };
  G4double GetETot() const { return m_eTot; };
  G4double GetEIP() const { return m_eIP; };

 private:
  G4int m_geoId = -1;

  G4int m_trackId = -1;
  G4int m_pdgId = -1;

  G4ThreeVector m_pos;
  G4ThreeVector m_vertex;

  G4ThreeVector m_momDir;
  G4ThreeVector m_momDirIP;

  G4double m_eDep = 0.;
  G4double m_eTot = 0.;
  G4double m_eIP = 0.;
};

using TrackerHitsCollection = G4THitsCollection<SamplingHit>;

extern G4ThreadLocal G4Allocator<SamplingHit>* TrackerHitAllocator;

inline void* SamplingHit::operator new(size_t) {
  if (!TrackerHitAllocator)
    TrackerHitAllocator = new G4Allocator<SamplingHit>;
  return (void*)TrackerHitAllocator->MallocSingle();
}

inline void SamplingHit::operator delete(void* hit) {
  TrackerHitAllocator->FreeSingle((SamplingHit*)hit);
}

#endif
