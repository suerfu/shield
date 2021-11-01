
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4SubtractionSolid.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "DetectorConstructionMessenger.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "Randomize.hh"

#include "G4IStore.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {

    fCheckOverlaps = true;
    fDetectorMessenger = new DetectorConstructionMessenger(this);

    // Set default values for the dimensions.
    
    // World
    // By default 1m x 1m x 1m
    world_x = 1.*m;
    world_y = 1.*m;
    world_z = 1.*m;

    //===============  Build Geometry ===============//
    
    G4NistManager* mat_man = G4NistManager::Instance(); //material mananger
    G4Material* world_material = mat_man->FindOrBuildMaterial("G4_Galactic");

    G4Box* world_solid = new G4Box( "world_solid", world_x/2.0, world_y/2.0, world_z/2.0);
    world_lv = new G4LogicalVolume( world_solid, world_material, "world_lv");
    world_pv = new G4PVPlacement( 0, G4ThreeVector(0,0,0), world_lv, "world", 0, false, 0,fCheckOverlaps);

/*
    list.push_back( ShieldInfo("SS", "G4_STAINLESS-STEEL", 100*cm, 1) );
    list.push_back( ShieldInfo("Pb",    "G4_Pb", 80*cm, 1, 1) );
    list.push_back( ShieldInfo("PE",    "G4_POLYETHYLENE", 70*cm, 1) );
    list.push_back( ShieldInfo("Cu",    "G4_Cu", 60*cm, 1, 1) );
    list.push_back( ShieldInfo("He",    "liquid_helium", 50*cm, 1) );
    list.push_back( ShieldInfo("C",     "G4_C", 40*cm, 1) );
    list.push_back( ShieldInfo("Ge",    "G4_Ge", 30*cm, 1) );
*/    
    list.push_back( ShieldInfo("Si4",    "G4_Si", 90*cm, 1) );
    list.push_back( ShieldInfo("He2",    "liquid_helium", 80*cm, 1) );
    list.push_back( ShieldInfo("Si3",    "G4_Si", 70*cm, 1) );
    list.push_back( ShieldInfo("He1",    "liquid_helium", 60*cm, 1) );
    list.push_back( ShieldInfo("C2",     "G4_C", 50*cm, 1) );

    list.push_back( ShieldInfo("Si2",    "G4_Si", 40*cm, 1) );
    list.push_back( ShieldInfo("C1",     "G4_C", 30*cm, 1) );

    list.push_back( ShieldInfo("Si1",    "G4_Si", 20*cm, 1) );
    list.push_back( ShieldInfo("center","G4_Galactic", 10*cm, 1) );

    list = ExpandList( list );

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


DetectorConstruction::~DetectorConstruction(){
    delete fDetectorMessenger;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
G4VPhysicalVolume* DetectorConstruction::AddStandaloneVolume( G4String name, G4String mat_name, G4double L1, G4double L2 ){

    G4NistManager* mat_man = G4NistManager::Instance(); //material mananger
    G4Material* mat = mat_man->FindOrBuildMaterial(mat_name);

    G4Box* solid1 = new G4Box( name+"_solid1", L1/2, L1/2, L1/2);
    G4Box* solid2 = new G4Box( name+"_solid2", L2/2, L2/2, L2/2);
    G4SubtractionSolid* solid = new G4SubtractionSolid( name+"_solid", solid1, solid2);
    G4LogicalVolume* log = new G4LogicalVolume( solid, mat, name+"_log");
    G4VPhysicalVolume* phy = new G4PVPlacement( 0, G4ThreeVector(0,0,0), log, name+"_phys", world_lv, false, 0, fCheckOverlaps);
}
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4VPhysicalVolume* DetectorConstruction::Construct(){
    DefineMaterials();
    return DefineVolumes();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4VPhysicalVolume* DetectorConstruction::DefineVolumes(){

    G4NistManager* mat_man = G4NistManager::Instance(); //material mananger
    G4Material* world_material = mat_man->FindOrBuildMaterial("G4_Galactic");
    
    // Add the shieldings
    // The list must be created from outside towards inside.
    // The outermost layer is not created, but instead the logical volume is returned.

    G4LogicalVolume* parent_log = world_lv;

    if( nested==true ){
        G4cout << "Creating logically nested geometries.\n";
    }
    else{
        G4cout << "Creating standalone geometries using boolean operations.\n";
    }

    for( unsigned int i=0; i<list.size(); i++ ){
        G4cout << "Constructing " << list[i].name << " shielding with importance " << list[i].bias << G4endl;
        
        G4VSolid* solid = 0;
        
        if( nested==true || i==list.size()-1 ){
            solid = new G4Box( list[i].name+"_solid", list[i].dim/2.0, list[i].dim/2.0, list[i].dim/2.0);
        }
        else{
            G4Box* solid1 = new G4Box( list[i].name+"_solid1", list[i].dim/2.0, list[i].dim/2.0, list[i].dim/2.0);
            G4Box* solid2 = new G4Box( list[i].name+"_solid2", list[i+1].dim/2.0, list[i+1].dim/2.0, list[i+1].dim/2.0);
            solid = new G4SubtractionSolid( list[i].name+"_solid", solid1, solid2 );
        }

        list[i].log = new G4LogicalVolume( solid, mat_man->FindOrBuildMaterial(list[i].material), list[i].name+"_log");

        G4ThreeVector fOffset(0,0,0);

        list[i].phy = new G4PVPlacement( 0, fOffset, list[i].log, list[i].name, parent_log, false, 0);
        if( nested ){
            parent_log = list[i].log;
        }

        list[i].log->SetVisAttributes( G4Color( G4UniformRand(), G4UniformRand(), G4UniformRand(), 0.5) );
    }

    //world_lv->SetVisAttributes( G4VisAttributes::Invisible );

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



G4VIStore* DetectorConstruction::CreateImportanceStore(){
    
    //===============  Importance Sampling to speed up simulation ==============//
    
    G4IStore *istore = G4IStore::GetInstance();

    istore->AddImportanceGeometryCell( 1, *world_pv);
    for( unsigned int i=0; i<list.size(); i++ ){
        istore->AddImportanceGeometryCell( list[i].bias, *list[i].phy);
    }
    
    return istore;
}


std::vector<ShieldInfo> DetectorConstruction::ExpandList( std::vector<ShieldInfo> tmp ){

    std::vector<ShieldInfo> rlist;

    // Go through the vector and identify all layers.
    // Last element is the center of the chamber, there cannot be layers.
    // If nlayer is 1, simply add.
    // If >1, get the next layer's dimension and generate the intermediate ones
    
    G4int bias = 1;

    for( unsigned int i=0; i<tmp.size(); i++){

        G4int nlayer = tmp[i].nlayer;

        if( nlayer<2 || i==tmp.size()-1 ){
            bias *= tmp[i].bias;
            tmp[i].bias = bias;
            rlist.push_back( tmp[i] );
        }
        else{
            G4double delta = (tmp[i+1].dim-tmp[i].dim)/nlayer;
                // incremental dimension for sublayers.

            for( int j=0; j<nlayer; j++ ){

                ShieldInfo sub( tmp[i] );
                    // Current parent layer as a start.

                sub.dim += j*delta;
                
                std::stringstream suffix;
                suffix << "_" << j;
                sub.name += suffix.str();

                bias *= tmp[i].bias;
                sub.bias = bias;
                
                sub.nlayer = 1;
                rlist.push_back( sub );
            }
        }
    }


    return rlist;
}
