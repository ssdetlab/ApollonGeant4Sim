#ifndef VacuumChamberFactory_h
#define VacuumChamberFactory_h

#include "G4LogicalBorderSurface.hh"
#include "G4RunManager.hh"
#include "G4Types.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4VUserDetectorConstruction.hh"
#include "GeometryConstants.hh"

class G4LogicalVolume;
class G4PhysicalVolume;

class VacuumChamberFactory {
 public:
  struct Config {
    /// VC instance name
    std::string name;

    /// VC position in the World
    G4double vcCenterX;
    G4double vcCenterY;
    G4double vcCenterZ;

    /// VC rotation parameters in the World frame
    G4double vcRotationAngleX;
    G4double vcRotationAngleY;
    G4double vcRotationAngleZ;

    /// VC parameters
    const GeometryConstants *gc;

    bool checkOverlaps;
  };

  VacuumChamberFactory() = default;
  ~VacuumChamberFactory() = default;

  G4VPhysicalVolume *construct(G4LogicalVolume *logicParent, const Config &cfg);

 private:
  std::pair<G4VSolid *, G4VSolid *> constructVcWalls(const Config &cfg);

  std::pair<G4VSolid *, G4VSolid *> constructVcDoors(const Config &cfg);

  std::pair<G4VSolid *, G4VSolid *> constructVcWindows(const Config &cfg);

  std::pair<G4VSolid *, G4VSolid *> constructVcFlange(const Config &cfg);
};

#endif
