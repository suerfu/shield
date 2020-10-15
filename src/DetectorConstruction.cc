
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4tgbVolumeMgr.hh"

#include "G4RunManager.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "DetectorConstructionMessenger.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4ExtrudedSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4VisExtent.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include<sstream>
using std::stringstream;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {
    fCheckOverlaps = true;
    fDetectorMessenger = new DetectorConstructionMessenger(this);

    // Set default values for the dimensions.
    
    // World
    world_x = 3.*m;
    world_y = 3.*m;
    world_z = 1.*m;

    farside_rot = new G4RotationMatrix();
    fs_count = 0;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


DetectorConstruction::~DetectorConstruction(){
    delete fDetectorMessenger;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4VPhysicalVolume* DetectorConstruction::Construct(){
    DefineMaterials();
    return DefineVolumes();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4VPhysicalVolume* DetectorConstruction::DefineVolumes(){
    
    /*
    // Clean old geometry, if any
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
    */

    //===============  Materials ===============//

    G4NistManager* mat_man = G4NistManager::Instance(); //material mananger

    G4Material* world_material = mat_man->FindOrBuildMaterial("G4_AIR");
    G4Material* NaI_material = mat_man->FindOrBuildMaterial("NaI");


    //===============  Build Geometry ===============//


    // World
    G4Box* world_solid = new G4Box( "world_solid", world_x/2.0, world_y/2.0, world_z/2.0);
    world_lv = new G4LogicalVolume( world_solid, world_material, "world_lv");
    G4VPhysicalVolume* world_pv = new G4PVPlacement( 0, G4ThreeVector(0,0,0), world_lv, "world", 0, false, 0,fCheckOverlaps);


    // NaI in the center

    G4double medium_dia = world_x;
    G4double medium_h   = 50*cm ;

    G4ThreeVector medium_offset( 0, 0, world_z/2-medium_h/2);
        // The additional 10 cm account for the rail system

    G4Tubs* medium_solid = new G4Tubs( "medium_solid", 0, medium_dia/2, medium_h/2, 0, CLHEP::twopi);
    G4LogicalVolume* medium_lv = new G4LogicalVolume( medium_solid, NaI_material, "medium_lv");
    new G4PVPlacement( 0, medium_offset, medium_lv, "medium", world_lv, false, 0, fCheckOverlaps);


    //===============  Visualization ===============//

    world_lv->SetVisAttributes( G4VisAttributes::Invisible );
    medium_lv->SetVisAttributes( G4VisAttributes(G4Colour( 1., 0, 1., 0.5)) );

    return world_pv;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void DetectorConstruction::DefineMaterials(){

    G4double a;  // mass of a mole;
    G4double z;  // z=mean number of protons;
    G4double density; //, fractionMass;
    G4String symbol, name;
    G4int ncomponents, natoms;

    G4Element* elNa = new G4Element( name="Sodium", symbol="Na", z = 11, a = 22.990 *g/mole );
    G4Element* elI  = new G4Element( name="Iodine", symbol="I" , z = 53, a = 253.81*g/mole );

    G4Material* NaI = new G4Material( "NaI", density= 3.67*g/cm3, ncomponents = 2);
    NaI->AddElement( elNa, natoms=1 );
    NaI->AddElement( elI,  natoms=1 );

    /*
    G4NistManager* nistManager =  G4NistManager::Instance();

    G4Element* elH  = new G4Element(name = "Hydrogen"   , symbol = "H"  , z = 1.  , a =   1.008*g/mole);
    G4Element* elC  = new G4Element(name = "Carbon"     , symbol = "C"  , z = 6.  , a =  12.011*g/mole);
    */

    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}


void DetectorConstruction::PlaceFarSideDetector(){

    // The following code is used in the simulation of scattering experiments.
    // It is not necessary in studying propagation of gamma rays in rocks.
    /*
    // Use stringstream to parameterize the farside detector name with count number
    stringstream ss;
    ss << "fs" << fs_count;

    G4NistManager* mat_man = G4NistManager::Instance(); //material mananger
    G4Material* NaI_material = mat_man->FindOrBuildMaterial("NaI");

    // Place the far-side detector.
    G4Tubs* center_solid = new G4Tubs( "fs_solid", 0, 2*2.54*cm/2, 2*2.54*cm/2, 0, CLHEP::twopi);
    G4LogicalVolume* center_lv = new G4LogicalVolume( center_solid, NaI_material, "fs_lv");

    G4RotationMatrix* rot = new G4RotationMatrix( *farside_rot );
    new G4PVPlacement( rot, farside_position, center_lv, ss.str(), world_lv, false, 0, fCheckOverlaps);

    // Inform run manager about geometry change.
    G4RunManager::GetRunManager()->GeometryHasBeenModified();

    // After the placement, increment the counter and reset the rotation matrix for the next farside detector.
    fs_count++;
    *farside_rot = G4RotationMatrix();
    */
}
