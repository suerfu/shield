
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4tgbVolumeMgr.hh"

#include "G4RunManager.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "DetectorConstructionMessenger.hh"
#include <sstream>

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4EllipticalTube.hh"
#include "G4Cons.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4VisExtent.hh"
#include "G4TwoVector.hh"

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
    world_x = 10.*m;
    world_y = 10.*m;
    world_z = 10*m;

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
    G4Material* utruss_hole_material = world_material;

    G4Material* Pb_material = mat_man->FindOrBuildMaterial("G4_Pb");
    G4Material* PE_material = mat_man->FindOrBuildMaterial("G4_POLYETHYLENE");
    G4Material* Cu_material = mat_man->FindOrBuildMaterial("G4_Cu");
    G4Material* chamber_material = G4Material::GetMaterial("liquid_helium");


    //===============  Build Geometry ===============//


    // World
    G4Box* world_solid = new G4Box( "world_solid", world_x/2.0, world_y/2.0, world_z/2.0);
    G4LogicalVolume* world_lv = new G4LogicalVolume( world_solid, world_material, "world_lv");
    G4VPhysicalVolume* world_pv = new G4PVPlacement( 0, G4ThreeVector(0,0,0), world_lv, "world_pv", 0, false, 0,fCheckOverlaps);

    G4double in = 25.4*mm;

    G4double utruss_thickness = 0.5*in;
    G4double ext_height = 0.71*in;

    G4ThreeVector utruss_origin(0,0,0);
        // This is the origin of the truss frame (lower-left corner of the right half) in the global coordinate.

    // Upper Truss
    for( int angle=0; angle<6; angle++ ){

        std::vector<G4TwoVector> utruss_polygon;
        utruss_polygon.push_back( G4TwoVector( 0.39, 0) );
            // This is the origin. 
            
        utruss_polygon.push_back( G4TwoVector( 2.83*in, 0) );
        utruss_polygon.push_back( G4TwoVector( 2.83*in, -ext_height) );
        utruss_polygon.push_back( G4TwoVector( 3.46*in, -ext_height) );
        utruss_polygon.push_back( G4TwoVector( 3.46*in, 0) );
            // first extruded part

        utruss_polygon.push_back( G4TwoVector( 8.82*in, 0) );
        utruss_polygon.push_back( G4TwoVector( 8.82*in, -ext_height) );
        utruss_polygon.push_back( G4TwoVector( 10.08*in, -ext_height) );
        utruss_polygon.push_back( G4TwoVector( 10.08*in, 0) );

        utruss_polygon.push_back( G4TwoVector( 16.22*in, 0) );
        utruss_polygon.push_back( G4TwoVector( 16.22*in, -ext_height) );
        utruss_polygon.push_back( G4TwoVector( 16.85*in, -ext_height) );
        utruss_polygon.push_back( G4TwoVector( 16.85*in, 0) );

        utruss_polygon.push_back( G4TwoVector( 20.35*in, 0) );
        utruss_polygon.push_back( G4TwoVector( 20.35*in, -ext_height) );
        utruss_polygon.push_back( G4TwoVector( 21.61*in, -ext_height) );
        utruss_polygon.push_back( G4TwoVector( 21.61*in, 0) );

        utruss_polygon.push_back( G4TwoVector( 23.31*in, 0) );
        utruss_polygon.push_back( G4TwoVector( 23.31*in, -ext_height) );
        utruss_polygon.push_back( G4TwoVector( 23.94*in, -ext_height) );
        utruss_polygon.push_back( G4TwoVector( 23.94*in, 0) );
        
        utruss_polygon.push_back( G4TwoVector( 28.34*in, 0) );
        utruss_polygon.push_back( G4TwoVector( 28.34*in, -ext_height) );
        utruss_polygon.push_back( G4TwoVector( 30.10*in, -ext_height) );
        utruss_polygon.push_back( G4TwoVector( 30.10*in, -ext_height-1.5*in) );
        utruss_polygon.push_back( G4TwoVector( 30.50*in, -ext_height-1.5*in) );
        
        G4double angle_incl = 4.96*degree;
            // angle of inclination on the side. Based on this, calculate the intersection at far-side edge.
        G4double y_edge = -tan(angle_incl)*(30.50*in-6.81*in) + (4.96*in-ext_height);
            // Note that in the drawing LZ-1002-1665A.pdf the reference in y direction is different from here.
            // Add the offset ext_height
        
        utruss_polygon.push_back( G4TwoVector( 30.50*in, y_edge) );
        utruss_polygon.push_back( G4TwoVector( 6.81*in, 4.96*in-ext_height) );
        utruss_polygon.push_back( G4TwoVector( 1.75*in, 4.96*in-ext_height) );
        utruss_polygon.push_back( G4TwoVector( 1.75*in, 5.58*in-ext_height) );
        utruss_polygon.push_back( G4TwoVector( 0.39*in, 5.58*in-ext_height) );
        
        std::vector<G4ExtrudedSolid::ZSection> utruss_zsections;
        utruss_zsections.push_back( G4ExtrudedSolid::ZSection( -utruss_thickness/2 , G4TwoVector(0,0), 1.0) );
        utruss_zsections.push_back( G4ExtrudedSolid::ZSection( utruss_thickness/2 , G4TwoVector(0,0), 1.0) );
        
        G4ExtrudedSolid* utruss_solid = new G4ExtrudedSolid( "utruss_solid", utruss_polygon, utruss_zsections );
        
        G4LogicalVolume* utruss_lv = new G4LogicalVolume( utruss_solid, frame_material, "utruss_lv");
        G4RotationMatrix* rot = new G4RotationMatrix();
        rot->rotateX(-90.*deg );
        rot->rotateY( 60.*deg*angle );
        
        std::stringstream ss;
        ss << "_" << angle;
        new G4PVPlacement( rot, utruss_origin, utruss_lv, "utruss_pv"+ss.str(), world_lv, false, 0,fCheckOverlaps);
    
        utruss_lv->SetVisAttributes( G4VisAttributes(G4Colour( 0.5, 0.5, 0.5, 0.5)) );

        //G4VPVParameterisation* utruss_param = 
        //    new TrussParameterisation( );

        //new G4PVParameterised("utruss", frame_lv, world_lv, kZAxis, 6, utruss_param, fCheckOverlaps);

        std::vector<G4ThreeVector> utruss_hole_pos;
            // location of upper truss hole positions.
            // the z position is used as radius
        utruss_hole_pos.push_back( G4ThreeVector( 3.63*in,  2.75*in-ext_height, 50.8*mm/2 ) );
        utruss_hole_pos.push_back( G4ThreeVector( 9.45*in,  2.75*in-ext_height, 50.8*mm/2 ) );
        utruss_hole_pos.push_back( G4ThreeVector( 14.62*in, 2.40*in-ext_height, 38.1*mm/2 ) );
        utruss_hole_pos.push_back( G4ThreeVector( 20.98*in, 1.97*in-ext_height, 25.4*mm/2 ) );
        utruss_hole_pos.push_back( G4ThreeVector( 18.60*in, 1.5*in-ext_height,  12.7*mm/2 ) );
        utruss_hole_pos.push_back( G4ThreeVector( 23.62*in, 1.5*in-ext_height,  12.7*mm/2 ) );
        utruss_hole_pos.push_back( G4ThreeVector( 26.12*in, 1.5*in-ext_height,  12.7*mm/2 ) );
        utruss_hole_pos.push_back( G4ThreeVector( 29.53*in, 1.5*in-ext_height,  12.7*mm/2 ) );

        for( unsigned int i=0; i<utruss_hole_pos.size(); i++){
            ss.clear();
            ss << angle << "_" << i;
            G4Tubs* hole_solid = new G4Tubs( "utruss_hole_sld_"+ss.str(), 0, utruss_hole_pos[i].z(), utruss_thickness/2, 0, CLHEP::twopi );
            G4LogicalVolume* hole_lv = new G4LogicalVolume( hole_solid, utruss_hole_material, G4String("utruss_hole_lv_")+ss.str() );
            G4ThreeVector hole_pos( utruss_hole_pos[i].x(), utruss_hole_pos[i].y() );
            new G4PVPlacement( 0, hole_pos, hole_lv, "utruss_lv"+ss.str(), utruss_lv, false, 0, fCheckOverlaps );
    
            hole_lv->SetVisAttributes( G4VisAttributes(G4Colour( 0.1, 0.1, 0.1, 0.9)) );
        }
    }

    G4Box* utruss_center1_solid = new G4Box("utruss_center1_solid", 0.39*in, utruss_thickness/2, (2.56*in-ext_height)/2);
    G4Box* utruss_center2_solid = new G4Box("utruss_center2_solid", 0.39*in, utruss_thickness/2, (4.21-2.56)*in/2);
    G4Box* utruss_center3_solid = new G4Box("utruss_center3_solid", 0.39*in, utruss_thickness/2, (5.58-4.21)*in/2);

    G4LogicalVolume* utruss_center1_lv = new G4LogicalVolume(utruss_center1_solid, frame_material, "utruss_center1_lv");
    G4LogicalVolume* utruss_center2_lv = new G4LogicalVolume(utruss_center2_solid, frame_material, "utruss_center2_lv");
    G4LogicalVolume* utruss_center3_lv = new G4LogicalVolume(utruss_center3_solid, frame_material, "utruss_center3_lv");

    G4RotationMatrix* rot1 = new G4RotationMatrix();
    new G4PVPlacement( rot1, utruss_origin+G4ThreeVector(0,0,(2.56*in-ext_height)/2), utruss_center1_lv, "utruss_center1_pv", world_lv, false, 0,fCheckOverlaps);
    
    G4RotationMatrix* rot2 = new G4RotationMatrix();
    rot2->rotateZ(60.*deg );
    new G4PVPlacement( rot2, utruss_origin+G4ThreeVector(0,0,2.56*in+(4.21-2.56)*in/2-ext_height), utruss_center2_lv, "utruss_center2_pv", world_lv, false, 0,fCheckOverlaps);
    
    G4RotationMatrix* rot3 = new G4RotationMatrix();
    rot3->rotateZ(60.*deg );
    new G4PVPlacement( rot3, utruss_origin+G4ThreeVector(0,0,4.21*in+(5.58-4.21)*in/2-ext_height), utruss_center3_lv, "utruss_center3_pv", world_lv, false, 0,fCheckOverlaps);
        

    G4double utruss_cap_center_thickness = 1.*in;
    G4Tubs* utruss_cap_center_solid = new G4Tubs("utruss_cap_center_solid", 0, 1.821*in, utruss_cap_center_thickness/2, 0, CLHEP::twopi);
    G4LogicalVolume* utruss_cap_center_lv = new G4LogicalVolume(utruss_cap_center_solid, frame_material, "utruss_cap_center_lv");
    new G4PVPlacement( 0, G4ThreeVector(0,0,5.58*in-ext_height+utruss_cap_center_thickness/2), utruss_cap_center_lv, "utruss_cap_center", world_lv, false, 0, fCheckOverlaps);

    G4Tubs* utruss_cap_side_solid = new G4Tubs("utruss_cap_side_solid", 1.821*in, 4.75*in/2, 1.5*in/2, 0, CLHEP::twopi);
    G4LogicalVolume* utruss_cap_side_lv = new G4LogicalVolume(utruss_cap_side_solid, frame_material, "utruss_cap_side_lv");
    new G4PVPlacement( 0, G4ThreeVector(0,0,5.58*in-ext_height-0.5*in+1.5*in/2), utruss_cap_side_lv, "utruss_cap_side", world_lv, false, 0, fCheckOverlaps);


    G4int Narch = 48;
    G4Tubs* utruss_ring_solid = new G4Tubs("utruss_pring_solid", 61*in/2, 61*in/2+0.22, 4.72*in/2, -CLHEP::twopi/Narch/2, CLHEP::twopi/Narch);

    G4EllipticalTube* utruss_ring_arch = new G4EllipticalTube( "utruss_ring_arch", 1.97*in+0.39*in, 1.4*in, 0.5*in);
    G4RotationMatrix* rot_arch = new G4RotationMatrix();
    rot_arch->rotateY(90*degree);
    G4ThreeVector trans_arch( 30.64*in, 0, -(1.97*in+0.39*in)/2);
    G4SubtractionSolid* utruss_ring_sub = new G4SubtractionSolid("utruss_ring_sec_solid", utruss_ring_solid, utruss_ring_arch, rot_arch, trans_arch);

    for(int i=0; i<Narch; i++){
        G4LogicalVolume* utruss_ring_lv = new G4LogicalVolume(utruss_ring_sub, frame_material, "utruss_ring_lv");

        G4RotationMatrix* rot_ring = new G4RotationMatrix();
        rot_ring->rotateZ( i*CLHEP::twopi/Narch );
        new G4PVPlacement( rot_ring, G4ThreeVector(0,0,0), utruss_ring_lv, "utruss_ring", world_lv, false, 0, fCheckOverlaps);
    
        utruss_ring_lv->SetVisAttributes( G4VisAttributes(G4Colour( 0., 1., 0., 0.5)) );
    }

    
/*
    std::vector<G4TwoVector> utruss_center_polygon;
    utruss_center_polygon.push_back( G4TwoVector( sqrt(3)*utruss_thickness/2, utruss_thickness/2) );
    utruss_center_polygon.push_back( G4TwoVector( 0, utruss_thickness) );
    utruss_center_polygon.push_back( G4TwoVector( -sqrt(3)*utruss_thickness/2, utruss_thickness/2) );
    utruss_center_polygon.push_back( G4TwoVector( -sqrt(3)*utruss_thickness/2, -utruss_thickness/2) );
    utruss_center_polygon.push_back( G4TwoVector( 0, -utruss_thickness) );
    utruss_center_polygon.push_back( G4TwoVector( sqrt(3)*utruss_thickness/2, -utruss_thickness/2) );

    std::vector<G4ExtrudedSolid::ZSection> utruss_center_zsections;
    utruss_center_zsections.push_back( G4ExtrudedSolid::ZSection( 0 , G4TwoVector(0,0), 1.0) );
    utruss_center_zsections.push_back( G4ExtrudedSolid::ZSection( 107.95*in , G4TwoVector(0,0), 1.0) );
        
    G4ExtrudedSolid* utruss_center_solid = new G4ExtrudedSolid( "utruss_center_solid", utruss_center_polygon, utruss_center_zsections );
    G4LogicalVolume* utruss_center_lv = new G4LogicalVolume( utruss_center_solid, frame_material, "utruss_center_lv");
    new G4PVPlacement( 0, utruss_origin, utruss_center_lv, "utruss_center", world_lv, false, 0, fCheckOverlaps);

    G4double utruss_cap_thickness = 15.88*in;
    G4Tubs* utruss_cap_solid = new G4Tubs( "utruss_cap", 0, 101.60*in, utruss_cap_thickness/2, 0, CLHEP::twopi);
    G4LogicalVolume* utruss_cap_lv = new G4LogicalVolume( utruss_cap_solid, frame_material, "utruss_cap_lv");

    G4ThreeVector utruss_cap_offset = utruss_origin + G4ThreeVector(0,0,107.95*in+utruss_cap_thickness/2);
    new G4PVPlacement( 0, utruss_cap_offset, utruss_cap_lv, "utruss_cap", world_lv, false, 0, fCheckOverlaps);
*/
    /*
    // Lead shielding
    G4double Pb_z = chamber_h+2*shielding_Cu+2*shielding_PE+2*shielding_Pb;
    G4double Pb_x = Pb_z;
    G4double Pb_y = Pb_z;

    G4Box* Pb_solid = new G4Box( "Pb_solid", Pb_x/2.0, Pb_y/2.0, Pb_z/2.0);
    G4LogicalVolume* Pb_lv = new G4LogicalVolume( Pb_solid, Pb_material, "Pb_lv");
    new G4PVPlacement( 0, G4ThreeVector(0,0,0), Pb_lv, "Pb_pv", frame_lv, false, 0,fCheckOverlaps);
    

    // Polyethylene shielding
    G4double PE_z = chamber_h+2*shielding_Cu+2*shielding_PE;
    G4double PE_x = PE_z;
    G4double PE_y = PE_z;

    G4Box* PE_solid = new G4Box( "PE_solid", PE_x/2.0, PE_y/2.0, PE_z/2.0);
    G4LogicalVolume* PE_lv = new G4LogicalVolume( PE_solid, PE_material, "PE_lv");
    new G4PVPlacement( 0, G4ThreeVector(0,0,0), PE_lv, "PE_pv", Pb_lv, false, 0,fCheckOverlaps);


    // Inner copper shielding
    G4double Cu_z = chamber_h+2*shielding_Cu;
    G4double Cu_x = Cu_z;
    G4double Cu_y = Cu_z;

    G4Box* Cu_solid = new G4Box( "Cu_solid", Cu_x/2.0, Cu_y/2.0, Cu_z/2.0);
    G4LogicalVolume* Cu_lv = new G4LogicalVolume( Cu_solid, Cu_material, "Cu_lv");
    new G4PVPlacement( 0, G4ThreeVector(0,0,0), Cu_lv, "Cu_pv", PE_lv, false, 0,fCheckOverlaps);


    // Experimental chamber
    G4double chamber_z = chamber_h;
    G4double chamber_x = chamber_z;
    G4double chamber_y = chamber_z;

    G4Box* chamber_solid = new G4Box( "chamber_solid", chamber_x/2.0, chamber_y/2.0, chamber_z/2.0);
    G4LogicalVolume* chamber_lv = new G4LogicalVolume( chamber_solid, chamber_material, "chamber_lv");
    new G4PVPlacement( 0, G4ThreeVector(0,0,0), chamber_lv, "chamber_pv", Cu_lv, false, 0,fCheckOverlaps);


    //===============  Visualization ===============//


    Pb_lv->SetVisAttributes( G4VisAttributes(G4Colour( 0.5, 0.5, 0.5, 0.5)) );
    PE_lv->SetVisAttributes( G4VisAttributes(G4Colour( 0.9, 0.9, 0.9, 0.5)) );
    Cu_lv->SetVisAttributes( G4VisAttributes(G4Colour( 0.9, 0.4, 0., 0.5)) );
    chamber_lv->SetVisAttributes( G4VisAttributes(G4Colour( 0.9, 0., 0.9, 0.5)) );
    */
    world_lv->SetVisAttributes( G4VisAttributes::Invisible );
    //utruss_lv->SetVisAttributes( G4VisAttributes(G4Colour( 0.5, 0.5, 0.5, 0.5)) );

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



void TrussParameterisation::ComputeTransformation( const G4int copyNo, G4VPhysicalVolume* physVol) const{
        physVol->SetTranslation( G4ThreeVector(0,0,0));
        G4RotationMatrix* rot = new G4RotationMatrix();
        rot->rotateX(-90.*deg);
        rot->rotateY(-60.*deg*copyNo);
        physVol->SetRotation( rot );
    }


