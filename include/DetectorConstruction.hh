/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"
#include "OxCryoMaterials.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"
#include "G4RotationMatrix.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;
class G4GlobalMagFieldMessenger;
class G4Event;
class DetectorConstructionMessenger;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction{

public:
    // constructor and destructor
    DetectorConstruction();
    virtual ~DetectorConstruction();

public:
    virtual G4VPhysicalVolume* Construct();
        // This method calls DefineMaterials and DefineVolumes successively.

    G4Material* FindMaterial(G4String);

    void SetChamberHeight();
    void SetShieldingThickness( G4String, G4double );

private:
    
    void DefineMaterials();

    G4VPhysicalVolume* DefineVolumes();

    DetectorConstructionMessenger* fDetectorMessenger;

    bool fCheckOverlaps;

    // Variables related to the dimensions of the setup.

    // World
    G4double world_x;
    G4double world_y;
    G4double world_z;

    G4LogicalVolume* world_lv;

    G4ThreeVector farside_position;
    G4RotationMatrix* farside_rot;

    friend DetectorConstructionMessenger;

    void SetFarSidePosition( G4ThreeVector x){
        farside_position = x;
    }

    void FarSideRotateX( G4double a){
        farside_rot->rotateX(a);
    }

    void FarSideRotateY( G4double a){
        farside_rot->rotateY(a);
    }

    void FarSideRotateZ( G4double a){
        farside_rot->rotateZ(a);
    }

    void PlaceFarSideDetector();

    G4int fs_count;
        // used to keep track of number of farside detectors.
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
