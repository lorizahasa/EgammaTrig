#ifndef SELECTOR_H
#define SELECTOR_H

#include<vector>
#include<iostream>
#include <iomanip>
#include<algorithm>
#include<TH1F.h>
#include<TMath.h>
#include<TLorentzVector.h>
#include"EventTree.h"
#include"TRandom3.h"
#include<bitset>
#include <random>

class Selector{
public:
	Selector();
	~Selector();
    bool isTrigMatched(EventTree *tree, int e);
    std::vector<int> filter_electrons(EventTree *tree);
    std::vector<int> filter_muons(EventTree *tree);
    bool filter_Z(EventTree *tree, int t, int p); 

    //delta R
    double deltaR(double eta1, double phi1, double eta2, double phi2){
        double dphi = phi2 - phi1;
        double deta = eta2 - eta1;
        static const double pi = TMath::Pi();
        dphi = TMath::Abs( TMath::Abs(dphi) - pi ) - pi;
        return TMath::Sqrt( dphi*dphi + deta*deta );
    }
private:
};
#endif
