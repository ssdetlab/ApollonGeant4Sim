#include <cstddef>
#include <string>

#include "BreadboardFactory.hh"
#include "DetectorConstruction.hh"
#include "G4Box.hh"
#include "G4FieldManager.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalVolumesSearchScene.hh"
#include "G4Region.hh"
#include "G4RotationMatrix.hh"
#include "G4SDManager.hh"
#include "G4Sphere.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4TransportationManager.hh"
#include "G4Tubs.hh"
#include "G4UniformMagField.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"
#include "GeometryPlacement.hh"
#include "MaterialFactory.hh"
#include "SamplingVolume.hh"
#include "TrackingChamberFactory.hh"
#include "VacuumChamberFactory.hh"
#include "WendellDipoleFactory.hh"

namespace gc = GeometryConstants;

DetectorConstruction::DetectorConstruction(double alongWindowTranslation1,
                                           double alongWindowTranslation2)
    : translation1(alongWindowTranslation1),
      translation2(alongWindowTranslation2),
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
      .vcCenterX = gc::GeometryPlacement::instance()->vcCenterX,
      .vcCenterY = gc::GeometryPlacement::instance()->vcCenterY,
      .vcCenterZ = gc::GeometryPlacement::instance()->vcCenterZ,

      .vcRotationAxis = gc::GeometryPlacement::instance()->vcRotationAxis,
      .vcRotationAngle = gc::GeometryPlacement::instance()->vcRotationAngle,

      .vcc = gc::VacuumChamber::instance(),

      .checkOverlaps = true};

  VacuumChamberFactory vcFactory(vcFactoryCfg);
  G4VPhysicalVolume *physVacuumChamber = vcFactory.construct(
      logicWorld, gc::GeometryPlacement::instance()->vcName);

  // ---------------------------------------------------
  // Dipole construciton

  WendellDipoleFactory::Config wdFactoryCfg{
      .dipoleCenterX = gc::GeometryPlacement::instance()->wdCenterX,
      .dipoleCenterY = gc::GeometryPlacement::instance()->wdCenterY,
      .dipoleCenterZ = gc::GeometryPlacement::instance()->wdCenterZ,

      .dipoleRotationAngle = gc::GeometryPlacement::instance()->wdRotationAngle,
      .dipoleRotationAxis = gc::GeometryPlacement::instance()->wdRotationAxis,

      .wdc = gc::WendellDipole::instance(),

      .checkOverlaps = true};

  WendellDipoleFactory wdFactory(wdFactoryCfg);
  G4VPhysicalVolume *physWendellDipole = wdFactory.construct(
      logicWorld, gc::GeometryPlacement::instance()->dipoleName);

  G4PhysicalVolumeModel wdSearchModel(physWendellDipole);
  G4ModelingParameters wdmp;
  wdSearchModel.SetModelingParameters(&wdmp);
  G4PhysicalVolumesSearchScene wdSearchScene(&wdSearchModel, "MagFieldVolume");
  wdSearchModel.DescribeYourselfTo(wdSearchScene);

  const auto &magFieldVolumeObject = wdSearchScene.GetFindings().at(0);
  G4ThreeVector magFieldVolumeTranslation =
      magFieldVolumeObject.fFoundObjectTransformation.getTranslation();
  G4RotationMatrix magFieldVolumeRotation =
      magFieldVolumeObject.fFoundObjectTransformation.getRotation();

  G4RotationMatrix rotm;
  rotm.rotateZ(M_PI_4);
  magFieldVolumeTranslation =
      rotm * *physWendellDipole->GetRotation() * magFieldVolumeTranslation;
  physWendellDipole->SetTranslation(
      G4ThreeVector(-magFieldVolumeTranslation.x(),
                    -magFieldVolumeTranslation.y(),
                    physWendellDipole->GetTranslation().z()) +
      G4ThreeVector(translation1, translation1, 0));

  // ---------------------------------------------------
  // First tracking chamber construction

  G4RotationMatrix rotm2;
  rotm2.rotateZ(M_PI_2);

  TrackingChamberFactory::Config tc1FactoryCfg{
      .tcCenterX = gc::GeometryPlacement::instance()->tc1CenterX,
      .tcCenterY = gc::GeometryPlacement::instance()->tc1CenterY,
      .tcCenterZ = gc::GeometryPlacement::instance()->tc1CenterZ,

      .tcRotationAngle = gc::GeometryPlacement::instance()->tc1RotationAngle,
      .tcRotationAxis = gc::GeometryPlacement::instance()->tc1RotationAxis,

      .tcc = gc::TrackingChamber::instance(),

      .checkOverlaps = true};

  TrackingChamberFactory tc1Factory(tc1FactoryCfg);
  G4VPhysicalVolume *physTrackingChamber1 = tc1Factory.construct(
      logicWorld, gc::GeometryPlacement::instance()->tc1Name);

  G4PhysicalVolumeModel tc1SearchModel(physTrackingChamber1);
  G4ModelingParameters tc1mp;
  tc1SearchModel.SetModelingParameters(&tc1mp);
  G4PhysicalVolumesSearchScene tc1SearchScene(&tc1SearchModel, "OPPPSensitive");
  tc1SearchModel.DescribeYourselfTo(tc1SearchScene);

  const auto &opppObject1 = tc1SearchScene.GetFindings().at(0);
  G4ThreeVector opppSensitiveTranslation1 =
      opppObject1.fFoundObjectTransformation.getTranslation();
  G4ThreeVector opppSensitiveRotation1 =
      opppObject1.fFoundObjectTransformation.getTranslation();
  opppSensitiveTranslation1 =
      G4RotationMatrix(gc::GeometryPlacement::instance()->tc1RotationAxis,
                       gc::GeometryPlacement::instance()->tc1RotationAngle) *
      opppSensitiveTranslation1;
  opppSensitiveTranslation1 = rotm2 * opppSensitiveTranslation1;
  physTrackingChamber1->SetTranslation(
      G4ThreeVector(-opppSensitiveTranslation1.x(),
                    -opppSensitiveTranslation1.y(),
                    physTrackingChamber1->GetTranslation().z()) +
      G4ThreeVector(translation1, translation1, 0));

  // ---------------------------------------------------
  // Second tracking chamber construction

  TrackingChamberFactory::Config tc2FactoryCfg{
      .tcCenterX = gc::GeometryPlacement::instance()->tc2CenterX,
      .tcCenterY = gc::GeometryPlacement::instance()->tc2CenterY,
      .tcCenterZ = gc::GeometryPlacement::instance()->tc2CenterZ,

      .tcRotationAngle = gc::GeometryPlacement::instance()->tc2RotationAngle,
      .tcRotationAxis = gc::GeometryPlacement::instance()->tc2RotationAxis,

      .tcc = gc::TrackingChamber::instance(),

      .checkOverlaps = true};

  TrackingChamberFactory tc2Factory(tc2FactoryCfg);
  G4VPhysicalVolume *physTrackingChamber2 = tc2Factory.construct(
      logicWorld, gc::GeometryPlacement::instance()->tc2Name);

  G4PhysicalVolumeModel tc2SearchModel(physTrackingChamber2);
  G4ModelingParameters tc2mp;
  tc2SearchModel.SetModelingParameters(&tc2mp);
  G4PhysicalVolumesSearchScene tc2SearchScene(&tc2SearchModel, "OPPPSensitive");
  tc2SearchModel.DescribeYourselfTo(tc2SearchScene);

  const auto &opppObject2 = tc2SearchScene.GetFindings().at(0);
  G4ThreeVector opppSensitiveTranslation2 =
      opppObject2.fFoundObjectTransformation.getTranslation();
  opppSensitiveTranslation2 =
      G4RotationMatrix(gc::GeometryPlacement::instance()->tc2RotationAxis,
                       gc::GeometryPlacement::instance()->tc2RotationAngle) *
      opppSensitiveTranslation2;
  opppSensitiveTranslation2 = rotm2 * opppSensitiveTranslation2;
  physTrackingChamber2->SetTranslation(
      G4ThreeVector(-opppSensitiveTranslation2.x(),
                    -opppSensitiveTranslation2.y(),
                    physTrackingChamber2->GetTranslation().z()) +
      G4ThreeVector(translation2, translation2, 0));

  // ---------------------------------------------------
  // Breadboard table construction

  BreadboardFactory::Config bbFactoryCfg{
      .bbCenterX = gc::GeometryPlacement::instance()->bbCenterX,
      .bbCenterY = physWendellDipole->GetTranslation().y() -
                   gc::WendellDipole::instance()->dipoleHalfZ -
                   gc::Breadboard::instance()->bbHalfZ,
      .bbCenterZ = gc::VacuumChamber::instance()->vcRad +
                   gc::Breadboard::instance()->bbHalfY,
      // .bbCenterZ =
      //              physWendellDipole->GetTranslation().z(),

      .bbRotationAngle = gc::GeometryPlacement::instance()->bbRotationAngle,
      .bbRotationAxis = G4ThreeVector(1, 0, 0),

      .bbc = gc::Breadboard::instance(),

      .checkOverlaps = true};
  // BreadboardFactory bbFactory(bbFactoryCfg);
  // G4VPhysicalVolume *physBreadboard = bbFactory.construct(
  //     logicWorld, gc::GeometryPlacement::instance()->bbName);

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
