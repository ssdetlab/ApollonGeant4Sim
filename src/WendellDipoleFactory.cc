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

  dipoleSamplingLayers1Distance = 2 * cm;

  /// Bounding box parameters
  dipoleHalfX = ironYokeBottomHalfX + 2 * alSidePlateHalfX;
  dipoleHalfY = ironYokeBottomHalfY;
  dipoleHalfZ = ironYokeBottomHalfZ + ironYokeSideHalfZ;
}

WendellDipole *WendellDipole::instance() {
  if (!m_instance) {
    m_instance = new WendellDipole();
  }
  return m_instance;
}

}  // namespace GeometryConstants

G4VPhysicalVolume *WendellDipoleFactory::construct(G4LogicalVolume *logicParent,
                                                   const std::string &name) {
  G4Material *alluminium = G4NistManager::Instance()->FindOrBuildMaterial(
      m_cfg.wdc->alPlateMaterial);
  G4Material *mildSteel = G4NistManager::Instance()->FindOrBuildMaterial(
      m_cfg.wdc->ironYokeMaterial);
  G4Material *neodymium = G4NistManager::Instance()->FindOrBuildMaterial(
      m_cfg.wdc->magPlateMaterial);
  G4Material *air = G4NistManager::Instance()->FindOrBuildMaterial(
      m_cfg.wdc->containerMaterial);

  G4VisAttributes *ironVis = new G4VisAttributes(G4Color::Grey());
  G4VisAttributes *alVis = new G4VisAttributes(G4Color::White());
  G4VisAttributes *ndVis = new G4VisAttributes(G4Color::Red());

  // ---------------------------------------------------
  // Mother volume construction

  G4Box *solidWendellDipole =
      new G4Box(name, m_cfg.wdc->dipoleHalfX, m_cfg.wdc->dipoleHalfY,
                m_cfg.wdc->dipoleHalfZ);

  G4LogicalVolume *logicWendellDipole =
      new G4LogicalVolume(solidWendellDipole, air, name);
  logicWendellDipole->SetVisAttributes(ironVis);

  G4RotationMatrix *dipoleRotation = new G4RotationMatrix();
  dipoleRotation->rotate(m_cfg.dipoleRotationAngle, m_cfg.dipoleRotationAxis);
  dipoleRotation->rotateZ(M_PI_2);
  dipoleRotation->rotateX(M_PI_4);
  G4VPhysicalVolume *physWendellDipole = new G4PVPlacement(
      dipoleRotation,
      G4ThreeVector(m_cfg.dipoleCenterX, m_cfg.dipoleCenterY,
                    m_cfg.dipoleCenterZ),
      logicWendellDipole, name, logicParent, false, 0, m_cfg.checkOverlaps);

  // ---------------------------------------------------
  // Bottom iron yoke construction

  G4Box *solidIronYokeBottom =
      new G4Box("IronYokeBottom", m_cfg.wdc->ironYokeBottomHalfX,
                m_cfg.wdc->ironYokeBottomHalfY, m_cfg.wdc->ironYokeBottomHalfZ);

  G4LogicalVolume *logicIronYokeBottom =
      new G4LogicalVolume(solidIronYokeBottom, mildSteel, "IronYokeBottom");
  logicIronYokeBottom->SetVisAttributes(ironVis);

  G4VPhysicalVolume *physIronYokeBottom = new G4PVPlacement(
      nullptr, G4ThreeVector(0, 0, -m_cfg.wdc->ironYokeSideHalfZ),
      logicIronYokeBottom, "IronYokeBottom", logicWendellDipole, false, 0,
      m_cfg.checkOverlaps);

  // ---------------------------------------------------
  // Side iron yoke construction

  G4Box *solidIronYokeSide =
      new G4Box("IronYokeSide", m_cfg.wdc->ironYokeSideHalfX,
                m_cfg.wdc->ironYokeSideHalfY, m_cfg.wdc->ironYokeSideHalfZ);

  G4LogicalVolume *logicIronYokeSide =
      new G4LogicalVolume(solidIronYokeSide, mildSteel, "IronYokeSide");
  logicIronYokeSide->SetVisAttributes(ironVis);

  G4VPhysicalVolume *physIronYokeSideRight = new G4PVPlacement(
      nullptr,
      G4ThreeVector(
          0, m_cfg.wdc->ironYokeBottomHalfY - m_cfg.wdc->ironYokeSideHalfY,
          m_cfg.wdc->ironYokeBottomHalfZ),
      logicIronYokeSide, "IronYokeSideRight", logicWendellDipole, false, 0,
      m_cfg.checkOverlaps);

  G4VPhysicalVolume *physIronYokeSideLeft = new G4PVPlacement(
      nullptr,
      G4ThreeVector(
          0, -m_cfg.wdc->ironYokeBottomHalfY + m_cfg.wdc->ironYokeSideHalfY,
          m_cfg.wdc->ironYokeBottomHalfZ),
      logicIronYokeSide, "IronYokeSideLeft", logicWendellDipole, false, 0,
      m_cfg.checkOverlaps);

  // ---------------------------------------------------
  // Magnetic plates construction

  G4Box *solidMagPlate =
      new G4Box("MagPlate", m_cfg.wdc->magPlateHalfX, m_cfg.wdc->magPlateHalfY,
                m_cfg.wdc->magPlateHalfZ);

  G4LogicalVolume *logicMagPlate =
      new G4LogicalVolume(solidMagPlate, neodymium, "MagPlate");
  logicMagPlate->SetVisAttributes(ndVis);

  G4VPhysicalVolume *physMagPlateRight = new G4PVPlacement(
      nullptr,
      G4ThreeVector(
          0,
          m_cfg.wdc->ironYokeBottomHalfY - 2 * m_cfg.wdc->ironYokeSideHalfY -
              m_cfg.wdc->magPlateHalfY,
          m_cfg.wdc->ironYokeBottomHalfZ - m_cfg.wdc->ironYokeSideHalfZ +
              2 * m_cfg.wdc->alSpacerHalfZ + m_cfg.wdc->magPlateHalfZ),
      logicMagPlate, "MagPlateRight", logicWendellDipole, false, 0,
      m_cfg.checkOverlaps);

  G4VPhysicalVolume *physMagPlateLeft = new G4PVPlacement(
      nullptr,
      G4ThreeVector(
          0,
          -m_cfg.wdc->ironYokeBottomHalfY + 2 * m_cfg.wdc->ironYokeSideHalfY +
              m_cfg.wdc->magPlateHalfY,
          m_cfg.wdc->ironYokeBottomHalfZ - m_cfg.wdc->ironYokeSideHalfZ +
              2 * m_cfg.wdc->alSpacerHalfZ + m_cfg.wdc->magPlateHalfZ),
      logicMagPlate, "MagPlateLeft", logicWendellDipole, false, 0,
      m_cfg.checkOverlaps);

  // ---------------------------------------------------
  // Iron plates construction

  G4Box *solidIronPlate =
      new G4Box("IronPlate", m_cfg.wdc->ironPlateHalfX,
                m_cfg.wdc->ironPlateHalfY, m_cfg.wdc->ironPlateHalfZ);

  G4LogicalVolume *logicIronPlate =
      new G4LogicalVolume(solidIronPlate, mildSteel, "IronPlate");
  logicIronPlate->SetVisAttributes(ironVis);

  G4VPhysicalVolume *physIronPlateRight = new G4PVPlacement(
      nullptr,
      G4ThreeVector(
          0,
          m_cfg.wdc->ironYokeBottomHalfY - 2 * m_cfg.wdc->ironYokeSideHalfY -
              2 * m_cfg.wdc->magPlateHalfY - m_cfg.wdc->ironPlateHalfY,
          m_cfg.wdc->ironYokeBottomHalfZ - m_cfg.wdc->ironYokeSideHalfZ +
              2 * m_cfg.wdc->alSpacerHalfZ + m_cfg.wdc->ironPlateHalfZ),
      logicIronPlate, "IronPlateRight", logicWendellDipole, false, 0,
      m_cfg.checkOverlaps);

  G4VPhysicalVolume *physIronPlateLeft = new G4PVPlacement(
      nullptr,
      G4ThreeVector(
          0,
          -m_cfg.wdc->ironYokeBottomHalfY + 2 * m_cfg.wdc->ironYokeSideHalfY +
              2 * m_cfg.wdc->magPlateHalfY + m_cfg.wdc->ironPlateHalfY,
          m_cfg.wdc->ironYokeBottomHalfZ - m_cfg.wdc->ironYokeSideHalfZ +
              2 * m_cfg.wdc->alSpacerHalfZ + m_cfg.wdc->ironPlateHalfZ),
      logicIronPlate, "IronPlateLeft", logicWendellDipole, false, 0,
      m_cfg.checkOverlaps);

  // ---------------------------------------------------
  // Aluminium spacer construction

  G4Box *solidAlSpacer =
      new G4Box("AlSpacer", m_cfg.wdc->alSpacerHalfX, m_cfg.wdc->alSpacerHalfY,
                m_cfg.wdc->alSpacerHalfZ);

  G4LogicalVolume *logicAlSpacer =
      new G4LogicalVolume(solidAlSpacer, alluminium, "AlSpacer");
  logicAlSpacer->SetVisAttributes(alVis);

  G4VPhysicalVolume *physAlSpacer =
      new G4PVPlacement(nullptr,
                        G4ThreeVector(0, 0,
                                      m_cfg.wdc->ironYokeBottomHalfZ -
                                          m_cfg.wdc->ironYokeSideHalfZ +
                                          m_cfg.wdc->alSpacerHalfZ),
                        logicAlSpacer, "AlSpacer", logicWendellDipole, false, 0,
                        m_cfg.checkOverlaps);

  // ---------------------------------------------------
  // Aluminium side plates construction

  G4Box *solidAlSidePlateRectangle =
      new G4Box("AlSidePlateRectangle", m_cfg.wdc->alSidePlateHalfX,
                m_cfg.wdc->alSidePlateHalfY, m_cfg.wdc->alSidePlateHalfZ);
  G4Box *solidAlSidePlateCutout = new G4Box(
      "AlSidePlateCutout", m_cfg.wdc->alSidePlateCutoutHalfX,
      m_cfg.wdc->alSidePlateCutoutHalfY, 2 * m_cfg.wdc->alSidePlateCutoutHalfZ);
  G4SubtractionSolid *solidAlSidePlate = new G4SubtractionSolid(
      "AlSidePlate", solidAlSidePlateRectangle, solidAlSidePlateCutout,
      G4Transform3D(G4RotationMatrix::IDENTITY,
                    G4ThreeVector(0, 0, m_cfg.wdc->alSidePlateHalfZ)));

  G4LogicalVolume *logicAlSidePlate =
      new G4LogicalVolume(solidAlSidePlate, alluminium, "AlSidePlate");
  logicAlSidePlate->SetVisAttributes(alVis);

  G4VPhysicalVolume *physAlSidePlateFront = new G4PVPlacement(
      nullptr,
      G4ThreeVector(
          m_cfg.wdc->ironYokeBottomHalfX + m_cfg.wdc->alSidePlateHalfX, 0, 0),
      logicAlSidePlate, "AlSidePlateFront", logicWendellDipole, false, 0,
      m_cfg.checkOverlaps);

  G4VPhysicalVolume *physAlSidePlateBack = new G4PVPlacement(
      nullptr,
      G4ThreeVector(
          -m_cfg.wdc->ironYokeBottomHalfX - m_cfg.wdc->alSidePlateHalfX, 0, 0),
      logicAlSidePlate, "AlSidePlateBack", logicWendellDipole, false, 0,
      m_cfg.checkOverlaps);

  // ---------------------------------------------------
  // Magnetic field volume construction

  G4UniformMagField *dipoleField =
      new G4UniformMagField(m_cfg.wdc->dipoleField);
  G4FieldManager *dipoleFieldMgr = new G4FieldManager(dipoleField);

  G4Box *solidMagFieldVolume =
      new G4Box("MagFieldVolume", m_cfg.wdc->alSpacerHalfX,
                m_cfg.wdc->alSpacerHalfY - 2 * m_cfg.wdc->magPlateHalfY -
                    2 * m_cfg.wdc->ironPlateHalfY,
                m_cfg.wdc->ironPlateHalfZ);

  G4LogicalVolume *logicMagFieldVolume =
      new G4LogicalVolume(solidMagFieldVolume, air, "MagFieldVolume");
  logicMagFieldVolume->SetFieldManager(dipoleFieldMgr, false);
  G4VPhysicalVolume *physMagFieldVolume = new G4PVPlacement(
      nullptr,
      G4ThreeVector(
          0, 0,
          m_cfg.wdc->ironYokeBottomHalfZ - m_cfg.wdc->ironYokeSideHalfZ +
              2 * m_cfg.wdc->alSpacerHalfZ + m_cfg.wdc->ironPlateHalfZ),
      logicMagFieldVolume, "MagFieldVolume", logicWendellDipole, false, 0,
      m_cfg.checkOverlaps);
  dipoleFieldMgr->CreateChordFinder(dipoleField);

  return physWendellDipole;
}
