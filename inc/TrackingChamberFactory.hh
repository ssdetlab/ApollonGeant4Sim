#ifndef TrackingChamberFactory_h
#define TrackingChamberFactory_h

#include "G4AssemblyVolume.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4RunManager.hh"
#include "G4Types.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4VUserDetectorConstruction.hh"
#include "GeometryConstants.hh"

class G4LogicalVolume;
class G4PhysicalVolume;

class TrackingChamberFactory {
 public:
  struct Config {
    /// TC namespace
    std::string name;

    /// TC position in the World
    G4double tcCenterX;
    G4double tcCenterY;
    G4double tcCenterZ;

    /// TC rotation parameters in the World frame
    G4double tcRotationAngleX;
    G4double tcRotationAngleY;
    G4double tcRotationAngleZ;

    const GeometryConstants *gc;

    /// Check overlaps flag
    G4bool checkOverlaps;
  };

  TrackingChamberFactory() = default;
  ~TrackingChamberFactory() = default;

  G4VPhysicalVolume *construct(G4LogicalVolume *logicParent, const Config &cfg);

 private:
  G4LogicalVolume *constructSensor(const Config &cfg);

  G4LogicalVolume *constructLConnector(const Config &cfg);

  G4AssemblyVolume *constructNineAlpidePCB(const Config &cfg);

  G4LogicalVolume *constructCarrierPCB(G4double &carrierPcbContainerY,
                                       const Config &cfg);
};

#endif
