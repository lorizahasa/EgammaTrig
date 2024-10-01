#include"Selector.h"

Selector::Selector(){
}

std::vector<int> Selector::filter_electrons(EventTree *tree){
    std::vector<int> selEles_;
    for(int eleInd = 0; eleInd < tree->nEle; ++eleInd){
        double absEta = TMath::Abs(tree->eleEta[eleInd]);
        double pt = tree->elePt[eleInd];
        bool passEtaEBEEGap = (absEta < 1.4442) || (absEta > 1.566);
        bool passTightID = (tree->eleID[eleInd] ==4);
        bool eleSel = (passEtaEBEEGap &&
                       pt > 40 &&
                       absEta <= 2.4 &&
                       passTightID);
        if(eleSel) selEles_.push_back(eleInd);
    }
    return selEles_;
}

std::vector<int> Selector::filter_muons(EventTree *tree){
    std::vector<int>selMuons_;
    for(UInt_t m = 0; m < tree->nMuon; ++m){
        double eta = tree->muEta[m];
        double pt = tree->muPt[m];
        //cout<<"Muons before Sel: "<< m <<endl;
        //Prompt muon (Medium ID)
        //cutbased for pt<=120, highPt for pt >120
        bool passPrompt     = false;
        if(pt > 55){
            passPrompt = (TMath::Abs(eta) <= 2.4
                && (int)tree->muTkIsoId[m] == 2 //1 for loose, 2 for tight
                &&  tree->muHighPurity[m]
                && (int)tree->muHighPtId[m]==2//1 = tracker high pT, 2 = global high pT, which includes tracker high pT
                && tree->muDxy[m]<0.2
                && tree->muDz[m]<0.5);
        }
        if(passPrompt){ 
            selMuons_.push_back(m);
            //cout<< "Passed Muon Sel: "<< selMuons_.size()<<endl;
        }
            }
    return selMuons_;
}

bool Selector::filter_Z(EventTree *tree, int t, int p){
    bool passZ = false;
	TLorentzVector ele1;
	TLorentzVector ele2;
	ele1.SetPtEtaPhiM(tree->elePt[t],
        tree->eleEta[t],
        tree->elePhi[t],
        tree->eleMass[t]);
	ele2.SetPtEtaPhiM(tree->elePt[p],
		  tree->eleEta[p],
		  tree->elePhi[p],
		  tree->eleMass[p]);
	if (abs((ele1 + ele2).M() - 91.1876) < 30 && (tree->eleCharge[t])*(tree->eleCharge[p]) == -1){
	    passZ = true;
	}
    return passZ;
}

/*
bool Selector::isTrigMatched(EventTree *tree, int e){
    bool isTM = false;
    for(int j=0;j<tree->nTrigObj;j++){
        if (!(tree->TrigObj_pt[j]>31)) continue;
        if (!(abs(tree->TrigObj_id[j])==11)) continue;
        if (!(tree->TrigObj_filterBits[j] & (0x1 << 1))) continue;
        // 0x1 << 1 = 2. The filterbit for HLT_Ele32WPTight is 2
        //  1 is serial number in NanoAODv11
        double dR = deltaR(tree->eleEta[e],  tree->elePhi[e], tree->TrigObj_eta[j], tree->TrigObj_phi[j]);
        if(dR < 0.1){
            isTM = true;
            break;
        }
    }
    return isTM;
}
*/

Selector::~Selector(){
}
