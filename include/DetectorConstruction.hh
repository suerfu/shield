/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
//#include "G4Cache.hh"
//#include "G4ThreeVector.hh"
//#include "tls.hh"
//#include "G4RotationMatrix.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UserLimits;
class DetectorConstructionMessenger;
class G4VIStore;

/// Detector construction class to define materials and geometry.

class ShieldInfo;

class DetectorConstruction : public G4VUserDetectorConstruction{

public:
    // constructor and destructor
    DetectorConstruction();
    virtual ~DetectorConstruction();

public:
    virtual G4VPhysicalVolume* Construct();
        // This method calls DefineMaterials and DefineVolumes successively.

    G4Material* FindMaterial(G4String);

    G4VIStore* CreateImportanceStore();

    void SetChamberHeight();
    void SetShieldingThickness( G4String, G4double );

private:
    
    void DefineMaterials();

    G4VPhysicalVolume* DefineVolumes();

    DetectorConstructionMessenger* fDetectorMessenger;

    bool fCheckOverlaps;

    // Variables related to the dimensions of the setup.

    // World
    G4VPhysicalVolume* world_pv;
    G4double world_x;
    G4double world_y;
    G4double world_z;

    // Offset of frame from the floor
    G4double offset;

    // Experimental chamber
    std::vector<ShieldInfo> list;

};



struct ShieldInfo{

    ShieldInfo( G4String n, G4String m, G4double l, G4int b):
        name(n), material(m), dim(l), bias(b){}

    G4String name;
    G4String material;
    G4double dim;
    G4int    bias;
    G4LogicalVolume* log;
    G4VPhysicalVolume* phy;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
