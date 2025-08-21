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

G4VPhysicalVolume *TrackingChamberFactory::construct(
    G4LogicalVolume *logicParent, const Config &cfg) {
  G4Material *protoTrackerContainerMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          cfg.gc->trackerContainerMaterial);
  G4Material *trackerPCBMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          cfg.gc->trackerPCBMaterial);
  G4Material *protoTrackeBoxMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          cfg.gc->ProtoTrackerBoxMaterial);

  G4double protoTrckContainerX = cfg.gc->ProtoTrackerBottomPanelX;
  G4double protoTrckContainerY =
      cfg.gc->ProtoTrackerFBPanelY + cfg.gc->ProtoTrackerBottomPanelY -
      cfg.gc->ProtoTrackerBottomPanelYposShift + cfg.gc->ProtoTrackerLDOBoxY;
  G4double protoTrckContainerZ = cfg.gc->ProtoTrackerHoldPanelZ +
                                 2.0 * cfg.gc->ProtoTrackerFBPanelZ +
                                 2.0 * cfg.gc->ProtoTrackerLConnectD;

  // This is sensor middle position with respect to top of the holding panel
  G4double SensMiddleToTopHold =
      protoTrckContainerY - cfg.gc->ProtoTrackerBoxCutY -
      cfg.gc->ProtoTrackerPcieConGapY - cfg.gc->ProtoTrkCarrierPcbY +
      cfg.gc->ProtoTrkCarrierPcbCutYpos + cfg.gc->ProtoTrkCarrierSensorOffset +
      0.5 * cfg.gc->OPPPSensorY;

  // Tracker prototype  Container
  G4Box *solidProtoTrackerContainer1 =
      new G4Box("solidProtoTrackerContainer1", protoTrckContainerX / 2.0,
                protoTrckContainerY / 2.0, protoTrckContainerZ / 2.0);
  G4Box *solidPTContainerCut =
      new G4Box("solidPTContainerCut", cfg.gc->ProtoTrackerBoxCutX,
                protoTrckContainerY / 2.0, protoTrckContainerZ);
  G4Transform3D cntcut(G4RotationMatrix(),
                       G4ThreeVector(protoTrckContainerX / 2.0,
                                     cfg.gc->ProtoTrackerBoxCutY, 0.0));
  G4SubtractionSolid *solidProtoTrackerContainer = new G4SubtractionSolid(
      "solidProtoTrackerContainer", solidProtoTrackerContainer1,
      solidPTContainerCut, cntcut);

  G4LogicalVolume *logicProtoTrackerContainer = new G4LogicalVolume(
      solidProtoTrackerContainer, protoTrackerContainerMaterial,
      "logicProtoTrackerContainer");

  G4double holderOffset = 0.0 * mm;
  // Top holding panel
  G4Box *solidProtoTrckHoldPanel =
      new G4Box("solidProtoTrckHoldPanel", cfg.gc->ProtoTrackerHoldPanelX / 2.0,
                cfg.gc->ProtoTrackerHoldPanelY / 2.0,
                cfg.gc->ProtoTrackerHoldPanelZ / 2.0);
  G4LogicalVolume *logicProtoTrckHoldPanel =
      new G4LogicalVolume(solidProtoTrckHoldPanel, protoTrackeBoxMaterial,
                          "logicProtoTrckHoldPanel");
  G4double hpxpos =
      0.5 * (protoTrckContainerX - cfg.gc->ProtoTrackerHoldPanelX) -
      cfg.gc->ProtoTrackerSidePanelX - cfg.gc->ProtoTrackerBoxCutX;
  G4double hpypos =
      0.5 * (protoTrckContainerY - cfg.gc->ProtoTrackerHoldPanelY) -
      holderOffset;
  new G4PVPlacement(0, G4ThreeVector(hpxpos, hpypos, 0.0),
                    logicProtoTrckHoldPanel, "ProtoTrckHoldPanel",
                    logicProtoTrackerContainer, false, 0, cfg.checkOverlaps);

  // Front and back panels
  G4Box *solidProtoTrckFBPanel1 = new G4Box(
      "solidProtoTrckFBPanel1", cfg.gc->ProtoTrackerFBPanelX / 2.0,
      cfg.gc->ProtoTrackerFBPanelY / 2.0, cfg.gc->ProtoTrackerFBPanelZ / 2.0);
  G4Box *solidProtoTrckFBPanelCut = new G4Box(
      "solidProtoTrckFBPanelCut", cfg.gc->ProtoTrackerFBPanelCutX / 2.0,
      cfg.gc->ProtoTrackerFBPanelCutY / 2.0, cfg.gc->ProtoTrackerFBPanelZ);
  G4Transform3D fbpcut(G4RotationMatrix(),
                       G4ThreeVector(0.0,
                                     0.5 * (cfg.gc->ProtoTrackerFBPanelCutY -
                                            cfg.gc->ProtoTrackerFBPanelY) +
                                         cfg.gc->ProtoTrackerFBPanelCutYpos,
                                     0.0));
  G4SubtractionSolid *solidProtoTrckFBPanel = new G4SubtractionSolid(
      "solidProtoTrackerContainer", solidProtoTrckFBPanel1,
      solidProtoTrckFBPanelCut, fbpcut);
  G4LogicalVolume *logicProtoTrckFBPanel = new G4LogicalVolume(
      solidProtoTrckFBPanel, protoTrackeBoxMaterial, "logicProtoTrckFBPanel");
  G4double fpypos =
      0.5 * (protoTrckContainerY - cfg.gc->ProtoTrackerFBPanelY) - holderOffset;
  G4double fpzpos =
      0.5 * (cfg.gc->ProtoTrackerFBPanelZ + cfg.gc->ProtoTrackerHoldPanelZ);
  new G4PVPlacement(0, G4ThreeVector(hpxpos, fpypos, fpzpos),
                    logicProtoTrckFBPanel, "ProtoTrckFBPanel",
                    logicProtoTrackerContainer, false, 0, cfg.checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(hpxpos, fpypos, -fpzpos),
                    logicProtoTrckFBPanel, "ProtoTrckFBPanel",
                    logicProtoTrackerContainer, false, 1, cfg.checkOverlaps);

  // Kapton covering the cuts in front and back panels
  G4Material *protoTrackerKaptonMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_KAPTON");
  G4Box *solidProtoTrckKaptonCover =
      new G4Box("solidProtoTrckKaptonCover", cfg.gc->ProtoTrackerFBPanelX / 2.0,
                0.6 * cfg.gc->ProtoTrackerFBPanelCutY,
                cfg.gc->ProtoTrckKaptonCoverZ / 2.0);
  G4LogicalVolume *logicProtoTrckKaptonCover =
      new G4LogicalVolume(solidProtoTrckKaptonCover, protoTrackerKaptonMaterial,
                          "logicProtoTrckKaptonCover");

  G4double kapypos =
      fpypos +
      0.5 * (cfg.gc->ProtoTrackerFBPanelCutY - cfg.gc->ProtoTrackerFBPanelY) +
      cfg.gc->ProtoTrackerFBPanelCutYpos;
  G4double kapzpos = fpzpos + 0.5 * (cfg.gc->ProtoTrackerFBPanelZ +
                                     cfg.gc->ProtoTrckKaptonCoverZ);
  new G4PVPlacement(0, G4ThreeVector(hpxpos, kapypos, kapzpos),
                    logicProtoTrckKaptonCover, "ProtoTrckKaptonCover",
                    logicProtoTrackerContainer, false, 0, cfg.checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(hpxpos, kapypos, -kapzpos),
                    logicProtoTrckKaptonCover, "ProtoTrckKaptonCover",
                    logicProtoTrackerContainer, false, 1, cfg.checkOverlaps);

  // Side panels
  G4Box *solidProtoTrckSidePanel =
      new G4Box("solidProtoTrckSidePanel", cfg.gc->ProtoTrackerSidePanelX / 2.0,
                cfg.gc->ProtoTrackerSidePanelY / 2.0,
                cfg.gc->ProtoTrackerSidePanelZ / 2.0);
  G4LogicalVolume *logicProtoTrckSidePanel =
      new G4LogicalVolume(solidProtoTrckSidePanel, protoTrackeBoxMaterial,
                          "logicProtoTrckSidePanel");
  G4double spxpos = hpxpos + 0.5 * (cfg.gc->ProtoTrackerHoldPanelX +
                                    cfg.gc->ProtoTrackerSidePanelX);
  G4double spypos =
      0.5 * (protoTrckContainerY - cfg.gc->ProtoTrackerSidePanelY) -
      holderOffset;
  new G4PVPlacement(0, G4ThreeVector(spxpos, spypos, 0.0),
                    logicProtoTrckSidePanel, "ProtoTrckSidePanel",
                    logicProtoTrackerContainer, false, 0, cfg.checkOverlaps);
  spxpos = hpxpos - 0.5 * (cfg.gc->ProtoTrackerHoldPanelX +
                           cfg.gc->ProtoTrackerSidePanelX);
  new G4PVPlacement(0, G4ThreeVector(spxpos, spypos, 0.0),
                    logicProtoTrckSidePanel, "ProtoTrckSidePanel",
                    logicProtoTrackerContainer, false, 1, cfg.checkOverlaps);

  // Bottom panel
  G4Box *solidProtoTrckBottomPanel = new G4Box(
      "solidProtoTrckBottomPanel", cfg.gc->ProtoTrackerBottomPanelX / 2.0,
      cfg.gc->ProtoTrackerBottomPanelY / 2.0,
      cfg.gc->ProtoTrackerBottomPanelZ / 2.0);
  G4LogicalVolume *logicProtoTrckBottomPanel =
      new G4LogicalVolume(solidProtoTrckBottomPanel, protoTrackeBoxMaterial,
                          "logicProtoTrckBottomPanel");
  G4double bpxpos = 0.0;
  G4double bpypos =
      0.5 * (protoTrckContainerY - cfg.gc->ProtoTrackerBottomPanelY) -
      cfg.gc->ProtoTrackerFBPanelY + cfg.gc->ProtoTrackerBottomPanelYposShift -
      holderOffset;
  new G4PVPlacement(0, G4ThreeVector(bpxpos, bpypos, 0.0),
                    logicProtoTrckBottomPanel, "ProtoTrckBottomPanel",
                    logicProtoTrackerContainer, false, 0, cfg.checkOverlaps);

  // L shape connector of the bottom panel to front and back
  G4LogicalVolume *logicProtoTrackerLConnector = constructLConnector(cfg);
  G4double lconypos = bpypos +
                      0.5 * (cfg.gc->ProtoTrackerLConnectYZ -
                             cfg.gc->ProtoTrackerBottomPanelY) -
                      cfg.gc->ProtoTrackerLConnectD;
  G4double lconzpos =
      fpzpos +
      0.5 * (cfg.gc->ProtoTrackerFBPanelZ - cfg.gc->ProtoTrackerLConnectYZ) +
      cfg.gc->ProtoTrackerLConnectD;
  new G4PVPlacement(0, G4ThreeVector(hpxpos, lconypos, -lconzpos),
                    logicProtoTrackerLConnector, "ProtoTrackerLConnector",
                    logicProtoTrackerContainer, false, 0, cfg.checkOverlaps);
  new G4PVPlacement(new G4RotationMatrix(G4ThreeVector(0.0, 1.0, 0.0), M_PI),
                    G4ThreeVector(hpxpos, lconypos, lconzpos),
                    logicProtoTrackerLConnector, "ProtoTrackerLConnector",
                    logicProtoTrackerContainer, false, 1, cfg.checkOverlaps);

  // LDO cover box
  G4Box *solidProtoTrckLDOBox1 = new G4Box(
      "solidProtoTrckLDOBox1", cfg.gc->ProtoTrackerLDOBoxX / 2.0,
      cfg.gc->ProtoTrackerLDOBoxY / 2.0, cfg.gc->ProtoTrackerLDOBoxZ / 2.0);
  G4double ldocutx =
      cfg.gc->ProtoTrackerLDOBoxX - 2.0 * cfg.gc->ProtoTrackerLDOBoxD;
  G4double ldocutz =
      cfg.gc->ProtoTrackerLDOBoxZ - 2.0 * cfg.gc->ProtoTrackerLDOBoxD;
  G4Box *solidProtoTrckLDOBoxCut =
      new G4Box("solidProtoTrckLDOBoxCut", ldocutx / 2.0,
                cfg.gc->ProtoTrackerLDOBoxY / 2.0, ldocutz / 2.0);
  G4Transform3D ldotrcut(G4RotationMatrix(),
                         G4ThreeVector(0.0, cfg.gc->ProtoTrackerLDOBoxD, 0.0));
  G4SubtractionSolid *solidProtoTrckLDOBox =
      new G4SubtractionSolid("solidProtoTrckLDOBox", solidProtoTrckLDOBox1,
                             solidProtoTrckLDOBoxCut, ldotrcut);
  G4LogicalVolume *logicProtoTrckLDOBox = new G4LogicalVolume(
      solidProtoTrckLDOBox, protoTrackeBoxMaterial, "logicProtoTrckLDOBox");

  G4double ldoxpos = 0.5 * (protoTrckContainerX - cfg.gc->ProtoTrackerLDOBoxX) -
                     cfg.gc->ProtoTrackerLDOBoxXpos;
  G4double ldoypos = bpypos - 0.5 * (cfg.gc->ProtoTrackerLDOBoxY +
                                     cfg.gc->ProtoTrackerBottomPanelY);
  new G4PVPlacement(0, G4ThreeVector(ldoxpos, ldoypos, 0.0),
                    logicProtoTrckLDOBox, "ProtoTrckLDOBox",
                    logicProtoTrackerContainer, false, 0, cfg.checkOverlaps);
  // LDO PCB
  G4Box *solidProtoTrckLDOPCB =
      new G4Box("solidProtoTrckLDOPCB", ldocutx / 2.0,
                cfg.gc->ProtoTrackerLDOPCBY / 2.0, ldocutz / 2.0);
  G4LogicalVolume *logicProtoTrckLDOPCB = new G4LogicalVolume(
      solidProtoTrckLDOPCB, trackerPCBMaterial, "logicProtoTrckLDOPCB");
  G4double ldopcbypos =
      ldoypos +
      0.5 * (cfg.gc->ProtoTrackerLDOBoxY - cfg.gc->ProtoTrackerLDOPCBY) -
      cfg.gc->ProtoTrackerLDOPCBYpos;
  new G4PVPlacement(0, G4ThreeVector(ldoxpos, ldopcbypos, 0.0),
                    logicProtoTrckLDOPCB, "ProtoTrckLDOPCB",
                    logicProtoTrackerContainer, false, 0, cfg.checkOverlaps);

  // Nine Alpide adapter PCB.
  G4AssemblyVolume *nineChipPcbAssy = constructNineAlpidePCB(cfg);
  G4double ninepcbxpos =
      0.5 * (protoTrckContainerX - cfg.gc->ProtoTrackerNinePcbX);
  G4double ninepcbypos =
      cfg.gc->ProtoTrackerBoxCutY -
      0.5 * (protoTrckContainerY + cfg.gc->ProtoTrackerNinePcbY);
  G4ThreeVector ninechiptr(ninepcbxpos, ninepcbypos, 0.0);
  nineChipPcbAssy->MakeImprint(logicProtoTrackerContainer, ninechiptr, 0, 0,
                               cfg.checkOverlaps);

  // Carrier PCBs and PCIE connctors
  G4double CarrierPcbContY;
  G4LogicalVolume *carrierPcb = constructCarrierPCB(CarrierPcbContY, cfg);
  G4double carierxpos = hpxpos;
  G4double carierypos =
      ninepcbypos + 0.5 * (cfg.gc->ProtoTrackerNinePcbY + CarrierPcbContY);
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
  G4double pcieypos = ninepcbypos + 0.5 * (cfg.gc->ProtoTrackerNinePcbY +
                                           cfg.gc->ProtoTrackerPcieConY);

  std::bitset<9> sensor_pos_mask(std::string("101010101"));

  for (int ii = 0; ii < sensor_pos_mask.size(); ++ii) {
    G4double carierzpos = cfg.gc->ProtoTrackerLayerDZ * (ii - 4.0);
    if (sensor_pos_mask.test(ii)) {
      new G4PVPlacement(0, G4ThreeVector(carierxpos, carierypos, carierzpos),
                        carrierPcb, "ProtoTrckCarrierPCB",
                        logicProtoTrackerContainer, false, ii /*ncarier*/,
                        cfg.checkOverlaps);
      ++ncarier;
    } else {
      new G4PVPlacement(0, G4ThreeVector(carierxpos, pcieypos, carierzpos),
                        logicProtoTrackerPcieCon, "ProtoTrckPCIEConnector",
                        logicProtoTrackerContainer, false, ii /*ii-ncarier*/,
                        cfg.checkOverlaps);
    }
  }

  G4RotationMatrix *tcRotation = new G4RotationMatrix();
  tcRotation->rotateX(cfg.tcRotationAngleX);
  tcRotation->rotateY(cfg.tcRotationAngleY);
  tcRotation->rotateZ(cfg.tcRotationAngleZ);
  G4PVPlacement *physProtoTrackerContainer = new G4PVPlacement(
      tcRotation, G4ThreeVector(cfg.tcCenterX, cfg.tcCenterY, cfg.tcCenterZ),
      logicProtoTrackerContainer, cfg.name, logicParent, false, 0,
      cfg.checkOverlaps);

  // AddSegmentation();

  return physProtoTrackerContainer;
}

G4LogicalVolume *TrackingChamberFactory::constructSensor(const Config &cfg) {
  G4Material *sensorMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(cfg.gc->trackerMaterial);
  G4Box *solidAlpideSensor =
      new G4Box("solidAlpideSensor", cfg.gc->OPPPSensorX / 2.0,
                cfg.gc->OPPPSensorY / 2.0, cfg.gc->OPPPSensorZ / 2.0);
  G4LogicalVolume *logicAlpideSensor = new G4LogicalVolume(
      solidAlpideSensor, sensorMaterial, "logicAlpideSensor");

  G4double sensx = cfg.gc->OPPPSensorNCellX * cfg.gc->OPPPSensorPixelX;
  G4double sensy = cfg.gc->OPPPSensorNCellY * cfg.gc->OPPPSensorPixelY;
  G4Box *solidAlpideSensitive =
      new G4Box("solidAlpideSensitive", sensx / 2.0, sensy / 2.0,
                cfg.gc->OPPPSensorPixelZ / 2.0);
  G4LogicalVolume *logicAlpideSensitive = new G4LogicalVolume(
      solidAlpideSensitive, sensorMaterial, "logicAlpideSensitive");

  new G4PVPlacement(
      0,
      G4ThreeVector(0.0, (cfg.gc->OPPPSensorY - sensy) / 2.0,
                    (cfg.gc->OPPPSensorPixelZ - cfg.gc->OPPPSensorZ) / 2.0),
      logicAlpideSensitive, "OPPPSensitive", logicAlpideSensor, false, 0,
      cfg.checkOverlaps);
  return logicAlpideSensor;
}

G4LogicalVolume *TrackingChamberFactory::constructLConnector(
    const Config &cfg) {
  G4Material *boxMaterial = G4NistManager::Instance()->FindOrBuildMaterial(
      cfg.gc->ProtoTrackerBoxMaterial);

  G4Box *solidProtoTrckLConnect1 =
      new G4Box("solidProtoTrckLConnect1", cfg.gc->ProtoTrackerLConnectX / 2.0,
                cfg.gc->ProtoTrackerLConnectYZ / 2.0,
                cfg.gc->ProtoTrackerLConnectYZ / 2.0);
  G4Box *solidProtoTrckLConnectCut =
      new G4Box("solidProtoTrckLConnectCut", cfg.gc->ProtoTrackerLConnectX,
                cfg.gc->ProtoTrackerLConnectYZ / 2.0,
                cfg.gc->ProtoTrackerLConnectYZ / 2.0);
  G4Transform3D trcut(G4RotationMatrix(),
                      G4ThreeVector(0.0, cfg.gc->ProtoTrackerLConnectD,
                                    cfg.gc->ProtoTrackerLConnectD));
  G4SubtractionSolid *solidProtoTrckLConnect =
      new G4SubtractionSolid("solidProtoTrckLConnect", solidProtoTrckLConnect1,
                             solidProtoTrckLConnectCut, trcut);

  G4LogicalVolume *logicProtoTrckLConnect = new G4LogicalVolume(
      solidProtoTrckLConnect, boxMaterial, "logicProtoTrckLConnect");

  return logicProtoTrckLConnect;
}

G4AssemblyVolume *TrackingChamberFactory::constructNineAlpidePCB(
    const Config &cfg) {
  G4Material *trackerPCBMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          cfg.gc->trackerPCBMaterial);
  G4Material *trackerPowerConMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          cfg.gc->trackerNinePcbConMaterial);

  G4AssemblyVolume *nineChipPcbAssembly = new G4AssemblyVolume();
  // PCB
  G4Box *solidProtoTrck9ChipPcb = new G4Box(
      "solidProtoTrck9ChipPcb", cfg.gc->ProtoTrackerNinePcbX / 2.0,
      cfg.gc->ProtoTrackerNinePcbY / 2.0, cfg.gc->ProtoTrackerNinePcbZ / 2.0);
  G4LogicalVolume *logicProtoTrck9ChipPcb = new G4LogicalVolume(
      solidProtoTrck9ChipPcb, trackerPCBMaterial, "logicProtoTrck9ChipPcb");
  // Power connectors
  G4Box *solidProtoTrck9ChipPcbCon1 = new G4Box(
      "solidProtoTrck9ChipPcbCon1", cfg.gc->ProtoTrackerNinePcbConX / 2.0,
      cfg.gc->ProtoTrackerNinePcbConY / 2.0,
      cfg.gc->ProtoTrackerNinePcbConZ / 2.0);
  G4double concutx =
      cfg.gc->ProtoTrackerNinePcbConX - 2.0 * cfg.gc->ProtoTrackerNinePcbConD;
  G4double concutz =
      cfg.gc->ProtoTrackerNinePcbConZ - 2.0 * cfg.gc->ProtoTrackerNinePcbConD;
  G4Box *solidProtoTrck9ChipPcbConCut =
      new G4Box("solidProtoTrck9ChipPcbConCut", concutx / 2.0,
                cfg.gc->ProtoTrackerNinePcbConY / 2.0, concutz / 2.0);
  G4Transform3D contrcut(
      G4RotationMatrix(),
      G4ThreeVector(0.0, -cfg.gc->ProtoTrackerNinePcbConD, 0.0));
  G4SubtractionSolid *solidProtoTrck9ChipPcbCon = new G4SubtractionSolid(
      "solidProtoTrck9ChipPcbCon", solidProtoTrck9ChipPcbCon1,
      solidProtoTrck9ChipPcbConCut, contrcut);
  G4LogicalVolume *logicProtoTrck9ChipPcbCon =
      new G4LogicalVolume(solidProtoTrck9ChipPcbCon, trackerPowerConMaterial,
                          "logicProtoTrck9ChipPcbCon");

  G4ThreeVector pcbtr(0.0, 0.0, 0.0);
  nineChipPcbAssembly->AddPlacedVolume(logicProtoTrck9ChipPcb, pcbtr, 0);

  G4double contrx =
      0.5 * (cfg.gc->ProtoTrackerNinePcbConX - cfg.gc->ProtoTrackerNinePcbX);
  G4double contry =
      0.5 * (cfg.gc->ProtoTrackerNinePcbConY + cfg.gc->ProtoTrackerNinePcbY);
  G4double contrz =
      0.5 * (cfg.gc->ProtoTrackerNinePcbConZ - cfg.gc->ProtoTrackerNinePcbZ) +
      cfg.gc->ProtoTrackerNinePcbConZpos;
  G4ThreeVector contr(contrx, contry, contrz);
  nineChipPcbAssembly->AddPlacedVolume(logicProtoTrck9ChipPcbCon, contr, 0);
  contr.setZ(-contrz);
  nineChipPcbAssembly->AddPlacedVolume(logicProtoTrck9ChipPcbCon, contr, 0);

  return nineChipPcbAssembly;
}

G4LogicalVolume *TrackingChamberFactory::constructCarrierPCB(
    G4double &carrierPcbContainerY, const Config &cfg) {
  G4Material *protoTrackerContainerMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          cfg.gc->trackerContainerMaterial);
  G4Material *carierPCBMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          cfg.gc->trackerPCBMaterial);
  G4Material *trackerPcieConMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          cfg.gc->trackerNinePcbConMaterial);
  G4Material *trackerPcieHoldMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          cfg.gc->ProtoTrackerPEEKMaterial);

  G4double protoTrckContainerY =
      cfg.gc->ProtoTrackerFBPanelY + cfg.gc->ProtoTrackerBottomPanelY -
      cfg.gc->ProtoTrackerBottomPanelYposShift + cfg.gc->ProtoTrackerLDOBoxY;

  G4double ccontx = cfg.gc->ProtoTrkCarrierPcbX;
  G4double cconty = protoTrckContainerY - cfg.gc->ProtoTrackerBoxCutY -
                    cfg.gc->ProtoTrackerHoldPanelY;
  G4double ccontz = cfg.gc->ProtoTrackerPcieConZ;
  G4Box *solidCarrierPcbContainer = new G4Box(
      "solidCarrierPcbContainer", ccontx / 2.0, cconty / 2.0, ccontz / 2.0);
  G4LogicalVolume *logicCarrierPcbContainer = new G4LogicalVolume(
      solidCarrierPcbContainer, protoTrackerContainerMaterial,
      "logicCarrierPcbContainer");

  // Carrier PCB
  G4Box *solidCarrierPcb1 = new G4Box(
      "solidCarrierPcb1", cfg.gc->ProtoTrkCarrierPcbX / 2.0,
      cfg.gc->ProtoTrkCarrierPcbY / 2.0, cfg.gc->ProtoTrkCarrierPcbZ / 2.0);
  G4Box *solidCarrPcbCut1 =
      new G4Box("solidCarrPcbCut1", cfg.gc->ProtoTrkCarrierPcbBCutX,
                cfg.gc->ProtoTrkCarrierPcbBCutY, cfg.gc->ProtoTrkCarrierPcbZ);

  G4double y1 =
      cfg.gc->ProtoTrkCarrierPcbCutY - cfg.gc->ProtoTrkCarrierPcbACutXY;
  G4double x2 = cfg.gc->ProtoTrkCarrierPcbACutXY;
  G4double y2 = cfg.gc->ProtoTrkCarrierPcbCutY;
  G4double x3 =
      cfg.gc->ProtoTrkCarrierPcbCutX - cfg.gc->ProtoTrkCarrierPcbACutXY;
  G4double x4 = cfg.gc->ProtoTrkCarrierPcbCutX;
  std::vector<G4TwoVector> cutvtx{G4TwoVector(0.0, 0.0), G4TwoVector(0.0, y1),
                                  G4TwoVector(x2, y2),   G4TwoVector(x3, y2),
                                  G4TwoVector(x4, y1),   G4TwoVector(x4, 0.0)};

  G4ExtrudedSolid *solidCarrPcbCut2 = new G4ExtrudedSolid(
      "solidCarrPcbCut2", cutvtx, cfg.gc->ProtoTrkCarrierPcbZ,
      G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);

  G4Transform3D trmcut(G4RotationMatrix(),
                       G4ThreeVector(cfg.gc->ProtoTrkCarrierPcbCutXpos -
                                         cfg.gc->ProtoTrkCarrierPcbX / 2.0,
                                     cfg.gc->ProtoTrkCarrierPcbY / 2.0 -
                                         cfg.gc->ProtoTrkCarrierPcbCutYpos -
                                         cfg.gc->ProtoTrkCarrierPcbCutY,
                                     0.0));
  G4SubtractionSolid *solidCarrierPcb2 = new G4SubtractionSolid(
      "solidCarrierPcb2", solidCarrierPcb1, solidCarrPcbCut2, trmcut);

  G4Transform3D trmcut1(G4RotationMatrix(),
                        G4ThreeVector(-cfg.gc->ProtoTrkCarrierPcbX / 2.0,
                                      -cfg.gc->ProtoTrkCarrierPcbY / 2.0, 0.0));
  G4SubtractionSolid *solidCarrierPcb3 = new G4SubtractionSolid(
      "solidCarrierPcb3", solidCarrierPcb2, solidCarrPcbCut1, trmcut1);

  G4Transform3D trmcut2(G4RotationMatrix(),
                        G4ThreeVector(cfg.gc->ProtoTrkCarrierPcbX / 2.0,
                                      -cfg.gc->ProtoTrkCarrierPcbY / 2.0, 0.0));
  G4SubtractionSolid *solidCarrierPcb = new G4SubtractionSolid(
      "solidCarrierPcb", solidCarrierPcb3, solidCarrPcbCut1, trmcut2);

  G4LogicalVolume *logicCarrierPcb = new G4LogicalVolume(
      solidCarrierPcb, carierPCBMaterial, "logicCarrierPcb");

  G4double crpcbypos = 0.5 * (cfg.gc->ProtoTrkCarrierPcbY - cconty) +
                       cfg.gc->ProtoTrackerPcieConGapY;
  new G4PVPlacement(0, G4ThreeVector(0.0, crpcbypos, 0.0), logicCarrierPcb,
                    "AlpideCarrierPCB", logicCarrierPcbContainer, false, 0,
                    cfg.checkOverlaps);

  // Alpide sensor
  G4LogicalVolume *logicAlpideSensor = constructSensor(cfg);

  G4double sensypos =
      crpcbypos + 0.5 * (cfg.gc->ProtoTrkCarrierPcbY - cfg.gc->OPPPSensorY) -
      cfg.gc->ProtoTrkCarrierPcbCutYpos - cfg.gc->ProtoTrkCarrierSensorOffset;
  G4double senszpos =
      -0.5 * (cfg.gc->ProtoTrkCarrierPcbZ + cfg.gc->OPPPSensorZ);
  G4PVPlacement *physAlpideSensor = new G4PVPlacement(
      0, G4ThreeVector(0.0, sensypos, senszpos), logicAlpideSensor,
      "AlpideSensor", logicCarrierPcbContainer, false, 0, cfg.checkOverlaps);

  // PCIE connector
  G4Box *solidProtoTrackerPcieCon1 = new G4Box(
      "solidProtoTrackerPcieCon1", cfg.gc->ProtoTrackerPcieConX / 2.0,
      cfg.gc->ProtoTrackerPcieConY / 2.0, cfg.gc->ProtoTrackerPcieConZ / 2.0);
  G4double pcieccutx =
      cfg.gc->ProtoTrkCarrierPcbX - 2.0 * cfg.gc->ProtoTrkCarrierPcbBCutX;
  G4Box *solidProtoTrackerPcieConCut = new G4Box(
      "solidProtoTrackerPcieConCut", pcieccutx / 2.0,
      cfg.gc->ProtoTrackerPcieConY, cfg.gc->ProtoTrkCarrierPcbZ / 2.0);
  G4Transform3D pcietrcut(G4RotationMatrix(), G4ThreeVector(0.0, 0.0, 0.0));
  G4SubtractionSolid *solidProtoTrackerPcieCon = new G4SubtractionSolid(
      "solidProtoTrackerPcieCon", solidProtoTrackerPcieCon1,
      solidProtoTrackerPcieConCut, pcietrcut);
  G4LogicalVolume *logicProtoTrackerPcieCon =
      new G4LogicalVolume(solidProtoTrackerPcieCon, trackerPcieConMaterial,
                          "logicProtoTrackerPcieCon");
  G4double pcieypos = 0.5 * (cfg.gc->ProtoTrackerPcieConY - cconty);
  new G4PVPlacement(0, G4ThreeVector(0.0, pcieypos, 0.0),
                    logicProtoTrackerPcieCon, "ProtoTrackerPcieConnector",
                    logicCarrierPcbContainer, false, 0, cfg.checkOverlaps);

  // Carrier PCB PEEK holder
  G4double carrierHolderX = cfg.gc->ProtoTrkCarrierPcbX;
  G4double carrierHolderY = cconty - cfg.gc->ProtoTrackerPcieConY;
  G4double carrierHolderZ =
      std::min(0.5 * (ccontz - cfg.gc->ProtoTrkCarrierPcbZ),
               cfg.gc->ProtoTrackerCarrierHoldZ);

  G4Box *solidProtoTrackerCarrierHold1 =
      new G4Box("solidProtoTrackerCarrierHold1", carrierHolderX / 2.0,
                carrierHolderY / 2.0, carrierHolderZ / 2.0);

  G4double carrierHolderCutX =
      carrierHolderX - 2.0 * cfg.gc->ProtoTrackerCarrierHoldD;
  G4Box *solidProtoTrackerCarrierHoldCut =
      new G4Box("solidProtoTrackerCarrierHoldCut", carrierHolderCutX / 2.0,
                carrierHolderY / 2.0, carrierHolderZ);
  G4double chCutYpos = cconty - cfg.gc->ProtoTrackerPcieConGapY -
                       cfg.gc->ProtoTrkCarrierPcbY +
                       cfg.gc->ProtoTrackerCarrierHoldD;
  G4Transform3D chtrcut(G4RotationMatrix(),
                        G4ThreeVector(0.0, -chCutYpos, 0.0));
  G4SubtractionSolid *solidProtoTrackerCarrierHold = new G4SubtractionSolid(
      "solidProtoTrackerCarrierHold", solidProtoTrackerCarrierHold1,
      solidProtoTrackerCarrierHoldCut, chtrcut);
  G4LogicalVolume *logicProtoTrackerCarrierHold =
      new G4LogicalVolume(solidProtoTrackerCarrierHold, trackerPcieHoldMaterial,
                          "logicProtoTrackerCarrierHold");

  G4double chypos = 0.5 * (cconty - carrierHolderY);
  G4double chzpos = 0.5 * (cfg.gc->ProtoTrkCarrierPcbZ + carrierHolderZ);
  new G4PVPlacement(0, G4ThreeVector(0.0, chypos, chzpos),
                    logicProtoTrackerCarrierHold, "ProtoTrackerCarrierHold",
                    logicCarrierPcbContainer, false, 0, cfg.checkOverlaps);

  carrierPcbContainerY = cconty;
  return logicCarrierPcbContainer;
}
