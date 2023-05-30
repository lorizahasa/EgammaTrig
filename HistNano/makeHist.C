#include<iostream>
#include<string>
#include<TFile.h>
#include<TTree.h>
#include<TDirectory.h>
#include<TObject.h>
#include<TCanvas.h>
#include<iomanip>

#include "src/EventTree.h"
#include "src/Selector.h"

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
    outFile->cd();

    //------------------------------------------
    // Declare histograms
    //------------------------------------------
    //https://cmssdt.cern.ch/lxr/source/DQMOffline/Trigger/python/HLTEGTnPMonitor_cfi.py
	TH1D* hEvents  = new TH1D("hEvents", "#events in NanoAOD", 3, -1.5, 1.5);
    const int ptN  = 22;
    const int etaN = 49;
    double ptBins[ptN]    = {5,10,12.5,15,17.5,20,22.5,25,30,35,40,45,50,60,80,100,150,200,250,300,350,400};
    double etaBins[etaN]  = {-2.5,-2.4,-2.3,-2.2,-2.1,-2.0,-1.9,-1.8,-1.7,-1.566,-1.4442,-1.3,-1.2,-1.1,-1.0,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1.1,1.2,1.3,1.4442,1.566,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5};
    TH1F *hPt       = new TH1F("probePt","probePt",ptN-1,ptBins);
    TH1F *hPtPass   = new TH1F("probePtPass","probePtPass",ptN-1,ptBins);
    TH1F *hPtFail   = new TH1F("probePtFail","probePtFail",ptN-1,ptBins);
    TH1F *hEta      = new TH1F("probeEta","probeEta",etaN-1,etaBins);
    TH1F *hEtaPass  = new TH1F("probeEtaPass","probeEtaPass",etaN-1,etaBins);
    TH1F *hEtaFail  = new TH1F("probeEtaFail","probeEtaFail",etaN-1,etaBins);

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
	for(Long64_t entry= startEntry; entry < endEntry; entry++){
        //if(entry>10000) break; 
        // Print time after 1% events are processed
		if(entry%(eventsPerJob/100) == 0){
            totalTime+= std::chrono::duration<double>(std::chrono::high_resolution_clock::now()-startClock).count();
            int sec = (int)(totalTime)%60;
            int min = (int)(totalTime)/60;
	        std::cout<<setw(10)<<100*entry/endEntry<<" %"<<setw(10)<<min<<"m "<<sec<<"s"<<std::endl;
			startClock = std::chrono::high_resolution_clock::now();			
		}
		tree->GetEntry(entry);
		hEvents->Fill(0.);

        // Apply dilepton selection 
        std::vector<int> selEles = selector->filter_electrons(tree);
        bool passZSel = selector->filter_Z(tree, selEles);
        if(!passZSel) continue;
        if(tree->nTrigObj < 2) continue;

        // Randomly we chose electrons out of two and do matching with trigObj
        int ind0 = rand()%2;
        int ind1 = (ind0+1)%2;
        int e1   = selEles[ind0];
        int e2   = selEles[ind1];
        vector<int> matchedTnP =selector->matchedTrig(tree, e1, e2);

        // Every event must have a tag electron matched with trigObj
        if(matchedTnP.size()==0) continue;

        // The fail and pass events are dicided by probMatch
        // All events
        hPt->Fill(tree->elePt[e2]);
        hEta->Fill(tree->eleEta[e2]);
        //passing probe
        if(matchedTnP.size()==2){
            hPtPass->Fill(tree->elePt[e2]);
            hEtaPass->Fill(tree->eleEta[e2]);
        }
        else{//faling probe
            hPtFail->Fill(tree->elePt[e2]);
            hEtaFail->Fill(tree->eleEta[e2]);
        }
	}
	hEvents->Write();
    hPt->Write();  
    hPtPass->Write();  
    hPtFail->Write();  
    hEta->Write();     
    hEtaPass->Write(); 
    hEtaFail->Write();
	outFile->Close();
    return 0;
}

