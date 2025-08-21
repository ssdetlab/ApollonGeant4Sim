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

G4VPhysicalVolume *WendellDipoleFactory::construct(G4LogicalVolume *logicParent,
                                                   const Config &cfg) {
  G4Material *alluminium =
      G4NistManager::Instance()->FindOrBuildMaterial(cfg.gc->wmAlPlateMaterial);
  G4Material *mildSteel = G4NistManager::Instance()->FindOrBuildMaterial(
      cfg.gc->wmIronYokeMaterial);
  G4Material *neodymium = G4NistManager::Instance()->FindOrBuildMaterial(
      cfg.gc->wmMagPlateMaterial);
  G4Material *air = G4NistManager::Instance()->FindOrBuildMaterial(
      cfg.gc->wmContainerMaterial);

  G4VisAttributes *ironVis = new G4VisAttributes(G4Color::Grey());
  G4VisAttributes *alVis = new G4VisAttributes(G4Color::White());
  G4VisAttributes *ndVis = new G4VisAttributes(G4Color::Red());

  // ---------------------------------------------------
  // Mother volume construction

  G4Box *solidWendellDipole =
      new G4Box(cfg.name, cfg.gc->wmHalfX, cfg.gc->wmHalfY, cfg.gc->wmHalfZ);

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
      new G4Box("IronYokeBottom", cfg.gc->wmIronYokeBottomHalfX,
                cfg.gc->wmIronYokeBottomHalfY, cfg.gc->wmIronYokeBottomHalfZ);

  G4LogicalVolume *logicIronYokeBottom =
      new G4LogicalVolume(solidIronYokeBottom, mildSteel, "IronYokeBottom");
  logicIronYokeBottom->SetVisAttributes(ironVis);

  G4VPhysicalVolume *physIronYokeBottom = new G4PVPlacement(
      nullptr, G4ThreeVector(0, 0, -cfg.gc->wmIronYokeSideHalfZ),
      logicIronYokeBottom, "IronYokeBottom", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  // ---------------------------------------------------
  // Side iron yoke construction

  G4Box *solidIronYokeSide =
      new G4Box("IronYokeSide", cfg.gc->wmIronYokeSideHalfX,
                cfg.gc->wmIronYokeSideHalfY, cfg.gc->wmIronYokeSideHalfZ);

  G4LogicalVolume *logicIronYokeSide =
      new G4LogicalVolume(solidIronYokeSide, mildSteel, "IronYokeSide");
  logicIronYokeSide->SetVisAttributes(ironVis);

  G4VPhysicalVolume *physIronYokeSideRight = new G4PVPlacement(
      nullptr,
      G4ThreeVector(0,
                    cfg.gc->wmIronYokeBottomHalfY - cfg.gc->wmIronYokeSideHalfY,
                    cfg.gc->wmIronYokeBottomHalfZ),
      logicIronYokeSide, "IronYokeSideRight", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  G4VPhysicalVolume *physIronYokeSideLeft = new G4PVPlacement(
      nullptr,
      G4ThreeVector(
          0, -cfg.gc->wmIronYokeBottomHalfY + cfg.gc->wmIronYokeSideHalfY,
          cfg.gc->wmIronYokeBottomHalfZ),
      logicIronYokeSide, "IronYokeSideLeft", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  // ---------------------------------------------------
  // Magnetic plates construction

  G4Box *solidMagPlate =
      new G4Box("MagPlate", cfg.gc->wmMagPlateHalfX, cfg.gc->wmMagPlateHalfY,
                cfg.gc->wmMagPlateHalfZ);

  G4LogicalVolume *logicMagPlate =
      new G4LogicalVolume(solidMagPlate, neodymium, "MagPlate");
  logicMagPlate->SetVisAttributes(ndVis);

  G4VPhysicalVolume *physMagPlateRight = new G4PVPlacement(
      nullptr,
      G4ThreeVector(
          0,
          cfg.gc->wmIronYokeBottomHalfY - 2 * cfg.gc->wmIronYokeSideHalfY -
              cfg.gc->wmMagPlateHalfY,
          cfg.gc->wmIronYokeBottomHalfZ - cfg.gc->wmIronYokeSideHalfZ +
              2 * cfg.gc->wmAlSpacerHalfZ + cfg.gc->wmMagPlateHalfZ),
      logicMagPlate, "MagPlateRight", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  G4VPhysicalVolume *physMagPlateLeft = new G4PVPlacement(
      nullptr,
      G4ThreeVector(
          0,
          -cfg.gc->wmIronYokeBottomHalfY + 2 * cfg.gc->wmIronYokeSideHalfY +
              cfg.gc->wmMagPlateHalfY,
          cfg.gc->wmIronYokeBottomHalfZ - cfg.gc->wmIronYokeSideHalfZ +
              2 * cfg.gc->wmAlSpacerHalfZ + cfg.gc->wmMagPlateHalfZ),
      logicMagPlate, "MagPlateLeft", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  // ---------------------------------------------------
  // Iron plates construction

  G4Box *solidIronPlate =
      new G4Box("IronPlate", cfg.gc->wmIronPlateHalfX, cfg.gc->wmIronPlateHalfY,
                cfg.gc->wmIronPlateHalfZ);

  G4LogicalVolume *logicIronPlate =
      new G4LogicalVolume(solidIronPlate, mildSteel, "IronPlate");
  logicIronPlate->SetVisAttributes(ironVis);

  G4VPhysicalVolume *physIronPlateRight = new G4PVPlacement(
      nullptr,
      G4ThreeVector(
          0,
          cfg.gc->wmIronYokeBottomHalfY - 2 * cfg.gc->wmIronYokeSideHalfY -
              2 * cfg.gc->wmMagPlateHalfY - cfg.gc->wmIronPlateHalfY,
          cfg.gc->wmIronYokeBottomHalfZ - cfg.gc->wmIronYokeSideHalfZ +
              2 * cfg.gc->wmAlSpacerHalfZ + cfg.gc->wmIronPlateHalfZ),
      logicIronPlate, "IronPlateRight", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  G4VPhysicalVolume *physIronPlateLeft = new G4PVPlacement(
      nullptr,
      G4ThreeVector(
          0,
          -cfg.gc->wmIronYokeBottomHalfY + 2 * cfg.gc->wmIronYokeSideHalfY +
              2 * cfg.gc->wmMagPlateHalfY + cfg.gc->wmIronPlateHalfY,
          cfg.gc->wmIronYokeBottomHalfZ - cfg.gc->wmIronYokeSideHalfZ +
              2 * cfg.gc->wmAlSpacerHalfZ + cfg.gc->wmIronPlateHalfZ),
      logicIronPlate, "IronPlateLeft", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  // ---------------------------------------------------
  // Aluminium spacer construction

  G4Box *solidAlSpacer =
      new G4Box("AlSpacer", cfg.gc->wmAlSpacerHalfX, cfg.gc->wmAlSpacerHalfY,
                cfg.gc->wmAlSpacerHalfZ);

  G4LogicalVolume *logicAlSpacer =
      new G4LogicalVolume(solidAlSpacer, alluminium, "AlSpacer");
  logicAlSpacer->SetVisAttributes(alVis);

  G4VPhysicalVolume *physAlSpacer = new G4PVPlacement(
      nullptr,
      G4ThreeVector(0, 0,
                    cfg.gc->wmIronYokeBottomHalfZ -
                        cfg.gc->wmIronYokeSideHalfZ + cfg.gc->wmAlSpacerHalfZ),
      logicAlSpacer, "AlSpacer", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  // ---------------------------------------------------
  // Aluminium side plates construction

  G4Box *solidAlSidePlateRectangle =
      new G4Box("AlSidePlateRectangle", cfg.gc->wmAlSidePlateHalfX,
                cfg.gc->wmAlSidePlateHalfY, cfg.gc->wmAlSidePlateHalfZ);
  G4Box *solidAlSidePlateCutout = new G4Box(
      "AlSidePlateCutout", cfg.gc->wmAlSidePlateCutoutHalfX,
      cfg.gc->wmAlSidePlateCutoutHalfY, 2 * cfg.gc->wmAlSidePlateCutoutHalfZ);
  G4SubtractionSolid *solidAlSidePlate = new G4SubtractionSolid(
      "AlSidePlate", solidAlSidePlateRectangle, solidAlSidePlateCutout,
      G4Transform3D(G4RotationMatrix::IDENTITY,
                    G4ThreeVector(0, 0, cfg.gc->wmAlSidePlateHalfZ)));

  G4LogicalVolume *logicAlSidePlate =
      new G4LogicalVolume(solidAlSidePlate, alluminium, "AlSidePlate");
  logicAlSidePlate->SetVisAttributes(alVis);

  G4VPhysicalVolume *physAlSidePlateFront = new G4PVPlacement(
      nullptr,
      G4ThreeVector(cfg.gc->wmIronYokeBottomHalfX + cfg.gc->wmAlSidePlateHalfX,
                    0, 0),
      logicAlSidePlate, "AlSidePlateFront", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  G4VPhysicalVolume *physAlSidePlateBack = new G4PVPlacement(
      nullptr,
      G4ThreeVector(-cfg.gc->wmIronYokeBottomHalfX - cfg.gc->wmAlSidePlateHalfX,
                    0, 0),
      logicAlSidePlate, "AlSidePlateBack", logicWendellDipole, false, 0,
      cfg.checkOverlaps);

  // ---------------------------------------------------
  // Magnetic field volume construction

  G4RotationMatrix fieldRotation = G4RotationMatrix::IDENTITY;
  fieldRotation.rotateY(cfg.angle);
  G4UniformMagField *dipoleField =
      new G4UniformMagField(fieldRotation * cfg.gc->wmField);
  G4FieldManager *dipoleFieldMgr = new G4FieldManager(dipoleField);

  G4Box *solidMagFieldVolume =
      new G4Box("MagFieldVolume", cfg.gc->wmAlSpacerHalfX,
                cfg.gc->wmAlSpacerHalfY - 2 * cfg.gc->wmMagPlateHalfY -
                    2 * cfg.gc->wmIronPlateHalfY,
                cfg.gc->wmIronPlateHalfZ);

  G4LogicalVolume *logicMagFieldVolume =
      new G4LogicalVolume(solidMagFieldVolume, air, "MagFieldVolume");
  logicMagFieldVolume->SetFieldManager(dipoleFieldMgr, false);
  G4VPhysicalVolume *physMagFieldVolume = new G4PVPlacement(
      nullptr,
      G4ThreeVector(0, 0,
                    cfg.gc->wmIronYokeBottomHalfZ -
                        cfg.gc->wmIronYokeSideHalfZ +
                        2 * cfg.gc->wmAlSpacerHalfZ + cfg.gc->wmIronPlateHalfZ),
      logicMagFieldVolume, "MagFieldVolume", logicWendellDipole, false, 0,
      cfg.checkOverlaps);
  dipoleFieldMgr->CreateChordFinder(dipoleField);

  return physWendellDipole;
}
