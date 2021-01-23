#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>

#include "TTree.h"
#include "TFile.h"

using namespace std;

// TrackInfor is used to hold information read from input ROOT file.
// While reading the input, Hit objects are created and filled into a vector owned by EventInfo
// EventInfo contains all informations needed for outputting reduced quantities.
// At the end of an event marker, Process and Reset methods are called to fill the output tree.

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

struct EventInfo{
    char file_name[128];

    Int_t ID = -1;
    Int_t evtID = -1;

    Double_t edep = 0;
    Double_t time = -1;

    vector<Hit> hit_collection;
};


// reset event info.
void ResetEventInfo( EventInfo* wdata ){
    wdata->ID = -1;
    wdata->evtID = -1;

    wdata->edep = 0;
    wdata->time = -1;

    wdata->hit_collection.clear();
}

void ProcessEventInfo( EventInfo* wdata );

int GetIndex( string );



int main( int argc, char* argv[]){ 

    bool print_usage = false;
    if( argc==1 ){
        print_usage = true;
    }
    else{
        for( int i=1; i<argc; i++){
            if( strncmp( argv[i], "-h", 2) ==0 || strncmp( argv[i], "--help", 6) ==0 ){
                print_usage = true;
            }
        }
    }

    if( print_usage){
        cout << "usage: " << argv[0] << " File-to-process-0 [ File-to-Process-1 ...] Output-ROOT-File\n";
        return 0;
    }

    string output_name = argv[argc-1];
    TFile* outfile = new TFile( output_name.c_str(), "NEW");
    if( !outfile ){
        cout << "Error creating file " << output_name << endl;
        return -2;
    }

    TTree* tree = new TTree( "events", "MC simulation for Compton scattering");
    EventInfo wdata;   // data for writing

    tree->Branch( "file", &wdata.file_name, "file[128]/C");
    tree->Branch( "ID", &wdata.ID, "ID/I");
    tree->Branch( "evtID", &wdata.evtID, "evtID/I");
    tree->Branch( "Edep", &wdata.edep, "Edep/D" );
    tree->Branch( "time", &wdata.time, "time/D");
    

    // ************************** //
    // * Process the input file * //
    // ************************** //


    for( int t = 1; t<argc-1; t++ ){

        string filename( argv[t] );
        TFile* infile = TFile::Open( filename.c_str(), "READ");

        if( !infile ){
            cout << "ERROR reading file " << filename << endl;
        }
        else{
            cout << "Processing " << filename << endl;
        }

        strncpy( wdata.file_name, argv[t], 128);

        TTree* events = (TTree*)infile->Get("events");
        int nentries = events->GetEntries();

        TrackInfo data;    // data for reading
        events -> SetBranchAddress("eventID", &data.eventID);
        events -> SetBranchAddress("trackID", &data.trackID);
        events -> SetBranchAddress("parentID", &data.parentID);
        events -> SetBranchAddress("particle", &data.particle_name);
        events -> SetBranchAddress("volume", &data.volume_name);
        events -> SetBranchAddress("Eki", &data.Eki);
        events -> SetBranchAddress("Ekf", &data.Ekf);
        events -> SetBranchAddress("Edep", &data.Edep);
        events -> SetBranchAddress("t", &data.time);
        events -> SetBranchAddress("process", &data.proc_name);


        // ************************** //
        // * Process the input file * //
        // ************************** //

        int evt_counter = -1;
        for( unsigned int i=0; i<nentries; i++){
            
            events->GetEntry(i);

            bool fillTree = false;

            if(  ( strncmp( data.proc_name, "newEvent", 8)==0 ) )
                fillTree = true;
            else if(  ( strncmp( data.proc_name, "timeReset", 9)==0 ) )
                fillTree = true;
            else if(i==nentries-1 )
                fillTree = true;
            else if(  ( data.parentID==0 && strncmp( data.proc_name, "initStep", 8)==0 ) )
                fillTree = true;

            if( fillTree==true ){

                if( i!=0 ){
                    ProcessEventInfo( &wdata );
                    //cout << wdata.edep_he_n << endl;
                    tree->Fill();
                    evt_counter++;
                    ResetEventInfo( &wdata );
                }
                
                wdata.ID = evt_counter;
                wdata.evtID = data.eventID;
            }
            else{
                string volume_name = data.volume_name;

                Hit hit;
                hit.parentID = data.parentID;
                hit.particle = data.particle_name;
                hit.process = data.proc_name;
                hit.volume = data.volume_name;
                hit.edep = data.Edep;
                hit.time = data.time;

                wdata.hit_collection.push_back( hit );
            }

        }

        infile->Close();
    }

    outfile->cd();
    tree->Write();
    outfile->Close();

    return 0;
}



void ProcessEventInfo( EventInfo* wdata ){
    
    vector<Hit> hc = wdata->hit_collection;

    for( unsigned int i=0; i<hc.size(); i++){
        // compute the time that the particle hit the liquid helium
        if( hc[i].volume=="chamber" ){
            wdata->edep += hc[i].edep;
        }
    }
}

int GetIndex( string fs_name){
    
    int nchar = 0;
    if( fs_name.find("NaI")!=string::npos)
        nchar = 3;
    else
        nchar = 2;
    
    stringstream ss( fs_name );
    char foo;
    for( unsigned int i=0; i<nchar; i++){
        ss >> foo;
    }

    unsigned index = 0;
    ss >> index;

    return index;
}
