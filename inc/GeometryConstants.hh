#ifndef GeometryParameters_h
#define GeometryParameters_h

#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "MaterialFactory.hh"

struct GeometryConstants {
  GeometryConstants() = default;
  ~GeometryConstants() = default;

  /// ---------------------------------------------------
  /// World volume

  const G4double worldHalfX = 20 * m;
  const G4double worldHalfY = 20 * m;
  const G4double worldHalfZ = 20 * m;

  /// --------------------------------------------------------------
  /// Vacuum chamber

  const std::string vcVacuumMaterial = "G4_Galactic";
  const std::string vcWallsMaterial = "G4_Al";
  const std::string vcDoorsMaterial = "G4_Al";
  const std::string vcWindowsMaterial = "G4_Al";
  const std::string vcFlangeMaterial = "G4_Al";

  /// Parameters of the encapsulating
  /// VC cylinder
  const G4double vcCylinderRadMin = 1000 * mm;
  const G4double vcCylinderThickness = 200 * mm;
  const G4double vcCylinderHalfZ = 825 * mm;

  /// Parameters of the outer
  /// cylinder VC cutout
  const G4double vcCylinderCutoutHalfX = 412.5 * mm;
  const G4double vcCylinderCutoutHalfY = 780 * mm;
  const G4double vcCylinderCutoutHalfZ = 100 * mm;

  /// Identity rotation door position
  /// in the chamber
  const G4double vcDoorCenterX = 0 * mm;
  const G4double vcDoorCenterY = 0 * mm;
  const G4double vcDoorCenterZ = 1122.5 * mm;

  /// VC exit door dimensions
  const G4double vcDoorHalfX = 350 * mm;
  const G4double vcDoorHalfY = 500 * mm;
  const G4double vcDoorHalfZ = 22.5 * mm;

  /// Identity rotation VC window
  /// position in the chamber
  const G4double vcWindowCenterX = 0 * mm;
  const G4double vcWindowCenterY = 0 * mm;
  const G4double vcWindowCenterZ = 1152.6 * mm;

  /// VC windows dimensions
  const G4double bigVcWindowRad = 254 * mm;
  const G4double smallVcWindowRad = 185 * mm;
  const G4double vcWindowHalfZ = 8.5 * mm;

  /// Identity rotation VC flange
  /// position in the chamber
  const G4double vcFlangeCenterX = 0 * mm;
  const G4double vcFlangeCenterY = 0 * mm;
  const G4double vcFlangeCenterZ = 1158.6 * mm;

  /// VC flange dimensions
  const G4double vcFlangeRad = 120 * mm;
  const G4double vcFlangeHalfZ = 6 * mm;

  /// VC flange exit window dimensions
  const G4double vcFlangeWindowHalfX = 95 * mm;
  const G4double vcFlangeWindowHalfY = 15 * mm;
  const G4double vcFlangeWindowHalfZ = 0.25 * mm;

  /// Angle between the VC exits
  const G4double vcExitAngleSpacing = M_PI / 4 * rad;

  /// Bounding box parameters
  const G4double vcRad = vcFlangeCenterZ + vcFlangeHalfZ;

  /// --------------------------------------------------------------
  /// Dipole magnet

  const std::string wmContainerMaterial = "G4_AIR";
  const std::string wmAlPlateMaterial = Material::Al6061::name;
  const std::string wmIronYokeMaterial = Material::MildSteel::name;
  const std::string wmMagPlateMaterial = Material::Neodymium::name;

  /// Magnet bottom iron yoke dimensions
  const G4double wmIronYokeBottomHalfX = 60.5 * mm;
  const G4double wmIronYokeBottomHalfY = 51 * mm;
  const G4double wmIronYokeBottomHalfZ = 15 * mm;

  /// Magnet bottom iron yoke dimensions
  const G4double wmIronYokeSideHalfX = 60.5 * mm;
  const G4double wmIronYokeSideHalfY = 15.5 * mm;
  const G4double wmIronYokeSideHalfZ = 41.25 * mm;

  /// Magnet plate dimensions
  const G4double wmMagPlateHalfX = 60.5 * mm;
  const G4double wmMagPlateHalfY = 2.5 * mm;
  const G4double wmMagPlateHalfZ = 30 * mm;

  /// Iron plate dimensions
  const G4double wmIronPlateHalfX = 60.5 * mm;
  const G4double wmIronPlateHalfY = 2.5 * mm;
  const G4double wmIronPlateHalfZ = 30 * mm;

  /// Aluminium spacer dimensions
  const G4double wmAlSpacerHalfX = 60.5 * mm;
  const G4double wmAlSpacerHalfY = 20 * mm;
  const G4double wmAlSpacerHalfZ = 11.25 * mm;

  /// Aluminium side plate dimensions
  const G4double wmAlSidePlateHalfX = 1 * mm;
  const G4double wmAlSidePlateHalfY = 51 * mm;
  const G4double wmAlSidePlateHalfZ = 56.25 * mm;

  /// Aluminium side plate cutout dimensions
  const G4double wmAlSidePlateCutoutHalfX = wmAlSidePlateHalfX * 1.1;
  const G4double wmAlSidePlateCutoutHalfY = 12.5 * mm;
  const G4double wmAlSidePlateCutoutHalfZ = 40.5 * mm;

  /// Magnetic field vector
  const G4ThreeVector wmField = G4ThreeVector(0.35 * tesla, 0.0, 0.0);

  const G4double wmSamplingLayers1Distance = 2 * cm;

  /// Bounding box parameters
  const G4double wmHalfX = wmIronYokeBottomHalfX + 2 * wmAlSidePlateHalfX;
  const G4double wmHalfY = wmIronYokeBottomHalfY;
  const G4double wmHalfZ = wmIronYokeBottomHalfZ + wmIronYokeSideHalfZ;

  /// --------------------------------------------------------------
  /// Vacuum chamber

  const G4double inch = 25.4 * mm;

  const std::string trackerContainerMaterial = "G4_AIR";
  const std::string trackerPCBMaterial = Material::FR4::name;
  const std::string trackerMaterial = "G4_Si";
  const std::string trackerNinePcbConMaterial = Material::PBT::name;

  const G4double OPPPSensorX = 30.0 * mm;
  const G4double OPPPSensorY = 15.0 * mm;
  const G4double OPPPSensorZ = 0.05 * mm;

  const G4double OPPPSensorNCellX = 1024;
  const G4double OPPPSensorNCellY = 512;
  const G4double OPPPSensorPixelX = 0.02924 * mm;
  const G4double OPPPSensorPixelY = 0.02688 * mm;
  const G4double OPPPSensorPixelZ = 0.025 * mm;

  const G4double ProtoTrackerLDOBoxX = 64.0 * mm;
  const G4double ProtoTrackerLDOBoxY = 22.0 * mm;
  const G4double ProtoTrackerLDOBoxZ = 60.0 * mm;
  const G4double ProtoTrackerLDOBoxD = 1.0 * mm;
  const G4double ProtoTrackerLDOBoxXpos = 48.0 * mm;
  const G4double ProtoTrackerLDOPCBY = 1.6 * mm;
  const G4double ProtoTrackerLDOPCBYpos = 2.0 * mm;

  const std::string ProtoTrackerBoxMaterial = "G4_Al";
  const G4double ProtoTrackerYpos = -4.825 * mm;

  const G4double ProtoTrackerWinFPnaeldz = 4.5 * inch - 1.745 * mm;

  const G4double ProtoTrackerBoxCutX = 9.65;
  const G4double ProtoTrackerBoxCutY = 7.6 * mm + ProtoTrackerLDOBoxY;

  const G4double ProtoTrkCarrierPcbX = 70.0 * mm;
  const G4double ProtoTrkCarrierPcbY = 83.7 * mm;
  const G4double ProtoTrkCarrierPcbZ = 1.7 * mm;
  const G4double ProtoTrkCarrierPcbBCutX = 9.35 * mm;
  const G4double ProtoTrkCarrierPcbBCutY = 13.7 * mm;

  const G4double ProtoTrkCarrierPcbACutXY = 4.5 * mm;
  const G4double ProtoTrkCarrierPcbCutX =
      22.45 * mm + 2.0 * ProtoTrkCarrierPcbACutXY;
  const G4double ProtoTrkCarrierPcbCutY = 8.55 * mm + ProtoTrkCarrierPcbACutXY;
  const G4double ProtoTrkCarrierPcbCutXpos = 19.25 * mm;
  const G4double ProtoTrkCarrierPcbCutYpos = 26.6 * mm;
  const G4double ProtoTrkCarrierSensorOffset = 0.75 * mm;
  const G4double ProtoTrackerCarrierHoldD = 8.0 * mm;
  const G4double ProtoTrackerCarrierHoldZ = 4.0 * mm;
  const std::string ProtoTrackerPEEKMaterial = Material::PEEK::name;

  const G4double ProtoTrackerFBPanelX = 70.5 * mm;
  const G4double ProtoTrackerFBPanelY = 105.9 * mm;
  const G4double ProtoTrackerFBPanelZ = 5.0 * mm;
  const G4double ProtoTrackerFBPanelCutX = 54.5 * mm;
  const G4double ProtoTrackerFBPanelCutY = 50.0 * mm;
  const G4double ProtoTrackerFBPanelCutXpos = 8.0 * mm;
  const G4double ProtoTrackerFBPanelCutYpos = 32.8 * mm;
  const G4double ProtoTrckKaptonCoverZ = 0.03 * mm;

  const G4double ProtoTrackerHoldPanelX = 70.5 * mm;
  const G4double ProtoTrackerHoldPanelY = 5.3 * mm;
  const G4double ProtoTrackerHoldPanelZ = 89.4 * mm;

  const G4double ProtoTrackerBottomPanelX = 137.0 * mm;
  const G4double ProtoTrackerBottomPanelY = 3.0 * mm;
  const G4double ProtoTrackerBottomPanelZ = ProtoTrackerHoldPanelZ;
  const G4double ProtoTrackerBottomPanelYposShift = 0.8 * mm;

  const G4double ProtoTrackerSidePanelX = 1.6 * mm;
  const G4double ProtoTrackerSidePanelY = 99.5 * mm;
  const G4double ProtoTrackerSidePanelZ =
      ProtoTrackerHoldPanelZ + 2.0 * ProtoTrackerFBPanelZ;

  const G4double ProtoTrackerLConnectX = 10.0 * mm;
  const G4double ProtoTrackerLConnectYZ = 21.0 * mm;
  const G4double ProtoTrackerLConnectD = 3.0 * mm;

  const G4double ProtoTrackerNinePcbX = 112.0 * mm;
  const G4double ProtoTrackerNinePcbY = 1.6 * mm;
  const G4double ProtoTrackerNinePcbZ = ProtoTrackerHoldPanelZ;
  const G4double ProtoTrackerNinePcbConX = 12.0 * mm;
  const G4double ProtoTrackerNinePcbConY = 8.4 * mm;
  const G4double ProtoTrackerNinePcbConZ = (5.08 * 4 + 2.0) * mm;
  const G4double ProtoTrackerNinePcbConD = 3.0 * mm;
  const G4double ProtoTrackerNinePcbConZpos = 10.0 * mm;
  const G4double ProtoTrackerLayerDZ = 10.0 * mm;

  const G4double ProtoTrackerPcieConX = 56.0 * mm;
  const G4double ProtoTrackerPcieConY = 10.0 * mm;
  const G4double ProtoTrackerPcieConZ = 7.5 * mm;
  const G4double ProtoTrackerPcieConGapY = 4.4 * mm;

  const G4double ProtoTrackerSupportX = 120.0 * mm;
  const G4double ProtoTrackerSupportY = 350.0 * mm;
  const G4double ProtoTrackerSupportZ = 100.0 * mm;
  const G4double ProtoTrackerSupportD = 30.0 * mm;
  const G4double ProtoTrackerSupSpacerX = 8.0 * mm;
  const G4double ProtoTrackerSupSpacerY = 135.0 * mm;
  const G4double ProtoTrackerSupSpacerZ = 75.0 * mm;
  const G4double ProtoTrackerSupHolderX = 28.0 * mm;
  const G4double ProtoTrackerSupHolderY = 150.0 * mm;
  const G4double ProtoTrackerSupHolderZ = 62.0 * mm;

  const G4double tcHalfX = ProtoTrackerBottomPanelX / 2.0;
  const G4double tcHalfY =
      ProtoTrackerFBPanelY / 2.0 + ProtoTrackerBottomPanelY / 2.0 -
      ProtoTrackerBottomPanelYposShift / 2.0 + ProtoTrackerLDOBoxY / 2.0;
  const G4double tcHalfZ = ProtoTrackerHoldPanelZ / 2.0 + ProtoTrackerFBPanelZ +
                           ProtoTrackerLConnectD;

  /// --------------------------------------------------------------
  /// Placement

  /// --------------------------------------------------------------
  /// Vacuum chamber

  const std::string vcName = "VacuumChamber";

  /// VC position in the World
  const G4double vcCenterX = 0 * cm;
  const G4double vcCenterY = 0 * cm;
  const G4double vcCenterZ = 0 * cm;

  /// VC rotation parameters in the World frame
  const G4double vcRotationAngleX = M_PI_2 * rad;
  const G4double vcRotationAngleY = 0 * rad;
  const G4double vcRotationAngleZ = 0 * rad;

  /// ---------------------------------------------------
  /// Tracking chamber 1

  const std::string tc1Name = "TrackingChamber1";

  const G4double tc1VaccumChamberDistance = 20 * mm;

  const G4double tc1CenterX = vcCenterX;
  const G4double tc1CenterY = vcCenterY;
  const G4double tc1CenterZ =
      vcCenterZ + vcRad + tc1VaccumChamberDistance + tcHalfZ;

  const G4double tc1RotationAngleX = 0;
  const G4double tc1RotationAngleY = 0;
  const G4double tc1RotationAngleZ = M_PI_2;

  /// ---------------------------------------------------
  /// Dipole

  const std::string dipoleName = "Dipole";

  const G4double dipoleSamplingLayers1Distance = 20 * mm;

  /// Magnet position in the World
  const G4double wdCenterX = vcCenterX;
  const G4double wdCenterY = vcCenterY;
  const G4double wdCenterZ =
      tc1CenterZ + tcHalfZ + dipoleSamplingLayers1Distance + wmHalfX;

  /// Dipole rotation parameters in the World frame
  const G4double wdRotationAngleX = M_PI_2 * rad;
  const G4double wdRotationAngleY = 0;
  const G4double wdRotationAngleZ = M_PI_2 * rad;

  /// ---------------------------------------------------
  /// Tracking chamber 2

  const std::string tc2Name = "TrackingChamber2";

  const G4double tc2DipoleDistance = 20 * mm;

  const G4double tc2CenterX = vcCenterX;
  const G4double tc2CenterY = vcCenterY;
  const G4double tc2CenterZ = wdCenterZ + wmHalfX + tc2DipoleDistance + tcHalfZ;

  const G4double tc2RotationAngleX = 0;
  const G4double tc2RotationAngleY = 0;
  const G4double tc2RotationAngleZ = M_PI_2;

  static const GeometryConstants* instance() {
    if (!m_instance) {
      m_instance = new GeometryConstants();
    }
    return m_instance;
  };

 protected:
  static GeometryConstants* m_instance;
};

#endif  // !GeometryParameters_h
