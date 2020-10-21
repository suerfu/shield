
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
    G4Material* medium_material = mat_man->FindOrBuildMaterial("rock_Homestake");


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
    G4LogicalVolume* medium_lv = new G4LogicalVolume( medium_solid, medium_material, "medium_lv");
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
    G4double density, fracM;
    G4String symbol, name;
    G4int nComponents;

    G4Material* rock = new G4Material("rock_Homestake", density= 3.26*g/cm3, nComponents = 12);
        // The density for Homestake formation is taken from:
        //      https://sdaos.org/wp-content/uploads/pdfs/2014/roggenthen%2033-41.pdf
        // The chemical composition is taken from:
        //      https://arxiv.org/pdf/0912.0211.pdf

    G4Element* elSi = new G4Element(name = "Silicon"    , symbol = "Si" , z = 14. , a =  28.086*g/mole);
    G4Element* elTi = new G4Element(name = "Titanium"   , symbol = "Ti" , z = 22. , a =  47.867*g/mole);
    G4Element* elAl = new G4Element(name = "Aluminum"   , symbol = "Al" , z = 13. , a =  26.981*g/mole);
    G4Element* elFe = new G4Element(name = "Iron"       , symbol = "Fe" , z = 26. , a =  55.845*g/mole);
    G4Element* elMn = new G4Element(name = "Manganese"  , symbol = "Mn" , z = 25. , a =  54.938*g/mole);
    G4Element* elMg = new G4Element(name = "Magnesium"  , symbol = "Mg" , z = 12. , a =  24.405*g/mole);
    G4Element* elCa = new G4Element(name = "Calsium"    , symbol = "Ca" , z = 20. , a =  40.078*g/mole);
    G4Element* elNa = new G4Element(name = "Sodium"     , symbol = "Na" , z = 11. , a =  22.990*g/mole);
    G4Element* elK  = new G4Element(name = "Potassium"  , symbol = "K" ,  z = 19. , a =  39.098*g/mole);
    G4Element* elP  = new G4Element(name = "Phosphorus" , symbol = "P"  , z = 15. , a =  30.974*g/mole);
    G4Element* elH  = new G4Element(name = "Hydrogen"   , symbol = "H"  , z = 1.  , a =   1.008*g/mole);
    G4Element* elO  = new G4Element(name = "Oxygen"     , symbol = "O"  , z = 8.  , a =  15.999*g/mole);
    

    G4double fracM_O = 0;
        // used to accumulate fractional mass of oxygen in the rock
    G4double fraction = 0;
        // fraction of weight of the composite in the rock.

    
    // ============ SiO2 =============
    fraction = 0.437;
    fracM = 28.086/(28.086+2*15.999);
    fracM_O += (1-fracM)*fraction;
    rock->AddElement( elSi, fracM * fraction );
    
    
    // ============ TiO2 =============
    fraction = 0.0122;
    fracM = 47.867/(47.867+2*15.999);
    fracM_O += (1-fracM)*fraction;
    rock->AddElement( elTi, fracM * fraction );
    
    
    // ============ Al2O3 =============
    fraction = 0.136;
    fracM = 2*26.981/(2*26.981+3*15.999);
    fracM_O += (1-fracM)*fraction;
    rock->AddElement( elAl, fracM * fraction );
    

    // ============ FeO =============
    fraction = 0.127;
    fracM = 55.845/(55.845+15.999);
    fracM_O += (1-fracM)*fraction;
    rock->AddElement( elFe, fracM * fraction );
    
    
    // ============ MnO =============
    fraction = 0.0013;
    fracM = 54.938/(54.938+15.999);
    fracM_O += (1-fracM)*fraction;
    rock->AddElement( elMn, fracM * fraction );
    
    
    // ============ MgO =============
    fraction = 0.07;
    fracM = 24.405/(24.405+15.999);
    fracM_O += (1-fracM)*fraction;
    rock->AddElement( elMg, fracM * fraction );
    
    
    // ============ CaO =============
    fraction = 0.079;
    fracM = 40.078 / ( 40.078 + 15.999 );
    fracM_O += (1-fracM)*fraction;
    rock->AddElement( elCa, fracM * fraction );


    // ============ Na2O =============
    fraction = 0.0287;
    fracM = 2*22.990 / ( 2*22.990 +15.999 );
    fracM_O += (1-fracM)*fraction;
    rock->AddElement( elNa, fracM * fraction );


    // ============ K2O =============
    fraction = 0.0021;
    fracM = 2*39.098 / ( 2*39.098 +15.999 );
    fracM_O += (1-fracM)*fraction;
    rock->AddElement( elK, fracM * fraction );


    // ============ P2O5 =============
    fraction = 0.0007;
    fracM = 2*30.974 / ( 2*30.974 +5*15.999 );
    fracM_O += (1-fracM)*fraction;
    rock->AddElement( elP, fracM * fraction );


    // ============ H2O =============
    fraction = 0.107;
    fracM = 2*1.008 / ( 2*1.008 +15.999 );
    fracM_O += (1-fracM)*fraction;
    rock->AddElement( elH, fracM * fraction );


    // ============ Oxygen =============
    rock->AddElement( elO, fracM_O );

    // Print materials
    G4cout << "Fraction of oxygem by mass is " << fracM_O << G4endl;
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
