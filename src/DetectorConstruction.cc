
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

    list.push_back( ShieldInfo("frame", "G4_STAINLESS-STEEL", 152*cm, 2) );
    list.push_back( ShieldInfo("Pb1",    "G4_Pb", 142*cm, 4) );
    list.push_back( ShieldInfo("Pb2",    "G4_Pb", 132*cm, 8) );
    list.push_back( ShieldInfo("Pb3",    "G4_Pb", 122*cm, 16) );
    list.push_back( ShieldInfo("Pb4",    "G4_Pb", 112*cm, 32) );
    list.push_back( ShieldInfo("PE",    "G4_POLYETHYLENE", 102*cm, 32) );
    list.push_back( ShieldInfo("Cu",    "G4_Cu", 42*cm, 32) );

//    list.push_back( ShieldInfo("Pb",    "G4_Pb", 150*cm, 1) );
//    list.push_back( ShieldInfo("PE",    "G4_POLYETHYLENE", 148*cm, 1) );
//    list.push_back( ShieldInfo("Cu",    "G4_Cu", 42*cm, 1) );
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
    G4Material* frame_material = mat_man->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    G4Material* Pb_material = mat_man->FindOrBuildMaterial("G4_Pb");
    G4Material* PE_material = mat_man->FindOrBuildMaterial("G4_POLYETHYLENE");
    G4Material* Cu_material = mat_man->FindOrBuildMaterial("G4_Cu");
    G4Material* chamber_material = G4Material::GetMaterial("liquid_helium");

    
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
        G4cout << "Constructing " << list[i].name << " shielding...\n";
        
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
