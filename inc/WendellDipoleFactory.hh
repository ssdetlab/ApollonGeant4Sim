#ifndef WendellDipoleFactory_h
#define WendellDipoleFactory_h

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

struct WendellDipole {
  WendellDipole();
  ~WendellDipole() = default;

  static WendellDipole *instance();

  std::string containerMaterial;
  std::string alPlateMaterial;
  std::string ironYokeMaterial;
  std::string magPlateMaterial;

  /// Magnet bottom iron yoke dimensions
  G4double ironYokeBottomHalfX;
  G4double ironYokeBottomHalfY;
  G4double ironYokeBottomHalfZ;

  /// Magnet bottom iron yoke dimensions
  G4double ironYokeSideHalfX;
  G4double ironYokeSideHalfY;
  G4double ironYokeSideHalfZ;

  /// Magnet plate dimensions
  G4double magPlateHalfX;
  G4double magPlateHalfY;
  G4double magPlateHalfZ;

  /// Iron plate dimensions
  G4double ironPlateHalfX;
  G4double ironPlateHalfY;
  G4double ironPlateHalfZ;

  /// Aluminium spacer dimensions
  G4double alSpacerHalfX;
  G4double alSpacerHalfY;
  G4double alSpacerHalfZ;

  /// Aluminium side plate dimensions
  G4double alSidePlateHalfX;
  G4double alSidePlateHalfY;
  G4double alSidePlateHalfZ;

  /// Aluminium side plate cutout dimensions
  G4double alSidePlateCutoutHalfX;
  G4double alSidePlateCutoutHalfY;
  G4double alSidePlateCutoutHalfZ;

  /// Magnetic field vector
  G4ThreeVector dipoleField;

  G4double dipoleSamplingLayers1Distance;

  /// Bounding box parameters
  G4double dipoleHalfX;
  G4double dipoleHalfY;
  G4double dipoleHalfZ;

 protected:
  static WendellDipole *m_instance;
};

}  // namespace GeometryConstants

class WendellDipoleFactory {
 public:
  struct Config {
    /// Magnet position in the World
    G4double dipoleCenterX;
    G4double dipoleCenterY;
    G4double dipoleCenterZ;

    /// Magnet rotation parameters in the World frame
    G4double dipoleRotationAngle;
    G4ThreeVector dipoleRotationAxis;

    /// Dipole parameters
    const GeometryConstants::WendellDipole* wdc;

    /// Check overlaps flag
    G4bool checkOverlaps;
  };

  WendellDipoleFactory(const Config &cfg) : m_cfg(cfg) {};
  ~WendellDipoleFactory() = default;

  G4VPhysicalVolume *construct(G4LogicalVolume *logicParent,
                               const std::string &name);

 private:
  Config m_cfg;
};

#endif
