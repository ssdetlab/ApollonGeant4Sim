#include "RunAction.hh"

#include "Run.hh"

RunAction::RunAction(const std::string& filePath, const std::string& treeName,
                     double pixelThreshold)
    : m_filePath(filePath),
      m_treeName(treeName),
      m_pixelThreshold(pixelThreshold),
      G4UserRunAction() {}

G4Run* RunAction::GenerateRun() {
  return new Run(m_filePath, m_treeName, m_pixelThreshold);
}

void RunAction::BeginOfRunAction(const G4Run* run) {}

void RunAction::EndOfRunAction(const G4Run* run) {}
