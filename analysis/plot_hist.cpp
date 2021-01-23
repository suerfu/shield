#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>

#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"

using namespace std;

// This program is used to plot histograms.
// Input ROOT file is expected to be RQ's, not track-level information.

int main( int argc, char* argv[]){ 

    // Get output file name.
    string outputName;
    cout << "Output file name: ";
    cin >> outputName;

    TFile* outputROOTFile = 0;

    string command;
    cout << "\nEnter main to add a main histogram: ";
    while( command=="main" || cin >> command ){

        if( command!="main" ){
            break;
        }
        command = "";

        if( outputROOTFile==0 ){
            outputROOTFile = new TFile( outputName.c_str(), "NEW");
        }

        // Get parameters of histogram to plot.
        int Nbins;
        float binMin, binMax;
        string histName;
        cout << "\nName, Nbins, min and max of the histogram: ";
        cin >> histName >> Nbins >> binMin >> binMax;
    
        TH1F* hist = new TH1F( histName.c_str(), "", Nbins,  binMin, binMax);

        // Variable / branch of TTree to plot.
        string plotVar;
        cout << "\nBranch to plot: ";
        cin >> plotVar;

        string vetoVar;
        float vetoThreshold(0);
        cout << "\nAdd veto & threshold (enter sub to skip veto option): ";
        cin >> vetoVar;
        if( vetoVar!="sub" ){
            cin >> vetoThreshold;
        }
        else{
            vetoVar = "";
            command = "sub";
        }

        // Enter event loop to read files, scale and legend.
        // Then process the files.
        string inputName;
        double scale;

        cout << "\nEnter main/sub to add a main/sub histogram: ";
        while( command=="sub" || cin >> command ){

            if( command!="sub" ){
                break;
            }

            cout << "\nFile to read and scale: ";
            cin >> inputName >> scale;

            cout << "\nReading " << inputName << endl;
            
            TH1F temp( "temp" ,"", Nbins, binMin, binMax );

            double energy = 0;
            double energy_veto = -1;

            TFile file( inputName.c_str(), "READ" );
            TTree* tree = (TTree*) file.Get( "events" );
            tree->SetBranchAddress( plotVar.c_str(), &energy);
            if( vetoVar != "" ){
                tree->SetBranchAddress( vetoVar.c_str(), &energy_veto);
            }

            for( unsigned long long n=0; n<tree->GetEntries(); n++){
                tree->GetEntry(n);
                if( 1.e9*energy>1 ){
                    if( 1000*energy_veto<vetoThreshold )
                        temp.Fill( 1000*energy );
                }
            }
            temp.Sumw2();
            temp.Scale( scale );
            hist->Add( &temp );

            cout << "\nEnter main/sub to add a main/sub histogram: ";
            command = "";
        }

        outputROOTFile->cd();
        hist->Write();
        if( command!="main" && cin.good() ){
            cout << "\nEnter main to add a main histogram: ";
        }
    }
    cout << "\nWriting output...\n";
    if( outputROOTFile!=0 )
        outputROOTFile->Write();

    return 0;

/*
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

            if( ( data.parentID==0 && strncmp( data.proc_name, "initStep", 8)==0 ) || i==nentries-1 ){

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
*/
}


