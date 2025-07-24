#include "BreadboardFactory.hh"

#include <cstddef>

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

  bbHalfX = 150 * mm;
  bbHalfY = 225 * mm;
  // bbHalfY = 125 * mm;
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
                                                const std::string &name) {
  G4Material *aluminium =
      G4NistManager::Instance()->FindOrBuildMaterial(m_cfg.bbc->bbMaterial);

  G4VisAttributes *alVis = new G4VisAttributes(G4Color::White());

  // Construct the solid breadboard
  G4Box *solidBreadboardBox = new G4Box("BreadboardBox", m_cfg.bbc->bbHalfX,
                                        m_cfg.bbc->bbHalfY, m_cfg.bbc->bbHalfZ);

  G4Tubs *solidBreadboardHole =
      new G4Tubs("BreadboardHole", 0, m_cfg.bbc->holeRad,
                 m_cfg.bbc->bbHalfZ * 1.01, 0, 2 * M_PI);

  G4MultiUnion *solidBreadboardHoleUnion =
      new G4MultiUnion("BreadboardHoleUnion");
  int nHolesX = m_cfg.bbc->bbHalfX / m_cfg.bbc->holeHalfSpacing;
  int nHolesY = m_cfg.bbc->bbHalfY / m_cfg.bbc->holeHalfSpacing;

  for (std::size_t i = 0; i < nHolesX; i++) {
    G4double xTranslation = -m_cfg.bbc->bbHalfX + m_cfg.bbc->holeHalfSpacing +
                            2 * i * m_cfg.bbc->holeHalfSpacing;
    for (std::size_t j = 0; j < nHolesY; j++) {
      G4double yTranslation = -m_cfg.bbc->bbHalfY + m_cfg.bbc->holeHalfSpacing +
                              2 * j * m_cfg.bbc->holeHalfSpacing;

      solidBreadboardHoleUnion->AddNode(
          solidBreadboardHole,
          G4Transform3D(G4RotationMatrix::IDENTITY,
                        G4ThreeVector(xTranslation, yTranslation, 0)));
    }
  }
  solidBreadboardHoleUnion->Voxelize();

  G4SubtractionSolid *solidBreadboard = new G4SubtractionSolid(
      name, solidBreadboardBox, solidBreadboardHoleUnion);

  G4LogicalVolume *logicBreadboard =
      new G4LogicalVolume(solidBreadboard, aluminium, name);
  logicBreadboard->SetVisAttributes(alVis);

  G4RotationMatrix *breadboardRotation = new G4RotationMatrix();
  breadboardRotation->rotate(m_cfg.bbRotationAngle, m_cfg.bbRotationAxis);
  G4VPhysicalVolume *physBreadboard = new G4PVPlacement(
      breadboardRotation,
      G4ThreeVector(m_cfg.bbCenterX, m_cfg.bbCenterY, m_cfg.bbCenterZ),
      logicBreadboard, name, logicParent, false, 0, m_cfg.checkOverlaps);

  return physBreadboard;
}
