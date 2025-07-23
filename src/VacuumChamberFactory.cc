#include "VacuumChamberFactory.hh"

#include "G4MultiUnion.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4VisAttributes.hh"

namespace GeometryConstants {

VacuumChamber *VacuumChamber::m_instance = nullptr;

VacuumChamber::VacuumChamber() {
  vcVacuumMaterial = "G4_Galactic";
  vcWallsMaterial = "G4_Al";
  vcDoorsMaterial = "G4_Al";
  vcWindowsMaterial = "G4_Al";
  vcFlangeMaterial = "G4_Al";

  /// Parameters of the encapsulating
  /// VC cylinder
  vcCylinderRadMin = 1000 * mm;
  vcCylinderThickness = 200 * mm;
  vcCylinderHalfZ = 825 * mm;

  /// Parameters of the outer
  /// cylinder VC cutout
  vcCylinderCutoutHalfX = 412.5 * mm;
  vcCylinderCutoutHalfY = 780 * mm;
  vcCylinderCutoutHalfZ = 100 * mm;

  /// Identity rotation door position
  /// in the chamber
  vcDoorCenterX = 0 * mm;
  vcDoorCenterY = 0 * mm;
  vcDoorCenterZ = 1122.5 * mm;

  /// VC exit door dimensions
  vcDoorHalfX = 350 * mm;
  vcDoorHalfY = 500 * mm;
  vcDoorHalfZ = 22.5 * mm;

  /// Identity rotation VC window
  /// position in the chamber
  vcWindowCenterX = 0 * mm;
  vcWindowCenterY = 0 * mm;
  vcWindowCenterZ = 1152.6 * mm;

  /// VC windows dimensions
  bigVcWindowRad = 254 * mm;
  smallVcWindowRad = 185 * mm;
  vcWindowHalfZ = 8.5 * mm;

  /// Identity rotation VC flange
  /// position in the chamber
  vcFlangeCenterX = 0 * mm;
  vcFlangeCenterY = 0 * mm;
  vcFlangeCenterZ = 1158.6 * mm;

  /// VC flange dimensions
  vcFlangeRad = 120 * mm;
  vcFlangeHalfZ = 6 * mm;

  /// VC flange exit window dimensions
  vcFlangeWindowHalfX = 95 * mm;
  vcFlangeWindowHalfY = 15 * mm;
  vcFlangeWindowHalfZ = 0.25 * mm;

  /// Angle between the VC exits
  vcExitAngleSpacing = M_PI / 4 * rad;

  /// Bounding box parameters
  vcRad = vcFlangeCenterZ + vcFlangeHalfZ;
}

VacuumChamber *VacuumChamber::instance() {
  if (!m_instance) {
    m_instance = new VacuumChamber();
  }
  return m_instance;
}

}  // namespace GeometryConstants

G4VPhysicalVolume *VacuumChamberFactory::construct(G4LogicalVolume *logicParent,
                                                   const std::string &name) {
  G4Material *vcVacuumMaterial = G4NistManager::Instance()->FindOrBuildMaterial(
      m_cfg.vcc->vcVacuumMaterial);
  G4Material *vcWallsMaterial = G4NistManager::Instance()->FindOrBuildMaterial(
      m_cfg.vcc->vcWallsMaterial);
  G4Material *vcDoorsMaterial = G4NistManager::Instance()->FindOrBuildMaterial(
      m_cfg.vcc->vcDoorsMaterial);
  G4Material *vcWindowsMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial(
          m_cfg.vcc->vcWindowsMaterial);
  G4Material *vcFlangeMaterial = G4NistManager::Instance()->FindOrBuildMaterial(
      m_cfg.vcc->vcFlangeMaterial);

  // ---------------------------------------------------
  // Solids construction

  // VC walls and vacuum construction
  auto [solidVcWalls, solidVcWallsVacuum] = constructVcWalls();

  // VC exit doors construction
  auto [solidVcDoors, solidVcDoorsVacuum] = constructVcDoors();

  // VC exit windows construction
  auto [solidVcWindows, solidVcWindowsVacuum] = constructVcWindows();

  // VC exit window flange construction
  auto [solidVcFlange, solidVcFlangeVacuum] = constructVcFlange();

  // ---------------------------------------------------
  // Vacuum solid assembly

  G4MultiUnion *solidVcVacuum = new G4MultiUnion("VcVacuum");
  G4Transform3D vacuumWallsTranform(G4RotationMatrix::IDENTITY,
                                    G4ThreeVector());
  solidVcVacuum->AddNode(solidVcWallsVacuum, vacuumWallsTranform);

  G4RotationMatrix vacuumDoorsRotationMatrix = G4RotationMatrix::IDENTITY;
  vacuumDoorsRotationMatrix.rotate(m_cfg.vcRotationAngle, m_cfg.vcRotationAxis);
  G4Transform3D vacuumDoorsTransform(vacuumDoorsRotationMatrix,
                                     G4ThreeVector());
  solidVcVacuum->AddNode(solidVcDoorsVacuum, vacuumDoorsTransform);

  G4RotationMatrix vacuumWindowsRotation = G4RotationMatrix::IDENTITY;
  vacuumWindowsRotation.rotate(m_cfg.vcRotationAngle, m_cfg.vcRotationAxis);
  G4Transform3D vacuumWindowsTransform(vacuumWindowsRotation, G4ThreeVector());
  solidVcVacuum->AddNode(solidVcWindowsVacuum, vacuumWindowsTransform);

  G4RotationMatrix vacuumFlangeRotation = G4RotationMatrix::IDENTITY;
  vacuumFlangeRotation.rotate(m_cfg.vcRotationAngle, m_cfg.vcRotationAxis);
  G4Transform3D vacuumFlangeTransform(
      vacuumFlangeRotation,
      vacuumFlangeRotation * G4ThreeVector(m_cfg.vcc->vcFlangeCenterX,
                                           m_cfg.vcc->vcFlangeCenterY,
                                           m_cfg.vcc->vcFlangeCenterZ));
  solidVcVacuum->AddNode(solidVcFlangeVacuum, vacuumFlangeTransform);
  solidVcVacuum->Voxelize();

  // ---------------------------------------------------
  // Vacuum log/phys construction

  G4LogicalVolume *logicVcVacuum =
      new G4LogicalVolume(solidVcVacuum, vcVacuumMaterial, "VcVacuum");

  G4VisAttributes *vcVacuumVis = new G4VisAttributes(G4Color::Green());
  vcVacuumVis->SetVisibility(false);
  logicVcVacuum->SetVisAttributes(vcVacuumVis);

  G4RotationMatrix *vcVacuumRotation = new G4RotationMatrix();
  vcVacuumRotation->rotate(m_cfg.vcRotationAngle, m_cfg.vcRotationAxis);
  G4VPhysicalVolume *physVcVacuum = new G4PVPlacement(
      vcVacuumRotation,
      G4ThreeVector(m_cfg.vcCenterX, m_cfg.vcCenterY, m_cfg.vcCenterZ),
      logicVcVacuum, "VcVacuum", logicParent, false, 0, m_cfg.checkOverlaps);

  // ---------------------------------------------------
  // VC walls log/phys construction

  G4LogicalVolume *logicVcWalls =
      new G4LogicalVolume(solidVcWalls, vcWallsMaterial, "VcWalls");
  G4VisAttributes *vcWallsVis = new G4VisAttributes(G4Color::Blue());
  logicVcWalls->SetVisAttributes(vcWallsVis);

  G4VPhysicalVolume *physVcWalls =
      new G4PVPlacement(nullptr, G4ThreeVector(), logicVcWalls, "VcWalls",
                        logicVcVacuum, false, 0, m_cfg.checkOverlaps);

  // ---------------------------------------------------
  // VC doors log/phys construction

  G4LogicalVolume *logicVcDoors =
      new G4LogicalVolume(solidVcDoors, vcDoorsMaterial, "VcDoors");
  G4VisAttributes *vcexitVis = new G4VisAttributes(G4Color::Red());
  logicVcDoors->SetVisAttributes(vcexitVis);

  G4RotationMatrix *vcDoorsRotation = new G4RotationMatrix();
  vcDoorsRotation->rotate(-m_cfg.vcRotationAngle, m_cfg.vcRotationAxis);
  G4VPhysicalVolume *physVcDoors = new G4PVPlacement(
      vcDoorsRotation,
      G4ThreeVector(m_cfg.vcCenterX, m_cfg.vcCenterY, m_cfg.vcCenterZ),
      logicVcDoors, "VcDoors", logicVcVacuum, false, 0, m_cfg.checkOverlaps);

  // ---------------------------------------------------
  // VC windows log/phys construction

  G4LogicalVolume *logicVcWindows =
      new G4LogicalVolume(solidVcWindows, vcWindowsMaterial, "VcWindows");
  logicVcWindows->SetVisAttributes(vcexitVis);

  G4RotationMatrix *vcWindowsRotation = new G4RotationMatrix();
  vcWindowsRotation->rotate(-m_cfg.vcRotationAngle, m_cfg.vcRotationAxis);
  G4VPhysicalVolume *physVcWindows = new G4PVPlacement(
      vcWindowsRotation,
      G4ThreeVector(m_cfg.vcCenterX, m_cfg.vcCenterY, m_cfg.vcCenterZ),
      logicVcWindows, "VcWindows", logicVcVacuum, false, 0,
      m_cfg.checkOverlaps);

  // ---------------------------------------------------
  // VC flange log/phys construction

  G4LogicalVolume *logicVcFlangeWindow =
      new G4LogicalVolume(solidVcFlange, vcFlangeMaterial, "VcFlangeWindow");
  logicVcFlangeWindow->SetVisAttributes(vcexitVis);

  G4RotationMatrix *vcFlangeRotation = new G4RotationMatrix();
  vcFlangeRotation->rotate(-m_cfg.vcRotationAngle, m_cfg.vcRotationAxis);
  G4VPhysicalVolume *physVcFlangeWindow = new G4PVPlacement(
      vcFlangeRotation,
      (*vcFlangeRotation).inverse() * G4ThreeVector(m_cfg.vcc->vcFlangeCenterX,
                                                    m_cfg.vcc->vcFlangeCenterY,
                                                    m_cfg.vcc->vcFlangeCenterZ),
      logicVcFlangeWindow, "VcFlangeWindow", logicVcVacuum, false, 0,
      m_cfg.checkOverlaps);

  return physVcVacuum;
}

std::pair<G4VSolid *, G4VSolid *> VacuumChamberFactory::constructVcWalls() {
  // Construct the cylinder structure
  G4Tubs *solidVcCylinder =
      new G4Tubs("VcCylinder", m_cfg.vcc->vcCylinderRadMin,
                 m_cfg.vcc->vcCylinderRadMin + m_cfg.vcc->vcCylinderThickness,
                 m_cfg.vcc->vcCylinderHalfZ, 0 * rad, 2 * M_PI * rad);

  G4Tubs *solidVcVacuumCylinder =
      new G4Tubs("VcVacuumCylinder", 0,
                 m_cfg.vcc->vcCylinderRadMin + m_cfg.vcc->vcCylinderThickness,
                 m_cfg.vcc->vcCylinderHalfZ, 0 * rad, 2 * M_PI * rad);

  // Construct cut panels for the outer cylinder wall
  G4Box *solidVcCylinderCutout = new G4Box(
      "VcCylinderCutout", m_cfg.vcc->vcCylinderCutoutHalfX,
      m_cfg.vcc->vcCylinderCutoutHalfY, m_cfg.vcc->vcCylinderCutoutHalfZ);
  G4ThreeVector vcCylinderCutoutTranslation(
      0, 0, m_cfg.vcc->vcCylinderRadMin + m_cfg.vcc->vcCylinderThickness);
  G4RotationMatrix vcCylinderCutoutRotation = G4RotationMatrix::IDENTITY;

  G4MultiUnion *solidVcCylinderCutoutUnion =
      new G4MultiUnion("VcCylinderCutoutUnion");
  for (std::size_t i = 0; i < 8; i++) {
    G4Transform3D cutoutTranform(
        vcCylinderCutoutRotation,
        vcCylinderCutoutRotation * vcCylinderCutoutTranslation);
    solidVcCylinderCutoutUnion->AddNode(solidVcCylinderCutout, cutoutTranform);
    vcCylinderCutoutRotation.rotateY(m_cfg.vcc->vcExitAngleSpacing);
  }
  solidVcCylinderCutoutUnion->Voxelize();

  // Cut the outer panel structure from the cylinder wall
  G4RotationMatrix subtractionRotation;
  subtractionRotation.rotateX(M_PI_2);
  G4Transform3D subtractionTransform(subtractionRotation, G4ThreeVector());
  G4SubtractionSolid *solidVcCylinderSubtraction =
      new G4SubtractionSolid("VcCylinderSubtraction", solidVcCylinder,
                             solidVcCylinderCutoutUnion, subtractionTransform);
  G4SubtractionSolid *solidVcVacuum =
      new G4SubtractionSolid("VcVacuum", solidVcVacuumCylinder,
                             solidVcCylinderCutoutUnion, subtractionTransform);

  // Construct cut panels for the VC exit doors
  G4Box *solidVcDoorCutout =
      new G4Box("VcDoorCutout", m_cfg.vcc->vcDoorHalfX, m_cfg.vcc->vcDoorHalfY,
                m_cfg.vcc->vcCylinderThickness * 1.01);
  G4ThreeVector vcDoorCutoutTranslation(
      0, 0,
      (m_cfg.vcc->vcCylinderRadMin + m_cfg.vcc->vcCylinderRadMin +
       m_cfg.vcc->vcCylinderThickness) /
          2);
  G4RotationMatrix vcDoorCutoutRotation = G4RotationMatrix::IDENTITY;

  G4MultiUnion *solidVcDoorCutoutUnion = new G4MultiUnion("VcDoorCutoutUnion");
  for (std::size_t i = 0; i < 8; i++) {
    G4Transform3D cutoutTranform(
        vcDoorCutoutRotation, vcDoorCutoutRotation * vcDoorCutoutTranslation);
    solidVcDoorCutoutUnion->AddNode(solidVcDoorCutout, cutoutTranform);
    vcDoorCutoutRotation.rotateY(m_cfg.vcc->vcExitAngleSpacing);
  }
  solidVcDoorCutoutUnion->Voxelize();

  // Cut the door openings in the VC paneled walls
  G4SubtractionSolid *solidVcWalls =
      new G4SubtractionSolid("VcWalls", solidVcCylinderSubtraction,
                             solidVcDoorCutoutUnion, subtractionTransform);

  return {solidVcWalls, solidVcVacuum};
}

std::pair<G4VSolid *, G4VSolid *> VacuumChamberFactory::constructVcDoors() {
  // Construct the VC doors
  G4Box *solidVcDoor =
      new G4Box("VcDoor", m_cfg.vcc->vcDoorHalfX, m_cfg.vcc->vcDoorHalfY,
                m_cfg.vcc->vcDoorHalfZ);
  G4ThreeVector vcDoorTranslation(m_cfg.vcc->vcDoorCenterX,
                                  m_cfg.vcc->vcDoorCenterY,
                                  m_cfg.vcc->vcDoorCenterZ);
  G4RotationMatrix vcDoorRotation = G4RotationMatrix::IDENTITY;

  // Construct union of all the doors in the structure
  G4MultiUnion *solidVcDoorUnion = new G4MultiUnion("VcDoorUnion");
  for (std::size_t i = 0; i < 8; i++) {
    G4Transform3D doorTransform(vcDoorRotation,
                                vcDoorRotation * vcDoorTranslation);
    solidVcDoorUnion->AddNode(solidVcDoor, doorTransform);
    vcDoorRotation.rotateY(m_cfg.vcc->vcExitAngleSpacing);
  }
  solidVcDoorUnion->Voxelize();

  // Construct the VC window cutout
  G4Tubs *solidVcWindowCutoutBig =
      new G4Tubs("VcWindowCutoutBig", 0, m_cfg.vcc->bigVcWindowRad,
                 m_cfg.vcc->vcDoorHalfZ * 1.01, 0 * rad, 2 * M_PI * rad);

  G4Tubs *solidVcWindowCutoutSmall =
      new G4Tubs("VcWindowCutoutSmall", 0, m_cfg.vcc->smallVcWindowRad,
                 m_cfg.vcc->vcDoorHalfZ * 1.01, 0 * rad, 2 * M_PI * rad);

  G4ThreeVector vcWindowCutoutTranslation(m_cfg.vcc->vcDoorCenterX,
                                          m_cfg.vcc->vcDoorCenterY,
                                          m_cfg.vcc->vcDoorCenterZ);
  G4RotationMatrix vcWindowCutoutRotation = G4RotationMatrix::IDENTITY;

  G4MultiUnion *solidVcWindowCutoutUnion =
      new G4MultiUnion("VcWindowCutoutUnion");
  for (std::size_t i = 0; i < 8; i++) {
    G4Transform3D windowTransform(
        vcWindowCutoutRotation,
        vcWindowCutoutRotation * vcWindowCutoutTranslation);
    if (i == 0 || i == 7) {
      solidVcWindowCutoutUnion->AddNode(solidVcWindowCutoutBig,
                                        windowTransform);
    } else {
      solidVcWindowCutoutUnion->AddNode(solidVcWindowCutoutSmall,
                                        windowTransform);
    }
    vcWindowCutoutRotation.rotateY(m_cfg.vcc->vcExitAngleSpacing);
  }
  solidVcWindowCutoutUnion->Voxelize();

  // Cut the windows in the doors
  G4SubtractionSolid *solidVcDoors = new G4SubtractionSolid(
      "VcDoors", solidVcDoorUnion, solidVcWindowCutoutUnion);

  return {solidVcDoors, solidVcDoorUnion};
}

std::pair<G4VSolid *, G4VSolid *> VacuumChamberFactory::constructVcWindows() {
  // Construct VC windows
  G4Tubs *solidVcWindowBig =
      new G4Tubs("VcWindowBig", 0, m_cfg.vcc->bigVcWindowRad,
                 m_cfg.vcc->vcWindowHalfZ, 0 * rad, 2 * M_PI * rad);

  G4Tubs *solidVcWindowSmall =
      new G4Tubs("VcWindowSmall", 0, m_cfg.vcc->smallVcWindowRad,
                 m_cfg.vcc->vcWindowHalfZ, 0 * rad, 2 * M_PI * rad);

  G4ThreeVector vcWindowTranslation(m_cfg.vcc->vcWindowCenterX,
                                    m_cfg.vcc->vcWindowCenterY,
                                    m_cfg.vcc->vcWindowCenterZ);
  G4RotationMatrix vcWindowRotation = G4RotationMatrix::IDENTITY;

  G4MultiUnion *solidVcWindowUnion = new G4MultiUnion("VcWindowUnion");
  for (std::size_t i = 0; i < 8; i++) {
    G4Transform3D windowTransform(vcWindowRotation,
                                  vcWindowRotation * vcWindowTranslation);
    if (i == 0 || i == 7) {
      solidVcWindowUnion->AddNode(solidVcWindowBig, windowTransform);
    } else {
      solidVcWindowUnion->AddNode(solidVcWindowSmall, windowTransform);
    }
    vcWindowRotation.rotateY(m_cfg.vcc->vcExitAngleSpacing);
  }
  solidVcWindowUnion->Voxelize();

  // Construct VC window flange cutout
  G4Tubs *solidVcFlangeCutout =
      new G4Tubs("VcFlangeCutout", 0, m_cfg.vcc->vcFlangeRad,
                 m_cfg.vcc->vcWindowHalfZ * 1.01, 0 * rad, 2 * M_PI * rad);

  // Cut the windows in the doors
  G4RotationMatrix subtractionRotation = G4RotationMatrix::IDENTITY;
  G4Transform3D subtractionTransform(subtractionRotation, vcWindowTranslation);
  G4SubtractionSolid *solidVcWindows =
      new G4SubtractionSolid("VcWindows", solidVcWindowUnion,
                             solidVcFlangeCutout, subtractionTransform);

  return {solidVcWindows, solidVcWindowUnion};
}

std::pair<G4VSolid *, G4VSolid *> VacuumChamberFactory::constructVcFlange() {
  // ---------------------------------------------------
  // Vacuum exit flange construction

  // Construct VC flange
  G4Tubs *solidVcFlange =
      new G4Tubs("VcFlange", 0, m_cfg.vcc->vcFlangeRad,
                 m_cfg.vcc->vcFlangeHalfZ, 0 * rad, 2 * M_PI * rad);

  // Construct VC window flange cutout
  G4Box *solidVcFlangeFlangeCutout = new G4Box(
      "solidVcFlangeCutout", m_cfg.vcc->vcFlangeWindowHalfX,
      m_cfg.vcc->vcFlangeWindowHalfY,
      2 * (m_cfg.vcc->vcFlangeHalfZ - m_cfg.vcc->vcFlangeWindowHalfZ));

  // Cut the exit window in the flange
  G4RotationMatrix subtractionRotation = G4RotationMatrix::IDENTITY;
  G4Transform3D subtractionTransform(
      subtractionRotation, G4ThreeVector(0, 0, -m_cfg.vcc->vcFlangeHalfZ));
  G4SubtractionSolid *solidVcFlangeMilled =
      new G4SubtractionSolid("VcFlangeMilled", solidVcFlange,
                             solidVcFlangeFlangeCutout, subtractionTransform);

  return {solidVcFlangeMilled, solidVcFlange};
}
