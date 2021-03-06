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
// $Id: ExN02RunAction.cc,v 1.9 2006-06-29 17:48:16 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN02RunAction.hh"
#include "ExN02Analysis.hh" // TODO

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02RunAction::ExN02RunAction()
  : G4UserRunAction()
{
  // set printing event number per each event

  G4RunManager::GetRunManager()->SetPrintProgress(1);
  // Create analysis manager                                                                                                                                                   
  // The choice of analysis technology is done via selectin of a namespace       
  // in B4Analysis.hh                                                                                                                                                    
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories                                                                                                                                                        
  //analysisManager->SetHistoDirectoryName("histograms");                            
  //analysisManager->SetNtupleDirectoryName("ntuple");                                                       
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);

  // Book histograms, ntuple
  //
  // Creating histograms 
  analysisManager->CreateH1("Edep_abs_tpcup","Edep in TPC Up", 100, 0., 800*MeV);
  analysisManager->CreateH1("Edep_abs_tpcdown","Edep in TPC Down", 100, 0., 800*MeV);
  analysisManager->CreateH1("Edep_abs_target","Edep in Target", 100, 0., 800*MeV);
  analysisManager->CreateH1("TrkLght_abs_tpcup","trackL in TPC Up", 100, 0., 2*m);
  analysisManager->CreateH1("TrkLght_abs_tpcdown","trackL in TPC Down", 100, 0., 2*m);
  analysisManager->CreateH1("TrkLght_abs_target","trackL in Target", 100, 0., 2*m);
  analysisManager->CreateH1("PrimInTPCUp","flag primary track in TPC up", 2, 0, 2);
  analysisManager->CreateH1("PrimInTPCDown","flag primary track in TPC down", 2, 0, 2);
  analysisManager->CreateH1("PrimInTarget","flag primary track in TPC target", 2, 0, 2);
  //analysisManager->CreateH1("PDGFirstTPCup","PDG of first step in TPC Up", 40, 0, 40);
  //analysisManager->CreateH1("PDGFirstTPCdown","PDG of first step in TPC Down", 40, 0, 40);
  //analysisManager->CreateH1("PDGFirstTarget","PDG of first step in Target", 40, 0, 40);
 
  // Creating ntuple
  //             
  analysisManager->CreateNtuple("EffStudy","Primary particle and total absorbed energy");
  analysisManager->CreateNtupleDColumn("EabsTPCup");
  analysisManager->CreateNtupleDColumn("EabsTPCdown");
  analysisManager->CreateNtupleDColumn("EabsTarget");  
  analysisManager->CreateNtupleDColumn("LabsTPCup");
  analysisManager->CreateNtupleDColumn("LabsTPCdown");
  analysisManager->CreateNtupleDColumn("LabsTarget");
  analysisManager->CreateNtupleDColumn("PrimInTPCUp");
  analysisManager->CreateNtupleDColumn("PrimInTPCDown");
  analysisManager->CreateNtupleDColumn("PrimInTarg");
  analysisManager->CreateNtupleDColumn("PDGPrimTPCup");
  analysisManager->CreateNtupleDColumn("PDGPrimTPCdown");
  analysisManager->CreateNtupleDColumn("PDGPrimTarget");
  analysisManager->CreateNtupleDColumn("PDGPrim");
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02RunAction::~ExN02RunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed  
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);      

  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file     
  //
  G4String fileName = "EffStudy";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RunAction::EndOfRunAction(const G4Run*)
{ 
  // print histogram statistics         
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(1) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl;
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl;
    }
    // G4cout << " EAbs : mean = "
    // 	   << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy")
    // 	   << " rms = "
    // 	   << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") << G4endl;
    // G4cout << " EGap : mean = "
    // 	   << G4BestUnit(analysisManager->GetH1(2)->mean(), "Energy")
    // 	   << " rms = "
    // 	   << G4BestUnit(analysisManager->GetH1(2)->rms(),  "Energy") << G4endl;
    // G4cout << " LAbs : mean = "
    // 	   << G4BestUnit(analysisManager->GetH1(3)->mean(), "Length")
    // 	   << " rms = "
    // 	   << G4BestUnit(analysisManager->GetH1(3)->rms(),  "Length") << G4endl;
    // G4cout << " LGap : mean = "
    // 	   << G4BestUnit(analysisManager->GetH1(4)->mean(), "Length")
    // 	   << " rms = "
    // 	   << G4BestUnit(analysisManager->GetH1(4)->rms(),  "Length") << G4endl;
    
  }
  
  // save histograms & ntuple   
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



