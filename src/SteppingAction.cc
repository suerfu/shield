
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

void SteppingAction::UserSteppingAction(const G4Step* step){

    // Collect energy and number of scatters step by step
    // Don't save the out of world step
    if(!step->GetPostStepPoint()->GetPhysicalVolume()) return;

    // Set event filter criteria.
    // In this application, will select transportation events where the particle leaves the surface.
    if( step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="Transportation" ){

        G4String preVolume = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
        G4String postVolume = step->GetPostStepPoint()->GetPhysicalVolume()->GetName();

        if( preVolume=="frame" && postVolume=="Pb" ){
            
            G4Track* track = step->GetTrack();
            G4String particle = track->GetParticleDefinition()->GetParticleName();

            if( particle=="gamma" || particle=="e+" || particle=="e-"){
                fEventAction->GetStepCollection().push_back(StepInfo(step));
            }
            //track->SetTrackStatus( fStopAndKill );
            track->SetTrackStatus( fKillTrackAndSecondaries );
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
