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
#include "G4VPhysicalVolume.hh"
#include "GeometryConstants.hh"
#include "MaterialFactory.hh"
#include "SamplingVolume.hh"
#include "TrackingChamberFactory.hh"
#include "VacuumChamberFactory.hh"
#include "WendellDipoleFactory.hh"

DetectorConstruction::DetectorConstruction(double alongSlitTranslation,
                                           double verticalStagger)
    : translation(alongSlitTranslation),
      stagger(verticalStagger),
      G4VUserDetectorConstruction() {
  const GeometryConstants &gc = *GeometryConstants::instance();
  double setupCenter = (gc.tc1CenterZ + gc.wdCenterZ + gc.tc2CenterZ) / 3.0;
  angle = std::asin(translation / setupCenter);
}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  checkOverlaps = true;
  MaterialFactory::instance()->constuctMaterial();

  const GeometryConstants &gc = *GeometryConstants::instance();
  G4ThreeVector setupTranslation(
      0, 0, (gc.tc1CenterZ + gc.wdCenterZ + gc.tc2CenterZ) / 3.0);
  G4Rotate3D setupRotation(angle, G4ThreeVector(0, 1, 0));

  // ---------------------------------------------------
  // World volume construction
  G4Box *solidWorld =
      new G4Box("World", gc.worldHalfX, gc.worldHalfY, gc.worldHalfZ);

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
      .name = gc.vcName,

      .vcCenterX = gc.vcCenterX,
      .vcCenterY = gc.vcCenterY,
      .vcCenterZ = gc.vcCenterZ,

      .vcRotationAngleX = gc.vcRotationAngleX,
      .vcRotationAngleY = gc.vcRotationAngleY,
      .vcRotationAngleZ = gc.vcRotationAngleZ,

      .gc = GeometryConstants::instance(),

      .checkOverlaps = true};

  VacuumChamberFactory vcFactory;
  G4VPhysicalVolume *physVacuumChamber =
      vcFactory.construct(logicWorld, vcFactoryCfg);

  // ---------------------------------------------------
  // Dipole construciton

  WendellDipoleFactory::Config wdFactoryCfg{
      .name = gc.dipoleName,

      .wdCenterX = gc.wdCenterX,
      .wdCenterY = gc.wdCenterY,
      .wdCenterZ = gc.wdCenterZ,

      .wdRotationAngleX = gc.wdRotationAngleX,
      .wdRotationAngleY = gc.wdRotationAngleY,
      .wdRotationAngleZ = gc.wdRotationAngleZ,

      .angle = angle,

      .gc = GeometryConstants::instance(),

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

  G4ThreeVector wdTranslation =
      G4ThreeVector(magFieldVolumeTranslation.x(),
                    -magFieldVolumeTranslation.z(),
                    physWendellDipole->GetTranslation().z()) +
      G4ThreeVector(0, translation, 0);
  physWendellDipole->GetRotation()->rotate(angle, G4ThreeVector(0, 0, -1));
  physWendellDipole->SetTranslation(setupRotation.getRotation() *
                                    wdTranslation);

  // ---------------------------------------------------
  // First tracking chamber construction

  TrackingChamberFactory tcFactory;

  TrackingChamberFactory::Config tc1FactoryCfg{
      .name = gc.tc1Name,

      .tcCenterX = gc.tc1CenterX,
      .tcCenterY = gc.tc1CenterY,
      .tcCenterZ = gc.tc1CenterZ,

      .tcRotationAngleX = gc.tc1RotationAngleX,
      .tcRotationAngleY = gc.tc1RotationAngleY,
      .tcRotationAngleZ = gc.tc1RotationAngleZ,

      .gc = GeometryConstants::instance(),

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
  rotm1.rotateX(gc.tc1RotationAngleX);
  rotm1.rotateY(gc.tc1RotationAngleY);
  rotm1.rotateZ(gc.tc1RotationAngleZ);

  opppSensitiveTranslation1 = rotm1 * opppSensitiveTranslation1;

  G4ThreeVector tc1Translation =
      G4ThreeVector(opppSensitiveTranslation1.x(),
                    opppSensitiveTranslation1.y(),
                    physTrackingChamber1->GetTranslation().z()) +
      G4ThreeVector(0, translation, 0);
  physTrackingChamber1->GetRotation()->rotate(angle, G4ThreeVector(1, 0, 0));
  physTrackingChamber1->SetTranslation(setupRotation.getRotation() *
                                       tc1Translation);

  // ---------------------------------------------------
  // Second tracking chamber construction

  TrackingChamberFactory::Config tc2FactoryCfg{
      .name = gc.tc2Name,

      .tcCenterX = gc.tc2CenterX,
      .tcCenterY = gc.tc2CenterY,
      .tcCenterZ = gc.tc2CenterZ,

      .tcRotationAngleX = gc.tc2RotationAngleX,
      .tcRotationAngleY = gc.tc2RotationAngleY,
      .tcRotationAngleZ = gc.tc2RotationAngleZ,

      .gc = GeometryConstants::instance(),

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
  rotm2.rotateX(gc.tc2RotationAngleX);
  rotm2.rotateY(gc.tc2RotationAngleY);
  rotm2.rotateZ(gc.tc2RotationAngleZ);

  opppSensitiveTranslation2 = rotm1 * opppSensitiveTranslation2;

  G4ThreeVector tc2Translation =
      G4ThreeVector(opppSensitiveTranslation2.x(),
                    opppSensitiveTranslation2.y(),
                    physTrackingChamber2->GetTranslation().z()) +
      G4ThreeVector(0, translation + stagger, 0);
  physTrackingChamber2->GetRotation()->rotate(angle, G4ThreeVector(1, 0, 0));
  physTrackingChamber2->SetTranslation(setupRotation.getRotation() *
                                       tc2Translation);

  return physWorld;
}

void DetectorConstruction::ConstructSDandField() {
  G4String senstitiveName = "/logicAlpideSensitive";
  auto samplingVolume = new SamplingVolume(senstitiveName, "HitsCollection",
                                           "ProtoTrckCarrierPCB");
  G4SDManager::GetSDMpointer()->AddNewDetector(samplingVolume);
  SetSensitiveDetector("logicAlpideSensitive", samplingVolume, true);
}
