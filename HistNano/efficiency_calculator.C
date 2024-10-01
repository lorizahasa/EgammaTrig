#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TCanvas.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <TLine.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TSystem.h>
#include <iostream>
#include <TString.h>
#include <vector>
#include <string>

using std::vector;
TString lumi_label;
TString year;
//=====================================================================
// Cosmetics for plotting
//=====================================================================
 
TString DrawText(TString name, bool draw = true){
  TString textBin = "";
  if(name.Contains("lowpt")) textBin = "p^{e}_{T} < 120 GeV";
  if(name.Contains("midpt")) textBin = "120 < p^{e}_{T} < 200 GeV";
  if(name.Contains("highpt")) textBin = "200 < p^{e}_{T} GeV";
  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);
  latex.SetTextFont(42);
  latex.SetTextAlign(31);
  latex.SetTextSize(0.04);
  if(draw) latex.DrawLatex(0.85, 0.825, textBin);
  return textBin;
}

TString getLumiLabel(const TString& year) {
    TString lumi = "2022G vs 2023C fb^{-1}";

    if (year.Contains("16Pre")) {
        lumi = "19.5 fb^{-1} (2016Pre)";
    } else if (year.Contains("16Post")) {
        lumi = "16.8 fb^{-1} (2016Post)";
    } else if (year.Contains("17")) {
        lumi = "41.5 fb^{-1} (2017)";
    } else if (year.Contains("18")) {
        lumi = "59.8 fb^{-1} (2018)";
    } else if (year.Contains("__")) {
        lumi = "138 fb^{-1} (Run2)";
    }

    return lumi;
}

void PrivateWorkLabel(double x=0.25, double y=0.83, TString extra = ""){
  TLatex *text3 = new TLatex(3.5, 24, extra);
  text3->SetNDC();
  text3->SetTextAlign(13);
  text3->SetX(x);
  text3->SetTextFont(52);
  text3->SetTextSize(0.04);
  text3->SetY(y);
  text3->Draw();
}

void CMSLabelOffset(double x=0.25, double y=0.83, double x1 = 0.0, double y1 = 0.0, TString extra = ""){
  TString cmstext = "CMS";
  TLatex *text = new TLatex(3.5, 24, cmstext);
  text->SetNDC();
  text->SetTextAlign(13);
  text->SetX(x);
  text->SetTextFont(62);
  text->SetTextSize(0.055);
  text->SetY(y);
  text->Draw();

  TLatex *text3 = new TLatex(3.5, 24, extra);
  text3->SetNDC();
  text3->SetTextAlign(13);
  text3->SetX(x+x1);
  text3->SetTextFont(52);
  text3->SetTextSize(0.04);
  text3->SetY(y+y1);
  text3->Draw();
}

void LumiInfo(TString lumi, bool bratio = false, double x=0.9, double y=0.9){
  TString infotext = lumi + " (13 TeV)"; 
  TLatex *text1 = new TLatex(3.5, 24, infotext);
  text1->SetNDC();
  text1->SetTextAlign(33);
  text1->SetTextFont(42);
  text1->SetX(x);
  text1->SetY(y);
  text1->SetTextSize(0.04);
  text1->Draw();
}
//=====================================================================================================================
// To Plot Efficiencies and SFs
//=====================================================================================================================
 
void PlotEfficiency(TGraphAsymmErrors* h_data, TGraphAsymmErrors* h_mc, TString xaxis, TString histname, TString year) {
    h_data->SetTitle(" ");
    h_data->GetXaxis()->SetTitle(xaxis);
    h_data->GetYaxis()->SetTitle("Efficiency");
    h_data->GetYaxis()->SetTitleSize(0.05);
    h_data->GetYaxis()->SetLabelSize(0.045);
    h_data->GetYaxis()->SetTitleOffset(1.3);
    h_data->GetXaxis()->SetTitleSize(0.05);
    h_data->GetXaxis()->SetLabelSize(0.045);
    h_data->GetXaxis()->SetTitleOffset(0.9);
    h_data->GetXaxis()->SetNdivisions(505);
    h_data->GetYaxis()->SetNdivisions(505);
    if (xaxis.Contains("p_{T}")) h_data->GetXaxis()->SetRangeUser(0.0, 1500);
    else h_data->GetXaxis()->SetTitle("#eta^{e}");
    h_data->GetYaxis()->SetRangeUser(0.0, 1.3);

    h_data->SetLineColor(kBlack);
    h_data->SetLineWidth(2);
    h_data->SetMarkerColor(kBlack);
    h_data->SetMarkerStyle(2);
    h_data->SetMarkerSize(0.2);

    h_mc->SetLineColor(kRed);
    h_mc->SetLineWidth(2);
    h_mc->SetMarkerColor(kRed);
    h_mc->SetMarkerStyle(2);
    h_mc->SetMarkerSize(0.2);

    gStyle->SetOptStat(kFALSE);
    gStyle->SetPadTickY(1);
    gStyle->SetPadTickX(1);
    gStyle->SetLegendBorderSize(0);

    TLine* line = new TLine(120, 0, 120, 1.1);
    line->SetLineWidth(1);

    TCanvas *A = new TCanvas("A", "A", 600, 600);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.1);
    h_data->Draw("APZ");
    h_mc->Draw("PZ SAME");

    TLegend *leg = new TLegend(0.23, 0.20, 0.56, 0.375);
    leg->AddEntry(h_data, "Data", "pl");
    leg->AddEntry(h_mc, "t#bar{t}", "pl");
    leg->Draw("");

    TString text = DrawText(histname, false);
    leg->SetHeader(text);
    gPad->RedrawAxis();

 // CMS text and other labels can be added here
   // PrivateWorkLabel(0.19, 0.85, "Private work (CMS data/simulation)");
    CMSLabelOffset(0.15, 0.95, 0.12, -0.012, "Private Work");
    lumi_label = getLumiLabel(year);
    LumiInfo(lumi_label, false, 0.9, 0.9475);

    A->SaveAs("plots/" + histname + "_" + year + ".pdf");
    delete A;
    return;
}
// ==================================================================================================

void PlotSF(vector<TH1F*> h_SF, TString xaxis, TString histname, TString year){
  if(h_SF.size() != 3) std::cout << "THERE ARE NOT EXACTLY 3 SF hists" << std::endl;

  h_SF[0]->SetTitle(" ");
  h_SF[0]->GetXaxis()->SetTitle(xaxis);
  if( xaxis == "#eta") h_SF[0]->GetXaxis()->SetTitle("#eta^{e}");

  h_SF[0]->GetYaxis()->SetTitle("scale factor");

  h_SF[0]->GetYaxis()->SetTitleSize(0.05);
  h_SF[0]->GetYaxis()->SetLabelSize(0.045);
  h_SF[0]->GetYaxis()->SetTitleOffset(1.3);
  h_SF[0]->GetXaxis()->SetTitleSize(0.05);
  h_SF[0]->GetXaxis()->SetLabelSize(0.045);
  h_SF[0]->GetXaxis()->SetTitleOffset(0.9);

  h_SF[0]->GetXaxis()->SetNdivisions(505);
  h_SF[0]->GetYaxis()->SetNdivisions(505);
  h_SF[0]->GetYaxis()->SetRangeUser(0.4, 1.6);
  h_SF[0]->SetLineColor(kBlack);
  h_SF[0]->SetLineWidth(3);
  h_SF[0]->SetFillColor(13);
  h_SF[0]->SetMarkerStyle(0);

  
  // For root file
  h_SF[1]->GetYaxis()->SetTitle("scale factor");
  h_SF[1]->GetYaxis()->SetRangeUser(0.7, 1.05);
  h_SF[2]->GetYaxis()->SetTitle("scale factor");
  h_SF[2]->GetYaxis()->SetRangeUser(0.7, 1.05);

  Color_t uncert_col = kOrange+9;

  h_SF[1]->SetLineColor(uncert_col);
  h_SF[1]->SetFillStyle(3144);
  h_SF[1]->SetFillColor(uncert_col);
  h_SF[2]->SetLineColor(uncert_col);
  h_SF[2]->SetFillColor(10);

  gStyle->SetOptStat(kFALSE);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);
  gStyle->SetLegendBorderSize(0);

  TCanvas *A = new TCanvas("A", "A", 600, 600);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.1);
  h_SF[0]->Draw("E1");
  h_SF[1]->Draw("HIST SAME");
  h_SF[2]->Draw("HIST SAME");
  h_SF[0]->Draw("E1 SAME");

  TLegend *leg = new TLegend(0.23,0.20,0.56,0.375);
  leg->AddEntry(h_SF[0],"scale factor","l");
  leg->AddEntry(h_SF[1],"uncertainty","f");
  
  TString text = DrawText(histname, false);
  leg->SetHeader(text);
  leg->Draw();

  gPad->RedrawAxis();

  // CMS text
  //PrivateWorkLabel(0.19, 0.85, "Private work (CMS data/simulation)");
  CMSLabelOffset(0.15, 0.95, 0.12, -0.012, "Private Work");
  lumi_label = getLumiLabel(year);
  LumiInfo(lumi_label, false, 0.9, 0.9475);

  A->SaveAs("plots/"+histname+"_"+year+".pdf");
  delete A;
  return;
}
//==================================================================================
// To deal with HCAL Gap
//==================================================================================
 
void FillEGap(TH1F* h){
  for(unsigned int i=1; i<=h->GetNbinsX(); i++){
    // high bin content to make error bars very small
    if(abs(h->GetBinCenter(i))==1.505) h->SetBinContent(i, 1000000);
  }
  return;
}
//===================================================================================
// To Calculate SF
//===================================================================================

vector<TH1F*> GetSF(TGraphAsymmErrors* h_data, TGraphAsymmErrors* h_mc, TH1F* hist){
  TH1F* h_central = (TH1F*) hist->Clone();
  TH1F* h_up = (TH1F*) hist->Clone();
  TH1F* h_down = (TH1F*) hist->Clone();
  int nbins = hist->GetSize() -2;

  for(unsigned int i=0; i<nbins; i++){
    double xvalue = hist->GetBinCenter(i+1);
    double value_data = h_data->Eval(xvalue);
    double e_data_hi = h_data->GetErrorYhigh(i);
    double e_data_lo = h_data->GetErrorYlow(i);

    double value_mc = h_mc->Eval(xvalue);
    double e_mc_hi = h_mc->GetErrorYhigh(i);
    double e_mc_lo = h_mc->GetErrorYlow(i);

    double central = 1;
    if(value_data != 0 && value_mc != 0) central = value_data/value_mc;

    double up = 0;
    if(value_mc != 0) up = -value_data/(value_mc*value_mc) * e_mc_hi + e_data_hi/value_mc;

    double down = 0;
    if(value_mc != 0) down = -value_data/(value_mc*value_mc) * e_mc_lo + e_data_lo/value_mc;

    // add 2% uncertainty to cover non-closure
    up += 0.02;
    down += 0.02;

    // cout << i<< ", " << xvalue << ", " << value_mc << ", " << value_data << ", "<< central << endl;
    h_central->SetBinContent(i+1, central);
    h_up->SetBinContent(i+1, central+up);
    h_down->SetBinContent(i+1, central-down);
    // bin errors are just set very small to not annoy in plotting
    h_central->SetBinError(i+1, 0.000000000000001);
    h_up->SetBinError(i+1, 0.000000000000001);
    h_down->SetBinError(i+1, 0.000000000000001);
  }
  vector<TH1F*> h_sf;
  h_sf.push_back(h_central);
  h_sf.push_back(h_up);
  h_sf.push_back(h_down);
  return h_sf;
}

//==============================================================================================
// Calculation of Efficiencies and SF
//==============================================================================================
 
void calculateEfficiencies(const char* dataFileName, const char* mcFileName, const char* year) {
    // Create the output directory if it doesn't exist
    gSystem->mkdir("plots", true);
    // Define number of bins and bin arrays
    double efficiencyBins[2] = {0, 1};
    int ptN = 12; 
    double ptBins[ptN] = {55, 75, 95, 115, 135, 155, 175, 200, 250, 300, 400, 500};//  pt bins
    int etaN = 13; 
    double etaBins[etaN] = {-2.5, -2.0, -1.566, -1.444, -0.8, 0.0, 0.8, 1.444, 1.566, 2.0, 2.5}; // eta bins

    // Load data and MC files
    TFile *dataFile = TFile::Open(dataFileName);
    TFile *mcFile = TFile::Open(mcFileName);

    if (!dataFile || !mcFile) {
        std::cerr << "Error: Unable to open data or MC file." << std::endl;
        return;
    }
    
    // Define histograms for data
    TH1F *hElePtPassMu_data = (TH1F*)dataFile->Get("hEle_Pt_Pass_Mu");
    TH1F *hElePtPassBoth_data = (TH1F*)dataFile->Get("hEle_Pt_Pass_Both");
    TH1F *hEleEtaPassMu_data = (TH1F*)dataFile->Get("hEle_Eta_Pass_Mu");
    TH1F *hEleEtaPassBoth_data = (TH1F*)dataFile->Get("hEle_Eta_Pass_Both");

    TH1F *hEtaLowPtPass_data = (TH1F*)dataFile->Get("hEta_Pass_lowpt");
    TH1F *hEtaMidPtPass_data = (TH1F*)dataFile->Get("hEta_Pass_midpt");
    TH1F *hEtaHighPtPass_data = (TH1F*)dataFile->Get("hEta_Pass_highpt");

    TH1F *hEtaLowPtPassMu_data = (TH1F*)dataFile->Get("hEta_PassMu_lowpt");
    TH1F *hEtaMidPtPassMu_data = (TH1F*)dataFile->Get("hEta_PassMu_midpt");
    TH1F *hEtaHighPtPassMu_data = (TH1F*)dataFile->Get("hEta_PassMu_highpt");
    
    // Define histograms for MC
    TH1F *hElePtPassMu_mc = (TH1F*)mcFile->Get("hEle_Pt_Pass_Mu");
    TH1F *hElePtPassBoth_mc = (TH1F*)mcFile->Get("hEle_Pt_Pass_Both");
    TH1F *hEleEtaPassMu_mc = (TH1F*)mcFile->Get("hEle_Eta_Pass_Mu");
    TH1F *hEleEtaPassBoth_mc = (TH1F*)mcFile->Get("hEle_Eta_Pass_Both");

    TH1F *hEtaLowPtPass_mc = (TH1F*)mcFile->Get("hEta_Pass_lowpt");
    TH1F *hEtaMidPtPass_mc = (TH1F*)mcFile->Get("hEta_Pass_midpt");
    TH1F *hEtaHighPtPass_mc = (TH1F*)mcFile->Get("hEta_Pass_highpt");

    TH1F *hEtaLowPtPassMu_mc = (TH1F*)mcFile->Get("hEta_PassMu_lowpt");
    TH1F *hEtaMidPtPassMu_mc = (TH1F*)mcFile->Get("hEta_PassMu_midpt");
    TH1F *hEtaHighPtPassMu_mc = (TH1F*)mcFile->Get("hEta_PassMu_highpt");

    //Fill Gap in ECAL to avoid division by zero
    FillEGap(hEleEtaPassMu_data);
    FillEGap(hEtaLowPtPassMu_data);
    FillEGap(hEtaMidPtPassMu_data);
    FillEGap(hEtaHighPtPassMu_data);
     
     
    FillEGap(hEleEtaPassMu_mc);
    FillEGap(hEtaLowPtPassMu_mc);   
    FillEGap(hEtaMidPtPassMu_mc);
    FillEGap(hEtaHighPtPassMu_mc);

    // Create TGraphAsymmErrors for efficiencies directly
    TGraphAsymmErrors *efficiencyPt_data = new TGraphAsymmErrors(hElePtPassBoth_data, hElePtPassMu_data, "cl=0.683 b(1,1) mode");
    TGraphAsymmErrors *efficiencyEta_data = new TGraphAsymmErrors(hEleEtaPassBoth_data, hEleEtaPassMu_data, "cl=0.683 b(1,1) mode");

    TGraphAsymmErrors *efficiencyEta_lowpt_data = new TGraphAsymmErrors(hEtaLowPtPass_data, hEtaLowPtPassMu_data, "cl=0.683 b(1,1) mode");
    TGraphAsymmErrors *efficiencyEta_midpt_data = new TGraphAsymmErrors(hEtaMidPtPass_data, hEtaMidPtPassMu_data, "cl=0.683 b(1,1) mode");
    TGraphAsymmErrors *efficiencyEta_highpt_data = new TGraphAsymmErrors(hEtaHighPtPass_data, hEtaHighPtPassMu_data, "cl=0.683 b(1,1) mode");

    // Repeat for MC
    TGraphAsymmErrors *efficiencyPt_mc = new TGraphAsymmErrors(hElePtPassBoth_mc, hElePtPassMu_mc, "cl=0.683 b(1,1) mode");
    TGraphAsymmErrors *efficiencyEta_mc = new TGraphAsymmErrors(hEleEtaPassBoth_mc, hEleEtaPassMu_mc, "cl=0.683 b(1,1) mode");

    TGraphAsymmErrors *efficiencyEta_lowpt_mc = new TGraphAsymmErrors(hEtaLowPtPass_mc, hEtaLowPtPassMu_mc, "cl=0.683 b(1,1) mode");
    TGraphAsymmErrors *efficiencyEta_midpt_mc = new TGraphAsymmErrors(hEtaMidPtPass_mc, hEtaMidPtPassMu_mc, "cl=0.683 b(1,1) mode");
    TGraphAsymmErrors *efficiencyEta_highpt_mc = new TGraphAsymmErrors(hEtaHighPtPass_mc, hEtaHighPtPassMu_mc, "cl=0.683 b(1,1) mode");
        
    // Define 2D histograms for pt vs eta 
    TH2F *hElePtEtaPassMu_data = (TH2F*)dataFile->Get("hEle_PtEta_Pass_Mu");
    TH2F *hElePtEtaPassBoth_data = (TH2F*)dataFile->Get("hEle_PtEta_Pass_Both");
    TH2F *hElePtEtaPassMu_mc = (TH2F*)mcFile->Get("hEle_PtEta_Pass_Mu");
    TH2F *hElePtEtaPassBoth_mc = (TH2F*)mcFile->Get("hEle_PtEta_Pass_Both");

    
    // Added Additional efficiency plots for low, mid, and high pt
    PlotEfficiency(efficiencyEta_lowpt_data, efficiencyEta_lowpt_mc, "#eta (low pt)", "eta_lowpt", year);
    PlotEfficiency(efficiencyEta_midpt_data, efficiencyEta_midpt_mc, "#eta (mid pt)", "eta_midpt", year);
    PlotEfficiency(efficiencyEta_highpt_data, efficiencyEta_highpt_mc, "#eta (high pt)", "eta_highpt", year);
    PlotEfficiency(efficiencyPt_data, efficiencyPt_mc, "p_{T}", "effi_pt", year);
    PlotEfficiency(efficiencyEta_data, efficiencyEta_mc, "#eta", "effi_eta", year);

    // Create a 2D histogram for efficiency pt vs eta
    /*
    TH2F *hEfficiencyPtEta_data = new TH2F("hEfficiencyPtEta_data", "Efficiency PT vs ETA (Data)",
                                            ptN-1, ptBins, etaN-1, etaBins);
    TH2F *hEfficiencyPtEta_mc = new TH2F("hEfficiencyPtEta_mc", "Efficiency PT vs ETA (MC)",
                                          ptN-1, ptBins, etaN-1, etaBins);

    // Fill efficiencies for the 2D case for data
    for (int xBin = 1; xBin <= hElePtEtaPassBoth_data->GetNbinsX(); ++xBin) {
        for (int yBin = 1; yBin <= hElePtEtaPassBoth_data->GetNbinsY(); ++yBin) {
            double nPass = hElePtEtaPassBoth_data->GetBinContent(xBin, yBin);
            double nTotal = hElePtEtaPassMu_data->GetBinContent(xBin, yBin);

            double efficiency = 0;
            double error = 0;

            if (nTotal > 0) {
                efficiency = nPass / nTotal;
                error = sqrt(nPass * (1 - efficiency)) / nTotal; // Poisson error
            } else {
                std::cout << "Warning: Division by zero at bin (" << xBin << ", " << yBin << "). Setting efficiency to 0." << std::endl;
            }

            hEfficiencyPtEta_data->SetBinContent(hEfficiencyPtEta_data->GetBin(xBin, yBin, 1), efficiency);
            hEfficiencyPtEta_data->SetBinError(hEfficiencyPtEta_data->GetBin(xBin, yBin, 1), error); */
            /*
           int pointIndex = efficiencyPtEta_data->GetN();
           efficiencyPtEta_data->SetPoint(pointIndex, 
                                           hElePtEtaPassBoth_data->GetXaxis()->GetBinCenter(xBin),
                                           hElePtEtaPassBoth_data->GetYaxis()->GetBinCenter(yBin));
           efficiencyPtEta_data->SetPointError(pointIndex, 0, 0, error, error);*/      
       /* }
    }

    // Repeat for MC
    for (int xBin = 1; xBin <= hElePtEtaPassBoth_mc->GetNbinsX(); ++xBin) {
        for (int yBin = 1; yBin <= hElePtEtaPassBoth_mc->GetNbinsY(); ++yBin) {
            double nPass = hElePtEtaPassBoth_mc->GetBinContent(xBin, yBin);
            double nTotal = hElePtEtaPassMu_mc->GetBinContent(xBin, yBin);

            double efficiency = 0;
            double error = 0;

            if (nTotal > 0) {
                efficiency = nPass / nTotal;
                error = sqrt(nPass * (1 - efficiency)) / nTotal; // Poisson error
            } else {
                std::cout << "Warning: Division by zero at bin (" << xBin << ", " << yBin << "). Setting efficiency to 0." << std::endl;
            }
            hEfficiencyPtEta_mc->SetBinContent(hEfficiencyPtEta_mc->GetBin(xBin, yBin, 1), efficiency);
            hEfficiencyPtEta_mc->SetBinError(hEfficiencyPtEta_mc->GetBin(xBin, yBin, 1), error);*/
            /*
            int pointIndex = efficiencyPtEta_mc->GetN();
            efficiencyPtEta_mc->SetPoint(pointIndex, 
                                          hElePtEtaPassBoth_mc->GetXaxis()->GetBinCenter(xBin),
                                          hElePtEtaPassBoth_mc->GetYaxis()->GetBinCenter(yBin));
            efficiencyPtEta_mc->SetPointError(pointIndex, 0, 0, error, error);  */
       // }
   // }
    /*
    // Create canvases and save plots
    TCanvas *c1 = new TCanvas("c1", "Efficiency PT vs ETA (Data)", 800, 600);
    hEfficiencyPtEta_data->Draw("COLZ");
    c1->SaveAs("efficiency_pt_eta_data.pdf"); // Save as PDF

    TCanvas *c2 = new TCanvas("c2", "Efficiency PT vs ETA (MC)", 800, 600);
    hEfficiencyPtEta_mc->Draw("COLZ");
    c2->SaveAs("efficiency_pt_eta_mc.pdf"); // Save as PDF
*/
    // Calculate SFs
    vector<TH1F*> h_SF_pt = GetSF(efficiencyPt_data, efficiencyPt_mc, hElePtPassMu_data);
    vector<TH1F*> h_SF_eta = GetSF(efficiencyEta_data, efficiencyEta_mc, hEleEtaPassMu_data);
    vector<TH1F*> h_SF_eta_lowpt = GetSF(efficiencyEta_lowpt_data, efficiencyEta_lowpt_mc, hEtaLowPtPassMu_data);
    vector<TH1F*> h_SF_eta_midpt = GetSF(efficiencyEta_midpt_data, efficiencyEta_midpt_mc, hEtaMidPtPassMu_data);
    vector<TH1F*> h_SF_eta_highpt = GetSF(efficiencyEta_highpt_data, efficiencyEta_highpt_mc, hEtaHighPtPassMu_data);

    // Plot SFs
    PlotSF(h_SF_pt, "p_{T}", "SF_pt", year);
    PlotSF(h_SF_eta, "#eta", "SF_eta", year);
    PlotSF(h_SF_eta_lowpt, "#eta", "SF_eta_lowpt", year);
    PlotSF(h_SF_eta_midpt, "#eta", "SF_eta_midpt", year);
    PlotSF(h_SF_eta_highpt, "#eta", "SF_eta_highpt", year);

    // Create a new ROOT file to store results
    TFile *outputFile = TFile::Open("efficiencies.root", "RECREATE");

    // Write histograms
    efficiencyPt_data->Write("efficiencyPt_data");
    efficiencyEta_data->Write("efficiencyEta_data");
   // efficiencyPtEta_data->Write("efficiencyPtEta_data");
    efficiencyPt_mc->Write("efficiencyPt_mc");
    efficiencyEta_mc->Write("efficiencyEta_mc");
   // efficiencyPtEta_mc->Write("efficiencyPtEta_mc");
    efficiencyEta_lowpt_mc->Write("efficiencyEta_lowpt_mc");
    efficiencyEta_lowpt_data->Write("efficiencyEta_lowpt_data");
    efficiencyEta_midpt_mc->Write("efficiencyEta_midpt_mc");
    efficiencyEta_midpt_data->Write("efficiencyEta_midpt_data");
    efficiencyEta_highpt_mc->Write("efficiencyEta_hightpt_mc");
    efficiencyEta_highpt_data->Write("efficiencyEta_highpt_data");

    // Close the output file
    outputFile->Close();

    // Close input files
    dataFile->Close();
    mcFile->Close();
}

// Main function to call calculateEfficiencies
int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " <dataFileName> <mcFileName> <year>" << std::endl;
        return 1;
    }
    // Define the base path where the files are located
    std::string basePath = "root://cmseos.fnal.gov//store/user/lhasa/Output/cms-TT-run2/EleTrig_Skim/merged/";
    std::string path = basePath + argv[3] + "/";

    std::string dataFilePath = path + argv[1];
    std::string mcFilePath = path + argv[2];

    // Call calculateEfficiencies with the full paths and year
    calculateEfficiencies(dataFilePath.c_str(), mcFilePath.c_str(), argv[3]);
   //calculateEfficiencies(argv[1], argv[2], argv[3]);
    return 0;
}
