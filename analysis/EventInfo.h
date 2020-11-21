#include <vector>
#include <map>
#include <sstream>
#include <string>

#include "TTree.h"

struct TrackInfo{
    int eventID;
    int trackID;
    int parentID;
    char particle_name[16];
    char volume_name[16];
    char proc_name[16];
    double Eki;
    double Ekf;
    double Edep;
    double time;
};

struct Hit{
    int parentID;
    string particle;
    string volume;
    string process;
    Double_t edep;
    Double_t time;
};


class EventInfo{

public:
    char file_name[128];

    Int_t ID = -1;
    Int_t evtID = -1;

    void SetTTree( TTree* t){ tree = t;}

    void ConfigWriteTree(){
        tree->Branch( "file", &file_name, "file[128]/C");
        tree->Branch( "ID", &ID, "ID/I");
        tree->Branch( "evtID", &evtID, "evtID/I");

        for( unsigned int i = 0; i<volumes.size(); i++ ){
            
            stringstream name;

        }

        tree->Branch( "edep_he_e", &wdata.edep_he_e, "edep_he_e/D" );
        tree->Branch( "edep_he_n", &wdata.edep_he_n, "edep_he_n/D" );
    }

    void Fill( TTree);

    void Reset();

private:
    TTree* tree;
    map<Hit> maphit_collection;
};

void ResetEventInfo( event_info* wdata ){
    wdata->ID = -1;
    wdata->evtID = -1;

    wdata->edep_he_e = 0;
    wdata->edep_he_n = 0;
    wdata->time_he = -1;

    wdata->nscatter = 0;

    for( unsigned int i=0; i<10; i++ ){
        wdata->edep_fs_n[i] = 0;
        wdata->edep_fs_e[i] = 0;
    }
    wdata->time_fs = -1;
    
    wdata->floor_time = -1;
    wdata->floor_flag = 0;

    wdata->hit_collection.clear();
}

