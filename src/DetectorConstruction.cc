
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


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {
    fCheckOverlaps = true;
    fDetectorMessenger = new DetectorConstructionMessenger(this);

    // Set default values for the dimensions.
    
    // World
    world_x = 11.*m;
    world_y = 12.*m;
    world_z = 3.2*m;

    // Experimental chamber
    chamber_h = 30.*cm;

    // Copper shielding
    shielding_Cu = 6*cm;
    shielding_PE = 30*cm;
    shielding_Pb = 20*cm;
    shielding_SF = 5*cm;

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

    G4Material* world_material = mat_man->FindOrBuildMaterial("G4_Galactic");
    G4Material* frame_material = mat_man->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    G4Material* Pb_material = mat_man->FindOrBuildMaterial("G4_Pb");
    G4Material* PE_material = mat_man->FindOrBuildMaterial("G4_POLYETHYLENE");
    G4Material* Cu_material = mat_man->FindOrBuildMaterial("G4_Cu");
    G4Material* chamber_material = G4Material::GetMaterial("liquid_helium");


    //===============  Build Geometry ===============//


    // World
    G4Box* world_solid = new G4Box( "world_solid", world_x/2.0, world_y/2.0, world_z/2.0);
    G4LogicalVolume* world_lv = new G4LogicalVolume( world_solid, world_material, "world_lv");
    G4VPhysicalVolume* world_pv = new G4PVPlacement( 0, G4ThreeVector(0,0,0), world_lv, "world", 0, false, 0,fCheckOverlaps);


    // Support Frame
    G4double frame_z = chamber_h+2*shielding_Cu+2*shielding_PE+2*shielding_Pb+2*shielding_SF;
    G4double frame_x = frame_z;
    G4double frame_y = frame_z;

    G4ThreeVector frame_offset( 0, 0, -world_z/2+frame_z/2+10*cm);
        // The additional 10 cm account for the rail system

    G4Box* frame_solid = new G4Box( "frame_solid", frame_x/2.0, frame_y/2.0, frame_z/2.0);
    G4LogicalVolume* frame_lv = new G4LogicalVolume( frame_solid, frame_material, "frame_lv");
    new G4PVPlacement( 0, frame_offset, frame_lv, "frame", world_lv, false, 0,fCheckOverlaps);


    // Lead shielding
    G4double Pb_z = chamber_h+2*shielding_Cu+2*shielding_PE+2*shielding_Pb;
    G4double Pb_x = Pb_z;
    G4double Pb_y = Pb_z;

    G4Box* Pb_solid = new G4Box( "Pb_solid", Pb_x/2.0, Pb_y/2.0, Pb_z/2.0);
    G4LogicalVolume* Pb_lv = new G4LogicalVolume( Pb_solid, Pb_material, "Pb_lv");
    new G4PVPlacement( 0, G4ThreeVector(0,0,0), Pb_lv, "Pb", frame_lv, false, 0,fCheckOverlaps);
    

    // Polyethylene shielding
    G4double PE_z = chamber_h+2*shielding_Cu+2*shielding_PE;
    G4double PE_x = PE_z;
    G4double PE_y = PE_z;

    G4Box* PE_solid = new G4Box( "PE_solid", PE_x/2.0, PE_y/2.0, PE_z/2.0);
    G4LogicalVolume* PE_lv = new G4LogicalVolume( PE_solid, PE_material, "PE_lv");
    new G4PVPlacement( 0, G4ThreeVector(0,0,0), PE_lv, "PE", Pb_lv, false, 0,fCheckOverlaps);


    // Inner copper shielding
    G4double Cu_z = chamber_h+2*shielding_Cu;
    G4double Cu_x = Cu_z;
    G4double Cu_y = Cu_z;

    G4Box* Cu_solid = new G4Box( "Cu_solid", Cu_x/2.0, Cu_y/2.0, Cu_z/2.0);
    G4LogicalVolume* Cu_lv = new G4LogicalVolume( Cu_solid, Cu_material, "Cu_lv");
    new G4PVPlacement( 0, G4ThreeVector(0,0,0), Cu_lv, "Cu", PE_lv, false, 0,fCheckOverlaps);


    // Experimental chamber
    G4double chamber_z = chamber_h;
    G4double chamber_x = chamber_z;
    G4double chamber_y = chamber_z;

    G4Box* chamber_solid = new G4Box( "chamber_solid", chamber_x/2.0, chamber_y/2.0, chamber_z/2.0);
    G4LogicalVolume* chamber_lv = new G4LogicalVolume( chamber_solid, chamber_material, "chamber_lv");
    new G4PVPlacement( 0, G4ThreeVector(0,0,0), chamber_lv, "chamber", Cu_lv, false, 0,fCheckOverlaps);


    //===============  Visualization ===============//

    //world_lv->SetVisAttributes( G4VisAttributes::Invisible );

    frame_lv->SetVisAttributes( G4VisAttributes(G4Colour( 0, 0, 1, 0.5)) );
    Pb_lv->SetVisAttributes( G4VisAttributes(G4Colour( 0.5, 0.5, 0.5, 0.5)) );
    PE_lv->SetVisAttributes( G4VisAttributes(G4Colour( 0.9, 0.9, 0.9, 0.5)) );
    Cu_lv->SetVisAttributes( G4VisAttributes(G4Colour( 0.9, 0.4, 0., 0.5)) );
    chamber_lv->SetVisAttributes( G4VisAttributes(G4Colour( 0.9, 0., 0.9, 0.5)) );

    return world_pv;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void DetectorConstruction::DefineMaterials(){

    G4double a;  // mass of a mole;
    G4double z;  // z=mean number of protons;
    G4double density; //, fractionMass;
    G4String symbol, name;
    // G4int nComponents, nAtoms;
    G4double temp;

    new G4Material("liquid_helium",   z=2., a= 4.00*g/mole,  density= 0.141*g/cm3, kStateLiquid, temp=3.*kelvin);

    /*
    G4NistManager* nistManager =  G4NistManager::Instance();

    G4Element* elH  = new G4Element(name = "Hydrogen"   , symbol = "H"  , z = 1.  , a =   1.008*g/mole);
    G4Element* elC  = new G4Element(name = "Carbon"     , symbol = "C"  , z = 6.  , a =  12.011*g/mole);
    */
    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}


