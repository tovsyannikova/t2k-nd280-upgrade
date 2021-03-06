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
//
// $Id: ExN02SteppingAction.cc,v 1.9 2006-06-29 17:48:18 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//#include "G4SteppingManager.hh"

#include "ExN02SteppingAction.hh"
#include "ExN02EventAction.hh"
#include "ExN02DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02SteppingAction::ExN02SteppingAction(
 					 const ExN02DetectorConstruction* detectorConstruction,
					 ExN02EventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction)
{
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02SteppingAction::~ExN02SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02SteppingAction::UserSteppingAction(const G4Step* step)
{
  // Collect energy and track length step by step

  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();
  if(edep==0.) return;
  
  G4StepPoint* prestep  = step->GetPreStepPoint();
  //G4StepPoint* poststep = step->GetPostStepPoint();
  G4Track *track = step->GetTrack(); // it's PostStepPoint!!!
  
  // get volume of the current step
  //G4VPhysicalVolume* volume = prestep->GetTouchableHandle()->GetVolume();

  // step length                                                    
  G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();
  }

  G4String namedet = prestep->GetTouchableHandle()->GetVolume()->GetName();        
  fEventAction->AddAbsTot(edep,stepLength);    
  if(namedet=="TPCUp1")
    fEventAction->AddAbsTPCup1(edep,stepLength);
  else if(namedet=="TPCUp2")
    fEventAction->AddAbsTPCup2(edep,stepLength);
  else if(namedet=="TPCDown1")
    fEventAction->AddAbsTPCdown1(edep,stepLength);
  else if(namedet=="TPCDown2")
    fEventAction->AddAbsTPCdown2(edep,stepLength);
  else if(namedet=="Target1") 
    fEventAction->AddAbsTarget1(edep,stepLength);
  else if(namedet=="Target2") 
    fEventAction->AddAbsTarget2(edep,stepLength);
  else if(namedet=="ForwTPC1/Half") 
    fEventAction->AddAbsForwTPC1(edep,stepLength);
  else if(namedet=="ForwTPC2/Half") 
    fEventAction->AddAbsForwTPC2(edep,stepLength);
  else if(namedet=="ForwTPC3/Half") 
    fEventAction->AddAbsForwTPC3(edep,stepLength);
  
  //if(namedet=="ForwTPC1/Half")
  //G4cout << "stepLength = " << stepLength << G4endl;

  // Get track info    
  fEventAction->SetTrack(step);


  // // Take track informations from its first step
  // G4int newtrkid      = track->GetTrackID();
  // G4int newparentid   = track->GetParentID();
  // G4int newtrkpdg     = track->GetDefinition()->GetPDGEncoding();
  
  // // Get the total initial energy of the track
  // G4double newtrkmass        = track->GetDefinition()->GetPDGMass();
  // G4double newtrkEkinVtx     = track->GetVertexKineticEnergy();  
  // G4double newtrkEtotVtx     = newtrkEkinVtx + newtrkmass;             
  
  // // Get the total initial momentum
  // //G4ThreeVector newVecMomVtx = track->GetMomentum(); // it's PostStep not PreStep!!!  
  // G4double newtrkMomX   = prestep->GetMomentum().x();
  // G4double newtrkMomY   = prestep->GetMomentum().y();
  // G4double newtrkMomZ   = prestep->GetMomentum().z();
  // G4double newtrkMomMag = prestep->GetMomentum().mag();
  
  // std::cout << "Track ID: = " << newtrkid 
  //  	    << " - Parent ID: = " << newparentid
  //  	    << " - Track PDG: = " << newtrkpdg   
  //  	    << " - Track E: = " << newtrkEtotVtx  
  //  	    << " - Track Mom: = " << newtrkMomMag   
  //   //<< " - PreStep Mom: = " << prestep->GetMomentum().mag()
  //   //<< " - PostStep Mom: = " << poststep->GetMomentum().mag()
  //  	    << std::endl;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......      
