#ifndef GEMATERIALS_H
#define GEMATERIALS_H 1

#include "globals.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"

class OxCryoMaterials {
    
public:
    virtual ~OxCryoMaterials();
    static OxCryoMaterials* GetInstance();
    G4Material* GetMaterial(const G4String);
    
private:
    OxCryoMaterials();
    G4NistManager*     fNistMan;
    static OxCryoMaterials* fInstance;
    void CreateMaterials();

    G4Material* Shap_powder;
    G4Material* synthetic_silica;
    G4Material* Cf;
    G4Material* IAEA_powder;
    G4Material* Pb210_powder;
    G4Material* MGS4_powder;
    G4Material* sand;
    G4Material* ceramic;
    G4Material* kovar;
    G4Material* epoxy;
    G4Material* dirt;
    G4Material* CaMoO4;
    G4Material* stainless_steel_304;
    G4Material* borosilicate_glass;
    G4Material* al6061;
    G4Material* soil;
    G4Material* wood;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
