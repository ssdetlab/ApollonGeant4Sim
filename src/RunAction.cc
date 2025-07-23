#include "RunAction.hh"

#include "Run.hh"

RunAction::RunAction(const std::string& filePath, const std::string& treeName)
    : m_filePath(filePath), m_treeName(treeName), G4UserRunAction() {}

G4Run* RunAction::GenerateRun() {
  return new Run(m_filePath, m_treeName);
}

void RunAction::BeginOfRunAction(const G4Run* run) {}

void RunAction::EndOfRunAction(const G4Run* run) {
}
