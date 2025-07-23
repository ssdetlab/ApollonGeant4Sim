#include <cstddef>
#include <string>

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

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}

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
  magFieldVolumeTranslation =
      G4RotationMatrix(gc::GeometryPlacement::instance()->wdRotationAxis,
                       gc::GeometryPlacement::instance()->wdRotationAngle) *
      magFieldVolumeTranslation;
  physWendellDipole->SetTranslation(G4ThreeVector(
      magFieldVolumeTranslation.x(), magFieldVolumeTranslation.y(),
      physWendellDipole->GetTranslation().z()));

  // ---------------------------------------------------
  // First tracking chamber construction

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
  opppSensitiveTranslation1 =
      G4RotationMatrix(gc::GeometryPlacement::instance()->tc1RotationAxis,
                       gc::GeometryPlacement::instance()->tc1RotationAngle) *
      opppSensitiveTranslation1;
  physTrackingChamber1->SetTranslation(G4ThreeVector(
      opppSensitiveTranslation1.x(), opppSensitiveTranslation1.y(),
      physTrackingChamber1->GetTranslation().z()));

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
  physTrackingChamber2->SetTranslation(G4ThreeVector(
      opppSensitiveTranslation2.x(), opppSensitiveTranslation2.y(),
      physTrackingChamber2->GetTranslation().z()));

  //  // ---------------------------------------------------
  //  // Sampling IP sphere
  //  G4Sphere *solidIpSphere = new G4Sphere("IpSphere", 99 * mm, 100 * mm, 0,
  //                                         2 * M_PI * rad, 0, M_PI * rad);
  //  G4VisAttributes *samplingPlaneVis = new G4VisAttributes(G4Color::Red());
  //  G4LogicalVolume *logicSamplingPlane =
  //      new G4LogicalVolume(solidIpSphere, worldMaterial, "SamplingPlane");
  //  logicSamplingPlane->SetVisAttributes(samplingPlaneVis);
  //
  //  G4VPhysicalVolume *physIpSphere = new G4PVPlacement(
  //      nullptr, G4ThreeVector(), logicSamplingPlane, "SamplingPlane30",
  //      physVacuumChamber->GetLogicalVolume(), false, 0, checkOverlaps);
  //
  //  // ---------------------------------------------------
  //  // Sampling layers 1 construciton
  //
  //  G4VPhysicalVolume *physSamplingLayers1 = constructSamplingLayers(
  //      nistManager, logicWorld, mat::samplingLayersName1,
  //      G4ThreeVector(mat::samplingLayers1CenterX,
  //      mat::samplingLayers1CenterY,
  //                    mat::samplingLayers1CenterZ),
  //      1);
  //
  //  // ---------------------------------------------------
  //  // Sampling volume 2 construciton
  //
  //  G4VPhysicalVolume *physSamplingLayers2 = constructSamplingLayers(
  //      nistManager, logicWorld, mat::samplingLayersName2,
  //      G4ThreeVector(mat::samplingLayers2CenterX,
  //      mat::samplingLayers2CenterY,
  //                    mat::samplingLayers2CenterZ),
  //      2);

  return physWorld;
}

void DetectorConstruction::ConstructSDandField() {
  G4String senstitiveName = "/logicAlpideSensitive";
  auto samplingVolume = new SamplingVolume(senstitiveName, "HitsCollection",
                                           "ProtoTrckCarrierPCB");
  G4SDManager::GetSDMpointer()->AddNewDetector(samplingVolume);
  SetSensitiveDetector("logicAlpideSensitive", samplingVolume, true);
}

// G4VPhysicalVolume *DetectorConstruction::constructSamplingLayers(
//     G4NistManager *nistManager, G4LogicalVolume *logicParent,
//     const std::string &name, const G4ThreeVector &center, int id) {
//   // ---------------------------------------------------
//   // Sampling volume construciton
//
//   G4Box *solidSamplingLayers =
//       new G4Box(name, mat::samplingLayersHalfX, mat::samplingLayersHalfY,
//                 mat::samplingLayersHalfZ);
//
//   G4Material *samplingLayersMaterial =
//       nistManager->FindOrBuildMaterial("G4_AIR");
//   G4LogicalVolume *logicSamplingLayers =
//       new G4LogicalVolume(solidSamplingLayers, samplingLayersMaterial, name);
//   G4VPhysicalVolume *physSamplingLayers =
//       new G4PVPlacement(nullptr, center, logicSamplingLayers, name,
//       logicParent,
//                         false, 0, checkOverlaps);
//
//   // ---------------------------------------------------
//   // Sampling planes construciton
//
//   G4Box *solidSamplingPlane =
//       new G4Box("SamplingPlane", gc::samplingPlaneHalfX,
//       gc::samplingPlaneHalfY,
//                 gc::samplingPlaneHalfZ);
//
//   G4Material *samplingPlaneMaterial =
//       nistManager->FindOrBuildMaterial(mat::samplingPlaneMaterial);
//   G4VisAttributes *samplingPlaneVis = new G4VisAttributes(G4Color::Red());
//   G4LogicalVolume *logicSamplingPlane = new G4LogicalVolume(
//       solidSamplingPlane, samplingPlaneMaterial, "SamplingPlane");
//   logicSamplingPlane->SetVisAttributes(samplingPlaneVis);
//
//   G4ThreeVector offset =
//       (mat::nSamplingPlanes % 2 == 0)
//           ? G4ThreeVector(0, 0,
//                           -(0.5 + (mat::nSamplingPlanes / 2.0 - 1)) *
//                               mat::samplingPlaneSpacing)
//           : G4ThreeVector(
//                 0, 0,
//                 -(mat::nSamplingPlanes - 1) / 2.0 *
//                 mat::samplingPlaneSpacing);
//
//   for (std::size_t i = 0; i < mat::nSamplingPlanes; i++) {
//     G4ThreeVector samplingPlaneCenter =
//         offset + G4ThreeVector(0, 0, i * mat::samplingPlaneSpacing);
//
//     G4VPhysicalVolume *physSamplingPlane =
//         new G4PVPlacement(nullptr, samplingPlaneCenter, logicSamplingPlane,
//                           "SamplingPlane" + std::to_string(id * 10 + i),
//                           logicSamplingLayers, false, 0, checkOverlaps);
//   }
//
//   return physSamplingLayers;
// };
//
// // G4VPhysicalVolume *DetectorConstruction::constructDipole(
// //     G4NistManager *nistManager, G4LogicalVolume *logicParent,
// //     const std::string &name) {
// //   // ---------------------------------------------------
// //   // Dipole internals construciton
// //
// //   return physDipole;
// // };
