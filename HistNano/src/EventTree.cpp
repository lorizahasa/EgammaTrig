#include<iostream>
#include<vector>
#include"EventTree.h"
EventTree::EventTree(int nFiles, bool xRootDAccess, string year, char** fileNames, bool isMC){
    chain = new TChain("Events");

    std::cout << "Start EventTree" << std::endl;
    chain->SetCacheSize(100*1024*1024);
    bool isCopy = false;
    if (xRootDAccess){
        //string dir = "root://cms-xrd-global.cern.ch/";
        string dir = "root://cmsxrootd.fnal.gov/";
        //string dir = "root://xrootd-cms.infn.it/";
        for(int fileI=0; fileI<nFiles; fileI++){
            string fName = (string) fileNames[fileI];
            if(isCopy){
                string singleFile = fName.substr(fName.find_last_of("/")+1,fName.size());
                string xrdcp_command = "xrdcp " + dir + fName + " " + singleFile ;
                cout << xrdcp_command.c_str() << endl;
                //system(xrdcp_command.c_str());
                chain->Add( singleFile.c_str());
                cout << singleFile << "  " << chain->GetEntries() << endl;
            }
            else{
                TFile* fCheck = TFile::Open((dir+fName).c_str());
                if(!fCheck || fCheck->IsZombie() || fCheck->GetSize()<100){
                    cout<<"fCheck: issue with file: "<<dir+fName<<endl;
                    continue;
                }
                if(!fCheck->GetListOfKeys()->Contains("Events")){
                    cout<<"fCheck: issue with tree Events: "<<dir+fName<<endl;
                    continue;
                }
                fCheck->Close();
                chain->Add( (dir + fName).c_str());
                cout << dir+fName << "  " << chain->GetEntries() << endl;
            }
        }
    }
    else{
        for(int fileI=0; fileI<nFiles; fileI++){
            chain->Add(fileNames[fileI]);
            cout <<fileNames[fileI]<<endl;
        }
    }
    std::cout << "Begin" << std::endl;
    chain->SetBranchStatus("*",0);
	
    // event
    chain->SetBranchStatus("run",1);
    chain->SetBranchAddress("run", &run_);
    chain->SetBranchStatus("event",1);
    chain->SetBranchAddress("event", &event_);
    chain->SetBranchStatus("luminosityBlock",1);
    chain->SetBranchAddress("luminosityBlock", &lumis_);

    // electrons    
    chain->SetBranchStatus("nElectron",1);
    chain->SetBranchAddress("nElectron", &nEle);

    chain->SetBranchStatus("Electron_charge",1);
    chain->SetBranchAddress("Electron_charge", &eleCharge);    
    
    chain->SetBranchStatus("Electron_pt",1);
    chain->SetBranchAddress("Electron_pt", &elePt);
    
    chain->SetBranchStatus("Electron_deltaEtaSC",1);
    chain->SetBranchAddress("Electron_deltaEtaSC", &eleDeltaEtaSC);
    
    chain->SetBranchStatus("Electron_eta",1);
    chain->SetBranchAddress("Electron_eta", &eleEta);
    
    chain->SetBranchStatus("Electron_phi",1);
    chain->SetBranchAddress("Electron_phi", &elePhi);
    
    chain->SetBranchStatus("Electron_mass",1);
    chain->SetBranchAddress("Electron_mass", &eleMass);

    chain->SetBranchStatus("Electron_cutBased",1);
    chain->SetBranchAddress("Electron_cutBased", &eleID);
    
    chain->SetBranchStatus("Electron_miniPFRelIso_all",1);
    chain->SetBranchAddress("Electron_miniPFRelIso_all", &eleMiniPFRelIso);

    // Trig object 
    chain->SetBranchStatus("nTrigObj",1);
    chain->SetBranchAddress("nTrigObj", &nTrigObj);
    
    chain->SetBranchStatus("TrigObj_pt",1);
    chain->SetBranchAddress("TrigObj_pt", &TrigObj_pt);
    
    chain->SetBranchStatus("TrigObj_eta",1);
    chain->SetBranchAddress("TrigObj_eta", &TrigObj_eta);
    
    chain->SetBranchStatus("TrigObj_phi",1);
    chain->SetBranchAddress("TrigObj_phi", &TrigObj_phi);
    
    chain->SetBranchStatus("TrigObj_id",1);
    chain->SetBranchAddress("TrigObj_id", &TrigObj_id);
    
    chain->SetBranchStatus("TrigObj_filterBits",1);
    chain->SetBranchAddress("TrigObj_filterBits", &TrigObj_filterBits);
}

EventTree::~EventTree(){
    delete chain;
    // will be some memory leak due to created vectors
}

Long64_t EventTree::GetEntries(){
    return chain->GetEntries();
}

Int_t EventTree::GetEntry(Long64_t entry){
    //chain->GetEntry(entry);
    //return chain->GetEntries();
    return chain->GetEntry(entry);
}
