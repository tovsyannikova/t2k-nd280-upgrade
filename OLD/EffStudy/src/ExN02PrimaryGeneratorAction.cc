//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B4PrimaryGeneratorAction.cc 75215 2013-10-29 16:07:06Z gcosmo $
// 
/// \file B4PrimaryGeneratorAction.cc
/// \brief Implementation of the B4PrimaryGeneratorAction class

#include "ExN02PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PrimaryGeneratorAction::ExN02PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0)
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic
  //

  G4String particlename = "pi-"; //"pi-" //"pi+"                        
  G4double particleenergy = 125 * MeV; // GeV 
  
  //G4String particlename = "proton";                        
  //G4double particleenergy = 50. * MeV; // GeV 

  G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle(particlename);
  fParticleGun->SetParticleDefinition(particleDefinition);
  
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.)); // Along Y
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0.)); // Along Z

  fParticleGun->SetParticleEnergy(particleenergy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PrimaryGeneratorAction::~ExN02PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore
  //
  G4double worldZHalfLength = 0;
  G4double targetYHalfLength = 0;

  G4LogicalVolume* worlLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Box* worldBox = 0;
  if ( worlLV) worldBox = dynamic_cast< G4Box*>(worlLV->GetSolid()); 
  if ( worldBox ) {
    worldZHalfLength = worldBox->GetZHalfLength();  
  }
  else  {
    G4ExceptionDescription msg;
    msg << "World volume of box not found." << G4endl;
    msg << "Perhaps you have changed geometry." << G4endl;
    msg << "The gun will be place in the center.";
    G4Exception("ExN02PrimaryGeneratorAction::GeneratePrimaries()",
      "MyCode0002", JustWarning, msg);
    exit(1);
  } 

  G4LogicalVolume* targetLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("Target");
  G4Box* targetBox = 0;
  if ( targetLV) targetBox = dynamic_cast< G4Box*>(targetLV->GetSolid()); 
  if ( targetBox ) {
    targetYHalfLength = targetBox->GetYHalfLength();  
  }
  else  {
    G4ExceptionDescription msg;
    msg << "Target volume of box not found." << G4endl;
    msg << "Perhaps you have changed geometry." << G4endl;
    msg << "The gun will be place in the center.";
    G4Exception("ExN02PrimaryGeneratorAction::GeneratePrimaries()",
      "MyCode0002", JustWarning, msg);
    exit(1);
  } 
  
  // Set gun position
  
  G4double positionZ = 0.; //-1. * worldZHalfLength;
  //G4double positionY = -2. * targetYHalfLength;
  G4double positionY = 0.;  


  //fParticleGun->SetParticlePosition(G4ThreeVector(0.*cm, positionY, positionZ));
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*cm, positionY, positionZ));
  
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

