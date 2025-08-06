#include "BreadboardFactory.hh"
#include "BreadboardMountFactory.hh"
#include "DetectorConstruction.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalVolumesSearchScene.hh"
#include "G4Region.hh"
#include "G4RotationMatrix.hh"
#include "G4SDManager.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4TransportationManager.hh"
#include "G4Tubs.hh"
#include "G4VPhysicalVolume.hh"
#include "GeometryPlacement.hh"
#include "MaterialFactory.hh"
#include "SamplingVolume.hh"
#include "TrackingChamberFactory.hh"
#include "VacuumChamberFactory.hh"
#include "WendellDipoleFactory.hh"

namespace gc = GeometryConstants;

DetectorConstruction::DetectorConstruction(double alongWindowTranslation1,
                                           double alongWindowTranslation2,
                                           double alpha)
    : translation1(alongWindowTranslation1),
      translation2(alongWindowTranslation2),
      angle(alpha),
      G4VUserDetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  checkOverlaps = true;
  MaterialFactory::instance()->constuctMaterial();

  // ---------------------------------------------------
  // World volume construction
  G4Box *solidWorld =
      new G4Box("World", gc::worldHalfX, gc::worldHalfY, gc::worldHalfZ);

  G4Material *worldMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  G4LogicalVolume *logicWorld =
      new G4LogicalVolume(solidWorld, worldMaterial, "World");
  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr,
                        false, 0, checkOverlaps);

  // ---------------------------------------------------
  // Vacuum chamber construciton

  VacuumChamberFactory::Config vcFactoryCfg{
      .name = gc::GeometryPlacement::instance()->vcName,

      .vcCenterX = gc::GeometryPlacement::instance()->vcCenterX,
      .vcCenterY = gc::GeometryPlacement::instance()->vcCenterY,
      .vcCenterZ = gc::GeometryPlacement::instance()->vcCenterZ,

      .vcRotationAngleX = gc::GeometryPlacement::instance()->vcRotationAngleX,
      .vcRotationAngleY = gc::GeometryPlacement::instance()->vcRotationAngleY,
      .vcRotationAngleZ = gc::GeometryPlacement::instance()->vcRotationAngleZ,

      .vcc = gc::VacuumChamber::instance(),

      .checkOverlaps = true};

  VacuumChamberFactory vcFactory;
  G4VPhysicalVolume *physVacuumChamber =
      vcFactory.construct(logicWorld, vcFactoryCfg);

  // ---------------------------------------------------
  // Dipole construciton

  WendellDipoleFactory::Config wdFactoryCfg{
      .name = gc::GeometryPlacement::instance()->dipoleName,

      .wdCenterX = gc::GeometryPlacement::instance()->wdCenterX,
      .wdCenterY = gc::GeometryPlacement::instance()->wdCenterY,
      .wdCenterZ = gc::GeometryPlacement::instance()->wdCenterZ,

      .wdRotationAngleX = gc::GeometryPlacement::instance()->wdRotationAngleX,
      .wdRotationAngleY = gc::GeometryPlacement::instance()->wdRotationAngleY,
      .wdRotationAngleZ =
          gc::GeometryPlacement::instance()->wdRotationAngleZ - angle,

      .angle = angle,

      .wdc = gc::WendellDipole::instance(),

      .checkOverlaps = true};

  WendellDipoleFactory wdFactory;
  G4VPhysicalVolume *physWendellDipole =
      wdFactory.construct(logicWorld, wdFactoryCfg);

  G4PhysicalVolumeModel wdSearchModel(physWendellDipole);
  G4ModelingParameters wdmp;
  wdSearchModel.SetModelingParameters(&wdmp);
  G4PhysicalVolumesSearchScene wdSearchScene(&wdSearchModel, "MagFieldVolume");
  wdSearchModel.DescribeYourselfTo(wdSearchScene);

  const auto &magFieldVolumeObject = wdSearchScene.GetFindings().at(0);
  G4ThreeVector magFieldVolumeTranslation =
      magFieldVolumeObject.fFoundObjectTransformation.getTranslation();
  physWendellDipole->SetTranslation(
      G4ThreeVector(magFieldVolumeTranslation.x(),
                    -magFieldVolumeTranslation.z(),
                    physWendellDipole->GetTranslation().z()) +
      G4ThreeVector(translation1, translation1, 0) +
      G4ThreeVector((physWendellDipole->GetTranslation().z() -
                     gc::VacuumChamber::instance()->vcRad) /
                        std::tan(M_PI_2 - angle),
                    0, 0));

  // ---------------------------------------------------
  // First tracking chamber construction

  TrackingChamberFactory tcFactory;

  TrackingChamberFactory::Config tc1FactoryCfg{
      .name = gc::GeometryPlacement::instance()->tc1Name,

      .tcCenterX = gc::GeometryPlacement::instance()->tc1CenterX,
      .tcCenterY = gc::GeometryPlacement::instance()->tc1CenterY,
      .tcCenterZ = gc::GeometryPlacement::instance()->tc1CenterZ,

      .tcRotationAngleX = gc::GeometryPlacement::instance()->tc1RotationAngleX,
      .tcRotationAngleY =
          gc::GeometryPlacement::instance()->tc1RotationAngleY - angle,
      .tcRotationAngleZ = gc::GeometryPlacement::instance()->tc1RotationAngleZ,

      .tcc = gc::TrackingChamber::instance(),

      .checkOverlaps = true};

  G4VPhysicalVolume *physTrackingChamber1 =
      tcFactory.construct(logicWorld, tc1FactoryCfg);

  G4PhysicalVolumeModel tc1SearchModel(physTrackingChamber1);
  G4ModelingParameters tc1mp;
  tc1SearchModel.SetModelingParameters(&tc1mp);
  G4PhysicalVolumesSearchScene tc1SearchScene(&tc1SearchModel, "OPPPSensitive");
  tc1SearchModel.DescribeYourselfTo(tc1SearchScene);

  const auto &opppObject1 = tc1SearchScene.GetFindings().at(0);
  G4ThreeVector opppSensitiveTranslation1 =
      opppObject1.fFoundObjectTransformation.getTranslation();

  G4RotationMatrix rotm1;
  rotm1.rotateX(gc::GeometryPlacement::instance()->tc1RotationAngleX);
  rotm1.rotateY(gc::GeometryPlacement::instance()->tc1RotationAngleY);
  rotm1.rotateZ(gc::GeometryPlacement::instance()->tc1RotationAngleZ);

  opppSensitiveTranslation1 = rotm1 * opppSensitiveTranslation1;
  physTrackingChamber1->SetTranslation(
      G4ThreeVector(opppSensitiveTranslation1.x(),
                    opppSensitiveTranslation1.y(),
                    physTrackingChamber1->GetTranslation().z()) +
      G4ThreeVector(translation1, translation1, 0) +
      G4ThreeVector((physTrackingChamber1->GetTranslation().z() -
                     gc::VacuumChamber::instance()->vcRad) /
                        std::tan(M_PI_2 - angle),
                    0, 0));

  // ---------------------------------------------------
  // Second tracking chamber construction

  TrackingChamberFactory::Config tc2FactoryCfg{
      .name = gc::GeometryPlacement::instance()->tc2Name,

      .tcCenterX = gc::GeometryPlacement::instance()->tc2CenterX,
      .tcCenterY = gc::GeometryPlacement::instance()->tc2CenterY,
      .tcCenterZ = gc::GeometryPlacement::instance()->tc2CenterZ,

      .tcRotationAngleX = gc::GeometryPlacement::instance()->tc2RotationAngleX,
      .tcRotationAngleY =
          gc::GeometryPlacement::instance()->tc2RotationAngleY - angle,
      .tcRotationAngleZ = gc::GeometryPlacement::instance()->tc2RotationAngleZ,

      .tcc = gc::TrackingChamber::instance(),

      .checkOverlaps = true};

  G4VPhysicalVolume *physTrackingChamber2 =
      tcFactory.construct(logicWorld, tc2FactoryCfg);

  G4PhysicalVolumeModel tc2SearchModel(physTrackingChamber2);
  G4ModelingParameters tc2mp;
  tc2SearchModel.SetModelingParameters(&tc2mp);
  G4PhysicalVolumesSearchScene tc2SearchScene(&tc2SearchModel, "OPPPSensitive");
  tc2SearchModel.DescribeYourselfTo(tc2SearchScene);

  const auto &opppObject2 = tc2SearchScene.GetFindings().at(0);
  G4ThreeVector opppSensitiveTranslation2 =
      opppObject2.fFoundObjectTransformation.getTranslation();

  G4RotationMatrix rotm2;
  rotm2.rotateX(gc::GeometryPlacement::instance()->tc2RotationAngleX);
  rotm2.rotateY(gc::GeometryPlacement::instance()->tc2RotationAngleY);
  rotm2.rotateZ(gc::GeometryPlacement::instance()->tc2RotationAngleZ);

  opppSensitiveTranslation2 = rotm1 * opppSensitiveTranslation2;
  physTrackingChamber2->SetTranslation(
      G4ThreeVector(opppSensitiveTranslation2.x(),
                    opppSensitiveTranslation2.y(),
                    physTrackingChamber2->GetTranslation().z()) +
      G4ThreeVector(translation1, translation1 + translation2, 0) +
      G4ThreeVector((physTrackingChamber2->GetTranslation().z() -
                     gc::VacuumChamber::instance()->vcRad) /
                        std::tan(M_PI_2 - angle),
                    0, 0));

  // ---------------------------------------------------
  // Breadboard table construction

  // BreadboardFactory bbFactory;

  // BreadboardFactory::Config bb1FactoryCfg{
  //     .name = gc::GeometryPlacement::instance()->bb1Name,

  //     .bbCenterX = physWendellDipole->GetTranslation().x() +
  //                  (gc::WendellDipole::instance()->wdHalfY +
  //                   gc::Breadboard::instance()->bbHalfZ) *
  //                      std::cos(M_PI_4),
  //     // 10 * mm * std::cos(M_PI_4),
  //     .bbCenterY = physWendellDipole->GetTranslation().y() -
  //                  (gc::WendellDipole::instance()->wdHalfY +
  //                   gc::Breadboard::instance()->bbHalfZ) *
  //                      std::cos(M_PI_4),
  //     // 10 * mm * std::cos(M_PI_4),
  //     .bbCenterZ = physWendellDipole->GetTranslation().z(),

  //     .bbRotationAngleX =
  //     gc::GeometryPlacement::instance()->bb1RotationAngleX, .bbRotationAngleY
  //     = gc::GeometryPlacement::instance()->bb1RotationAngleY,
  //     .bbRotationAngleZ =
  //     gc::GeometryPlacement::instance()->bb1RotationAngleZ,

  //     .bbc = gc::Breadboard::instance(),

  //     .checkOverlaps = true};

  // G4VPhysicalVolume *physBreadboard1 =
  //     bbFactory.construct(logicWorld, bb1FactoryCfg);

  // ---------------------------------------------------
  // Breadboard table construction

  // BreadboardMountFactory bmFactory;

  // BreadboardMountFactory::Config bm1FactoryCfg{
  //     .name = gc::GeometryPlacement::instance()->bm1Name,

  //     .bmCenterX = physBreadboard1->GetTranslation().x() +
  //                  (gc::Breadboard::instance()->bbHalfZ +
  //                   2 * gc::BreadboardMount::instance()->bmSideFaceHalfLength
  //                   *
  //                       std::cos(M_PI_4)) *
  //                      std::cos(M_PI_4),
  //     .bmCenterY = physBreadboard1->GetTranslation().y() -
  //                  (gc::Breadboard::instance()->bbHalfZ +
  //                   2 * gc::BreadboardMount::instance()->bmSideFaceHalfLength
  //                   *
  //                       std::cos(M_PI_4)) *
  //                      std::cos(M_PI_4),
  //     .bmCenterZ = physBreadboard1->GetTranslation().z() -
  //                  gc::Breadboard::instance()->bbHalfY +
  //                  gc::BreadboardMount::instance()->bmHalfY + 2 * cm,

  //     .bmRotationAngleX =
  //     gc::GeometryPlacement::instance()->bmRotationAngleX, .bmRotationAngleY
  //     = gc::GeometryPlacement::instance()->bmRotationAngleY,
  //     .bmRotationAngleZ =
  //     gc::GeometryPlacement::instance()->bmRotationAngleZ,

  //     .bmc = gc::BreadboardMount::instance(),

  //     .checkOverlaps = true};

  // G4VPhysicalVolume *physBreadboardMount1 =
  //     bmFactory.construct(logicWorld, bm1FactoryCfg);

  // BreadboardMountFactory::Config bm2FactoryCfg{
  //     .name = gc::GeometryPlacement::instance()->bm2Name,

  //     .bmCenterX = physBreadboard1->GetTranslation().x() +
  //                  (gc::Breadboard::instance()->bbHalfZ +
  //                   2 * gc::BreadboardMount::instance()->bmSideFaceHalfLength
  //                   *
  //                       std::cos(M_PI_4)) *
  //                      std::cos(M_PI_4),
  //     .bmCenterY = physBreadboard1->GetTranslation().y() -
  //                  (gc::Breadboard::instance()->bbHalfZ +
  //                   2 * gc::BreadboardMount::instance()->bmSideFaceHalfLength
  //                   *
  //                       std::cos(M_PI_4)) *
  //                      std::cos(M_PI_4),
  //     .bmCenterZ = physBreadboard1->GetTranslation().z() +
  //                  gc::Breadboard::instance()->bbHalfY -
  //                  gc::BreadboardMount::instance()->bmHalfY - 2 * cm,

  //     .bmRotationAngleX =
  //     gc::GeometryPlacement::instance()->bmRotationAngleX, .bmRotationAngleY
  //     = gc::GeometryPlacement::instance()->bmRotationAngleY,
  //     .bmRotationAngleZ =
  //     gc::GeometryPlacement::instance()->bmRotationAngleZ,

  //     .bmc = gc::BreadboardMount::instance(),

  //     .checkOverlaps = true};

  // G4VPhysicalVolume *physBreadboardMount2 =
  //     bmFactory.construct(logicWorld, bm2FactoryCfg);

  // BreadboardFactory::Config bb2FactoryCfg{
  //     .name = gc::GeometryPlacement::instance()->bb2Name,

  //     .bbCenterX = physBreadboardMount1->GetTranslation().x(),
  //     .bbCenterY = physBreadboardMount1->GetTranslation().y() -
  //                  gc::BreadboardMount::instance()->bmHalfX -
  //                  2 * gc::BreadboardMount::instance()->bmSpacerHalfZ -
  //                  gc::Breadboard::instance()->bbHalfZ,
  //     .bbCenterZ = physBreadboard1->GetTranslation().z(),

  //     .bbRotationAngleX =
  //     gc::GeometryPlacement::instance()->bb2RotationAngleX, .bbRotationAngleY
  //     = gc::GeometryPlacement::instance()->bb2RotationAngleY,
  //     .bbRotationAngleZ =
  //     gc::GeometryPlacement::instance()->bb2RotationAngleZ,

  //     .bbc = gc::Breadboard::instance(),

  //     .checkOverlaps = true};

  // G4VPhysicalVolume *physBreadboard2 =
  //     bbFactory.construct(logicWorld, bb2FactoryCfg);

  // ---------------------------------------------------
  // Temp sampling plane

  G4Tubs *solidVcExitSampling = new G4Tubs(
      "VcExitSampling", 0, gc::VacuumChamber::instance()->vcFlangeRad, 2 * mm,
      0, 2 * M_PI);
  G4LogicalVolume *logicVcExitSampling = new G4LogicalVolume(
      solidVcExitSampling,
      G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"),
      "VcExitSampling");
  G4RotationMatrix *vcExitSamplingRotation = new G4RotationMatrix();
  vcExitSamplingRotation->rotateX(M_PI_2);
  G4VPhysicalVolume *physVcExitSampling = new G4PVPlacement(
      vcExitSamplingRotation,
      (*vcExitSamplingRotation) *
          G4ThreeVector(0, 0,
                        gc::VacuumChamber::instance()->vcRad -
                            2 * gc::VacuumChamber::instance()->vcFlangeHalfZ -
                            2 * mm),
      logicVcExitSampling, "VcExitSampling",
      physVacuumChamber->GetLogicalVolume(), false, 0, true);

  return physWorld;
}

void DetectorConstruction::ConstructSDandField() {
  G4String senstitiveName = "/logicAlpideSensitive";
  auto samplingVolume = new SamplingVolume(senstitiveName, "HitsCollection",
                                           "ProtoTrckCarrierPCB");
  G4SDManager::GetSDMpointer()->AddNewDetector(samplingVolume);
  SetSensitiveDetector("logicAlpideSensitive", samplingVolume, true);
  SetSensitiveDetector("VcExitSampling", samplingVolume, true);
}
