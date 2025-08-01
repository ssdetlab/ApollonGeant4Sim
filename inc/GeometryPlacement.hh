#ifndef GeometryConstants_h
#define GeometryConstants_h

#include <cmath>
#include <string>

#include "G4SystemOfUnits.hh"
#include "G4Types.hh"

namespace GeometryConstants {

/// ---------------------------------------------------
/// World volume

const G4double worldHalfX = 20 * m;
const G4double worldHalfY = 20 * m;
const G4double worldHalfZ = 20 * m;

struct GeometryPlacement {
  GeometryPlacement();
  ~GeometryPlacement() = default;

  static GeometryPlacement* instance();

  std::string vcName;

  /// VC position in the World
  G4double vcCenterX;
  G4double vcCenterY;
  G4double vcCenterZ;

  /// VC rotation parameters in the World frame
  G4double vcRotationAngleX;
  G4double vcRotationAngleY;
  G4double vcRotationAngleZ;

  /// ---------------------------------------------------
  /// Tracking chamber 1

  std::string tc1Name;

  G4double tc1VaccumChamberDistance;

  G4double tc1CenterX;
  G4double tc1CenterY;
  G4double tc1CenterZ;

  G4double tc1RotationAngleX;
  G4double tc1RotationAngleY;
  G4double tc1RotationAngleZ;

  /// ---------------------------------------------------
  /// Dipole

  std::string dipoleName;

  G4double dipoleSamplingLayers1Distance;

  /// Magnet position in the World
  G4double wdCenterX;
  G4double wdCenterY;
  G4double wdCenterZ;

  /// Dipole rotation parameters in the World frame
  G4double wdRotationAngleX;
  G4double wdRotationAngleY;
  G4double wdRotationAngleZ;

  /// ---------------------------------------------------
  /// Tracking chamber 2

  std::string tc2Name;

  G4double tc2DipoleDistance;

  G4double tc2CenterX;
  G4double tc2CenterY;
  G4double tc2CenterZ;

  G4double tc2RotationAngleX;
  G4double tc2RotationAngleY;
  G4double tc2RotationAngleZ;

  /// ---------------------------------------------------
  /// Angled breadboard

  std::string bb1Name;

  G4double bb1RotationAngleX;
  G4double bb1RotationAngleY;
  G4double bb1RotationAngleZ;

  /// ---------------------------------------------------
  /// Support breadboard

  std::string bb2Name;

  G4double bb2RotationAngleX;
  G4double bb2RotationAngleY;
  G4double bb2RotationAngleZ;

  /// ---------------------------------------------------
  /// Breadboard mount

  std::string bm1Name;
  std::string bm2Name;

  G4double bmRotationAngleX;
  G4double bmRotationAngleY;
  G4double bmRotationAngleZ;

 protected:
  static GeometryPlacement* m_instance;
};

}  // namespace GeometryConstants

#endif
