
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Neutron.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "StepInfo.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction( const DetectorConstruction* detectorConstruction, EventAction* eventAction)
      : G4UserSteppingAction(),
        fDetConstruction(detectorConstruction),
        fEventAction(eventAction){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction( const G4Step* step){
    
    G4Track* track = step->GetTrack();

    G4int parentID = track->GetParentID();
    G4int stepNo =  track->GetCurrentStepNumber();
    
    G4StepPoint* preStep = step->GetPreStepPoint();
    G4StepPoint* postStep = step->GetPostStepPoint();

    G4String particle = track->GetParticleDefinition()->GetParticleName();
    G4String particleType = track->GetParticleDefinition()->GetParticleType();
    G4String procName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

    // Collect energy and number of scatters step by step
    // Don't save the out of world step
    // if(!postStep->GetPhysicalVolume()) return;

    if( procName=="Transportation" && step->GetTotalEnergyDeposit()<1.e-6*CLHEP::eV )
        return;

    /*
    // Record only EM interactions.
    if( particle=="gamma" || particle=="e+" || particle=="e-"){
        fEventAction->GetStepCollection().push_back(StepInfo(step));
    }
    */
    
    fEventAction->GetStepCollection().push_back(StepInfo(step));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
