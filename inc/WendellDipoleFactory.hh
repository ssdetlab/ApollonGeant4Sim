#ifndef WendellDipoleFactory_h
#define WendellDipoleFactory_h

#include "G4LogicalBorderSurface.hh"
#include "G4RunManager.hh"
#include "G4Types.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4VUserDetectorConstruction.hh"
#include "GeometryConstants.hh"

class G4LogicalVolume;
class G4PhysicalVolume;

class WendellDipoleFactory {
 public:
  struct Config {
    /// Dipole name
    std::string name;

    /// Magnet position in the World
    G4double wdCenterX;
    G4double wdCenterY;
    G4double wdCenterZ;

    /// Magnet rotation parameters in the World frame
    G4double wdRotationAngleX;
    G4double wdRotationAngleY;
    G4double wdRotationAngleZ;

    G4double angle;

    /// Dipole parameters
    const GeometryConstants *gc;

    /// Check overlaps flag
    G4bool checkOverlaps;
  };

  WendellDipoleFactory() = default;
  ~WendellDipoleFactory() = default;

  G4VPhysicalVolume *construct(G4LogicalVolume *logicParent, const Config &cfg);
};

#endif
