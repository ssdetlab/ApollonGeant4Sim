#include "GeometryPlacement.hh"

#include "TrackingChamberFactory.hh"
#include "VacuumChamberFactory.hh"
#include "WendellDipoleFactory.hh"

namespace GeometryConstants {

GeometryPlacement *GeometryPlacement::m_instance = nullptr;

GeometryPlacement::GeometryPlacement() {
  vcName = "VacuumChamber";

  /// VC position in the World
  vcCenterX = 0 * cm;
  vcCenterY = 0 * cm;
  vcCenterZ = 0 * cm;

  /// VC rotation parameters in the World frame
  vcRotationAngle = M_PI_2 * rad;
  vcRotationAxis = G4ThreeVector(1, 0, 0);

  /// ---------------------------------------------------
  /// Tracking chamber 1

  tc1Name = "TrackingChamber1";

  tc1VaccumChamberDistance = 2 * cm;

  G4double tcHalfZ = TrackingChamber::instance()->trackingChamberHalfZ;

  tc1CenterX = vcCenterX;
  tc1CenterY = vcCenterY;
  tc1CenterZ = vcCenterZ + VacuumChamber::instance()->vcRad +
               tc1VaccumChamberDistance + tcHalfZ;

  tc1RotationAxis = G4ThreeVector(0, 0, 1);
  tc1RotationAngle = M_PI_2;

  /// ---------------------------------------------------
  /// Dipole

  dipoleName = "Dipole";

  dipoleSamplingLayers1Distance = 2 * cm;

  /// Magnet position in the World
  wdCenterX = vcCenterX;
  wdCenterY = vcCenterY;

  wdCenterZ = tc1CenterZ + tcHalfZ + dipoleSamplingLayers1Distance +
              WendellDipole::instance()->dipoleHalfZ;

  /// Dipole rotation parameters in the World frame
  wdRotationAngle = M_PI_2 * rad;
  wdRotationAxis = G4ThreeVector(1, 0, 0);

  /// ---------------------------------------------------
  /// Tracking chamber 2

  tc2Name = "TrackingChamber2";

  tc2DipoleDistance = 2 * cm;

  tc2CenterX = vcCenterX;
  tc2CenterY = vcCenterY;
  tc2CenterZ = wdCenterZ + WendellDipole::instance()->dipoleHalfZ +
               tc2DipoleDistance + tcHalfZ;

  tc2RotationAxis = G4ThreeVector(0, 0, 1);
  tc2RotationAngle = M_PI_2;

  /// ---------------------------------------------------
  /// Breadboard

  bbName = "BreadboardTable";

  /// Breadboard position in the World
  bbCenterX = vcCenterX;
  bbCenterY = vcCenterY;
  bbCenterZ = tc1CenterZ + tcHalfZ + dipoleSamplingLayers1Distance +
              WendellDipole::instance()->dipoleHalfZ;

  /// Dipole rotation parameters in the World frame
  bbRotationAngle = M_PI_2 * rad;
  bbRotationAxis = G4ThreeVector(1, 0, 0);
}

GeometryPlacement *GeometryPlacement::instance() {
  if (!m_instance) {
    m_instance = new GeometryPlacement();
  }
  return m_instance;
}

}  // namespace GeometryConstants
