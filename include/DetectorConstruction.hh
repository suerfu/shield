/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"
#include "G4RotationMatrix.hh"
#include "G4VPVParameterisation.hh"
#include "G4PVParameterised.hh"

#include "G4ExtrudedSolid.hh"

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

};


class TrussParameterisation : public G4VPVParameterisation {

public:
    TrussParameterisation( ){}

    virtual ~TrussParameterisation(){}

    void ComputeTransformation( const G4int copyNo, G4VPhysicalVolume* physVol) const;

    void ComputeDimensions (G4ExtrudedSolid&,const G4int,
                            const G4VPhysicalVolume*) const {}

private:  // Dummy declarations to get rid of warnings ...

    void ComputeDimensions (G4Box&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions( G4Tubs&,const G4int,
                            const G4VPhysicalVolume* ) const{}
    void ComputeDimensions (G4Trd&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Trap&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Cons&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Sphere&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Orb&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Ellipsoid&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Torus&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Para&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Hype&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Polycone&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Polyhedra&,const G4int,
                            const G4VPhysicalVolume*) const {}

};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
