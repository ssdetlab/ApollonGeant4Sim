#ifndef TrackingChamberSupportFactory_h
#define TrackingChamberSupportFactory_h

#include "G4AssemblyVolume.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4VUserDetectorConstruction.hh"

class G4LogicalVolume;
class G4PhysicalVolume;

namespace GeometryConstants {

struct TrackingChamberSupport {
  TrackingChamberSupport();
  ~TrackingChamberSupport() = default;

  static TrackingChamberSupport *instance();

  std::string tcsMaterial;

  G4double tcsBottomPlaneHalfZ;
  G4double tcsTopPlaneHalfZ;
  G4double tcsHalfY;
  G4double tcsHalfZ;

  G4double tcsRotationAxis;
  G4double tcsRotationAngle;

  G4double holeRad;
  G4double holeHalfSpacing;

 protected:
  static TrackingChamberSupport *m_instance;
};

}  // namespace GeometryConstants

class TrackingChamberSupportFactory {
 public:
  struct Config {
    /// TC position in the World
    G4double tcsCenterX;
    G4double tcsCenterY;
    G4double tcsCenterZ;

    /// TC rotation parameters in the World frame
    G4double tcsRotationAngle;
    G4ThreeVector tcsRotationAxis;

    const GeometryConstants::TrackingChamberSupport *tcsc;

    /// Check overlaps flag
    G4bool checkOverlaps;
  };

  TrackingChamberSupportFactory(const Config &cfg) : m_cfg(cfg) {};
  ~TrackingChamberSupportFactory() = default;

  G4VPhysicalVolume *construct(G4LogicalVolume *logicParent,
                               const std::string &name);

 private:
  Config m_cfg;
};

#endif
