
// $Id: GeneratorAction.hh $
//
/// \file GeneratorAction.hh
/// \brief Definition of the GeneratorAction class

#ifndef GeneratorAction_h
#define GeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"

#include <string>

#include "TFile.h"
#include "TTree.h"

class G4GeneralParticleSource;
class G4ParticleGun;
class G4Event;
class GeneratorMessenger;

using std::string;

class GeneratorAction : public G4VUserPrimaryGeneratorAction {

public:
    GeneratorAction();
    virtual ~GeneratorAction();

    virtual void GeneratePrimaries(G4Event* event);

    void SetSpectrum( string str );
        // used to specify the gamma spectrum from which to sample energy & momentum

    void Sample( int n = -1 );

    void SetPosition();
        // sample and set position from the specified ROOT file.

    void SetDirection();
        // sample and set momentum direction from the specified ROOT file.

    void SetDirNormal();
        // sample and set momentum direction. with theta & phi relative to the local surface.
        // Note: this method requires first specifying the momentum direction through macro first.

    void SetEnergy();
        // sample and set energy from the specified ROOT file.


private:
    GeneratorMessenger* primaryGeneratorMessenger;

    G4ParticleGun*  fgps;
    //G4GeneralParticleSource*  fgps;
        // GPS is not enabled since Geant4 does not allow to set position and momentum using GPS.

    TFile* file;
        // Pointer to the ROOT file.

    TTree* tree;
        // Pointer to the TTree object.

    int nentries;
    int index;
    // Following variables used to read and hold sampled values from the ROOT file.
    double x;
    double y;
    double z;
    double px;
    double py;
    double pz;
    double E;
    double theta;
    double phi;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
