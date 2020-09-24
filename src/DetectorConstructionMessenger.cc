
// $Id: DetectorConstructionMessenger.cc $
//
/// \file DetectorConstructionMessenger.cc
/// \brief Definition of the DetectorConstructionMessenger class

#include "DetectorConstructionMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction* placement) : G4UImessenger(), DetectorPlacement(placement){

//    DetectorPlacementDir = new G4UIdirectory("/placement/");
//    DetectorPlacementDir->SetGuidance("Placement (angle and distance) of the farside detector.");

//    FSDistanceCmd = new G4UIcmdWithADouble("/placement/setDistance", this);
//    FSDistanceCmd->SetGuidance("Set the distance between the target and the latest added far-side detector in unit of cm.");
//    FSDistanceCmd->SetParameterName("fs_distance", false);
//    FSDistanceCmd->AvailableForStates(G4State_Idle);
//    FSDistanceCmd->SetDefaultValue(140);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

DetectorConstructionMessenger::~DetectorConstructionMessenger(){
//    delete FSDistanceCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void DetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
//    if(command == FSDistanceCmd){
//        DetectorPlacement->setFarSideDistance(FSDistanceCmd->GetNewDoubleValue(newValue));
//    }
}
