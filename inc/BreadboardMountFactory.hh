#ifndef BreadboardMountFactory_h
#define BreadboardMountFactory_h

#include "G4AssemblyVolume.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4RunManager.hh"
#include "G4Types.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4VUserDetectorConstruction.hh"

class G4LogicalVolume;
class G4PhysicalVolume;

struct BreadboardMount {
  BreadboardMount();
  ~BreadboardMount() = default;

  static BreadboardMount *instance();

  std::string bmMaterial;

  G4double bmHalfX;
  G4double bmHalfY;
  G4double bmHalfZ;

  G4double bmSideFaceHalfLength;

  G4double bmTriangularCutoutX;
  G4double bmTriangularCutoutZ;
  G4double bmTriangularCutoutHypot;
  G4double bmTriangularCutoutRadSmall;
  G4double bmTriangularCutoutRadBig;

  G4double bmBoxCutoutHalfX;
  G4double bmBoxCutoutHalfY;
  G4double bmBoxCutoutHalfZ;

  G4double bmInsidesCutoutLiftX;

  G4double bmSpacerHalfX;
  G4double bmSpacerHalfY;
  G4double bmSpacerHalfZ;

  G4double bmSpacerBigHoleThroughRad;
  G4double bmSpacerBigHoleBackRad;
  G4double bmSpacerBigHoleBackHalfDepth;

  G4double bmSpacerSmallHoleRad;

  G4double bmClearanceSlotBigRad;
  G4double bmClearanceSlotSmallRad;
  G4double bmClearanceSlotHalfX;
  G4double bmClearanceSlotHalfY;
  G4double bmClearanceSlotHalfDepth;

  G4double holeSpacing;

 protected:
  static BreadboardMount *m_instance;
};

class BreadboardMountFactory {
 public:
  struct Config {
    /// Mount name
    std::string name;

    /// Mount position in the World
    G4double bmCenterX;
    G4double bmCenterY;
    G4double bmCenterZ;

    /// Mount rotation parameters in the World frame
    G4double bmRotationAngleX;
    G4double bmRotationAngleY;
    G4double bmRotationAngleZ;

    const BreadboardMount *bmc;

    /// Check overlaps flag
    G4bool checkOverlaps;
  };

  BreadboardMountFactory() = default;
  ~BreadboardMountFactory() = default;

  G4VPhysicalVolume *construct(G4LogicalVolume *logicParent, const Config &cfg);

 private:
  G4VSolid *contructTriangularCut(const Config &cfg);

  G4VSolid *constructSpacer(const Config &cfg);
};

#endif
