#ifndef BreadboardFactory_h
#define BreadboardFactory_h

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

struct Breadboard {
  Breadboard();
  ~Breadboard() = default;

  static Breadboard *instance();

  std::string bbMaterial;

  G4double bbHalfX;
  G4double bbHalfY;
  G4double bbHalfZ;

  G4double bbRotationAxis;
  G4double bbRotationAngle;

  G4double holeRad;
  G4double holeHalfSpacing;

 protected:
  static Breadboard *m_instance;
};

}  // namespace GeometryConstants

class BreadboardFactory {
 public:
  struct Config {
    /// TC position in the World
    G4double bbCenterX;
    G4double bbCenterY;
    G4double bbCenterZ;

    /// TC rotation parameters in the World frame
    G4double bbRotationAngle;
    G4ThreeVector bbRotationAxis;

    const GeometryConstants::Breadboard *bbc;

    /// Check overlaps flag
    G4bool checkOverlaps;
  };

  BreadboardFactory(const Config &cfg) : m_cfg(cfg) {};
  ~BreadboardFactory() = default;

  G4VPhysicalVolume *construct(G4LogicalVolume *logicParent,
                               const std::string &name);

 private:
  Config m_cfg;
};

#endif
