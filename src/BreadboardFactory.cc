#include "BreadboardFactory.hh"

#include <cstddef>

#include <G4Transform3D.hh>
#include <G4UnionSolid.hh>

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

Breadboard *Breadboard::m_instance = nullptr;

Breadboard::Breadboard() {
  bbMaterial = "G4_Al";

  bbHalfX = 75 * mm;
  bbHalfY = 180 * mm;
  bbHalfZ = 6.35 * mm;

  holeRad = 3 * mm;
  holeHalfSpacing = 12.5 * mm;
}

Breadboard *Breadboard::instance() {
  if (!m_instance) {
    m_instance = new Breadboard();
  }
  return m_instance;
}

}  // namespace GeometryConstants

G4VPhysicalVolume *BreadboardFactory::construct(G4LogicalVolume *logicParent,
                                                const Config &cfg) {
  G4Material *aluminium =
      G4NistManager::Instance()->FindOrBuildMaterial(cfg.bbc->bbMaterial);

  G4VisAttributes *alVis = new G4VisAttributes(G4Color::White());

  // Construct the solid breadboard
  G4Box *solidBreadboardBox = new G4Box("BreadboardBox", cfg.bbc->bbHalfX,
                                        cfg.bbc->bbHalfY, cfg.bbc->bbHalfZ);

  G4Tubs *solidBreadboardHole =
      new G4Tubs("BreadboardHole", 0, cfg.bbc->holeRad, cfg.bbc->bbHalfZ * 1.01,
                 0, 2 * M_PI);

  G4MultiUnion *solidBreadboardHoleUnion =
      new G4MultiUnion("BreadboardHoleUnion");

  int nHolesX = cfg.bbc->bbHalfX / cfg.bbc->holeHalfSpacing;
  int nHolesY = cfg.bbc->bbHalfY / cfg.bbc->holeHalfSpacing;

  for (std::size_t i = 0; i < nHolesX; i++) {
    G4double xTranslation = -cfg.bbc->bbHalfX + cfg.bbc->holeHalfSpacing +
                            2 * i * cfg.bbc->holeHalfSpacing;
    for (std::size_t j = 0; j < nHolesY; j++) {
      G4double yTranslation = -cfg.bbc->bbHalfY + cfg.bbc->holeHalfSpacing +
                              2 * j * cfg.bbc->holeHalfSpacing;

      solidBreadboardHoleUnion->AddNode(
          solidBreadboardHole,
          G4Transform3D(G4RotationMatrix::IDENTITY,
                        G4ThreeVector(xTranslation, yTranslation, 0)));
    }
  }
  solidBreadboardHoleUnion->Voxelize();

  // Add double density spacing
  G4UnionSolid *ddSolidBreadboardHoleUnion = new G4UnionSolid(
      "DoubleDensityBreadboardHoleUnion", solidBreadboardHoleUnion,
      solidBreadboardHoleUnion,
      G4Transform3D(
          G4RotationMatrix::IDENTITY,
          G4ThreeVector(-cfg.bbc->holeHalfSpacing, cfg.bbc->holeHalfSpacing)));

  G4SubtractionSolid *solidBreadboard = new G4SubtractionSolid(
      cfg.name, solidBreadboardBox, ddSolidBreadboardHoleUnion);

  G4LogicalVolume *logicBreadboard =
      new G4LogicalVolume(solidBreadboard, aluminium, cfg.name);
  logicBreadboard->SetVisAttributes(alVis);

  G4RotationMatrix *breadboardRotation = new G4RotationMatrix();
  breadboardRotation->rotateX(cfg.bbRotationAngleX);
  breadboardRotation->rotateY(cfg.bbRotationAngleY);
  breadboardRotation->rotateZ(cfg.bbRotationAngleZ);
  G4VPhysicalVolume *physBreadboard = new G4PVPlacement(
      breadboardRotation,
      G4ThreeVector(cfg.bbCenterX, cfg.bbCenterY, cfg.bbCenterZ),
      logicBreadboard, cfg.name, logicParent, false, 0, cfg.checkOverlaps);

  return physBreadboard;
}
