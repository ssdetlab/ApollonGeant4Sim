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
  vcRotationAngleX = M_PI_2 * rad;
  vcRotationAngleY = 0 * rad;
  vcRotationAngleZ = 0 * rad;

  /// ---------------------------------------------------
  /// Tracking chamber 1

  tc1Name = "TrackingChamber1";

  tc1VaccumChamberDistance = 2 * cm;

  G4double tcHalfZ = TrackingChamber::instance()->trackingChamberHalfZ;

  tc1CenterX = vcCenterX;
  tc1CenterY = vcCenterY;
  tc1CenterZ = vcCenterZ + VacuumChamber::instance()->vcRad +
               tc1VaccumChamberDistance + tcHalfZ;

  tc1RotationAngleX = 0;
  tc1RotationAngleY = 0;
  tc1RotationAngleZ = M_PI_2;

  /// ---------------------------------------------------
  /// Dipole

  dipoleName = "Dipole";

  dipoleSamplingLayers1Distance = 2 * cm;

  /// Magnet position in the World
  wdCenterX = vcCenterX;
  wdCenterY = vcCenterY;

  wdCenterZ = tc1CenterZ + tcHalfZ + dipoleSamplingLayers1Distance +
              WendellDipole::instance()->wdHalfZ;

  /// Dipole rotation parameters in the World frame
  wdRotationAngleX = M_PI_2 * rad;
  wdRotationAngleY = 0;
  wdRotationAngleZ = M_PI_2 * rad;

  /// ---------------------------------------------------
  /// Tracking chamber 2

  tc2Name = "TrackingChamber2";

  tc2DipoleDistance = 2 * cm;

  tc2CenterX = vcCenterX;
  tc2CenterY = vcCenterY;
  tc2CenterZ = wdCenterZ + WendellDipole::instance()->wdHalfZ +
               tc2DipoleDistance + tcHalfZ;

  tc2RotationAngleX = 0;
  tc2RotationAngleY = 0;
  tc2RotationAngleZ = M_PI_2;

  /// ---------------------------------------------------
  /// Angled breadboard

  bb1Name = "AngledBreadboard";

  /// Breadboard rotation parameters in the World frame
  bb1RotationAngleX = M_PI_2 * rad;
  bb1RotationAngleY = M_PI_4 * rad;
  bb1RotationAngleZ = 0 * rad;

  /// ---------------------------------------------------
  /// Support breadboard

  bb2Name = "SupportBreadboard";

  /// Breadboard rotation parameters in the World frame
  bb2RotationAngleX = M_PI_2 * rad;
  bb2RotationAngleY = 0 * rad;
  bb2RotationAngleZ = 0 * rad;

  /// ---------------------------------------------------
  /// Breadboard mount

  bm1Name = "BreadboardMount1";
  bm2Name = "BreadboardMount2";

  /// Breadboard rotation parameters in the World frame
  bmRotationAngleX = M_PI_2 * rad;
  bmRotationAngleY = M_PI_2 * rad;
  bmRotationAngleZ = 0 * rad;
}

GeometryPlacement *GeometryPlacement::instance() {
  if (!m_instance) {
    m_instance = new GeometryPlacement();
  }
  return m_instance;
}

}  // namespace GeometryConstants
