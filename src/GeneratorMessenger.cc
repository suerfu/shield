// $Id: GeneratorMessenger.cc $
//
/// \file GeneratorMessenger.cc
/// \brief Definition of the GeneratorMessenger class

#include "GeneratorMessenger.hh"

#include "GeneratorAction.hh"

GeneratorMessenger::GeneratorMessenger( GeneratorAction* generator )  : G4UImessenger(), primaryGenerator(generator), primaryGeneratorDir(0) {

	primaryGeneratorDir = new G4UIdirectory("/generator/");
	primaryGeneratorDir->SetGuidance("Generator position, momentum and energy spectrum control.");

    cmdSetSpectrum = new G4UIcmdWithAString("/generator/spectrum", this);
    cmdSetSpectrum->SetGuidance("Set the ROOT file from which to sample position, direction and energy.");
    cmdSetSpectrum->SetParameterName("foo.root", false);
    cmdSetSpectrum->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmdSample = new G4UIcmdWithoutParameter("/generator/sample", this);
    cmdSample->SetGuidance("Increment the sample event index of the ROOT tree of energy spectrum.");
    cmdSample->AvailableForStates(G4State_PreInit, G4State_Idle);
    
    cmdSetPosition = new G4UIcmdWithoutParameter("/generator/setPos", this);
    cmdSetPosition->SetGuidance("Set the position of next primary vertex from ROOT file.");
    cmdSetPosition->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmdSetDir = new G4UIcmdWithoutParameter("/generator/setDir", this);
    cmdSetDir->SetGuidance("Set the direction of next primary vertex from ROOT file.");
    cmdSetDir->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmdSetDirNormal = new G4UIcmdWithoutParameter("/generator/setDirNormal", this);
    cmdSetDirNormal->SetGuidance("Set the theta and phi angle from ROOT file.\nThis method requres 1) ROOT file contains theta and phi,\n2) User first sets via macro/UI the direction to be the normal direction. (This function modifies this normal direction based on theta and phi.) ");
    cmdSetDirNormal->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmdSetEnergy = new G4UIcmdWithoutParameter("/generator/setEnergy", this);
    cmdSetEnergy->SetGuidance("Set the energy of next primary vertex from ROOT file.");
    cmdSetEnergy->AvailableForStates(G4State_PreInit, G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GeneratorMessenger::~GeneratorMessenger(){

    delete cmdSetSpectrum;
    delete cmdSample;
    delete cmdSetPosition;
    delete cmdSetDir;
    delete cmdSetDirNormal;
    delete cmdSetEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue){

	if( command == cmdSetSpectrum )	{
		primaryGenerator->SetSpectrum( newValue);
	}

    else if( command == cmdSample ){
        primaryGenerator->Sample();
    }

    else if( command == cmdSetPosition ){
        primaryGenerator->SetPosition();
    }

    else if( command == cmdSetDir ){
        primaryGenerator->SetDirection();
    }

    else if( command == cmdSetDirNormal ){
        primaryGenerator->SetDirNormal();
    }

    else if( command == cmdSetEnergy ){
        primaryGenerator->SetEnergy();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
