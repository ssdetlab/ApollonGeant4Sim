#include <cstddef>

#include "G4MultiUnion.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Trap.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4VisAttributes.hh"
#include "TrackingChamberSupportFactory.hh"

namespace GeometryConstants {

TrackingChamberSupport *TrackingChamberSupport::m_instance = nullptr;

TrackingChamberSupport::TrackingChamberSupport() {
  tcsMaterial = "G4_Al";

  tcsBottomPlaneHalfZ = 127 * mm;
  tcsTopPlaneHalfZ = 29 * mm;
  tcsHalfY = 18.5;
  tcsHalfZ = 298.5;

  holeRad = 3 * mm;
  holeHalfSpacing = 12.5 * mm;
}

TrackingChamberSupport *TrackingChamberSupport::instance() {
  if (!m_instance) {
    m_instance = new TrackingChamberSupport();
  }
  return m_instance;
}

}  // namespace GeometryConstants

G4VPhysicalVolume *TrackingChamberSupportFactory::construct(
    G4LogicalVolume *logicParent, const std::string &name) {
  G4Material *aluminium =
      G4NistManager::Instance()->FindOrBuildMaterial(m_cfg.tcsc->tcsMaterial);

  G4VisAttributes *alVis = new G4VisAttributes(G4Color::White());

  // Construct the solid breadboard
  G4Trap *solidTrackingChamberSupportTrap = new G4Trap(
      "TrackingChamberSupportTrap", m_cfg.tcsc->tcsHalfZ, m_cfg.tcsc->tcsHalfY,
      m_cfg.tcsc->tcsBottomPlaneHalfZ, m_cfg.tcsc->tcsTopPlaneHalfZ);

  G4LogicalVolume *logicTrackingChamberSupport =
      new G4LogicalVolume(solidTrackingChamberSupportTrap, aluminium, name);
  logicTrackingChamberSupport->SetVisAttributes(alVis);

  G4RotationMatrix *tcsRotation = new G4RotationMatrix();
  tcsRotation->rotate(m_cfg.tcsRotationAngle, m_cfg.tcsRotationAxis);
  G4VPhysicalVolume *physTrackingChamberSupport = new G4PVPlacement(
      tcsRotation,
      G4ThreeVector(m_cfg.tcsCenterX, m_cfg.tcsCenterY, m_cfg.tcsCenterZ),
      logicTrackingChamberSupport, name, logicParent, false, 0,
      m_cfg.checkOverlaps);

  return physTrackingChamberSupport;
}
