
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

//#include "G4tgbVolumeMgr.hh"

//#include "G4RunManager.hh"
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
    world_x = 11.*m;
    world_y = 12.*m;
    world_z = 3.2*m;

    offset = 5*cm;
/*
    // Constructor format: Name, Material, Largest dim, Weight, No. of layers.
    list.push_back( ShieldInfo("frame", "G4_STAINLESS-STEEL",   132*cm, 1   ) );
    list.push_back( ShieldInfo("Pb",    "G4_Pb",                122*cm, 1, 1   ) );
    //list.push_back( ShieldInfo("Pb",    "G4_Pb",                122*cm, 1   ) );
    list.push_back( ShieldInfo("PE",    "G4_POLYETHYLENE",      82*cm,  1   ) );
        // PE should be 122 102 82 for 10 20 and 30 cm of lead.
    list.push_back( ShieldInfo("Cu",    "G4_Cu",                42*cm,  1, 1   ) );
    list.push_back( ShieldInfo("veto",  "G4_Cu",                30*cm,  1   ) );
    list.push_back( ShieldInfo("chamber",    "liquid_helium",   20*cm,  1   ) );
*/

    list.push_back( ShieldInfo("frame", "G4_STAINLESS-STEEL",   132*cm, 1   ) );
    list.push_back( ShieldInfo("Pb",    "G4_Pb",                122*cm, 2, 10  ) );
    list.push_back( ShieldInfo("PE",    "G4_POLYETHYLENE",      82*cm,  1   ) );
    list.push_back( ShieldInfo("Cu",    "G4_Cu",                42*cm,  2, 2   ) );
    list.push_back( ShieldInfo("veto",  "G4_Cu",                30*cm,  1   ) );
    list.push_back( ShieldInfo("chamber",    "liquid_helium",   20*cm,  1   ) );

    list = ExpandList( list );
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

    //===============  Materials ===============//

    G4NistManager* mat_man = G4NistManager::Instance(); //material mananger
    G4Material* world_material = mat_man->FindOrBuildMaterial("G4_Galactic");
    
    //===============  Build Geometry ===============//

    // World
    G4Box* world_solid = new G4Box( "world_solid", world_x/2.0, world_y/2.0, world_z/2.0);
    G4LogicalVolume* world_lv = new G4LogicalVolume( world_solid, world_material, "world_lv");
    world_pv = new G4PVPlacement( 0, G4ThreeVector(0,0,0), world_lv, "world", 0, false, 0,fCheckOverlaps);

    
    // Add the shieldings
    // The list must be created from outside towards inside.
    // The outermost layer is not created, but instead the logical volume is returned.

    G4LogicalVolume* parent_log = 0;

    for( unsigned int i=0; i<list.size(); i++ ){
        G4cout << "Constructing " << list[i].name << " shielding with importance " << list[i].bias << G4endl;
        
        G4Box* solid = new G4Box( list[i].name+"_solid", list[i].dim/2.0, list[i].dim/2.0, list[i].dim/2.0);

        list[i].log = new G4LogicalVolume( solid, mat_man->FindOrBuildMaterial(list[i].material), list[i].name+"_log");

        G4ThreeVector fOffset(0,0,0);
        if( i==0 ){
            parent_log = world_lv;
            fOffset = G4ThreeVector(0,0,-world_z/2+list[i].dim/2+offset);
        }
        list[i].phy = new G4PVPlacement( 0, fOffset, list[i].log, list[i].name, parent_log, false, 0);
        parent_log = list[i].log;
        
        list[i].log->SetVisAttributes( G4Color( G4UniformRand(), G4UniformRand(), G4UniformRand(), 0.5) );

        G4cout << "Constructed " << list[i].name << G4endl;
        G4cout << "\tx from " << fOffset.x()-list[i].dim/2*mm << " mm to " << fOffset.x()+list[i].dim/2*mm << " mm,";
        G4cout << "\ty from " << fOffset.y()-list[i].dim/2*mm << " mm to " << fOffset.y()+list[i].dim/2*mm << " mm,";
        G4cout << "\tz from " << fOffset.z()-list[i].dim/2*mm << " mm to " << fOffset.z()+list[i].dim/2*mm << " mm\n";

    }

    //world_lv->SetVisAttributes( G4VisAttributes::Invisible );

    return world_pv;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void DetectorConstruction::DefineMaterials(){

    G4double A;  // mass of a mole;
    G4double Z;  // z=mean number of protons;
    G4double density; //, fractionMass;
    G4String symbol, name;
    // G4int nComponents, nAtoms;
    G4double temp;

    new G4Material("liquid_helium",   Z=2., A= 4.00*g/mole,  density= 0.141*g/cm3, kStateLiquid, temp=3.*kelvin);
    
    // NaI crystal
    G4Element* elNa = new G4Element("Sodium",  "Na", Z=11., A= 22.989768*g/mole);
    G4Element* elI  = new G4Element("Iodine",  "I",  Z=53., A= 126.90447*g/mole);
    density = 3.67 *g/cm3;
    G4Material* NaI = new G4Material("NaI", density, 2);
    NaI-> AddElement(elNa, 1);
    NaI-> AddElement(elI,  1);

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
