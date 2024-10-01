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

    // muons
    chain->SetBranchStatus("nMuon",1);
    chain->SetBranchAddress("nMuon", &nMuon);

    chain->SetBranchStatus("Muon_charge",1);
    chain->SetBranchAddress("Muon_charge", &muCharge);

    chain->SetBranchStatus("Muon_pt",1);
    chain->SetBranchAddress("Muon_pt", &muPt);

    chain->SetBranchStatus("Muon_eta",1);
    chain->SetBranchAddress("Muon_eta", &muEta);

    chain->SetBranchStatus("Muon_phi",1);
    chain->SetBranchAddress("Muon_phi", &muPhi);

    chain->SetBranchStatus("Muon_mass",1);
    chain->SetBranchAddress("Muon_mass", &muMass);

    chain->SetBranchStatus("Muon_mediumId",1);
    chain->SetBranchAddress("Muon_mediumId", &muMediumId);

    chain->SetBranchStatus("Muon_tightId",1);
    chain->SetBranchAddress("Muon_tightId", &muTightId);

    chain->SetBranchStatus("Muon_highPtId",1);
    chain->SetBranchAddress("Muon_highPtId", &muHighPtId);

    chain->SetBranchStatus("Muon_highPurity",1);
    chain->SetBranchAddress("Muon_highPurity", &muHighPurity);

    chain->SetBranchStatus("Muon_tkIsoId",1);
    chain->SetBranchAddress("Muon_tkIsoId", &muTkIsoId);

    chain->SetBranchStatus("Muon_pfRelIso04_all",1);
    chain->SetBranchAddress("Muon_pfRelIso04_all", &muPFRelIso);

    chain->SetBranchStatus("Muon_tkRelIso",1);
    chain->SetBranchAddress("Muon_tkRelIso", &muTkRelIso);

    chain->SetBranchStatus("Muon_isPFcand",1);
    chain->SetBranchAddress("Muon_isPFcand", &muIsPFMuon);

    chain->SetBranchStatus("Muon_isGlobal",1);
    chain->SetBranchAddress("Muon_isGlobal", &muIsGlobal);

    chain->SetBranchStatus("Muon_isTracker",1);
    chain->SetBranchAddress("Muon_isTracker", &muIsTracker);

    chain->SetBranchStatus("Muon_dxy",1);
    chain->SetBranchAddress("Muon_dxy", &muDxy);

    chain->SetBranchStatus("Muon_dz",1);
    chain->SetBranchAddress("Muon_dz", &muDz);

        //Filters
    //https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2
    chain->SetBranchStatus("Flag_goodVertices",1);
    chain->SetBranchAddress("Flag_goodVertices",&Flag_goodVertices_);
    chain->SetBranchStatus("Flag_globalSuperTightHalo2016Filter",1);
    chain->SetBranchAddress("Flag_globalSuperTightHalo2016Filter", &Flag_globalSuperTightHalo2016Filter_);
    chain->SetBranchStatus("Flag_HBHENoiseFilter",1);
    chain->SetBranchAddress("Flag_HBHENoiseFilter", &Flag_HBHENoiseFilter_);
    chain->SetBranchStatus("Flag_HBHENoiseIsoFilter",1);
    chain->SetBranchAddress("Flag_HBHENoiseIsoFilter", &Flag_HBHENoiseIsoFilter_);
    chain->SetBranchStatus("Flag_EcalDeadCellTriggerPrimitiveFilter",1);
    chain->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter_);
    chain->SetBranchStatus("Flag_BadPFMuonFilter",1);
    chain->SetBranchAddress("Flag_BadPFMuonFilter",&Flag_BadPFMuonFilter_);
    chain->SetBranchStatus("Flag_eeBadScFilter",1);
    chain->SetBranchAddress("Flag_eeBadScFilter", &Flag_eeBadScFilter_);
    if(year =="2017" || year == "2018"){
	    chain->SetBranchStatus("Flag_ecalBadCalibFilter",1);
	    chain->SetBranchAddress("Flag_ecalBadCalibFilter",&Flag_ecalBadCalibFilter_);
    }


//High Level Triggers
    //https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgHLTRunIISummary
    //https://twiki.cern.ch/twiki/bin/view/CMS/MuonHLT2016
    //https://twiki.cern.ch/twiki/bin/view/CMS/MuonHLT2017
    //https://twiki.cern.ch/twiki/bin/view/CMS/MuonHLT2018
    std::cout << "Triggers" << std::endl;
    TString  im24, itm24, im27, m50, tm50, m100, tm100;
    TString  e27, e32, e35, e32D, e115, e45j200, e50j165, p175, p200;
   // im24    = "HLT_IsoMu24"   ;
  //  itm24   = "HLT_IsoTkMu24" ;
    im27    = "HLT_IsoMu27"   ;
    m50     = "HLT_Mu50"      ;
    tm50    = "HLT_TkMu50"    ;
    m100    = "HLT_OldMu100"     ;
    tm100   = "HLT_TkMu100"   ;
    
    e27     = "HLT_Ele27_WPTight_Gsf"                         ;
    e32     = "HLT_Ele32_WPTight_Gsf"                         ;
    e35     = "HLT_Ele35_WPTight_Gsf"                         ;
    //e32D    = "HLT_Ele32_WPTight_Gsf_L1DoubleEG"              ;
    e115    = "HLT_Ele115_CaloIdVT_GsfTrkIdT"                 ;
   // e45j200 = "HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50" ;
    //e50j165 = "HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165"         ;
    p175    = "HLT_Photon175"                                 ;
    p200    = "HLT_Photon200"                                 ;
    
    if (year.find("2016")!=std::string::npos){
       // chain->SetBranchStatus(im24 , 1);
       // chain->SetBranchStatus(itm24, 1);
        chain->SetBranchStatus(m50  , 1);
        chain->SetBranchStatus(tm50 , 1);
        chain->SetBranchStatus(e27    , 1);
        chain->SetBranchStatus(e115    , 1);
        //chain->SetBranchStatus(e45j200, 1);
        //chain->SetBranchStatus(e50j165, 1);
        chain->SetBranchStatus(p175   , 1);
        
        //chain->SetBranchAddress(im24 , &im24_ );
        //chain->SetBranchAddress(itm24, &itm24_);
        chain->SetBranchAddress(m50  , &m50_  );
        chain->SetBranchAddress(tm50 , &tm50_);
        chain->SetBranchAddress(e27    , &e27_);
        chain->SetBranchAddress(e115    , &e115_);
        //chain->SetBranchAddress(e45j200, &e45j200_);
        //chain->SetBranchAddress(e50j165, &e50j165_);
        chain->SetBranchAddress(p175   , &p175_);
    }
    if (year.find("2017")!=std::string::npos){
        chain->SetBranchStatus(im27 , 1);
        chain->SetBranchStatus(m50  , 1);
        chain->SetBranchStatus(m100 , 1);
        chain->SetBranchStatus(tm100, 1);
        chain->SetBranchStatus(e35    , 1);
        chain->SetBranchStatus(e115    , 1);
        //chain->SetBranchStatus(e50j165, 1);
        chain->SetBranchStatus(p200, 1);
        
        chain->SetBranchAddress(im27 , &im27_ );
        chain->SetBranchAddress(m50  , &m50_  );
        chain->SetBranchAddress(m100 , &m100_);
        chain->SetBranchAddress(tm100, &tm100_);
        chain->SetBranchAddress(e35    , &e35_);
        chain->SetBranchAddress(e115    , &e115_);
       // chain->SetBranchAddress(e50j165, &e50j165_);
        chain->SetBranchAddress(p200   , &p200_);
    }
    if (year.find("2018")!=std::string::npos){
       // chain->SetBranchStatus(im24 , 1);
        chain->SetBranchStatus(m50  , 1);
        chain->SetBranchStatus(m100 , 1);
        chain->SetBranchStatus(tm100, 1);
        chain->SetBranchStatus(e32    , 1);
        chain->SetBranchStatus(e115    , 1);
        //chain->SetBranchStatus(e50j165, 1);
        chain->SetBranchStatus(p200   , 1);
        
        //chain->SetBranchAddress(im24 , &im24_ );
        chain->SetBranchAddress(m50  , &m50_  );
        chain->SetBranchAddress(m100 , &m100_);
        chain->SetBranchAddress(tm100, &tm100_);
        chain->SetBranchAddress(e32    , &e32_);
        chain->SetBranchAddress(e115    , &e115_);
        //chain->SetBranchAddress(e50j165, &e50j165_);
        chain->SetBranchAddress(p200   , &p200_);
    }
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
