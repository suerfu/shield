/// \file DetectorConstructionMessenger.hh
/// \brief Definition of the DetectorConstructionMessenger class

#ifndef DetectorConstructionMessenger_h
#define DetectorConstructionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;

class DetectorConstructionMessenger: public G4UImessenger{

public:
    DetectorConstructionMessenger( DetectorConstruction* );
    virtual ~DetectorConstructionMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

private:
    DetectorConstruction* DetectorPlacement;

    G4UIdirectory* DetectorPlacementDir;
//    G4UIcmdWithADouble* AngleCmd;
//    G4UIcmdWithADouble* FSDistanceCmd;
};

#endif
