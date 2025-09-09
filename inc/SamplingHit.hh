#ifndef SamplingHit_h
#define SamplingHit_h 1

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4TwoVector.hh"
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

  /// Methods from base class
  void Draw() override;
  void Print() override;

  /// Set methods
  void SetGeometryId(int id) { m_geoId = id; };
  void SetPixelId(int idX, int idY) {
    m_pixIdX = idX;
    m_pixIdY = idY;
  };

  void SetParentTrackId(int id) { m_parentTrackId = id; };
  void SetTrackId(int id) { m_trackId = id; };
  void SetPdgId(int id) { m_pdgId = id; };

  void SetPixCenterGlobal(G4ThreeVector xyz) { m_pixCenterGlobal = xyz; };
  void SetPixCenterLocal(G4TwoVector xy) { m_pixCenterLocal = xy; };

  void SetHitPosGlobal(G4ThreeVector xyz) { m_hitPosGlobal = xyz; };
  void SetHitPosLocal(G4TwoVector xy) { m_hitPosLocal = xy; };
  void SetVertex(G4ThreeVector xyz) { m_vertex = xyz; };

  void SetMomDir(G4ThreeVector xyz) { m_momDir = xyz; };
  void SetMomDirIP(G4ThreeVector xyz) { m_momDirIP = xyz; };

  void SetEDep(double de) { m_eDep = de; };
  void SetETot(double e) { m_eTot = e; };
  void SetPTot(double p) { m_pTot = p; };
  void SetEIP(double e) { m_eIP = e; };
  void SetPIP(double p) { m_pIP = p; };

  /// Get methods
  int GetGeometryId() const { return m_geoId; };

  std::pair<int, int> GetPixelId() const { return {m_pixIdX, m_pixIdY}; };

  int GetParentTrackId() const { return m_parentTrackId; };
  int GetTrackId() const { return m_trackId; };
  int GetPdgId() const { return m_pdgId; };

  G4ThreeVector GetPixCenterGlobal() const { return m_pixCenterGlobal; };
  G4TwoVector GetPixCenterLocal() const { return m_pixCenterLocal; };

  G4ThreeVector GetHitPosGlobal() const { return m_hitPosGlobal; };
  G4TwoVector GetHitPosLocal() const { return m_hitPosLocal; };
  G4ThreeVector GetVertex() const { return m_vertex; };

  G4ThreeVector GetMomDir() const { return m_momDir; };
  G4ThreeVector GetMomDirIP() const { return m_momDirIP; };

  double GetEDep() const { return m_eDep; };
  double GetETot() const { return m_eTot; };
  double GetPTot() const { return m_pTot; };
  double GetEIP() const { return m_eIP; };
  double GetPIP() const { return m_pIP; };

 private:
  int m_geoId = -1;

  int m_pixIdX = -1;
  int m_pixIdY = -1;

  int m_parentTrackId = -1;
  int m_trackId = -1;
  int m_pdgId = -1;

  G4ThreeVector m_pixCenterGlobal;
  G4TwoVector m_pixCenterLocal;

  G4ThreeVector m_hitPosGlobal;
  G4TwoVector m_hitPosLocal;
  G4ThreeVector m_vertex;

  G4ThreeVector m_momDir;
  G4ThreeVector m_momDirIP;

  double m_eDep = 0.;
  double m_eTot = 0.;
  double m_pTot = 0.;
  double m_eIP = 0.;
  double m_pIP = 0.;
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
