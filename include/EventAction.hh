
// $Id: EventAction.hh $
//
/// \file EventAction.hh
/// \brief Definition of the EventAction class

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "StepInfo.hh"
#include "RunAction.hh"

class EventAction : public G4UserEventAction{

public:

    EventAction( RunAction* input_run_action );
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    vector<StepInfo>& GetStepCollection();

private:
     
    RunAction* run_action;
    
    TTree* data_tree;
    
    // methods
    void PrintEventStatistics() const;

    vector<StepInfo> stepCollection;

    int eventID;
    int trackID;
    int stepID;
    int parentID;
    int nParticle;

    int max_char_len;
    char particle_name[16];
    char volume_name[16];
    char process_name[16];

    G4String tmp_particle_name;
    G4String tmp_volume_name;
    G4String tmp_process_name;

    int volume_copy_number;
    double Eki;
    double Ekf;
    double edep;

    G4ThreeVector position;
    G4ThreeVector momentum;

    double x;
    double y;
    double z;
    double r;
    double rphi;

    double px;
    double py;
    double pz;
    double theta;
    double phi;

    double global_time;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
