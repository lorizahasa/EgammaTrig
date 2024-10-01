#ifndef EVENTTREE_H
#define EVENTTREE_H

#include<TFile.h>
#include<TTree.h>
#include<TChain.h>
#include<TMath.h>

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
    //Triggers
    Bool_t  im24_, itm24_, im27_, m50_, tm50_, m100_, tm100_;
    Bool_t  e27_, e32_, e35_, e115_, e45j200_, e50j165_, p175_, p200_;
    //Filters
    Bool_t   Flag_goodVertices_ ;
    Bool_t   Flag_globalSuperTightHalo2016Filter_ ;
    Bool_t   Flag_HBHENoiseFilter_ ;
    Bool_t   Flag_HBHENoiseIsoFilter_ ;
    Bool_t   Flag_EcalDeadCellTriggerPrimitiveFilter_ ;
    Bool_t   Flag_BadPFMuonFilter_ ;
    Bool_t   Flag_ecalBadCalibFilter_ ;
    Bool_t   Flag_eeBadScFilter_;

    // variables
    // for 2022G 
    /*
    UInt_t          nEle;
    Int_t           eleID[100];
    UInt_t          nTrigObj;
    Int_t           TrigObj_id[100];   
    
    // for 2023C 
    Int_t           nEle;
    UChar_t         eleID[100];
    Int_t           nTrigObj;
    UShort_t        TrigObj_id[100];   
    */
    UInt_t          nEle;
    Int_t           eleID[100];
    UInt_t          nTrigObj;
    Int_t           TrigObj_id[100];

    Float_t         elePhi[100];
    Float_t         elePt[100];
    Float_t         eleEta[100];
    Float_t         eleDeltaEtaSC[100];
    Int_t           eleCharge[100];
    Float_t         eleMass[100];
    Float_t         eleMiniPFRelIso[100];

       

    // Muon
    UInt_t          nMuon;
    Float_t         muPhi[100];
    Float_t         muPt[100];
    Float_t         muEta[100];
    Int_t           muCharge[100];
    Float_t         muMass[100];
    Float_t         muPFRelIso[100];
    Float_t         muTkRelIso[100];
    Bool_t          muMediumId[100];
    Bool_t          muTightId[100];
    UChar_t         muHighPtId[100];
    Bool_t          muHighPurity[100];
    UChar_t         muTkIsoId[100];
    Bool_t          muIsPFMuon[100];
    Bool_t          muIsGlobal[100];
    Bool_t          muIsTracker[100];
    Float_t         muDxy[100];
    Float_t         muDz[100];

    double deltaR(double eta1, double phi1, double eta2, double phi2){
        double dphi = phi2 - phi1;
        double deta = eta2 - eta1;
        static const double pi = TMath::Pi();
        dphi = TMath::Abs( TMath::Abs(dphi) - pi ) - pi;
        return TMath::Sqrt( dphi*dphi + deta*deta );
    }
};
#endif
