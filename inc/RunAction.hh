#ifndef RunAction_h
#define RunAction_h

#include "G4Run.hh"
#include "G4UserRunAction.hh"

class G4Run;

class RunAction : public G4UserRunAction {
 public:
  RunAction(const std::string& filePath, const std::string& treeName,
            double pixelThreshold);
  ~RunAction() override = default;

  void BeginOfRunAction(const G4Run* run) override;
  void EndOfRunAction(const G4Run* run) override;
  G4Run* GenerateRun() override;

 private:
  std::string m_filePath;
  std::string m_treeName;
  double m_pixelThreshold;
};

#endif
