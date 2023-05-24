#ifndef EVENTTREE_H
#define EVENTTREE_H

#include<TFile.h>
#include<TTree.h>
#include<TChain.h>

#include<vector>

using namespace std;
const Int_t maxP = 600;

class EventTree{
 public:
    EventTree(int nFiles, bool xRootDAccess, string year, char** fileNames, bool isMC);
    ~EventTree();
    Long64_t GetEntries();
    Int_t GetEntry(Long64_t entry);
    
    TChain* chain;
    
    // include all variables just in case
    UInt_t    run_;
    ULong64_t event_;
    UInt_t    lumis_;
    bool isData_;
    
    // variables
    // for 2022G 
    /*
    UInt_t          nEle;
    UInt_t          nTrigObj;
    Int_t           TrigObj_id[100];   
    */
    // for 2023C 
    Int_t           nEle;
    Int_t           nTrigObj;
    UShort_t        TrigObj_id[100];   


    Float_t         elePhi[100];
    Float_t         elePt[100];
    Float_t         eleEta[100];
    Float_t         eleDeltaEtaSC[100];
    Int_t           eleCharge[100];
    Float_t         eleMass[100];
    Int_t           eleID[100];
    Float_t         eleMiniPFRelIso[100];

    Float_t         TrigObj_pt[100];   
    Float_t         TrigObj_eta[100];   
    Float_t         TrigObj_phi[100];  
    Int_t           TrigObj_filterBits[100];   
};
#endif
