#ifndef GeometryConstants_h
#define GeometryConstants_h

#include <cmath>
#include <string>

#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"

namespace GeometryConstants {

/// ---------------------------------------------------
/// World volume

const G4double worldHalfX = 20 * m;
const G4double worldHalfY = 20 * m;
const G4double worldHalfZ = 20 * m;

// /// ---------------------------------------------------
// /// Sampling plane
//
// G4double samplingPlaneHalfX = 6.881280 * mm;
// G4double samplingPlaneHalfY = 14.97088 * mm;
// G4double samplingPlaneHalfZ = 50 * um;
//
// std::string samplingPlaneMaterial = "G4_Si";
//
// /// ---------------------------------------------------
// /// Sampling volume
//
// G4int nSamplingPlanes = 5;
// G4double samplingPlaneSpacing = 10 * mm;
//
// G4double tcHalfX = samplingPlaneHalfX + 1 * um;
// G4double tcHalfY = samplingPlaneHalfY + 1 * um;
// G4double tcHalfZ =
//     (samplingPlaneHalfZ + samplingPlaneSpacing) * (nSamplingPlanes - 1) / 2.0
//     + 1 * um;

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
  G4double vcRotationAngle;
  G4ThreeVector vcRotationAxis;

  /// ---------------------------------------------------
  /// Sampling volume 1

  std::string tc1Name;

  G4double tc1VaccumChamberDistance;

  G4double tc1CenterX;
  G4double tc1CenterY;
  G4double tc1CenterZ;

  G4ThreeVector tc1RotationAxis;
  G4double tc1RotationAngle;

  /// ---------------------------------------------------
  /// Dipole

  std::string dipoleName;

  G4double dipoleSamplingLayers1Distance;

  /// Magnet position in the World
  G4double wdCenterX;
  G4double wdCenterY;
  G4double wdCenterZ;

  /// Dipole rotation parameters in the World frame
  G4double wdRotationAngle;
  G4ThreeVector wdRotationAxis;

  /// ---------------------------------------------------
  /// Sampling volume 2

  std::string tc2Name;

  G4double tc2DipoleDistance;

  G4double tc2CenterX;
  G4double tc2CenterY;
  G4double tc2CenterZ;

  G4ThreeVector tc2RotationAxis;
  G4double tc2RotationAngle;

 protected:
  static GeometryPlacement* m_instance;
};

}  // namespace GeometryConstants

#endif
