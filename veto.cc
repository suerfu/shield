//
/// \file veto.cc
/// \brief Program used to study the shielding and veto efficiencies of different materials and geometries.

#include "DetectorConstruction.hh"
#include "GeneratorAction.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"

#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "Shielding.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void PrintUsage() {
    G4cerr << "Usage: veto [-m macro.mac ] [-u] [-f output.root] [-r seed0 seed1] [-g generator]" << G4endl;
    G4cerr << "\t-m, used to spefify the macro file to execute.\n";
    G4cerr << "\t-u, enter interactive session.\n";
    G4cerr << "\t-f, spefify output ROOT file.\n";
    G4cerr << "\t-r, spefify two random seeds to be used.\n" << G4endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


int main(int argc,char** argv){

    // Evaluate arguments
    //

    // Use the flags to determine whether program run in batch mode or interactive mode.
    bool exit = false;
        // if exit is set true (no valid argument), the program will print usage and exit.
        // user must explicitly specify batch mode by providing a macro, or the interactive mode by -u.


    // macro and output filename.
    // These variables are set by inspecting the commandline argument.
    G4String macro;
    G4String filename = "";


    // Random engine.
    // The seed is first set by the current time. Later it will be updated by the commandline parameter if provided.
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4long seeds[2];
    time_t systime = time(NULL);
    seeds[0] = (long) systime;
    seeds[1] = (long) (systime*G4UniformRand());

    
    // Loop over the commandline arguments.
    // 
    for ( G4int i=1; i<argc; i++ ) {
        if ( G4String(argv[i] ) == "-m" ){
            macro = argv[++i];
            exit = false;
                // batch mode.
        }
        else if ( G4String(argv[i]) == "-u" ){
            exit = false;
                // enter user interactive mode.
        }
        else if ( G4String(argv[i]) == "-f" ){
            filename = G4String(argv[++i]);
                // output filename
        }
        else if ( G4String(argv[i]) == "-r" ){
            seeds[0] = G4long(argv[++i]);
            seeds[1] = G4long(argv[++i]);
                // random seeds
        }
    }


    // Neither batch mode (no macro) nor interactive session, print usage and exit.
    if( exit==true ){
        PrintUsage();
        return 0;
    }


    // Choose the Random engine
    //
    G4cout << "Seeds for random generator are " << seeds[0] << ", " << seeds[1] << G4endl;
    G4Random::setTheSeeds(seeds);

  
    // Detect interactive mode (if no macro provided) and define UI session
    //
    G4UIExecutive* ui = 0;
    if ( ! macro.size() ) {
        ui = new G4UIExecutive( argc, argv );
    }


    // Construct the default run manager. At this step, don't consider multi-threading yet.
    //
    G4RunManager * runManager = new G4RunManager;

    
    // Construct detector geometry
    DetectorConstruction* detConstruction = new DetectorConstruction();
    runManager->SetUserInitialization( detConstruction );
  
    
    // Physics list. Use a ready-to-use list.
    G4VModularPhysicsList* physicsList = new Shielding;
    runManager->SetUserInitialization( physicsList );
  
    // avoid using ActionInitialization for now
    /*
    ActionInitialization* actionInitialization
        = new ActionInitialization( detConstruction, filename);
    runManager->SetUserInitialization( actionInitialization );
    */


    // Primary generator
    runManager->SetUserAction( new GeneratorAction() );


    // Run action
    RunAction* runAction = new RunAction;
    runAction->SetOutputFileName( filename );
    runAction->AddRandomSeeds( seeds, 2);
    runManager->SetUserAction( runAction );


    // Event action
    EventAction* eventAction = new EventAction( runAction );
    runManager->SetUserAction( eventAction );

    
    // Tracking and stepping
    runManager->SetUserAction( new TrackingAction( eventAction ) );
    runManager->SetUserAction( new SteppingAction( detConstruction, eventAction ) );


    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();


    // Process macro or start UI session
  
    if ( macro.size() ){
        // batch mode
        // actionInitialization
        runAction->AddMacro( macro );
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);
    }
    else{
        // interactive mode : define UI session
        G4VisManager* visManager = new G4VisExecutive;
        visManager->Initialize();

        UImanager->ApplyCommand("/control/execute init_vis.mac");
        if (ui->IsGUI()) {
            UImanager->ApplyCommand("/control/execute gui.mac");
        }
        ui->SessionStart();

        delete visManager;
        delete ui;
    }


    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !

    delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
