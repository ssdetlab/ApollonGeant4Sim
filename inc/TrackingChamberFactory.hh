#ifndef TrackingChamberFactory_h
#define TrackingChamberFactory_h

#include "G4AssemblyVolume.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4VUserDetectorConstruction.hh"

class G4LogicalVolume;
class G4PhysicalVolume;

namespace GeometryConstants {

struct TrackingChamber {
  TrackingChamber();
  ~TrackingChamber() = default;

  static TrackingChamber *instance();

  G4double inch;

  std::string trackerContainerMaterial;
  std::string trackerPCBMaterial;
  std::string trackerMaterial;
  std::string trackerNinePcbConMaterial;

  G4double OPPPSensorX;
  G4double OPPPSensorY;
  G4double OPPPSensorZ;

  G4double OPPPSensorNCellX;
  G4double OPPPSensorNCellY;
  G4double OPPPSensorPixelX;
  G4double OPPPSensorPixelY;
  G4double OPPPSensorPixelZ;

  ////// Tracker prototype
  G4double ProtoTrackerLDOBoxX;
  G4double ProtoTrackerLDOBoxY;
  G4double ProtoTrackerLDOBoxZ;
  G4double ProtoTrackerLDOBoxD;
  G4double ProtoTrackerLDOBoxXpos;
  G4double ProtoTrackerLDOPCBY;
  G4double ProtoTrackerLDOPCBYpos;

  G4String ProtoTrackerBoxMaterial;
  G4double ProtoTrackerYpos;  // distance in Y between bottom of the box and
                              // bottom of the window (-2 works -5 not).
  G4double ProtoTrackerWinFPnaeldz;  // distance in Z between upsteram side of
                                     // the window panel of PDC and downstrem
                                     // panel of prototype box.

  G4double ProtoTrackerBoxCutX;  // Container cut in X
  G4double ProtoTrackerBoxCutY;  // Container size after cut for the beam pipe

  G4double ProtoTrkCarrierPcbX;
  G4double ProtoTrkCarrierPcbY;
  G4double ProtoTrkCarrierPcbZ;
  G4double ProtoTrkCarrierPcbBCutX;
  G4double ProtoTrkCarrierPcbBCutY;

  G4double ProtoTrkCarrierPcbACutXY;
  G4double ProtoTrkCarrierPcbCutX;
  G4double ProtoTrkCarrierPcbCutY;
  G4double ProtoTrkCarrierPcbCutXpos;
  G4double ProtoTrkCarrierPcbCutYpos;
  G4double ProtoTrkCarrierSensorOffset;
  G4double ProtoTrackerCarrierHoldD;
  G4double ProtoTrackerCarrierHoldZ;
  G4String ProtoTrackerPEEKMaterial;

  G4double ProtoTrackerFBPanelX;
  G4double ProtoTrackerFBPanelY;
  G4double ProtoTrackerFBPanelZ;
  G4double ProtoTrackerFBPanelCutX;
  G4double ProtoTrackerFBPanelCutY;
  G4double
      ProtoTrackerFBPanelCutXpos;  // The cut is in the middle in x direction
  G4double ProtoTrackerFBPanelCutYpos;
  G4double ProtoTrckKaptonCoverZ;

  G4double ProtoTrackerHoldPanelX;
  G4double ProtoTrackerHoldPanelY;
  G4double ProtoTrackerHoldPanelZ;

  G4double ProtoTrackerBottomPanelX;
  G4double ProtoTrackerBottomPanelY;
  G4double ProtoTrackerBottomPanelZ;
  G4double ProtoTrackerBottomPanelYposShift;

  G4double ProtoTrackerSidePanelX;
  G4double ProtoTrackerSidePanelY;
  G4double ProtoTrackerSidePanelZ;

  G4double ProtoTrackerLConnectX;
  G4double ProtoTrackerLConnectYZ;
  G4double ProtoTrackerLConnectD;

  G4double ProtoTrackerNinePcbX;
  G4double ProtoTrackerNinePcbY;
  G4double ProtoTrackerNinePcbZ;
  G4double ProtoTrackerNinePcbConX;
  G4double ProtoTrackerNinePcbConY;
  G4double ProtoTrackerNinePcbConZ;
  G4double ProtoTrackerNinePcbConD;
  G4double ProtoTrackerNinePcbConZpos;
  G4double ProtoTrackerLayerDZ;

  G4double ProtoTrackerPcieConX;
  G4double ProtoTrackerPcieConY;
  G4double ProtoTrackerPcieConZ;
  G4double ProtoTrackerPcieConGapY;

  G4double ProtoTrackerSupportX;
  G4double ProtoTrackerSupportY;
  G4double ProtoTrackerSupportZ;
  G4double ProtoTrackerSupportD;
  G4double ProtoTrackerSupSpacerX;
  G4double ProtoTrackerSupSpacerY;
  G4double ProtoTrackerSupSpacerZ;
  G4double ProtoTrackerSupHolderX;
  G4double ProtoTrackerSupHolderY;
  G4double ProtoTrackerSupHolderZ;

  G4double trackingChamberHalfX;
  G4double trackingChamberHalfY;
  G4double trackingChamberHalfZ;

 protected:
  static TrackingChamber *m_instance;
};

}  // namespace GeometryConstants

class TrackingChamberFactory {
 public:
  struct Config {
    /// TC position in the World
    G4double tcCenterX;
    G4double tcCenterY;
    G4double tcCenterZ;

    /// TC rotation parameters in the World frame
    G4double tcRotationAngle;
    G4ThreeVector tcRotationAxis;

    const GeometryConstants::TrackingChamber *tcc;

    /// Check overlaps flag
    G4bool checkOverlaps;
  };

  TrackingChamberFactory(const Config &cfg) : m_cfg(cfg) {};
  ~TrackingChamberFactory() = default;

  G4VPhysicalVolume *construct(G4LogicalVolume *logicParent,
                               const std::string &name);

 private:
  G4LogicalVolume *constructSensor();

  G4LogicalVolume *constructLConnector();

  G4AssemblyVolume *constructNineAlpidePCB();

  G4LogicalVolume *constructCarrierPCB(G4double &carrierPcbContainerY);

  Config m_cfg;
};

#endif
