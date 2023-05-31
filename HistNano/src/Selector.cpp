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
                       pt > 31 &&
                       absEta <= 2.5 &&
                       passTightID);
        if(eleSel) selEles_.push_back(eleInd);
    }
    return selEles_;
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

Selector::~Selector(){
}
