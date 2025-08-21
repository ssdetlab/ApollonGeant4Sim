#ifndef BreadboardFactory_h
#define BreadboardFactory_h

#include "G4AssemblyVolume.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4RunManager.hh"
#include "G4Types.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4VUserDetectorConstruction.hh"

class G4LogicalVolume;
class G4PhysicalVolume;

struct Breadboard {
  Breadboard();
  ~Breadboard() = default;

  static Breadboard *instance();

  std::string bbMaterial;

  G4double bbHalfX;
  G4double bbHalfY;
  G4double bbHalfZ;

  G4double holeRad;
  G4double holeHalfSpacing;

 protected:
  static Breadboard *m_instance;
};

class BreadboardFactory {
 public:
  struct Config {
    /// Breadboard name
    std::string name;

    /// Breadboard position in the World
    G4double bbCenterX;
    G4double bbCenterY;
    G4double bbCenterZ;

    /// Breadboard rotation parameters in the World frame
    G4double bbRotationAngleX;
    G4double bbRotationAngleY;
    G4double bbRotationAngleZ;

    const Breadboard *bbc;

    /// Check overlaps flag
    G4bool checkOverlaps;
  };

  BreadboardFactory() = default;
  ~BreadboardFactory() = default;

  G4VPhysicalVolume *construct(G4LogicalVolume *logicParent, const Config &cfg);
};

#endif
