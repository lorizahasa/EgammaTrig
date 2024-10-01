#include<iostream>
#include<string>
#include<TFile.h>
#include<TH2F.h>
#include<TTree.h>
#include<TDirectory.h>
#include<TObject.h>
#include<TCanvas.h>
#include<iomanip>

#include "src/EventTree.h"
#include "src/Selector.h"
#include "src/LumiMask.h"

int main(int ac, char** av){
	if(ac < 3){
		std::cout << "usage: ./makeHist year outputFileName inputFile[s]" << std::endl;
		return -1;
	}
	std::cout << "Starting" << std::endl;
	bool isMC = true;
	bool xRootDAccess = false;
	std::string year(av[1]);	

    //------------------------------------------
    // A sample has many files. Based on the input, 
    // specify how many files will go with one job
    // e.g. if there are total 200 files and the
    // input is 1of10 (i.e. 10%) then each job will
    // run on 20 files. The first job will run on
    // 1-20, the second on 21-40 files and so on.
    //------------------------------------------
	int nJob = -1;
	int totJob = -1;
	std::string checkJobs(av[2]);
	size_t pos = checkJobs.find("of");
	if (pos != std::string::npos){
	    nJob = std::stoi(checkJobs.substr(0,pos));
	    totJob = std::stoi(checkJobs.substr(pos+2,checkJobs.length()));
	    for (int i = 2; i < ac-1; i++){
		av[i] = av[i+1];
		//cout << av[i] << " ";
	    }
	    ac = ac-1;
	}
	cout << nJob << " of " << totJob << endl;
 
	std::string outFileName(av[2]);
	if( outFileName.find("Data") != std::string::npos){
	    cout << "IsData" << endl;
	    isMC = false;
	}
	bool splitByEvents=false;
	int nFiles = ac-3;
	int startFile = 0;
	if (nJob>0 && totJob>1){
	    if (ac-3 >= totJob){
		double filesPerJob = 1.*(ac-3)/totJob;
		cout << "Processing " << filesPerJob << " files per job on average" << endl;
		startFile = int((nJob-1)*filesPerJob);
		nFiles = int(nJob*filesPerJob) - startFile;
		cout << "   total of " << (ac-3) << " files" << endl;
		cout << "   this job will process files " << startFile << " to " << startFile+nFiles << endl;
	    } else {
		splitByEvents = true;
	    }
	}
	char** fileList(av+3+startFile);
	cout << "HERE" << endl;
	//check for xrootd argument before file list
	if (std::string(av[3])=="xrootd"){
	    xRootDAccess=true;
	    std::cout << "Will access files from xRootD" << std::endl;
	    for (int i = 3; i < ac-1; i++){
		av[i] = av[i+1];
		//cout << av[i] << " ";
	    }
	    ac = ac-1;
	}

    // Declare EventTree and other clases
	EventTree* tree;
	tree = new EventTree(nFiles, xRootDAccess, year, fileList, isMC);
	tree->isData_ = !isMC;
	Long64_t nEntr = tree->GetEntries();
	std::cout << "Sample has "<<nEntr << " entries" << std::endl;
    Selector* selector = new Selector(); 

    // Name of the output file
	if (nJob>0 && totJob>0){
	    pos = outFileName.find(".root");
	    outFileName = outFileName.substr(0,pos) + "_" + checkJobs + ".root";
	    cout << "new output file name: "<< outFileName << endl;
	}
	TFile* outFile = TFile::Open( outFileName.c_str() ,"RECREATE","",207 );
    TTree* newTree = tree->chain->GetTree()->CloneTree(0);
	newTree->SetCacheSize(50*1024*1024);
    outFile->cd();

    //------------------------------------------
    // Declare histograms
    //------------------------------------------
    //https://cmssdt.cern.ch/lxr/source/DQMOffline/Trigger/python/HLTEGTnPMonitor_cfi.py
	TH1D* hEvents  = new TH1D("hEvents", "events in NanoAOD", 3, -1.5, 1.5);
    const int ptN  = 12;
    const int etaN = 11;
    double ptBins[ptN]    = {55, 75, 95, 115, 135, 155, 175, 200, 250, 300, 400, 500};
    double etaBins[etaN]  = {-2.5, -2.0, -1.566, -1.444, -0.8, 0.0, 0.8, 1.444, 1.566, 2.0, 2.5};
    TH1F *hElePtPassMu     = new TH1F("hEle_Pt_Pass_Mu","Electron Pt Pass Mu",ptN-1,ptBins);
    TH1F *hElePtPassBoth   = new TH1F("hEle_Pt_Pass_Both","Electron Pt Pass Both",ptN-1,ptBins);
    TH1F *hEleEtaPassMu    = new TH1F("hEle_Eta_Pass_Mu","Electron Eta Pass Mu",etaN-1,etaBins);
    TH1F *hEleEtaPassBoth  = new TH1F("hEle_Eta_Pass_Both","Electron Eta Pass Both",etaN-1,etaBins);
    TH1F *hEta_lowpt_pass  = new TH1F("hEta_Pass_lowpt","Electron Eta Pass Both Low Pt",etaN-1,etaBins);
    TH1F *hEta_lowpt_mu    = new TH1F("hEta_PassMu_lowpt","Electron Eta Pass Mu Trig Low Pt",etaN-1,etaBins);
    TH1F *hEta_midpt_pass  = new TH1F("hEta_Pass_midpt","Electron Eta Pass Both Mid Pt",etaN-1,etaBins);
    TH1F *hEta_midpt_mu    = new TH1F("hEta_PassMu_midpt","Electron Eta Pass Mu Trig Mid Pt",etaN-1,etaBins);
    TH1F *hEta_highpt_pass = new TH1F("hEta_Pass_highpt","Electron Eta Pass Both High Pt",etaN-1,etaBins);
    TH1F *hEta_highpt_mu   = new TH1F("hEta_PassMu_highpt","Electron Eta Pass Mu Trig High Pt",etaN-1,etaBins);
    // 2D histograms
    TH2F *hElePtEtaPassMu = new TH2F("hEle_PtEta_Pass_Mu", "Electron Pt vs Eta Pass Mu", ptN - 1, ptBins, etaN - 1, etaBins);
    TH2F *hElePtEtaPassBoth = new TH2F("hEle_PtEta_Pass_Both", "Electron Pt vs Eta Pass Both", ptN - 1, ptBins, etaN - 1, etaBins);

    //--------------------------------
    // Trigger flow histograms
    //--------------------------------
    TString  im24, itm24, im27, m50, tm50, m100, tm100;
    TString  e27, e32, e35, e32D, e115, e45j200, e50j165, p175, p200;
    im24    = "HLT_IsoMu24"   ;
    itm24   = "HLT_IsoTkMu24" ;
    im27    = "HLT_IsoMu27"   ;
    m50     = "HLT_Mu50"      ;
    tm50    = "HLT_TkMu50"    ;
    m100    = "HLT_OldMu100"     ;
    tm100   = "HLT_TkMu100"   ;
    
    e27     = "HLT_Ele27_WPTight_Gsf"                         ;
    e32     = "HLT_Ele32_WPTight_Gsf"                         ;
    e35     = "HLT_Ele35_WPTight_Gsf"                         ;
    e32D    = "HLT_Ele32_WPTight_Gsf_L1DoubleEG"              ;
    e115    = "HLT_Ele115_CaloIdVT_GsfTrkIdT"                 ;
    e45j200 = "HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50" ;
    e50j165 = "HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165"         ;
    p175    = "HLT_Photon175"                                 ;
    p200    = "HLT_Photon200"                                 ;

  
	TH1F* hAll           = new TH1F("hAll_MuTrig",  "events in NanoAOD",   6, 0.5, 6.5);
	TH1F* hAllE          = new TH1F("hAll_EleTrig", "events in NanoAOD",  6, 0.5, 6.5);
	TH1F* hPass          = new TH1F("hPass_MuTrigFlow", "OR flow of HLT paths",  6, 0.5, 6.5);
	TH1F* hPassE         = new TH1F("hPass_EleTrigFlow","OR flow of HLT paths", 6, 0.5, 6.5);
	TH1F* hPassBoth      = new TH1F("hPass_MuonAndEleTrig","OR flow of HLT paths", 6, 0.5, 6.5);

    bool isTrigMu; 
    bool isTrigE;
    Int_t passTrigMu, passTrigEle;
    TBranch* passTrigMu_  = newTree->Branch("passTrigMu",  &passTrigMu, "passTrigMu/I");
    TBranch* passTrigEle_ = newTree->Branch("passTrigEle", &passTrigEle, "passTrigEle/I");
    //------------------------------------------
    // Loop over the events of tree 
    //------------------------------------------
	int startEntry = 0;
	int endEntry = nEntr;
	int eventsPerJob = nEntr;
	if (splitByEvents) {
	    eventsPerJob = int(1.*nEntr/totJob);
	    startEntry = (nJob-1)*eventsPerJob;
	    endEntry = nJob*eventsPerJob;
	    if (nJob==totJob){
		endEntry=nEntr;
	    }
	}
    
	cout << "Processing events "<<startEntry<< " to " << endEntry << endl;
	auto startClock = std::chrono::high_resolution_clock::now();
    double totalTime = 0.0;
    std::cout<<setw(10)<<"Progress"<<setw(10)<<"Time"<<std::endl;
    int count_BadLumi=0;

    //std::string lumiJSON = "json/Cert_Collisions2023_366442_368823_Golden.json";
    std::map<std::string, string> lumiJSON;
    string comJSON = "json/";
    lumiJSON["2016Pre"]     = comJSON+"Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt";
    lumiJSON["2016Post"]    = comJSON+"Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt";
    lumiJSON["2017"]        = comJSON+"Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt";
    lumiJSON["2018"]        = comJSON+"Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt";
    lumiJSON["2023"]        = comJSON+"Cert_Collisions2023_366442_368823_Golden.json";
    LumiMask* lumiMask   = new LumiMask(lumiJSON[year]);

   // bool Trig_preSel=true;

	for(Long64_t entry= startEntry; entry < endEntry; entry++){
        //if(entry>100000) break; 
        // Print time after 1% events are processed
		if(entry%(eventsPerJob/100) == 0){
            totalTime+= std::chrono::duration<double>(std::chrono::high_resolution_clock::now()-startClock).count();
            int sec = (int)(totalTime)%60;
            int min = (int)(totalTime)/60;
	        std::cout<<setw(10)<<100*entry/endEntry<<" %"<<setw(10)<<min<<"m "<<sec<<"s"<<std::endl;
			startClock = std::chrono::high_resolution_clock::now();			
		}
		tree->GetEntry(entry);
        if(!isMC){
            bool valLumi = lumiMask->isValidLumi(tree->run_, tree->lumis_);
            if(!valLumi){
                count_BadLumi++;
                continue;
            }
        }
		hEvents->Fill(0.);//All Events
        //--------------------------------
        // MET filters
        //--------------------------------
        bool filters =
            (tree->Flag_goodVertices_ &&
		    tree->Flag_globalSuperTightHalo2016Filter_ &&
		    tree->Flag_HBHENoiseFilter_ &&
		    tree->Flag_HBHENoiseIsoFilter_ &&
		    tree->Flag_EcalDeadCellTriggerPrimitiveFilter_ &&
		    tree->Flag_BadPFMuonFilter_ &&
		    tree->Flag_eeBadScFilter_ );
        if (year=="2017" || year=="2018") filters = filters && tree->Flag_ecalBadCalibFilter_ ;


        //Filter electrons and muons
        //std::cout<<" Electrons and Muons before selection: " <<std::endl;
        std::vector<int> electrons   = selector->filter_electrons(tree);
        std::vector<int> muons  = selector->filter_muons(tree);
        //std::cout<< "Electrons: "<< electrons.size() << " Muons: "<< muons.size()<< std::endl;
        
        //--------------------     
	    // Lepton Selection 
        //--------------------
        //  

        if (electrons.size() != 1 || muons.size() != 1) {
        continue;
        }    

        hEvents->Fill(1.); //Events with exactly 1 mu and 1 ele

        isTrigMu=false;
        isTrigE=false;
        
        //---------------------------------
        //Fill Hist that pass trigger paths
        //---------------------------------
        if (year.find("2016")!=std::string::npos){
            if(tree->m50_ || tree->tm50_){
                isTrigMu = true;
                hPass->Fill(1.0);
            } 
            if(tree->e27_ || tree->e115_ || tree->p175_){
                hPassE->Fill(1.0);
                isTrigE = true;
            }    
        }
    
        if (year.find("2017")!=std::string::npos){
            if(tree->m50_ || tree->tm100_ || tree->m100_ ){
                hPass->Fill(1.0);
                isTrigMu = true;
            }        
        
            if(tree->e35_ || tree->e115_ || tree->p200_){
                hPassE->Fill(1.0);
                isTrigE = true;
            }
       }

        if (year.find("2018")!=std::string::npos){ 
            if(tree->m50_ || tree->tm100_ || tree->m100_){
                hPass->Fill(1.0);
                isTrigMu = true;
            }
            
            if(tree->e32_ || tree->e115_ || tree->p200_){
                hPassE->Fill(1.0);
                isTrigE = true;
            }
        }
        passTrigMu  = isTrigMu;
        passTrigEle = isTrigE;

        if(isTrigMu){
            //cout<<"Pass Muon Triger: "<< electrons.size()<<endl;
            hElePtPassMu->Fill(tree->elePt[0]);
            hEleEtaPassMu->Fill(tree->eleEta[0]);
            hElePtEtaPassMu->Fill(tree->elePt[0], tree->eleEta[0]);
            
            if(tree->elePt[0] <= 120){
                 hEta_lowpt_mu->Fill(tree->eleEta[0]);
            }
            
            if(tree->elePt[0] > 120 && tree->elePt[0] <= 200){
                 hEta_midpt_mu->Fill(tree->eleEta[0]);
            }

            else if(tree->elePt[0] > 200){
                 hEta_highpt_mu->Fill(tree->eleEta[0]);
            }

        }

        if(isTrigMu && isTrigE){
			newTree->Fill();
            hPassBoth->Fill(1.0);
           // cout<<"Pass Both Triggers: "<< electrons.size()<<endl;
            hElePtPassBoth->Fill(tree->elePt[0]);
            hEleEtaPassBoth->Fill(tree->eleEta[0]);
            hElePtEtaPassBoth->Fill(tree->elePt[0], tree->eleEta[0]);
            
            if(tree->elePt[0] <= 120){
                 hEta_lowpt_pass->Fill(tree->eleEta[0]);
            }

            else if(tree->elePt[0] > 120 && tree->elePt[0] <= 200){
                 hEta_midpt_pass->Fill(tree->eleEta[0]);
             }

            else if(tree->elePt[0] > 200){
                 hEta_highpt_pass->Fill(tree->eleEta[0]);
             }


            }

    }
    cout<<"Total events masked = "<<count_BadLumi<<endl;
    cout<<"nEvents_Skim = "<<newTree->GetEntries()<<endl;
	hEvents->Write();
    hPass->Write();
    hPassE->Write();
    newTree->Write();
    hPassBoth->Write();
    hElePtPassBoth->Write();  
    hElePtPassMu->Write();  
    hEleEtaPassBoth->Write();     
    hEleEtaPassMu->Write(); 
    hElePtEtaPassMu->Write();
    hElePtEtaPassBoth->Write();
    hEta_lowpt_pass->Write();
    hEta_lowpt_mu->Write();
    hEta_midpt_pass->Write();
    hEta_midpt_mu->Write();
    hEta_highpt_pass->Write();
    hEta_highpt_mu->Write();
   	outFile->Close();
    return 0;
}

