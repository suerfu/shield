# Simulation of Gamma-Ray Shielding

This application is developed based on the Geant4 simulation framework and is used to study the effectiveness of different types of shielding material and configurations.

## Compiling the Program

The source code for this application can be obtained by
```
git clone https://github.com/suerfu/shield.git
cd shield
mkdir build
cd build
cmake ..
```
This program assumes one already has Geant4 and ROOT built with CMake.

## Running the Program

This program is run by
```
./shield [-m foo.mac] [-u] [-f output.root] [-r A B]
```
+ -m option specifies a macro file to be executed. In this case, the program runs in batch mode without graphics.
+ -u option allows users to run the program in interactive mode.
+ -f option specifies output ROOT file that records track-level information.
+ -r A B specifies two random seeds to be used. If not specified, the program uses system time-based random seeds.

## Geometry

The default geometry is a series of nested cubes located in a room. The cubes are assumed to have equal sides and share the same center. The dimentions of the room, the dimension and material of each layer are defined in the constructor of DetectorConstruction.cc and have to be changed if a different configuration is to be simulated.

### Room Dimensions

The dimensions of the room are defined in ```DetectorConstruction::DetectorConstruction()``` in DetectorConstruction.cc.
```
world_x = 11.*m;                                                                                                          
world_y = 12.*m;                                                                                                          
world_z = 3.2*m;  // Length of x, y and z dimension of the room, respectively.
```

### Shielding Geometry

The shielding is instantiated via a for-loop later in the code. The required layers are first inserted into a list with
+ name of the volume
+ material of the volume
+ dimension of the volume
+ importance increment
+ No. of segments.

The general syntax is
```
list.push_back( ShieldInfo( "Name", "Material", ImportanceIncr, NoLayers) );
```

An example instantiation is shown below where an experiment chamber made of liquid helium is shielded by 6 cm of copper, 30 cm of polyethylene, 20 cm of lead and 5 cm of stainless steel.
```
list.push_back( ShieldInfo("frame", "G4_STAINLESS-STEEL", 152*cm, 1) );                                                   
list.push_back( ShieldInfo("Pb",    "G4_Pb", 142*cm, 2, 5) );                                                             
list.push_back( ShieldInfo("PE",    "G4_POLYETHYLENE", 102*cm, 1) );                                                      
list.push_back( ShieldInfo("Cu",    "G4_Cu", 42*cm, 2, 2) );                                                              
list.push_back( ShieldInfo("chamber",    "liquid_helium", 30*cm, 1) );
```

#### Dimensions

The code assumes from outer to inner and each volume will be located inside the previously-defined volume. Note that the dimension in the above reference code is the full length, and the thickness would be the difference in dimension (between the layer of concern and the inner layer) divided by 2.

#### Material

The material is obtained by material manager. The name of the material should be either Geant4 built-in (starts with G4, such as G4_Pb) or has been defined somewhere else in the code. If one wishes to define new materials, see ```DetectorConstruction::DefineMaterials()``` for reference where liquid_helium is defined.

#### No. of Segments and Importance

Direct simulation of shielding can be very inefficient since the probability of a gamma ray making through heavy shielding can be very small, and simulating such process directly can be time-consuming. To make it more efficient, the code uses geometry importance-based biasing: an importance value is assumed to each geometry, and when a particle moves from low-importance volume to high-importance volume, the particle is duplicated by the ratio of the importance values. If a particle moves in the opposite direction, the particle is killed at the corresponding probability.

When instantiating the shielding list, user can give a number of segments argument to shield info. In this case, the particular layer of shielding will be segmented into the specified number of layers and each layers will be assigned an importance value that is larger than the immediate outer layer by the specified importance increment value. Note that the importance increment still applies even if the number of layers is 1. Usually partitioning the shielding into layers each approximately one attenuation length of the gamma ray is a conventional practice.

## Custom Commands

This program has a few generator-related custom commands. 

### Custom Source Spectrum

One such custom command is ```/generator/spectrum /path/to/foo.root```. This command can be used to sample a custom source spectrum. This is very useful when, for example, one has generated radioactive decays in a medium and has propagated the resultant gamma rays to some surface and wishes to pick up simulation from there. 

To use this feature, one simply needs to specify the full path of the root file containing the TTree that records the spectrum. The TTree must have the following branches defined and filled:
+ particle: name of the particle
+ nParticle: how many particle is contained in this event.
+ x, y, z: location of the source particle
+ px, py, pz: momentum direction of the source particle
+ Eki: kinetic energy
+ (required for on-wall) theta, phi: polar and azimuth angle of the particle with respect to the originating surface.

When one does not use the custom spectrum feature, this application defaults to Geant4's built-in G4GeneralParticleSource. Thus all commands supported by gps applies.

### Confine the Source to the Wall Surface

If the xyz information in the custom source spectrum is irrelavent and one wishes to generate gamma rays/electrons from the room surface, then this can be achieved by
```
/generator/confineOnWall
```
With this command, the direction of the particles are set with the specified polar and azimuthal angle (required in this case) with respect to the normal direction of the room wall surface.

## Output ROOT File

The output ROOT tree named events contains the following information:
+ eventID
+ trackID
+ stepID
+ particle - name of the particle
+ parentID - track ID of the particle that created this particle
+ volume
+ x, y, z, pz, py, pz
+ t - time of interaction
+ Eki, Ekf - initial and final kinetic energy of this step
+ Edep - energy deposit of the interaction
+ process - name of Geant4 process that defines this step.

Note that each row (entry) in the ROOT file is one step of a track, instead of the reduced quantities of this event. If one wishes to find reduced quantities such as total energy deposit, this ROOT file has to be processed once again.

There are two special processes: newEvent and timeReset. newEvent is noted when the particular step marks the first step of a new event. Thus all previous steps up to the previous special marker are one event.

timeReset is used to note the case where radioactive decay created a long-lived interaction beyond a typical interaction time-window. In this case, the daughter's time is reset to 0 and the first of the daughter events are marked with timeReset. The threshold time window to be grouped as one interaction is 1 ms. This number is specified in StackingAction.cc and can be changed there.
```
G4double window = 1.e6*CLHEP::ns;
```

In the output ROOT file, only the entire events where there is non-zero energy deposit in the chamber volume is recorded. The name of this sensitive volume can be changed in EndOfEventAction in EventAction.cc.
```
// Filter for event recording.                                                                                        
// For different application, this should be changed.                                                                 
bool record = false;                                                                                                  
for( size_t i=0; i < stepCollection.size(); ++i ){                                                                    
  if( stepCollection[i].GetVolumeName()=="chamber" ){                                                               
    record = true;                                                                                                
    break;                                                                                                        
  }
}
```

## Processing the ROOT File



# Making Histograms
