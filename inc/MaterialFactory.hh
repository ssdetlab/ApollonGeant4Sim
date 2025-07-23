#ifndef MaterialFactory_h
#define MaterialFactory_h

#include <string>

#include "G4SystemOfUnits.hh"
#include "G4Types.hh"

namespace GeometryConstants {

/// ---------------------------------------------------
/// Custom materials

namespace Material {

namespace Al6061 {

const std::string name = "Al6061";

const G4double alFrac = 0.9856;
const G4double mgFrac = 0.008;
const G4double siFrac = 0.004;
const G4double cuFrac = 0.002;

const G4double density = 2.7 * g / cm3;

}  // namespace Al6061

namespace MildSteel {

const std::string name = "MildSteel";

const G4double feFrac = 0.9865;
const G4double cFrac = 0.0025;
const G4double mnFrac = 0.0075;
const G4double siFrac = 0.0035;

const G4double density = 7.85 * g / cm3;

}  // namespace MildSteel

namespace Neodymium {

const std::string name = "Neodymium";

const G4double ndFrac = 0.27;
const G4double feFrac = 0.67;
const G4double bFrac = 0.06;

const G4double density = 7.61 * g / cm3;

}  // namespace Neodymium

namespace Epoxy {

const std::string name = "Epoxy";

const G4double hFrac = 0.1310;
const G4double cFrac = 0.5357;
const G4double oFrac = 0.3333;

const G4double density = 1.3 * g / cm3;

}  // namespace Epoxy

namespace PEEK {

const std::string name = "PEEK";

const G4int cAtoms = 19;
const G4int hAtoms = 12;
const G4int oAtoms = 3;

const G4double density = 1.32 * g / cm3;

}  // namespace PEEK

namespace Fiberglass {

const std::string name = "Fiberglass";

const G4int siAtoms = 1;
const G4int oAtoms = 2;

const G4double density = 2.61 * g / cm3;

}  // namespace Fiberglass

namespace FR4 {

const std::string name = "FR4";

const G4double epoxyFrac = 0.34;
const G4double fiberglassFrac = 0.56;
const G4double copperFrac = 0.1;

const G4double density = 1.93 * g / cm3;

}  // namespace FR4

namespace PBT {

const std::string name = "PBT";

const G4int cAtoms = 12;
const G4int hAtoms = 12;
const G4int oAtoms = 4;

const G4double density = 1.31 * g / cm3;

}  // namespace PBT

}  // namespace Material

}  // namespace GeometryConstants

class MaterialFactory {
 public:
  MaterialFactory() = default;
  ~MaterialFactory() = default;

  static MaterialFactory* instance();

  void constuctMaterial();

 private:
  void constructAl6061();

  void constructMildSteel();

  void constructNeodymium();

  void constructEpoxy();

  void constructPEEK();

  void constructFiberglass();

  void constructFR4();

  void constructPBT();

 protected:
  static MaterialFactory* m_instance;
};

#endif
