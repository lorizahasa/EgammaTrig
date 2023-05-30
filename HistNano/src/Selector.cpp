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
        //select electrons
        bool eleSel = (passEtaEBEEGap && 
                       absEta <= 2.5 &&
                       pt >= 31.0 &&
                       passTightID);
        if(eleSel) selEles_.push_back(eleInd);
    }
    return selEles_;
}

bool Selector::filter_Z(EventTree *tree, vector<int> selEles__){
    bool passZ = true;
    if(selEles__.size() == 2) {
		int idx_ele1 = selEles__.at(0);
		int idx_ele2 = selEles__.at(1);
		if((tree->eleCharge[idx_ele1])*(tree->eleCharge[idx_ele2]) == 1){
		    passZ = false;
		}
		TLorentzVector ele1;
		TLorentzVector ele2;
		ele1.SetPtEtaPhiM(tree->elePt[idx_ele1],
			  tree->eleEta[idx_ele1],
			  tree->elePhi[idx_ele1],
			  tree->eleMass[idx_ele1]);
		ele2.SetPtEtaPhiM(tree->elePt[idx_ele2],
			  tree->eleEta[idx_ele2],
			  tree->elePhi[idx_ele2],
			  tree->eleMass[idx_ele2]);
		if ( abs((ele1 + ele2).M() - 91.1876) > 30 ){
		    passZ = false;
		}
    }
    else { 
        passZ = false;
    }
    return passZ;
}

// the tag and probe should match to two different tribObj
// We first match the tag and then exclude the corresponding trigObj while
// matching with the probe
vector<int> Selector::matchedTrig(EventTree *tree, int e1, int e2){
    std::vector<int> matched;
    // match tag with one trigObj
    double dR = 0.1;
    int tagTrigIndex = -1;
    for(int j=0;j<tree->nTrigObj;j++){
        if (!(tree->TrigObj_pt[j]>31)) continue;
        if (!(abs(tree->TrigObj_id[j])==11)) continue;
        if (!(tree->TrigObj_filterBits[j] & 1)) continue;
        double dR_ = deltaR(tree->eleEta[e1],  tree->elePhi[e1], tree->TrigObj_eta[j], tree->TrigObj_phi[j]);
        //std::cout<<ind<<", "<<dR<<", "<<tree->TrigObj_pt[j]<<", "<<abs(tree->TrigObj_id[j])<<", "<<tree->TrigObj_filterBits[j]<<std::endl;
        if(dR_ < dR){
            dR = dR_;
            tagTrigIndex = j;
        }
    }
    // match probe with trigObj
    if(tagTrigIndex > -1){
        matched.push_back(tagTrigIndex);
        for(int j=0;j<tree->nTrigObj;j++){
            if (!(tree->TrigObj_pt[j]>31)) continue;
            if (!(abs(tree->TrigObj_id[j])==11)) continue;
            if (!(tree->TrigObj_filterBits[j] & 1)) continue;
            if (j==tagTrigIndex) continue;// exclude tag
            double dR2 = deltaR(tree->eleEta[e2],  tree->elePhi[e2], tree->TrigObj_eta[j], tree->TrigObj_phi[j]);
            if(dR2 < 0.1){
                matched.push_back(j);
                break;
            }
        }
    }
    return matched;
}


Selector::~Selector(){
}
