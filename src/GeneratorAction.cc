//
/// \file GeneratorAction.cc
/// \brief Implementation of the GeneratorAction class

#include "GeneratorAction.hh"
#include "GeneratorMessenger.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include "G4RandomDirection.hh"
#include "G4IonTable.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


GeneratorAction::GeneratorAction() : G4VUserPrimaryGeneratorAction() {
    
    fgps = new G4ParticleGun();
    //GeneralParticleSource();
        // GPS must be initialized here.
    
    file = 0;
    tree = 0;
    index = 0;

    primaryGeneratorMessenger = new GeneratorMessenger(this);
}


GeneratorAction::~GeneratorAction(){
    
    if( !file==0 ){
        file->Close();
        delete file;
    }

    delete primaryGeneratorMessenger;
}


void GeneratorAction::SetSpectrum( string str ){

    if( file->IsOpen() ){
        file->Close();
    }

    G4cout << "Opening " << str << G4endl;

    file = new TFile( str.c_str() );
        // open in read mode (default)
    if( !file->IsOpen() ){
        G4cerr << "Error opening " << s << G4endl;
        return;
    }

    tree = (TTree*)file->Get("events");

    if( tree!=0 ){
        nentries = tree->GetEntries();
        tree->SetBranchAddress( "x", &x );
        tree->SetBranchAddress( "y", &y );
        tree->SetBranchAddress( "z", &z );
        tree->SetBranchAddress( "px", &px );
        tree->SetBranchAddress( "py", &py );
        tree->SetBranchAddress( "pz", &pz );
        tree->SetBranchAddress( "Eki", &E );
        if( tree->GetListOfBranches()->FindObject("theta") )
            tree->SetBranchAddress( "theta", &theta );
        if( tree->GetListOfBranches()->FindObject("theta") )
            tree->SetBranchAddress( "phi", &phi );            
    }
}


void GeneratorAction::Sample( int n ){
    if( n>0 && tree!=0 ){
        index = n;
    }

    tree->GetEntry( index%nentries );
    index++;
}


void GeneratorAction::SetPosition(){
    fgps->SetParticlePosition( G4ThreeVector(x,y,z) );
}


void GeneratorAction::SetDirection(){
    fgps->SetParticleMomentumDirection( G4ThreeVector(px,py,pz) );
}


void GeneratorAction::SetEnergy(){
    fgps->SetParticleEnergy( E );
}


void GeneratorAction::SetDirNormal(){
    
    G4ThreeVector original = fgps->GetParticleMomentumDirection( );

    G4ThreeVector rotate1 = original.orthogonal();
    G4ThreeVector rotate2 = original;
    original.rotate( theta, rotate1);
    original.rotate( phi, rotate2);

    fgps->SetParticleMomentumDirection( original );
}


void GeneratorAction::GeneratePrimaries(G4Event* anEvent){
    fgps->GeneratePrimaryVertex(anEvent);
}
