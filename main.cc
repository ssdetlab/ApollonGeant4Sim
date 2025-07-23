#include <string>

#include "DetectorConstruction.hh"
#include "FTFP_BERT.hh"
#include "G4RunManager.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "PrimaryGeneratorAction.hh"
#include "ReadoutPrimaryGeneratorAction.hh"
#include "RunAction.hh"

// #define G4VIS_USE
// #define G4UI_USE
// #define G4VERBOSE

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc, char *argv[]) {
  int noe = 82584690;
  // int noe = 1e5;
  int nParticles = 1;

  double particleEnergyMin = 1.0 * GeV;
  double particleEnergyMax = 1.0 * GeV;

  // int noe = std::stoi(argv[1]);
  // int nParticles = std::stoi(argv[2]);

  // double particleEnergyMin = std::stod(argv[3]) / 1e1 * GeV;
  // double particleEnergyMax = std::stod(argv[3]) / 1e1 * GeV;

  double sigmaPhi = 0.035 * rad;
  double sigmaTheta = 0.035 * rad;

  std::string filePath =
      // "particles.root";
      "/home/romanurmanov/work/Apollon/geant4_sims/al_window_flange/out_data/"
      "particles.root";
  std::string treeName = "particles";

  G4RunManager *runManager = new G4RunManager();

  runManager->SetUserInitialization(new DetectorConstruction());
  auto physicsList = new FTFP_BERT;
  physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  runManager->SetUserInitialization(physicsList);

  // runManager->SetUserAction(new PrimaryGeneratorAction(
  //     nParticles, particleEnergyMin, particleEnergyMax, sigmaTheta,
  //     sigmaPhi));
  runManager->SetUserAction(new ReadoutPrimaryGeneratorAction(
      "/home/romanurmanov/work/Apollon/data/Xe_10^22_pxpypz.txt"));
  runManager->SetUserAction(new RunAction(filePath, treeName));

  runManager->Initialize();

#ifdef G4VIS_USE
  G4VisManager *visManager = new G4VisExecutive();
  visManager->Initialize();
#endif

#ifdef G4UI_USE
  G4UImanager *uiMan = G4UImanager::GetUIpointer();
  G4UIExecutive *uiEx = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
  uiMan->ApplyCommand("/control/execute init_vis.mac");
#endif

  uiEx->SessionStart();

  delete uiEx;
#else
  runManager->BeamOn(noe);
#endif

#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;
  return 0;
}
