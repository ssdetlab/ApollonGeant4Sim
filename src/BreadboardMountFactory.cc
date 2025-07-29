#include "BreadboardMountFactory.hh"

#include <G4ExtrudedSolid.hh>
#include <G4MultiUnion.hh>
#include <G4Para.hh>
#include <G4Transform3D.hh>
#include <G4Tubs.hh>
#include <G4TwoVector.hh>
#include <G4UnionSolid.hh>

#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4VisAttributes.hh"

namespace GeometryConstants {

BreadboardMount *BreadboardMount::m_instance = nullptr;

BreadboardMount::BreadboardMount() {
  bmMaterial = "G4_Al";
  bmHalfX = 31.75 * mm;
  bmHalfY = 36.6 * mm;
  bmHalfZ = bmHalfX;

  bmSideFaceHalfLength = 4.5 * mm;

  bmTriangularCutoutX = 27.43 * mm;
  bmTriangularCutoutZ = bmTriangularCutoutX;
  bmTriangularCutoutHypot = 43.28 * mm;
  bmTriangularCutoutRadSmall = 3.17 * mm;
  bmTriangularCutoutRadBig = 6.35 * mm;

  bmBoxCutoutHalfX = 18.48 * mm;
  bmBoxCutoutHalfY = 9.525 * mm;
  bmBoxCutoutHalfZ = 6.285 * mm;

  bmInsidesCutoutLiftX = 8.76 * mm;

  bmSpacerHalfX = 37.5 * mm;
  bmSpacerHalfY = 25 * mm;
  bmSpacerHalfZ = 12.5 * mm;

  bmSpacerBigHoleThroughRad = 3.37 * mm;
  bmSpacerBigHoleBackRad = 5.16 * mm;
  bmSpacerBigHoleBackHalfDepth = 3.43 * mm;

  bmSpacerSmallHoleRad = 3 * mm;

  bmClearanceSlotBigRad = 6.48 * mm;
  bmClearanceSlotSmallRad = 3.37 * mm;
  bmClearanceSlotHalfX = bmClearanceSlotSmallRad;
  bmClearanceSlotHalfY = 15.875 * mm;
  bmClearanceSlotHalfDepth = 4.19 * mm;

  holeSpacing = 12.5 * mm;
}

BreadboardMount *BreadboardMount::instance() {
  if (!m_instance) {
    m_instance = new BreadboardMount();
  }
  return m_instance;
}

}  // namespace GeometryConstants

G4VPhysicalVolume *BreadboardMountFactory::construct(
    G4LogicalVolume *logicParent, const Config &cfg) {
  G4Material *aluminium =
      G4NistManager::Instance()->FindOrBuildMaterial(cfg.bmc->bmMaterial);

  G4VisAttributes *alVis = new G4VisAttributes(G4Color::White());

  // Construct the box to be cut
  G4Box *solidMountBox = new G4Box("MountBox", cfg.bmc->bmHalfX,
                                   cfg.bmc->bmHalfY, cfg.bmc->bmHalfZ);

  // Constuct the off-diagonal cut box subtraction
  G4double ldiff = 2 * (cfg.bmc->bmHalfX - cfg.bmc->bmSideFaceHalfLength);
  G4double cutoutHalfX = ldiff / (2 * std::cos(M_PI_4));
  G4double cutoutHalfY = cfg.bmc->bmHalfY * 1.01;
  G4double cutoutHalfZ = std::sqrt(ldiff * ldiff - cutoutHalfX * cutoutHalfX);
  G4Box *solidMountHalfCutout =
      new G4Box("MountHalfCutout", cutoutHalfX, cutoutHalfY, cutoutHalfZ);

  // Subtract half-box off-diagonally
  G4RotationMatrix halfBoxCutoutRotation = G4RotationMatrix::IDENTITY;
  halfBoxCutoutRotation.rotateY(-M_PI_4);
  G4ThreeVector halfBoxCutoutTranslation(cfg.bmc->bmHalfX, 0, cfg.bmc->bmHalfZ);
  G4SubtractionSolid *solidMountHalfSubtracted = new G4SubtractionSolid(
      cfg.name, solidMountBox, solidMountHalfCutout,
      G4Transform3D(halfBoxCutoutRotation, halfBoxCutoutTranslation));

  // Construct the back-side rectangular cutouts
  G4Box *solidInsidesBoxCutout =
      new G4Box("InsidesBoxCutout", cfg.bmc->bmBoxCutoutHalfX,
                2 * cfg.bmc->bmBoxCutoutHalfY, cfg.bmc->bmBoxCutoutHalfZ);

  // Make a symmetric union
  G4UnionSolid *solidInsidesBoxCutoutUnion = new G4UnionSolid(
      "InsidesBoxCutoutUnion", solidInsidesBoxCutout, solidInsidesBoxCutout,
      G4Transform3D(G4RotationMatrix::IDENTITY,
                    G4ThreeVector(0, 2 * cfg.bmc->bmHalfY, 0)));

  // Subtract the back-cuts
  G4SubtractionSolid *solidMountBoxSubtracted = new G4SubtractionSolid(
      cfg.name, solidMountHalfSubtracted, solidInsidesBoxCutoutUnion,
      G4Transform3D(
          G4RotationMatrix::IDENTITY,
          G4ThreeVector(-cfg.bmc->bmHalfX + cfg.bmc->bmBoxCutoutHalfX +
                            cfg.bmc->bmInsidesCutoutLiftX,
                        -cfg.bmc->bmHalfY,
                        -(cfg.bmc->bmHalfZ - cfg.bmc->bmBoxCutoutHalfZ))));

  // Construct the smoothed triangle cutout
  G4VSolid *solidCutTriangle = contructTriangularCut(cfg);

  // Cut the insides of the mount with the smoothed triangle
  G4RotationMatrix smoothedCutTriangeRotation = G4RotationMatrix::IDENTITY;
  smoothedCutTriangeRotation.rotateY(M_PI_2);
  smoothedCutTriangeRotation.rotateZ(M_PI_2);
  smoothedCutTriangeRotation.rotateY(M_PI_2);
  G4SubtractionSolid *solidAngleMount = new G4SubtractionSolid(
      "AngleMount", solidMountBoxSubtracted, solidCutTriangle,
      G4Transform3D(
          smoothedCutTriangeRotation,
          G4ThreeVector(-cfg.bmc->bmHalfX + cfg.bmc->bmInsidesCutoutLiftX +
                            2 * cfg.bmc->bmBoxCutoutHalfX -
                            cfg.bmc->bmTriangularCutoutRadSmall,
                        0,
                        -(cfg.bmc->bmHalfZ - 2 * cfg.bmc->bmBoxCutoutHalfZ))));

  G4VSolid *solidSpacer = constructSpacer(cfg);

  G4RotationMatrix spacerRotation = G4RotationMatrix::IDENTITY;
  spacerRotation.rotateY(M_PI_2);
  G4UnionSolid *solidMount = new G4UnionSolid(
      cfg.name, solidAngleMount, solidSpacer,
      G4Transform3D(
          spacerRotation,
          G4ThreeVector(-cfg.bmc->bmHalfX - cfg.bmc->bmSpacerHalfZ, 0, 0)));

  G4LogicalVolume *logicMount =
      new G4LogicalVolume(solidMount, aluminium, cfg.name);
  G4VisAttributes *testVis = new G4VisAttributes(G4Color::White());
  testVis->SetForceCloud();
  logicMount->SetVisAttributes(testVis);

  G4RotationMatrix *mountRotation = new G4RotationMatrix();
  mountRotation->rotateX(cfg.bmRotationAngleX);
  mountRotation->rotateY(cfg.bmRotationAngleY);
  mountRotation->rotateZ(cfg.bmRotationAngleZ);
  G4VPhysicalVolume *physMount = new G4PVPlacement(
      mountRotation, G4ThreeVector(cfg.bmCenterX, cfg.bmCenterY, cfg.bmCenterZ),
      logicMount, cfg.name, logicParent, false, 0, cfg.checkOverlaps);

  return physMount;
}

G4VSolid *BreadboardMountFactory::contructTriangularCut(const Config &cfg) {
  double r1 = cfg.bmc->bmTriangularCutoutRadSmall;
  double r2 = cfg.bmc->bmTriangularCutoutRadBig;
  double l1 = cfg.bmc->bmTriangularCutoutHypot;
  double l2 = cfg.bmc->bmTriangularCutoutZ;

  G4TwoVector vertex0(0, -r1);
  G4TwoVector vertex1(-r1 * std::cos(M_PI_4), r1 * std::cos(M_PI_4));
  G4TwoVector vertex2((l1 - r1) * std::cos(M_PI_4),
                      (l1 + r1) * std::cos(M_PI_4));
  G4TwoVector vertex3(l2 + r2, l2 + r2 - r1);
  G4TwoVector vertex4(l2 + r2, r2 - r1);
  G4TwoVector vertex5(l2, r2 - r1);
  G4TwoVector vertex6(l2, -r1);

  G4Tubs *smallCylinder = new G4Tubs("CutTriangleCylinderSmall", 0, r1,
                                     cfg.bmc->bmHalfY * 1.01, 0, 2 * M_PI);

  G4Tubs *bigCylinder = new G4Tubs("CutTriangleCylinderBig", 0, r2,
                                   cfg.bmc->bmHalfY * 1.01, 0, 2 * M_PI);

  G4ExtrudedSolid *solidCutTriangle = new G4ExtrudedSolid(
      "CutTriangle",
      {vertex0, vertex1, vertex2, vertex3, vertex4, vertex5, vertex6},
      cfg.bmc->bmHalfY * 1.01);

  G4MultiUnion *solidSmoothedCutTriangle =
      new G4MultiUnion("SmoothedCutTriange");
  solidSmoothedCutTriangle->AddNode(
      solidCutTriangle,
      G4Transform3D(G4RotationMatrix::IDENTITY, G4ThreeVector()));

  solidSmoothedCutTriangle->AddNode(
      smallCylinder,
      G4Transform3D(G4RotationMatrix::IDENTITY, G4ThreeVector()));

  solidSmoothedCutTriangle->AddNode(
      smallCylinder,
      G4Transform3D(G4RotationMatrix::IDENTITY,
                    G4ThreeVector(l2 + r2 - r1, l2 + r2 - r1, 0)));

  solidSmoothedCutTriangle->AddNode(
      bigCylinder,
      G4Transform3D(G4RotationMatrix::IDENTITY, G4ThreeVector(l2, r2 - r1, 0)));

  solidSmoothedCutTriangle->Voxelize();

  return solidSmoothedCutTriangle;
}

G4VSolid *BreadboardMountFactory::constructSpacer(const Config &cfg) {
  G4Box *solidSpacerBox =
      new G4Box("SpacerBox", cfg.bmc->bmSpacerHalfX, cfg.bmc->bmSpacerHalfY,
                cfg.bmc->bmSpacerHalfZ);

  // Construct small hole pattern
  G4Tubs *solidSmallHoleCutout =
      new G4Tubs("SmallHoleCutout", 0, cfg.bmc->bmSpacerSmallHoleRad,
                 cfg.bmc->bmSpacerHalfZ * 1.01, 0, 2 * M_PI);

  G4MultiUnion *solidSmallHoleCutoutUnion =
      new G4MultiUnion("SmallHoleCutoutUnion");

  G4RotationMatrix smallHoleCutoutRotation = G4RotationMatrix::IDENTITY;
  solidSmallHoleCutoutUnion->AddNode(
      solidSmallHoleCutout,
      G4Transform3D(
          smallHoleCutoutRotation,
          G4ThreeVector(-cfg.bmc->holeSpacing, -cfg.bmc->holeSpacing, 0)));

  solidSmallHoleCutoutUnion->AddNode(
      solidSmallHoleCutout,
      G4Transform3D(smallHoleCutoutRotation,
                    G4ThreeVector(-cfg.bmc->holeSpacing, 0, 0)));

  solidSmallHoleCutoutUnion->AddNode(
      solidSmallHoleCutout,
      G4Transform3D(
          smallHoleCutoutRotation,
          G4ThreeVector(-cfg.bmc->holeSpacing, cfg.bmc->holeSpacing, 0)));

  solidSmallHoleCutoutUnion->AddNode(
      solidSmallHoleCutout,
      G4Transform3D(
          smallHoleCutoutRotation,
          G4ThreeVector(cfg.bmc->holeSpacing, -cfg.bmc->holeSpacing, 0)));

  solidSmallHoleCutoutUnion->AddNode(
      solidSmallHoleCutout,
      G4Transform3D(smallHoleCutoutRotation,
                    G4ThreeVector(cfg.bmc->holeSpacing, 0, 0)));

  solidSmallHoleCutoutUnion->AddNode(
      solidSmallHoleCutout,
      G4Transform3D(
          smallHoleCutoutRotation,
          G4ThreeVector(cfg.bmc->holeSpacing, cfg.bmc->holeSpacing, 0)));

  solidSmallHoleCutoutUnion->Voxelize();

  // Subtract small hole pattern
  G4SubtractionSolid *solidSpacerSmallHolePatterned = new G4SubtractionSolid(
      "SpacerSmallHolePatterned", solidSpacerBox, solidSmallHoleCutoutUnion);

  // Construct big hole pattern
  G4Tubs *solidBigHoleThroughCutout =
      new G4Tubs("BigHoleThroughCutout", 0, cfg.bmc->bmSpacerBigHoleThroughRad,
                 cfg.bmc->bmSpacerHalfZ * 1.01, 0, 2 * M_PI);

  G4Tubs *solidBigHoleBackCutout =
      new G4Tubs("BigHoleBackCutout", 0, cfg.bmc->bmSpacerBigHoleBackRad,
                 2 * cfg.bmc->bmSpacerBigHoleBackHalfDepth, 0, 2 * M_PI);

  G4MultiUnion *solidBigHoleCutoutUnion =
      new G4MultiUnion("BigHoleCutoutUnion");

  G4RotationMatrix bigHoleCutoutRotation = G4RotationMatrix::IDENTITY;
  solidBigHoleCutoutUnion->AddNode(
      solidBigHoleThroughCutout,
      G4Transform3D(bigHoleCutoutRotation,
                    G4ThreeVector(0, -cfg.bmc->holeSpacing, 0)));

  solidBigHoleCutoutUnion->AddNode(
      solidBigHoleThroughCutout,
      G4Transform3D(bigHoleCutoutRotation, G4ThreeVector(0, 0, 0)));

  solidBigHoleCutoutUnion->AddNode(
      solidBigHoleThroughCutout,
      G4Transform3D(bigHoleCutoutRotation,
                    G4ThreeVector(0, cfg.bmc->holeSpacing, 0)));

  solidBigHoleCutoutUnion->AddNode(
      solidBigHoleBackCutout,
      G4Transform3D(
          bigHoleCutoutRotation,
          G4ThreeVector(0, -cfg.bmc->holeSpacing, -cfg.bmc->bmSpacerHalfZ)));

  solidBigHoleCutoutUnion->AddNode(
      solidBigHoleBackCutout,
      G4Transform3D(bigHoleCutoutRotation,
                    G4ThreeVector(0, 0, -cfg.bmc->bmSpacerHalfZ)));

  solidBigHoleCutoutUnion->AddNode(
      solidBigHoleBackCutout,
      G4Transform3D(
          bigHoleCutoutRotation,
          G4ThreeVector(0, cfg.bmc->holeSpacing, -cfg.bmc->bmSpacerHalfZ)));

  solidBigHoleCutoutUnion->Voxelize();

  // Subtract big hole pattern
  G4SubtractionSolid *solidSpacerHolePatterned = new G4SubtractionSolid(
      "SpacerSmallHolePatterned", solidSpacerSmallHolePatterned,
      solidBigHoleCutoutUnion);

  // Construct through clearance slot
  G4Box *solidThroughClearanceSlotBox =
      new G4Box("ThroughClearanceSlotBox", cfg.bmc->bmClearanceSlotSmallRad,
                cfg.bmc->bmClearanceSlotHalfY, cfg.bmc->bmSpacerHalfZ * 1.01);

  G4Tubs *solidThroughClearanceSlotCylinder = new G4Tubs(
      "ThroughClearanceSlotCylinder", 0, cfg.bmc->bmClearanceSlotSmallRad,
      cfg.bmc->bmSpacerHalfZ * 1.01, 0, 2 * M_PI);

  G4Box *solidTopClearanceSlotBox = new G4Box(
      "TopClearanceSlotBox", cfg.bmc->bmClearanceSlotBigRad,
      cfg.bmc->bmClearanceSlotHalfY, 2 * cfg.bmc->bmClearanceSlotHalfDepth);

  G4Tubs *solidTopClearanceSlotCylinder =
      new G4Tubs("TopClearanceSlotCylinder", 0, cfg.bmc->bmClearanceSlotBigRad,
                 2 * cfg.bmc->bmClearanceSlotHalfDepth, 0, 2 * M_PI);

  G4MultiUnion *solidClearanceSlotCutout =
      new G4MultiUnion("ThroughClearanceSlotCutoutUnion");

  G4RotationMatrix clearanceSlotRotation = G4RotationMatrix::IDENTITY;
  solidClearanceSlotCutout->AddNode(
      solidThroughClearanceSlotBox,
      G4Transform3D(clearanceSlotRotation,
                    G4ThreeVector(-2 * cfg.bmc->holeSpacing, 0, 0)));

  solidClearanceSlotCutout->AddNode(
      solidThroughClearanceSlotCylinder,
      G4Transform3D(clearanceSlotRotation,
                    G4ThreeVector(-2 * cfg.bmc->holeSpacing,
                                  cfg.bmc->bmClearanceSlotHalfY, 0)));

  solidClearanceSlotCutout->AddNode(
      solidThroughClearanceSlotCylinder,
      G4Transform3D(clearanceSlotRotation,
                    G4ThreeVector(-2 * cfg.bmc->holeSpacing,
                                  -cfg.bmc->bmClearanceSlotHalfY, 0)));

  solidClearanceSlotCutout->AddNode(
      solidTopClearanceSlotBox,
      G4Transform3D(
          clearanceSlotRotation,
          G4ThreeVector(-2 * cfg.bmc->holeSpacing, 0, cfg.bmc->bmSpacerHalfZ)));

  solidClearanceSlotCutout->AddNode(
      solidTopClearanceSlotCylinder,
      G4Transform3D(clearanceSlotRotation,
                    G4ThreeVector(-2 * cfg.bmc->holeSpacing,
                                  cfg.bmc->bmClearanceSlotHalfY,
                                  cfg.bmc->bmSpacerHalfZ)));

  solidClearanceSlotCutout->AddNode(
      solidTopClearanceSlotCylinder,
      G4Transform3D(clearanceSlotRotation,
                    G4ThreeVector(-2 * cfg.bmc->holeSpacing,
                                  -cfg.bmc->bmClearanceSlotHalfY,
                                  cfg.bmc->bmSpacerHalfZ)));

  solidClearanceSlotCutout->AddNode(
      solidThroughClearanceSlotBox,
      G4Transform3D(clearanceSlotRotation,
                    G4ThreeVector(2 * cfg.bmc->holeSpacing, 0, 0)));

  solidClearanceSlotCutout->AddNode(
      solidThroughClearanceSlotCylinder,
      G4Transform3D(clearanceSlotRotation,
                    G4ThreeVector(2 * cfg.bmc->holeSpacing,
                                  cfg.bmc->bmClearanceSlotHalfY, 0)));

  solidClearanceSlotCutout->AddNode(
      solidThroughClearanceSlotCylinder,
      G4Transform3D(clearanceSlotRotation,
                    G4ThreeVector(2 * cfg.bmc->holeSpacing,
                                  -cfg.bmc->bmClearanceSlotHalfY, 0)));

  solidClearanceSlotCutout->AddNode(
      solidTopClearanceSlotBox,
      G4Transform3D(
          clearanceSlotRotation,
          G4ThreeVector(2 * cfg.bmc->holeSpacing, 0, cfg.bmc->bmSpacerHalfZ)));

  solidClearanceSlotCutout->AddNode(
      solidTopClearanceSlotCylinder,
      G4Transform3D(
          clearanceSlotRotation,
          G4ThreeVector(2 * cfg.bmc->holeSpacing, cfg.bmc->bmClearanceSlotHalfY,
                        cfg.bmc->bmSpacerHalfZ)));

  solidClearanceSlotCutout->AddNode(
      solidTopClearanceSlotCylinder,
      G4Transform3D(clearanceSlotRotation,
                    G4ThreeVector(2 * cfg.bmc->holeSpacing,
                                  -cfg.bmc->bmClearanceSlotHalfY,
                                  cfg.bmc->bmSpacerHalfZ)));

  solidClearanceSlotCutout->Voxelize();

  // Subtract clearance pattern
  G4SubtractionSolid *solidSpacer = new G4SubtractionSolid(
      "Spacer", solidSpacerHolePatterned, solidClearanceSlotCutout);

  return solidSpacer;
}
