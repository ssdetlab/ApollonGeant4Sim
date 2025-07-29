#ifndef DetectorConstruction_h
#define DetectorConstruction_h

#include "G4LogicalBorderSurface.hh"
#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"

class G4LogicalVolume;
class G4PhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
 public:
  DetectorConstruction(double alongWindowTranslation1,
                       double alongWindowTranslation2, 
                       double alpha);
  ~DetectorConstruction() override;

  G4VPhysicalVolume* Construct() override;
  void ConstructSDandField() override;

  G4VPhysicalVolume* constructSamplingLayers(G4NistManager* nistManager,
                                             G4LogicalVolume* logicParent,
                                             const std::string& name,
                                             const G4ThreeVector& center,
                                             int id);
  G4bool checkOverlaps;

  double translation1;
  double translation2;
  double angle;
};

#endif
