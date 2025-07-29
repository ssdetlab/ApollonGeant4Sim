#include "WendellDipoleFactory.hh"

#include "G4Box.hh"
#include "G4FieldManager.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4UniformMagField.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4VisAttributes.hh"
#include "MaterialFactory.hh"

namespace GeometryConstants {

WendellDipole *WendellDipole::m_instance = nullptr;

WendellDipole::WendellDipole() {
  containerMaterial = "G4_AIR";
  alPlateMaterial = Material::Al6061::name;
  ironYokeMaterial = Material::MildSteel::name;
  magPlateMaterial = Material::Neodymium::name;

  /// Magnet bottom iron yoke dimensions
  ironYokeBottomHalfX = 60.5 * mm;
  ironYokeBottomHalfY = 51 * mm;
  ironYokeBottomHalfZ = 15 * mm;

  /// Magnet bottom iron yoke dimensions
  ironYokeSideHalfX = 60.5 * mm;
  ironYokeSideHalfY = 15.5 * mm;
  ironYokeSideHalfZ = 41.25 * mm;

  /// Magnet plate dimensions
  magPlateHalfX = 60.5 * mm;
  magPlateHalfY = 2.5 * mm;
  magPlateHalfZ = 30 * mm;

  /// Iron plate dimensions
  ironPlateHalfX = 60.5 * mm;
  ironPlateHalfY = 2.5 * mm;
  ironPlateHalfZ = 30 * mm;

  /// Aluminium spacer dimensions
  alSpacerHalfX = 60.5 * mm;
  alSpacerHalfY = 20 * mm;
  alSpacerHalfZ = 11.25 * mm;

  /// Aluminium side plate dimensions
  alSidePlateHalfX = 1 * mm;
  alSidePlateHalfY = 51 * mm;
  alSidePlateHalfZ = 56.25 * mm;

  /// Aluminium side plate cutout dimensions
  alSidePlateCutoutHalfX = alSidePlateHalfX * 1.1;
  alSidePlateCutoutHalfY = 12.5 * mm;
  alSidePlateCutoutHalfZ = 40.5 * mm;

  /// Magnetic field vector
  dipoleField = G4ThreeVector(0.35 * tesla, 0.0, 0.0);
  // dipoleField = G4ThreeVector(0.24748737 * tesla, -0.24748737 * tesla, 0);

  dipoleSamplingLayers1Distance = 2 * cm;

  /// Bounding box parameters
  wdHalfX = ironYokeBottomHalfX + 2 * alSidePlateHalfX;
  wdHalfY = ironYokeBottomHalfY;
  wdHalfZ = ironYokeBottomHalfZ + ironYokeSideHalfZ;
}

WendellDipole *WendellDipole::instance() {
  if (!m_instance) {
    m_instance = new WendellDipole();
  }
  return m_instance;
}

}  // namespace GeometryConstants

G4VPhysicalVolume *WendellDipoleFactory::construct(G4LogicalVolume *logicParent,
                                                   const Config &cfg) {
  G4Material *alluminium =
      G4NistManager::Instance()->FindOrBuildMaterial(cfg.wdc->alPlateMaterial);
  G4Material *mildSteel =
      G4NistManager::Instance()->FindOrBuildMaterial(cfg.wdc->ironYokeMaterial);
  G4Material *neodymium =
      G4NistManager::Instance()->FindOrBuildMaterial(cfg.wdc->magPlateMaterial);
  G4Material *air = G4NistManager::Instance()->FindOrBuildMaterial(
      cfg.wdc->containerMaterial);

  G4VisAttributes *ironVis = new G4VisAttributes(G4Color::Grey());
  G4VisAttributes *alVis = new G4VisAttributes(G4Color::White());
  G4VisAttributes *ndVis = new G4VisAttributes(G4Color::Red());

  // ---------------------------------------------------
  // Mother volume construction

  G4Box *solidWendellDipole =
      new G4Box(cfg.name, cfg.wdc->wdHalfX, cfg.wdc->wdHalfY, cfg.wdc->wdHalfZ);

  G4LogicalVolume *logicWendellDipole =
      new G4LogicalVolume(solidWendellDipole, air, cfg.name);
  logicWendellDipole->SetVisAttributes(ironVis);

  G4RotationMatrix *dipoleRotation = new G4RotationMatrix();
  dipoleRotation->rotateX(cfg.wdRotationAngleX);
  dipoleRotation->rotateY(cfg.wdRotationAngleY);
  dipoleRotation->rotateZ(cfg.wdRotationAngleZ);
  G4VPhysicalVolume *physWendellDipole = new G4PVPlacement(
      dipoleRotation,
      G4ThreeVector(cfg.wdCenterX, cfg.wdCenterY, cfg.wdCenterZ),
      logicWendellDipole, cfg.name, logicParent, false, 0, cfg.checkOverlaps);

  // ---------------------------------------------------
  // Bottom iron yoke construction

  G4Box *solidIronYokeBottom =
      new G4Box("IronYokeBottom", cfg.wdc->ironYokeBottomHalfX,
                cfg.wdc->ironYokeBottomHalfY, cfg.wdc->ironYokeBottomHalfZ);

  G4LogicalVolume *logicIronYokeBottom =
      new G4LogicalVolume(solidIronYokeBottom, mildSteel, "IronYokeBottom");
  logicIronYokeBottom->SetVisAttributes(ironVis);

  G4VPhysicalVolume *physIronYokeBottom = new G4PVPlacement(
      nullptr, G4ThreeVector(0, 0, -cfg.wdc->ironYokeSideHalfZ),
      logicIronYokeBottom, "IronYokeBottom", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  // ---------------------------------------------------
  // Side iron yoke construction

  G4Box *solidIronYokeSide =
      new G4Box("IronYokeSide", cfg.wdc->ironYokeSideHalfX,
                cfg.wdc->ironYokeSideHalfY, cfg.wdc->ironYokeSideHalfZ);

  G4LogicalVolume *logicIronYokeSide =
      new G4LogicalVolume(solidIronYokeSide, mildSteel, "IronYokeSide");
  logicIronYokeSide->SetVisAttributes(ironVis);

  G4VPhysicalVolume *physIronYokeSideRight = new G4PVPlacement(
      nullptr,
      G4ThreeVector(0,
                    cfg.wdc->ironYokeBottomHalfY - cfg.wdc->ironYokeSideHalfY,
                    cfg.wdc->ironYokeBottomHalfZ),
      logicIronYokeSide, "IronYokeSideRight", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  G4VPhysicalVolume *physIronYokeSideLeft = new G4PVPlacement(
      nullptr,
      G4ThreeVector(0,
                    -cfg.wdc->ironYokeBottomHalfY + cfg.wdc->ironYokeSideHalfY,
                    cfg.wdc->ironYokeBottomHalfZ),
      logicIronYokeSide, "IronYokeSideLeft", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  // ---------------------------------------------------
  // Magnetic plates construction

  G4Box *solidMagPlate =
      new G4Box("MagPlate", cfg.wdc->magPlateHalfX, cfg.wdc->magPlateHalfY,
                cfg.wdc->magPlateHalfZ);

  G4LogicalVolume *logicMagPlate =
      new G4LogicalVolume(solidMagPlate, neodymium, "MagPlate");
  logicMagPlate->SetVisAttributes(ndVis);

  G4VPhysicalVolume *physMagPlateRight = new G4PVPlacement(
      nullptr,
      G4ThreeVector(0,
                    cfg.wdc->ironYokeBottomHalfY -
                        2 * cfg.wdc->ironYokeSideHalfY - cfg.wdc->magPlateHalfY,
                    cfg.wdc->ironYokeBottomHalfZ - cfg.wdc->ironYokeSideHalfZ +
                        2 * cfg.wdc->alSpacerHalfZ + cfg.wdc->magPlateHalfZ),
      logicMagPlate, "MagPlateRight", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  G4VPhysicalVolume *physMagPlateLeft = new G4PVPlacement(
      nullptr,
      G4ThreeVector(0,
                    -cfg.wdc->ironYokeBottomHalfY +
                        2 * cfg.wdc->ironYokeSideHalfY + cfg.wdc->magPlateHalfY,
                    cfg.wdc->ironYokeBottomHalfZ - cfg.wdc->ironYokeSideHalfZ +
                        2 * cfg.wdc->alSpacerHalfZ + cfg.wdc->magPlateHalfZ),
      logicMagPlate, "MagPlateLeft", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  // ---------------------------------------------------
  // Iron plates construction

  G4Box *solidIronPlate =
      new G4Box("IronPlate", cfg.wdc->ironPlateHalfX, cfg.wdc->ironPlateHalfY,
                cfg.wdc->ironPlateHalfZ);

  G4LogicalVolume *logicIronPlate =
      new G4LogicalVolume(solidIronPlate, mildSteel, "IronPlate");
  logicIronPlate->SetVisAttributes(ironVis);

  G4VPhysicalVolume *physIronPlateRight = new G4PVPlacement(
      nullptr,
      G4ThreeVector(0,
                    cfg.wdc->ironYokeBottomHalfY -
                        2 * cfg.wdc->ironYokeSideHalfY -
                        2 * cfg.wdc->magPlateHalfY - cfg.wdc->ironPlateHalfY,
                    cfg.wdc->ironYokeBottomHalfZ - cfg.wdc->ironYokeSideHalfZ +
                        2 * cfg.wdc->alSpacerHalfZ + cfg.wdc->ironPlateHalfZ),
      logicIronPlate, "IronPlateRight", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  G4VPhysicalVolume *physIronPlateLeft = new G4PVPlacement(
      nullptr,
      G4ThreeVector(0,
                    -cfg.wdc->ironYokeBottomHalfY +
                        2 * cfg.wdc->ironYokeSideHalfY +
                        2 * cfg.wdc->magPlateHalfY + cfg.wdc->ironPlateHalfY,
                    cfg.wdc->ironYokeBottomHalfZ - cfg.wdc->ironYokeSideHalfZ +
                        2 * cfg.wdc->alSpacerHalfZ + cfg.wdc->ironPlateHalfZ),
      logicIronPlate, "IronPlateLeft", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  // ---------------------------------------------------
  // Aluminium spacer construction

  G4Box *solidAlSpacer =
      new G4Box("AlSpacer", cfg.wdc->alSpacerHalfX, cfg.wdc->alSpacerHalfY,
                cfg.wdc->alSpacerHalfZ);

  G4LogicalVolume *logicAlSpacer =
      new G4LogicalVolume(solidAlSpacer, alluminium, "AlSpacer");
  logicAlSpacer->SetVisAttributes(alVis);

  G4VPhysicalVolume *physAlSpacer = new G4PVPlacement(
      nullptr,
      G4ThreeVector(0, 0,
                    cfg.wdc->ironYokeBottomHalfZ - cfg.wdc->ironYokeSideHalfZ +
                        cfg.wdc->alSpacerHalfZ),
      logicAlSpacer, "AlSpacer", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  // ---------------------------------------------------
  // Aluminium side plates construction

  G4Box *solidAlSidePlateRectangle =
      new G4Box("AlSidePlateRectangle", cfg.wdc->alSidePlateHalfX,
                cfg.wdc->alSidePlateHalfY, cfg.wdc->alSidePlateHalfZ);
  G4Box *solidAlSidePlateCutout = new G4Box(
      "AlSidePlateCutout", cfg.wdc->alSidePlateCutoutHalfX,
      cfg.wdc->alSidePlateCutoutHalfY, 2 * cfg.wdc->alSidePlateCutoutHalfZ);
  G4SubtractionSolid *solidAlSidePlate = new G4SubtractionSolid(
      "AlSidePlate", solidAlSidePlateRectangle, solidAlSidePlateCutout,
      G4Transform3D(G4RotationMatrix::IDENTITY,
                    G4ThreeVector(0, 0, cfg.wdc->alSidePlateHalfZ)));

  G4LogicalVolume *logicAlSidePlate =
      new G4LogicalVolume(solidAlSidePlate, alluminium, "AlSidePlate");
  logicAlSidePlate->SetVisAttributes(alVis);

  G4VPhysicalVolume *physAlSidePlateFront = new G4PVPlacement(
      nullptr,
      G4ThreeVector(cfg.wdc->ironYokeBottomHalfX + cfg.wdc->alSidePlateHalfX, 0,
                    0),
      logicAlSidePlate, "AlSidePlateFront", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  G4VPhysicalVolume *physAlSidePlateBack = new G4PVPlacement(
      nullptr,
      G4ThreeVector(-cfg.wdc->ironYokeBottomHalfX - cfg.wdc->alSidePlateHalfX,
                    0, 0),
      logicAlSidePlate, "AlSidePlateBack", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  // ---------------------------------------------------
  // Magnetic field volume construction

  G4UniformMagField *dipoleField = new G4UniformMagField(cfg.wdc->dipoleField);
  G4FieldManager *dipoleFieldMgr = new G4FieldManager(dipoleField);

  G4Box *solidMagFieldVolume =
      new G4Box("MagFieldVolume", cfg.wdc->alSpacerHalfX,
                cfg.wdc->alSpacerHalfY - 2 * cfg.wdc->magPlateHalfY -
                    2 * cfg.wdc->ironPlateHalfY,
                cfg.wdc->ironPlateHalfZ);

  G4LogicalVolume *logicMagFieldVolume =
      new G4LogicalVolume(solidMagFieldVolume, air, "MagFieldVolume");
  logicMagFieldVolume->SetFieldManager(dipoleFieldMgr, false);
  G4VPhysicalVolume *physMagFieldVolume = new G4PVPlacement(
      nullptr,
      G4ThreeVector(0, 0,
                    cfg.wdc->ironYokeBottomHalfZ - cfg.wdc->ironYokeSideHalfZ +
                        2 * cfg.wdc->alSpacerHalfZ + cfg.wdc->ironPlateHalfZ),
      logicMagFieldVolume, "MagFieldVolume", logicWendellDipole, false, 0,
      cfg.checkOverlaps);
  dipoleFieldMgr->CreateChordFinder(dipoleField);

  return physWendellDipole;
}
