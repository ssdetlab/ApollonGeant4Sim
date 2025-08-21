#include "MaterialFactory.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

namespace mat = Material;

MaterialFactory *MaterialFactory::m_instance = nullptr;

MaterialFactory *MaterialFactory::instance() {
  if (!m_instance) {
    m_instance = new MaterialFactory();
  }
  return m_instance;
}

void MaterialFactory::constructAl6061() {
  G4Element *elAl = G4NistManager::Instance()->FindOrBuildElement("Al");
  G4Element *elMg = G4NistManager::Instance()->FindOrBuildElement("Mg");
  G4Element *elSi = G4NistManager::Instance()->FindOrBuildElement("Si");
  G4Element *elCu = G4NistManager::Instance()->FindOrBuildElement("Cu");

  G4Material *al6061 =
      new G4Material(mat::Al6061::name, mat::Al6061::density, 4);
  al6061->AddElement(elAl, mat::Al6061::alFrac);
  al6061->AddElement(elMg, mat::Al6061::mgFrac);
  al6061->AddElement(elSi, mat::Al6061::siFrac);
  al6061->AddElement(elCu, mat::Al6061::cuFrac);
}

void MaterialFactory::constructMildSteel() {
  G4Element *elFe = G4NistManager::Instance()->FindOrBuildElement("Fe");
  G4Element *elC = G4NistManager::Instance()->FindOrBuildElement("C");
  G4Element *elMn = G4NistManager::Instance()->FindOrBuildElement("Mn");
  G4Element *elSi = G4NistManager::Instance()->FindOrBuildElement("Si");

  G4Material *mildSteel =
      new G4Material(mat::MildSteel::name, mat::Al6061::density, 4);
  mildSteel->AddElement(elFe, mat::MildSteel::feFrac);
  mildSteel->AddElement(elC, mat::MildSteel::cFrac);
  mildSteel->AddElement(elMn, mat::MildSteel::mnFrac);
  mildSteel->AddElement(elSi, mat::MildSteel::siFrac);
}

void MaterialFactory::constructNeodymium() {
  G4Element *elNd = G4NistManager::Instance()->FindOrBuildElement("Nd");
  G4Element *elFe = G4NistManager::Instance()->FindOrBuildElement("Fe");
  G4Element *elB = G4NistManager::Instance()->FindOrBuildElement("B");

  G4Material *neodymium =
      new G4Material(mat::Neodymium::name, mat::Neodymium::density, 3);
  neodymium->AddElement(elNd, mat::Neodymium::ndFrac);
  neodymium->AddElement(elFe, mat::Neodymium::feFrac);
  neodymium->AddElement(elB, mat::Neodymium::bFrac);
}

void MaterialFactory::constructEpoxy() {
  G4Element *elH = G4NistManager::Instance()->FindOrBuildElement("H");
  G4Element *elC = G4NistManager::Instance()->FindOrBuildElement("C");
  G4Element *elO = G4NistManager::Instance()->FindOrBuildElement("O");

  G4Material *epoxy = new G4Material(mat::Epoxy::name, mat::Epoxy::density, 3);
  epoxy->AddElement(elH, mat::Epoxy::hFrac);
  epoxy->AddElement(elC, mat::Epoxy::cFrac);
  epoxy->AddElement(elO, mat::Epoxy::oFrac);
}

void MaterialFactory::constructPEEK() {
  G4Element *elH = G4NistManager::Instance()->FindOrBuildElement("H");
  G4Element *elC = G4NistManager::Instance()->FindOrBuildElement("C");
  G4Element *elO = G4NistManager::Instance()->FindOrBuildElement("O");

  G4Material *peek = new G4Material(mat::PEEK::name, mat::PEEK::density, 3);
  peek->AddElement(elC, mat::PEEK::cAtoms);
  peek->AddElement(elH, mat::PEEK::hAtoms);
  peek->AddElement(elO, mat::PEEK::oAtoms);
}

void MaterialFactory::constructFiberglass() {
  G4Element *elSi = G4NistManager::Instance()->FindOrBuildElement("Si");
  G4Element *elO = G4NistManager::Instance()->FindOrBuildElement("O");

  G4Material *fiberglass =
      new G4Material(mat::Fiberglass::name, mat::Fiberglass::density, 2);
  fiberglass->AddElement(elSi, mat::Fiberglass::siAtoms);
  fiberglass->AddElement(elO, mat::Fiberglass::oAtoms);
}

void MaterialFactory::constructFR4() {
  G4Material *elCu = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  G4Material *fr4 = new G4Material(mat::FR4::name, mat::FR4::density, 3);

  fr4->AddMaterial(
      G4NistManager::Instance()->FindOrBuildMaterial(mat::Epoxy::name),
      mat::FR4::epoxyFrac);
  fr4->AddMaterial(
      G4NistManager::Instance()->FindOrBuildMaterial(mat::Fiberglass::name),
      mat::FR4::fiberglassFrac);
  fr4->AddMaterial(elCu, mat::FR4::copperFrac);
}

void MaterialFactory::constructPBT() {
  G4Element *elH = G4NistManager::Instance()->FindOrBuildElement("H");
  G4Element *elC = G4NistManager::Instance()->FindOrBuildElement("C");
  G4Element *elO = G4NistManager::Instance()->FindOrBuildElement("O");

  G4Material *peek = new G4Material(mat::PBT::name, mat::PBT::density, 3);
  peek->AddElement(elC, mat::PBT::cAtoms);
  peek->AddElement(elH, mat::PBT::hAtoms);
  peek->AddElement(elO, mat::PBT::oAtoms);
}

void MaterialFactory::constuctMaterial() {
  constructAl6061();

  constructMildSteel();

  constructNeodymium();

  constructEpoxy();

  constructPEEK();

  constructFiberglass();

  constructFR4();

  constructPBT();
}
