#ifndef VacuumChamberFactory_h
#define VacuumChamberFactory_h

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

struct VacuumChamber {
  VacuumChamber();
  ~VacuumChamber() = default;

  static VacuumChamber *instance();

  std::string vcVacuumMaterial;
  std::string vcWallsMaterial;
  std::string vcDoorsMaterial;
  std::string vcWindowsMaterial;
  std::string vcFlangeMaterial;

  /// Parameters of the encapsulating
  /// VC cylinder
  G4double vcCylinderRadMin;
  G4double vcCylinderThickness;
  G4double vcCylinderHalfZ;

  /// Parameters of the outer
  /// cylinder VC cutout
  G4double vcCylinderCutoutHalfX;
  G4double vcCylinderCutoutHalfY;
  G4double vcCylinderCutoutHalfZ;

  /// Identity rotation door position
  /// in the chamber
  G4double vcDoorCenterX;
  G4double vcDoorCenterY;
  G4double vcDoorCenterZ;

  /// VC exit door dimensions
  G4double vcDoorHalfX;
  G4double vcDoorHalfY;
  G4double vcDoorHalfZ;

  /// Identity rotation VC window
  /// position in the chamber
  G4double vcWindowCenterX;
  G4double vcWindowCenterY;
  G4double vcWindowCenterZ;

  /// VC windows dimensions
  G4double bigVcWindowRad;
  G4double smallVcWindowRad;
  G4double vcWindowHalfZ;

  /// Identity rotation VC flange
  /// position in the chamber
  G4double vcFlangeCenterX;
  G4double vcFlangeCenterY;
  G4double vcFlangeCenterZ;

  /// VC flange dimensions
  G4double vcFlangeRad;
  G4double vcFlangeHalfZ;

  /// VC flange exit window dimensions
  G4double vcFlangeWindowHalfX;
  G4double vcFlangeWindowHalfY;
  G4double vcFlangeWindowHalfZ;

  /// Angle between the VC exits
  G4double vcExitAngleSpacing;

  /// Bounding box parameters
  G4double vcRad;

 protected:
  static VacuumChamber *m_instance;
};

}  // namespace GeometryConstants
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
    const GeometryConstants::VacuumChamber *vcc;

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
