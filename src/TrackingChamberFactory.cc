#include "TrackingChamberFactory.hh"

#include <bitset>

#include <G4PhysicalVolumeStore.hh>
#include <G4TransportationManager.hh>
#include <G4Types.hh>

#include "G4ExtrudedSolid.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "MaterialFactory.hh"

namespace GeometryConstants {

TrackingChamber *TrackingChamber::m_instance = nullptr;

TrackingChamber::TrackingChamber() {
  trackerPCBMaterial = Material::FR4::name;
  trackerNinePcbConMaterial = Material::PBT::name;
  trackerContainerMaterial = "G4_AIR";
  trackerMaterial = "G4_Si";

  OPPPSensorX = 30.0 * mm;
  OPPPSensorY = 15.0 * mm;
  OPPPSensorZ = 0.05 * mm;
  OPPPSensorNCellX = 1024;
  OPPPSensorNCellY = 512;
  OPPPSensorPixelX = 0.02924 * mm;
  OPPPSensorPixelY = 0.02688 * mm;
  OPPPSensorPixelZ = 0.025 * mm;

  ////// Tracker prototype
  ProtoTrackerLDOBoxX = 64.0 * mm;
  ProtoTrackerLDOBoxY = 22.0 * mm;
  ProtoTrackerLDOBoxZ = 60.0 * mm;
  ProtoTrackerLDOBoxD = 1.0 * mm;
  ProtoTrackerLDOBoxXpos = 48.0 * mm;
  ProtoTrackerLDOPCBY = 1.6 * mm;
  ProtoTrackerLDOPCBYpos = 2.0 * mm;  // gap between bottom panel and ldo pcb

  ProtoTrackerBoxMaterial = "G4_Al";
  ProtoTrackerYpos =
      -4.825 * mm;  //-1.075 *mm;     // distance in Y between bottom of the box
                    // and bottom of the window (-2 works -5 not).
  ProtoTrackerWinFPnaeldz =
      4.5 * inch -
      1.745 * mm;  // distance in Z between upsteram side of the window panel of
                   // PDC and downstrem panel of prototype box.

  ProtoTrackerBoxCutX =
      9.65;  // 11.75 *mm;                       // Container cut in X
  ProtoTrackerBoxCutY =
      7.6 * mm +
      ProtoTrackerLDOBoxY;  // Container size after cut for the beam pipe

  ProtoTrkCarrierPcbX = 70.0 * mm;
  ProtoTrkCarrierPcbY = 83.7 * mm;
  ProtoTrkCarrierPcbZ = 1.7 * mm;
  ProtoTrkCarrierPcbBCutX = 9.35 * mm;  // (70 - 51.3)/2
  ProtoTrkCarrierPcbBCutY = 13.7 * mm;

  ProtoTrkCarrierPcbACutXY = 4.5 * mm;
  ProtoTrkCarrierPcbCutX = 22.45 * mm + 2.0 * ProtoTrkCarrierPcbACutXY;
  ProtoTrkCarrierPcbCutY = 8.55 * mm + ProtoTrkCarrierPcbACutXY;
  ProtoTrkCarrierPcbCutXpos = 19.25 * mm;  // from left pcb side
  ProtoTrkCarrierPcbCutYpos = 26.6 * mm;   // from top
  ProtoTrkCarrierSensorOffset =
      0.75 *
      mm;  // sensor displacement with respect to the upper edge of the cut
  ProtoTrackerCarrierHoldD = 8.0 * mm;
  ProtoTrackerCarrierHoldZ =
      4.0 *
      mm;  // It is a bit reuced in the code to stay within a layer (~2.9mm)
  ProtoTrackerPEEKMaterial = "PEEK";

  ProtoTrackerFBPanelX = 70.5 * mm;
  ProtoTrackerFBPanelY = 105.9 * mm;
  ProtoTrackerFBPanelZ = 5.0 * mm;
  ProtoTrackerFBPanelCutX = 54.5 * mm;
  ProtoTrackerFBPanelCutY = 50.0 * mm;
  ProtoTrackerFBPanelCutXpos =
      8.0 * mm;  // The cut is in the middle in x direction
  ProtoTrackerFBPanelCutYpos = 32.8 * mm;
  ProtoTrckKaptonCoverZ = 0.03 * mm;

  ProtoTrackerHoldPanelX = 70.5 * mm;
  ProtoTrackerHoldPanelY = 5.3 * mm;
  ProtoTrackerHoldPanelZ = 89.4 * mm;

  ProtoTrackerBottomPanelX = 137.0 * mm;
  ProtoTrackerBottomPanelY = 3.0 * mm;
  ProtoTrackerBottomPanelZ = ProtoTrackerHoldPanelZ;
  ProtoTrackerBottomPanelYposShift = 0.8 * mm;

  ProtoTrackerSidePanelX = 1.6 * mm;
  ProtoTrackerSidePanelY = 99.5 * mm;
  ProtoTrackerSidePanelZ = ProtoTrackerHoldPanelZ + 2.0 * ProtoTrackerFBPanelZ;

  ProtoTrackerLConnectX = 10.0 * mm;
  ProtoTrackerLConnectYZ = 21.0 * mm;
  ProtoTrackerLConnectD = 3.0 * mm;

  ProtoTrackerNinePcbX = 112.0 * mm;
  ProtoTrackerNinePcbY = 1.6 * mm;
  ProtoTrackerNinePcbZ = ProtoTrackerHoldPanelZ;
  ProtoTrackerNinePcbConX = 12.0 * mm;
  ProtoTrackerNinePcbConY = 8.4 * mm;
  ProtoTrackerNinePcbConZ = (5.08 * 4 + 2.0) * mm;
  ProtoTrackerNinePcbConD = 3.0 * mm;
  ProtoTrackerNinePcbConZpos = 10.0 * mm;  // Shift from the PCB edge
  ProtoTrackerLayerDZ = 10.0 * mm;

  ProtoTrackerPcieConX = 56.0 * mm;
  ProtoTrackerPcieConY = 10.0 * mm;
  ProtoTrackerPcieConZ = 7.5 * mm;
  ProtoTrackerPcieConGapY = 4.4 * mm;

  ProtoTrackerSupportX = 120.0 * mm;
  ProtoTrackerSupportY = 350.0 * mm;
  ProtoTrackerSupportZ = 100.0 * mm;
  ProtoTrackerSupportD = 30.0 * mm;
  ProtoTrackerSupSpacerX = 8.0 * mm;
  ProtoTrackerSupSpacerY = 135.0 * mm;
  ProtoTrackerSupSpacerZ = 75.0 * mm;
  ProtoTrackerSupHolderX = 28.0 * mm;
  ProtoTrackerSupHolderY = 150.0 * mm;
  ProtoTrackerSupHolderZ = 62.0 * mm;

  trackingChamberHalfX = ProtoTrackerBottomPanelX / 2.0;
  trackingChamberHalfY =
      ProtoTrackerFBPanelY / 2.0 + ProtoTrackerBottomPanelY / 2.0 -
      ProtoTrackerBottomPanelYposShift / 2.0 + ProtoTrackerLDOBoxY / 2.0;
  trackingChamberHalfZ = ProtoTrackerHoldPanelZ / 2.0 + ProtoTrackerFBPanelZ +
                         ProtoTrackerLConnectD;
}

TrackingChamber *TrackingChamber::instance() {
  if (!m_instance) {
    m_instance = new TrackingChamber();
  }
  return m_instance;
}

}  // namespace GeometryConstants

G4VPhysicalVolume *TrackingChamberFactory::construct(
    G4LogicalVolume *logicParent, const std::string &name) {
  G4Material *protoTrackerContainerMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          m_cfg.tcc->trackerContainerMaterial);
  G4Material *trackerPCBMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          m_cfg.tcc->trackerPCBMaterial);
  G4Material *protoTrackeBoxMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          m_cfg.tcc->ProtoTrackerBoxMaterial);

  G4double protoTrckContainerX = m_cfg.tcc->ProtoTrackerBottomPanelX;
  G4double protoTrckContainerY = m_cfg.tcc->ProtoTrackerFBPanelY +
                                 m_cfg.tcc->ProtoTrackerBottomPanelY -
                                 m_cfg.tcc->ProtoTrackerBottomPanelYposShift +
                                 m_cfg.tcc->ProtoTrackerLDOBoxY;
  G4double protoTrckContainerZ = m_cfg.tcc->ProtoTrackerHoldPanelZ +
                                 2.0 * m_cfg.tcc->ProtoTrackerFBPanelZ +
                                 2.0 * m_cfg.tcc->ProtoTrackerLConnectD;

  // This is sensor middle position with respect to top of the holding panel
  G4double SensMiddleToTopHold =
      protoTrckContainerY - m_cfg.tcc->ProtoTrackerBoxCutY -
      m_cfg.tcc->ProtoTrackerPcieConGapY - m_cfg.tcc->ProtoTrkCarrierPcbY +
      m_cfg.tcc->ProtoTrkCarrierPcbCutYpos +
      m_cfg.tcc->ProtoTrkCarrierSensorOffset + 0.5 * m_cfg.tcc->OPPPSensorY;

  // Tracker prototype  Container
  G4Box *solidProtoTrackerContainer1 =
      new G4Box("solidProtoTrackerContainer1", protoTrckContainerX / 2.0,
                protoTrckContainerY / 2.0, protoTrckContainerZ / 2.0);
  G4Box *solidPTContainerCut =
      new G4Box("solidPTContainerCut", m_cfg.tcc->ProtoTrackerBoxCutX,
                protoTrckContainerY / 2.0, protoTrckContainerZ);
  G4Transform3D cntcut(G4RotationMatrix(),
                       G4ThreeVector(protoTrckContainerX / 2.0,
                                     m_cfg.tcc->ProtoTrackerBoxCutY, 0.0));
  G4SubtractionSolid *solidProtoTrackerContainer = new G4SubtractionSolid(
      "solidProtoTrackerContainer", solidProtoTrackerContainer1,
      solidPTContainerCut, cntcut);

  G4LogicalVolume *logicProtoTrackerContainer = new G4LogicalVolume(
      solidProtoTrackerContainer, protoTrackerContainerMaterial,
      "logicProtoTrackerContainer");

  G4double holderOffset = 0.0 * mm;
  // Top holding panel
  G4Box *solidProtoTrckHoldPanel = new G4Box(
      "solidProtoTrckHoldPanel", m_cfg.tcc->ProtoTrackerHoldPanelX / 2.0,
      m_cfg.tcc->ProtoTrackerHoldPanelY / 2.0,
      m_cfg.tcc->ProtoTrackerHoldPanelZ / 2.0);
  G4LogicalVolume *logicProtoTrckHoldPanel =
      new G4LogicalVolume(solidProtoTrckHoldPanel, protoTrackeBoxMaterial,
                          "logicProtoTrckHoldPanel");
  G4double hpxpos =
      0.5 * (protoTrckContainerX - m_cfg.tcc->ProtoTrackerHoldPanelX) -
      m_cfg.tcc->ProtoTrackerSidePanelX - m_cfg.tcc->ProtoTrackerBoxCutX;
  G4double hpypos =
      0.5 * (protoTrckContainerY - m_cfg.tcc->ProtoTrackerHoldPanelY) -
      holderOffset;
  new G4PVPlacement(0, G4ThreeVector(hpxpos, hpypos, 0.0),
                    logicProtoTrckHoldPanel, "ProtoTrckHoldPanel",
                    logicProtoTrackerContainer, false, 0, m_cfg.checkOverlaps);

  // Front and back panels
  G4Box *solidProtoTrckFBPanel1 =
      new G4Box("solidProtoTrckFBPanel1", m_cfg.tcc->ProtoTrackerFBPanelX / 2.0,
                m_cfg.tcc->ProtoTrackerFBPanelY / 2.0,
                m_cfg.tcc->ProtoTrackerFBPanelZ / 2.0);
  G4Box *solidProtoTrckFBPanelCut = new G4Box(
      "solidProtoTrckFBPanelCut", m_cfg.tcc->ProtoTrackerFBPanelCutX / 2.0,
      m_cfg.tcc->ProtoTrackerFBPanelCutY / 2.0,
      m_cfg.tcc->ProtoTrackerFBPanelZ);
  G4Transform3D fbpcut(G4RotationMatrix(),
                       G4ThreeVector(0.0,
                                     0.5 * (m_cfg.tcc->ProtoTrackerFBPanelCutY -
                                            m_cfg.tcc->ProtoTrackerFBPanelY) +
                                         m_cfg.tcc->ProtoTrackerFBPanelCutYpos,
                                     0.0));
  G4SubtractionSolid *solidProtoTrckFBPanel = new G4SubtractionSolid(
      "solidProtoTrackerContainer", solidProtoTrckFBPanel1,
      solidProtoTrckFBPanelCut, fbpcut);
  G4LogicalVolume *logicProtoTrckFBPanel = new G4LogicalVolume(
      solidProtoTrckFBPanel, protoTrackeBoxMaterial, "logicProtoTrckFBPanel");
  G4double fpypos =
      0.5 * (protoTrckContainerY - m_cfg.tcc->ProtoTrackerFBPanelY) -
      holderOffset;
  G4double fpzpos = 0.5 * (m_cfg.tcc->ProtoTrackerFBPanelZ +
                           m_cfg.tcc->ProtoTrackerHoldPanelZ);
  new G4PVPlacement(0, G4ThreeVector(hpxpos, fpypos, fpzpos),
                    logicProtoTrckFBPanel, "ProtoTrckFBPanel",
                    logicProtoTrackerContainer, false, 0, m_cfg.checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(hpxpos, fpypos, -fpzpos),
                    logicProtoTrckFBPanel, "ProtoTrckFBPanel",
                    logicProtoTrackerContainer, false, 1, m_cfg.checkOverlaps);

  // Kapton covering the cuts in front and back panels
  G4Material *protoTrackerKaptonMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_KAPTON");
  G4Box *solidProtoTrckKaptonCover = new G4Box(
      "solidProtoTrckKaptonCover", m_cfg.tcc->ProtoTrackerFBPanelX / 2.0,
      0.6 * m_cfg.tcc->ProtoTrackerFBPanelCutY,
      m_cfg.tcc->ProtoTrckKaptonCoverZ / 2.0);
  G4LogicalVolume *logicProtoTrckKaptonCover =
      new G4LogicalVolume(solidProtoTrckKaptonCover, protoTrackerKaptonMaterial,
                          "logicProtoTrckKaptonCover");

  G4double kapypos = fpypos +
                     0.5 * (m_cfg.tcc->ProtoTrackerFBPanelCutY -
                            m_cfg.tcc->ProtoTrackerFBPanelY) +
                     m_cfg.tcc->ProtoTrackerFBPanelCutYpos;
  G4double kapzpos = fpzpos + 0.5 * (m_cfg.tcc->ProtoTrackerFBPanelZ +
                                     m_cfg.tcc->ProtoTrckKaptonCoverZ);
  new G4PVPlacement(0, G4ThreeVector(hpxpos, kapypos, kapzpos),
                    logicProtoTrckKaptonCover, "ProtoTrckKaptonCover",
                    logicProtoTrackerContainer, false, 0, m_cfg.checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(hpxpos, kapypos, -kapzpos),
                    logicProtoTrckKaptonCover, "ProtoTrckKaptonCover",
                    logicProtoTrackerContainer, false, 1, m_cfg.checkOverlaps);

  // Side panels
  G4Box *solidProtoTrckSidePanel = new G4Box(
      "solidProtoTrckSidePanel", m_cfg.tcc->ProtoTrackerSidePanelX / 2.0,
      m_cfg.tcc->ProtoTrackerSidePanelY / 2.0,
      m_cfg.tcc->ProtoTrackerSidePanelZ / 2.0);
  G4LogicalVolume *logicProtoTrckSidePanel =
      new G4LogicalVolume(solidProtoTrckSidePanel, protoTrackeBoxMaterial,
                          "logicProtoTrckSidePanel");
  G4double spxpos = hpxpos + 0.5 * (m_cfg.tcc->ProtoTrackerHoldPanelX +
                                    m_cfg.tcc->ProtoTrackerSidePanelX);
  G4double spypos =
      0.5 * (protoTrckContainerY - m_cfg.tcc->ProtoTrackerSidePanelY) -
      holderOffset;
  new G4PVPlacement(0, G4ThreeVector(spxpos, spypos, 0.0),
                    logicProtoTrckSidePanel, "ProtoTrckSidePanel",
                    logicProtoTrackerContainer, false, 0, m_cfg.checkOverlaps);
  spxpos = hpxpos - 0.5 * (m_cfg.tcc->ProtoTrackerHoldPanelX +
                           m_cfg.tcc->ProtoTrackerSidePanelX);
  new G4PVPlacement(0, G4ThreeVector(spxpos, spypos, 0.0),
                    logicProtoTrckSidePanel, "ProtoTrckSidePanel",
                    logicProtoTrackerContainer, false, 1, m_cfg.checkOverlaps);

  // Bottom panel
  G4Box *solidProtoTrckBottomPanel = new G4Box(
      "solidProtoTrckBottomPanel", m_cfg.tcc->ProtoTrackerBottomPanelX / 2.0,
      m_cfg.tcc->ProtoTrackerBottomPanelY / 2.0,
      m_cfg.tcc->ProtoTrackerBottomPanelZ / 2.0);
  G4LogicalVolume *logicProtoTrckBottomPanel =
      new G4LogicalVolume(solidProtoTrckBottomPanel, protoTrackeBoxMaterial,
                          "logicProtoTrckBottomPanel");
  G4double bpxpos = 0.0;
  G4double bpypos =
      0.5 * (protoTrckContainerY - m_cfg.tcc->ProtoTrackerBottomPanelY) -
      m_cfg.tcc->ProtoTrackerFBPanelY +
      m_cfg.tcc->ProtoTrackerBottomPanelYposShift - holderOffset;
  new G4PVPlacement(0, G4ThreeVector(bpxpos, bpypos, 0.0),
                    logicProtoTrckBottomPanel, "ProtoTrckBottomPanel",
                    logicProtoTrackerContainer, false, 0, m_cfg.checkOverlaps);

  // L shape connector of the bottom panel to front and back
  G4LogicalVolume *logicProtoTrackerLConnector = constructLConnector();
  G4double lconypos = bpypos +
                      0.5 * (m_cfg.tcc->ProtoTrackerLConnectYZ -
                             m_cfg.tcc->ProtoTrackerBottomPanelY) -
                      m_cfg.tcc->ProtoTrackerLConnectD;
  G4double lconzpos = fpzpos +
                      0.5 * (m_cfg.tcc->ProtoTrackerFBPanelZ -
                             m_cfg.tcc->ProtoTrackerLConnectYZ) +
                      m_cfg.tcc->ProtoTrackerLConnectD;
  new G4PVPlacement(0, G4ThreeVector(hpxpos, lconypos, -lconzpos),
                    logicProtoTrackerLConnector, "ProtoTrackerLConnector",
                    logicProtoTrackerContainer, false, 0, m_cfg.checkOverlaps);
  new G4PVPlacement(new G4RotationMatrix(G4ThreeVector(0.0, 1.0, 0.0), M_PI),
                    G4ThreeVector(hpxpos, lconypos, lconzpos),
                    logicProtoTrackerLConnector, "ProtoTrackerLConnector",
                    logicProtoTrackerContainer, false, 1, m_cfg.checkOverlaps);

  // LDO cover box
  G4Box *solidProtoTrckLDOBox1 =
      new G4Box("solidProtoTrckLDOBox1", m_cfg.tcc->ProtoTrackerLDOBoxX / 2.0,
                m_cfg.tcc->ProtoTrackerLDOBoxY / 2.0,
                m_cfg.tcc->ProtoTrackerLDOBoxZ / 2.0);
  G4double ldocutx =
      m_cfg.tcc->ProtoTrackerLDOBoxX - 2.0 * m_cfg.tcc->ProtoTrackerLDOBoxD;
  G4double ldocutz =
      m_cfg.tcc->ProtoTrackerLDOBoxZ - 2.0 * m_cfg.tcc->ProtoTrackerLDOBoxD;
  G4Box *solidProtoTrckLDOBoxCut =
      new G4Box("solidProtoTrckLDOBoxCut", ldocutx / 2.0,
                m_cfg.tcc->ProtoTrackerLDOBoxY / 2.0, ldocutz / 2.0);
  G4Transform3D ldotrcut(
      G4RotationMatrix(),
      G4ThreeVector(0.0, m_cfg.tcc->ProtoTrackerLDOBoxD, 0.0));
  G4SubtractionSolid *solidProtoTrckLDOBox =
      new G4SubtractionSolid("solidProtoTrckLDOBox", solidProtoTrckLDOBox1,
                             solidProtoTrckLDOBoxCut, ldotrcut);
  G4LogicalVolume *logicProtoTrckLDOBox = new G4LogicalVolume(
      solidProtoTrckLDOBox, protoTrackeBoxMaterial, "logicProtoTrckLDOBox");

  G4double ldoxpos =
      0.5 * (protoTrckContainerX - m_cfg.tcc->ProtoTrackerLDOBoxX) -
      m_cfg.tcc->ProtoTrackerLDOBoxXpos;
  G4double ldoypos = bpypos - 0.5 * (m_cfg.tcc->ProtoTrackerLDOBoxY +
                                     m_cfg.tcc->ProtoTrackerBottomPanelY);
  new G4PVPlacement(0, G4ThreeVector(ldoxpos, ldoypos, 0.0),
                    logicProtoTrckLDOBox, "ProtoTrckLDOBox",
                    logicProtoTrackerContainer, false, 0, m_cfg.checkOverlaps);
  // LDO PCB
  G4Box *solidProtoTrckLDOPCB =
      new G4Box("solidProtoTrckLDOPCB", ldocutx / 2.0,
                m_cfg.tcc->ProtoTrackerLDOPCBY / 2.0, ldocutz / 2.0);
  G4LogicalVolume *logicProtoTrckLDOPCB = new G4LogicalVolume(
      solidProtoTrckLDOPCB, trackerPCBMaterial, "logicProtoTrckLDOPCB");
  G4double ldopcbypos =
      ldoypos +
      0.5 * (m_cfg.tcc->ProtoTrackerLDOBoxY - m_cfg.tcc->ProtoTrackerLDOPCBY) -
      m_cfg.tcc->ProtoTrackerLDOPCBYpos;
  new G4PVPlacement(0, G4ThreeVector(ldoxpos, ldopcbypos, 0.0),
                    logicProtoTrckLDOPCB, "ProtoTrckLDOPCB",
                    logicProtoTrackerContainer, false, 0, m_cfg.checkOverlaps);

  // Nine Alpide adapter PCB.
  G4AssemblyVolume *nineChipPcbAssy = constructNineAlpidePCB();
  G4double ninepcbxpos =
      0.5 * (protoTrckContainerX - m_cfg.tcc->ProtoTrackerNinePcbX);
  G4double ninepcbypos =
      m_cfg.tcc->ProtoTrackerBoxCutY -
      0.5 * (protoTrckContainerY + m_cfg.tcc->ProtoTrackerNinePcbY);
  G4ThreeVector ninechiptr(ninepcbxpos, ninepcbypos, 0.0);
  nineChipPcbAssy->MakeImprint(logicProtoTrackerContainer, ninechiptr, 0, 0,
                               m_cfg.checkOverlaps);

  // Carrier PCBs and PCIE connctors
  G4double CarrierPcbContY;
  G4LogicalVolume *carrierPcb = constructCarrierPCB(CarrierPcbContY);
  G4double carierxpos = hpxpos;
  G4double carierypos =
      ninepcbypos + 0.5 * (m_cfg.tcc->ProtoTrackerNinePcbY + CarrierPcbContY);
  G4int ncarier = 0;

  G4LogicalVolume *logicProtoTrackerPcieCon =
      G4LogicalVolumeStore::GetInstance()->GetVolume(
          "logicProtoTrackerPcieCon");
  if (!logicProtoTrackerPcieCon) {
    G4String msgstr(
        "Logical volume of PCIE connctor is not found in the "
        "G4LogicalVolumeStore.");
    G4Exception("EttProtoTracker::", "Construct()", FatalException,
                msgstr.c_str());
  }
  G4double pcieypos = ninepcbypos + 0.5 * (m_cfg.tcc->ProtoTrackerNinePcbY +
                                           m_cfg.tcc->ProtoTrackerPcieConY);

  std::bitset<9> sensor_pos_mask(std::string("101010101"));

  for (int ii = 0; ii < sensor_pos_mask.size(); ++ii) {
    G4double carierzpos = m_cfg.tcc->ProtoTrackerLayerDZ * (ii - 4.0);
    if (sensor_pos_mask.test(ii)) {
      new G4PVPlacement(0, G4ThreeVector(carierxpos, carierypos, carierzpos),
                        carrierPcb, "ProtoTrckCarrierPCB",
                        logicProtoTrackerContainer, false, ii /*ncarier*/,
                        m_cfg.checkOverlaps);
      ++ncarier;
    } else {
      new G4PVPlacement(0, G4ThreeVector(carierxpos, pcieypos, carierzpos),
                        logicProtoTrackerPcieCon, "ProtoTrckPCIEConnector",
                        logicProtoTrackerContainer, false, ii /*ii-ncarier*/,
                        m_cfg.checkOverlaps);
    }
  }

  G4PVPlacement *physProtoTrackerContainer = new G4PVPlacement(
      new G4RotationMatrix(m_cfg.tcRotationAxis, m_cfg.tcRotationAngle),
      G4ThreeVector(m_cfg.tcCenterX, m_cfg.tcCenterY, m_cfg.tcCenterZ),
      logicProtoTrackerContainer, name, logicParent, false, 0,
      m_cfg.checkOverlaps);

  // AddSegmentation();

  return physProtoTrackerContainer;
}

G4LogicalVolume *TrackingChamberFactory::constructSensor() {
  G4Material *sensorMaterial = G4NistManager::Instance()->FindOrBuildMaterial(
      m_cfg.tcc->trackerMaterial);
  G4Box *solidAlpideSensor =
      new G4Box("solidAlpideSensor", m_cfg.tcc->OPPPSensorX / 2.0,
                m_cfg.tcc->OPPPSensorY / 2.0, m_cfg.tcc->OPPPSensorZ / 2.0);
  G4LogicalVolume *logicAlpideSensor = new G4LogicalVolume(
      solidAlpideSensor, sensorMaterial, "logicAlpideSensor");

  G4double sensx = m_cfg.tcc->OPPPSensorNCellX * m_cfg.tcc->OPPPSensorPixelX;
  G4double sensy = m_cfg.tcc->OPPPSensorNCellY * m_cfg.tcc->OPPPSensorPixelY;
  G4Box *solidAlpideSensitive =
      new G4Box("solidAlpideSensitive", sensx / 2.0, sensy / 2.0,
                m_cfg.tcc->OPPPSensorPixelZ / 2.0);
  G4LogicalVolume *logicAlpideSensitive = new G4LogicalVolume(
      solidAlpideSensitive, sensorMaterial, "logicAlpideSensitive");

  new G4PVPlacement(
      0,
      G4ThreeVector(
          0.0, (m_cfg.tcc->OPPPSensorY - sensy) / 2.0,
          (m_cfg.tcc->OPPPSensorPixelZ - m_cfg.tcc->OPPPSensorZ) / 2.0),
      logicAlpideSensitive, "OPPPSensitive", logicAlpideSensor, false, 0,
      m_cfg.checkOverlaps);
  return logicAlpideSensor;
}

G4LogicalVolume *TrackingChamberFactory::constructLConnector() {
  G4Material *boxMaterial = G4NistManager::Instance()->FindOrBuildMaterial(
      m_cfg.tcc->ProtoTrackerBoxMaterial);

  G4Box *solidProtoTrckLConnect1 = new G4Box(
      "solidProtoTrckLConnect1", m_cfg.tcc->ProtoTrackerLConnectX / 2.0,
      m_cfg.tcc->ProtoTrackerLConnectYZ / 2.0,
      m_cfg.tcc->ProtoTrackerLConnectYZ / 2.0);
  G4Box *solidProtoTrckLConnectCut =
      new G4Box("solidProtoTrckLConnectCut", m_cfg.tcc->ProtoTrackerLConnectX,
                m_cfg.tcc->ProtoTrackerLConnectYZ / 2.0,
                m_cfg.tcc->ProtoTrackerLConnectYZ / 2.0);
  G4Transform3D trcut(G4RotationMatrix(),
                      G4ThreeVector(0.0, m_cfg.tcc->ProtoTrackerLConnectD,
                                    m_cfg.tcc->ProtoTrackerLConnectD));
  G4SubtractionSolid *solidProtoTrckLConnect =
      new G4SubtractionSolid("solidProtoTrckLConnect", solidProtoTrckLConnect1,
                             solidProtoTrckLConnectCut, trcut);

  G4LogicalVolume *logicProtoTrckLConnect = new G4LogicalVolume(
      solidProtoTrckLConnect, boxMaterial, "logicProtoTrckLConnect");

  return logicProtoTrckLConnect;
}

G4AssemblyVolume *TrackingChamberFactory::constructNineAlpidePCB() {
  G4Material *trackerPCBMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          m_cfg.tcc->trackerPCBMaterial);
  G4Material *trackerPowerConMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          m_cfg.tcc->trackerNinePcbConMaterial);

  G4AssemblyVolume *nineChipPcbAssembly = new G4AssemblyVolume();
  // PCB
  G4Box *solidProtoTrck9ChipPcb =
      new G4Box("solidProtoTrck9ChipPcb", m_cfg.tcc->ProtoTrackerNinePcbX / 2.0,
                m_cfg.tcc->ProtoTrackerNinePcbY / 2.0,
                m_cfg.tcc->ProtoTrackerNinePcbZ / 2.0);
  G4LogicalVolume *logicProtoTrck9ChipPcb = new G4LogicalVolume(
      solidProtoTrck9ChipPcb, trackerPCBMaterial, "logicProtoTrck9ChipPcb");
  // Power connectors
  G4Box *solidProtoTrck9ChipPcbCon1 = new G4Box(
      "solidProtoTrck9ChipPcbCon1", m_cfg.tcc->ProtoTrackerNinePcbConX / 2.0,
      m_cfg.tcc->ProtoTrackerNinePcbConY / 2.0,
      m_cfg.tcc->ProtoTrackerNinePcbConZ / 2.0);
  G4double concutx = m_cfg.tcc->ProtoTrackerNinePcbConX -
                     2.0 * m_cfg.tcc->ProtoTrackerNinePcbConD;
  G4double concutz = m_cfg.tcc->ProtoTrackerNinePcbConZ -
                     2.0 * m_cfg.tcc->ProtoTrackerNinePcbConD;
  G4Box *solidProtoTrck9ChipPcbConCut =
      new G4Box("solidProtoTrck9ChipPcbConCut", concutx / 2.0,
                m_cfg.tcc->ProtoTrackerNinePcbConY / 2.0, concutz / 2.0);
  G4Transform3D contrcut(
      G4RotationMatrix(),
      G4ThreeVector(0.0, -m_cfg.tcc->ProtoTrackerNinePcbConD, 0.0));
  G4SubtractionSolid *solidProtoTrck9ChipPcbCon = new G4SubtractionSolid(
      "solidProtoTrck9ChipPcbCon", solidProtoTrck9ChipPcbCon1,
      solidProtoTrck9ChipPcbConCut, contrcut);
  G4LogicalVolume *logicProtoTrck9ChipPcbCon =
      new G4LogicalVolume(solidProtoTrck9ChipPcbCon, trackerPowerConMaterial,
                          "logicProtoTrck9ChipPcbCon");

  G4ThreeVector pcbtr(0.0, 0.0, 0.0);
  nineChipPcbAssembly->AddPlacedVolume(logicProtoTrck9ChipPcb, pcbtr, 0);

  G4double contrx = 0.5 * (m_cfg.tcc->ProtoTrackerNinePcbConX -
                           m_cfg.tcc->ProtoTrackerNinePcbX);
  G4double contry = 0.5 * (m_cfg.tcc->ProtoTrackerNinePcbConY +
                           m_cfg.tcc->ProtoTrackerNinePcbY);
  G4double contrz = 0.5 * (m_cfg.tcc->ProtoTrackerNinePcbConZ -
                           m_cfg.tcc->ProtoTrackerNinePcbZ) +
                    m_cfg.tcc->ProtoTrackerNinePcbConZpos;
  G4ThreeVector contr(contrx, contry, contrz);
  nineChipPcbAssembly->AddPlacedVolume(logicProtoTrck9ChipPcbCon, contr, 0);
  contr.setZ(-contrz);
  nineChipPcbAssembly->AddPlacedVolume(logicProtoTrck9ChipPcbCon, contr, 0);

  return nineChipPcbAssembly;
}

G4LogicalVolume *TrackingChamberFactory::constructCarrierPCB(
    G4double &carrierPcbContainerY) {
  G4Material *protoTrackerContainerMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          m_cfg.tcc->trackerContainerMaterial);
  G4Material *carierPCBMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          m_cfg.tcc->trackerPCBMaterial);
  G4Material *trackerPcieConMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          m_cfg.tcc->trackerNinePcbConMaterial);
  G4Material *trackerPcieHoldMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          m_cfg.tcc->ProtoTrackerPEEKMaterial);

  G4double protoTrckContainerY = m_cfg.tcc->ProtoTrackerFBPanelY +
                                 m_cfg.tcc->ProtoTrackerBottomPanelY -
                                 m_cfg.tcc->ProtoTrackerBottomPanelYposShift +
                                 m_cfg.tcc->ProtoTrackerLDOBoxY;

  G4double ccontx = m_cfg.tcc->ProtoTrkCarrierPcbX;
  G4double cconty = protoTrckContainerY - m_cfg.tcc->ProtoTrackerBoxCutY -
                    m_cfg.tcc->ProtoTrackerHoldPanelY;
  G4double ccontz = m_cfg.tcc->ProtoTrackerPcieConZ;
  G4Box *solidCarrierPcbContainer = new G4Box(
      "solidCarrierPcbContainer", ccontx / 2.0, cconty / 2.0, ccontz / 2.0);
  G4LogicalVolume *logicCarrierPcbContainer = new G4LogicalVolume(
      solidCarrierPcbContainer, protoTrackerContainerMaterial,
      "logicCarrierPcbContainer");

  // Carrier PCB
  G4Box *solidCarrierPcb1 =
      new G4Box("solidCarrierPcb1", m_cfg.tcc->ProtoTrkCarrierPcbX / 2.0,
                m_cfg.tcc->ProtoTrkCarrierPcbY / 2.0,
                m_cfg.tcc->ProtoTrkCarrierPcbZ / 2.0);
  G4Box *solidCarrPcbCut1 = new G4Box(
      "solidCarrPcbCut1", m_cfg.tcc->ProtoTrkCarrierPcbBCutX,
      m_cfg.tcc->ProtoTrkCarrierPcbBCutY, m_cfg.tcc->ProtoTrkCarrierPcbZ);

  G4double y1 =
      m_cfg.tcc->ProtoTrkCarrierPcbCutY - m_cfg.tcc->ProtoTrkCarrierPcbACutXY;
  G4double x2 = m_cfg.tcc->ProtoTrkCarrierPcbACutXY;
  G4double y2 = m_cfg.tcc->ProtoTrkCarrierPcbCutY;
  G4double x3 =
      m_cfg.tcc->ProtoTrkCarrierPcbCutX - m_cfg.tcc->ProtoTrkCarrierPcbACutXY;
  G4double x4 = m_cfg.tcc->ProtoTrkCarrierPcbCutX;
  std::vector<G4TwoVector> cutvtx{G4TwoVector(0.0, 0.0), G4TwoVector(0.0, y1),
                                  G4TwoVector(x2, y2),   G4TwoVector(x3, y2),
                                  G4TwoVector(x4, y1),   G4TwoVector(x4, 0.0)};

  G4ExtrudedSolid *solidCarrPcbCut2 = new G4ExtrudedSolid(
      "solidCarrPcbCut2", cutvtx, m_cfg.tcc->ProtoTrkCarrierPcbZ,
      G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);

  G4Transform3D trmcut(G4RotationMatrix(),
                       G4ThreeVector(m_cfg.tcc->ProtoTrkCarrierPcbCutXpos -
                                         m_cfg.tcc->ProtoTrkCarrierPcbX / 2.0,
                                     m_cfg.tcc->ProtoTrkCarrierPcbY / 2.0 -
                                         m_cfg.tcc->ProtoTrkCarrierPcbCutYpos -
                                         m_cfg.tcc->ProtoTrkCarrierPcbCutY,
                                     0.0));
  G4SubtractionSolid *solidCarrierPcb2 = new G4SubtractionSolid(
      "solidCarrierPcb2", solidCarrierPcb1, solidCarrPcbCut2, trmcut);

  G4Transform3D trmcut1(
      G4RotationMatrix(),
      G4ThreeVector(-m_cfg.tcc->ProtoTrkCarrierPcbX / 2.0,
                    -m_cfg.tcc->ProtoTrkCarrierPcbY / 2.0, 0.0));
  G4SubtractionSolid *solidCarrierPcb3 = new G4SubtractionSolid(
      "solidCarrierPcb3", solidCarrierPcb2, solidCarrPcbCut1, trmcut1);

  G4Transform3D trmcut2(
      G4RotationMatrix(),
      G4ThreeVector(m_cfg.tcc->ProtoTrkCarrierPcbX / 2.0,
                    -m_cfg.tcc->ProtoTrkCarrierPcbY / 2.0, 0.0));
  G4SubtractionSolid *solidCarrierPcb = new G4SubtractionSolid(
      "solidCarrierPcb", solidCarrierPcb3, solidCarrPcbCut1, trmcut2);

  G4LogicalVolume *logicCarrierPcb = new G4LogicalVolume(
      solidCarrierPcb, carierPCBMaterial, "logicCarrierPcb");

  G4double crpcbypos = 0.5 * (m_cfg.tcc->ProtoTrkCarrierPcbY - cconty) +
                       m_cfg.tcc->ProtoTrackerPcieConGapY;
  new G4PVPlacement(0, G4ThreeVector(0.0, crpcbypos, 0.0), logicCarrierPcb,
                    "AlpideCarrierPCB", logicCarrierPcbContainer, false, 0,
                    m_cfg.checkOverlaps);

  // Alpide sensor
  G4LogicalVolume *logicAlpideSensor = constructSensor();

  G4double sensypos =
      crpcbypos +
      0.5 * (m_cfg.tcc->ProtoTrkCarrierPcbY - m_cfg.tcc->OPPPSensorY) -
      m_cfg.tcc->ProtoTrkCarrierPcbCutYpos -
      m_cfg.tcc->ProtoTrkCarrierSensorOffset;
  G4double senszpos =
      -0.5 * (m_cfg.tcc->ProtoTrkCarrierPcbZ + m_cfg.tcc->OPPPSensorZ);
  G4PVPlacement *physAlpideSensor = new G4PVPlacement(
      0, G4ThreeVector(0.0, sensypos, senszpos), logicAlpideSensor,
      "AlpideSensor", logicCarrierPcbContainer, false, 0, m_cfg.checkOverlaps);

  // PCIE connector
  G4Box *solidProtoTrackerPcieCon1 = new G4Box(
      "solidProtoTrackerPcieCon1", m_cfg.tcc->ProtoTrackerPcieConX / 2.0,
      m_cfg.tcc->ProtoTrackerPcieConY / 2.0,
      m_cfg.tcc->ProtoTrackerPcieConZ / 2.0);
  G4double pcieccutx =
      m_cfg.tcc->ProtoTrkCarrierPcbX - 2.0 * m_cfg.tcc->ProtoTrkCarrierPcbBCutX;
  G4Box *solidProtoTrackerPcieConCut = new G4Box(
      "solidProtoTrackerPcieConCut", pcieccutx / 2.0,
      m_cfg.tcc->ProtoTrackerPcieConY, m_cfg.tcc->ProtoTrkCarrierPcbZ / 2.0);
  G4Transform3D pcietrcut(G4RotationMatrix(), G4ThreeVector(0.0, 0.0, 0.0));
  G4SubtractionSolid *solidProtoTrackerPcieCon = new G4SubtractionSolid(
      "solidProtoTrackerPcieCon", solidProtoTrackerPcieCon1,
      solidProtoTrackerPcieConCut, pcietrcut);
  G4LogicalVolume *logicProtoTrackerPcieCon =
      new G4LogicalVolume(solidProtoTrackerPcieCon, trackerPcieConMaterial,
                          "logicProtoTrackerPcieCon");
  G4double pcieypos = 0.5 * (m_cfg.tcc->ProtoTrackerPcieConY - cconty);
  new G4PVPlacement(0, G4ThreeVector(0.0, pcieypos, 0.0),
                    logicProtoTrackerPcieCon, "ProtoTrackerPcieConnector",
                    logicCarrierPcbContainer, false, 0, m_cfg.checkOverlaps);

  // Carrier PCB PEEK holder
  G4double carrierHolderX = m_cfg.tcc->ProtoTrkCarrierPcbX;
  G4double carrierHolderY = cconty - m_cfg.tcc->ProtoTrackerPcieConY;
  G4double carrierHolderZ =
      std::min(0.5 * (ccontz - m_cfg.tcc->ProtoTrkCarrierPcbZ),
               m_cfg.tcc->ProtoTrackerCarrierHoldZ);

  G4Box *solidProtoTrackerCarrierHold1 =
      new G4Box("solidProtoTrackerCarrierHold1", carrierHolderX / 2.0,
                carrierHolderY / 2.0, carrierHolderZ / 2.0);

  G4double carrierHolderCutX =
      carrierHolderX - 2.0 * m_cfg.tcc->ProtoTrackerCarrierHoldD;
  G4Box *solidProtoTrackerCarrierHoldCut =
      new G4Box("solidProtoTrackerCarrierHoldCut", carrierHolderCutX / 2.0,
                carrierHolderY / 2.0, carrierHolderZ);
  G4double chCutYpos = cconty - m_cfg.tcc->ProtoTrackerPcieConGapY -
                       m_cfg.tcc->ProtoTrkCarrierPcbY +
                       m_cfg.tcc->ProtoTrackerCarrierHoldD;
  G4Transform3D chtrcut(G4RotationMatrix(),
                        G4ThreeVector(0.0, -chCutYpos, 0.0));
  G4SubtractionSolid *solidProtoTrackerCarrierHold = new G4SubtractionSolid(
      "solidProtoTrackerCarrierHold", solidProtoTrackerCarrierHold1,
      solidProtoTrackerCarrierHoldCut, chtrcut);
  G4LogicalVolume *logicProtoTrackerCarrierHold =
      new G4LogicalVolume(solidProtoTrackerCarrierHold, trackerPcieHoldMaterial,
                          "logicProtoTrackerCarrierHold");

  G4double chypos = 0.5 * (cconty - carrierHolderY);
  G4double chzpos = 0.5 * (m_cfg.tcc->ProtoTrkCarrierPcbZ + carrierHolderZ);
  new G4PVPlacement(0, G4ThreeVector(0.0, chypos, chzpos),
                    logicProtoTrackerCarrierHold, "ProtoTrackerCarrierHold",
                    logicCarrierPcbContainer, false, 0, m_cfg.checkOverlaps);

  carrierPcbContainerY = cconty;
  return logicCarrierPcbContainer;
}
