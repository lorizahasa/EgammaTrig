import os
import sys
import json
sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("PlotHist", ""))
from PlotFunc import *
from Inputs import *
from PlotCMSLumi import *
from PlotTDRStyle import *
from ROOT import TFile, TLegend, gPad, gROOT, TCanvas, THStack, TF1, TH1F, TGraphAsymmErrors

padGap = 0.01
iPeriod = 4;
iPosX = 10;
ModTDRStyle()
xPadRange = [0.0,1.0]
yPadRange = [0.0,0.30-padGap, 0.30+padGap,1.0]

os.system("mkdir -p /eos/uscms/%s"%outPlotDir)

for var in xVars:
    gROOT.SetBatch(True)
    canvas = TCanvas()
    if len(forRatio)>0: 
        canvas.Divide(1, 2)
        canvas.cd(1)
        gPad.SetRightMargin(0.03);
        gPad.SetPad(xPadRange[0],yPadRange[2],xPadRange[1],yPadRange[3]);
        gPad.SetTopMargin(0.09);
        gPad.SetBottomMargin(padGap);
        #gPad.SetTickx(0);
        gPad.RedrawAxis();
    else:
        canvas.cd()

    #get files
    files = []
    for ys in list(forOverlay.keys()):
        year = forOverlay[ys][0]
        samp = forOverlay[ys][1]
        inHistDir  = "/eos/uscms/%s/%s/merged"%(outDir, year)
        inFile = TFile.Open("%s/%s.root"%(inHistDir, samp))
        files.append(inFile)

    #get effs 
    effs = []
    for f in files: 
        hEff   = getEff(f, var, "2023C")
        effs.append(hEff)
    
    #plot effs
    leg = TLegend(0.55,0.40,0.95,0.50); 
    decoLegend(leg, 4, 0.030)
    index = 0
    for eff in effs: 
        xTitle = var
        yTitle = "Efficiency"
        eff.GetXaxis().SetTitle(xTitle)
        eff.GetYaxis().SetTitle(yTitle)
        eff.SetLineColor(index+1)
        eff.SetMaximum(1.3*eff.GetMaximum())
        if index==0:
            eff.Draw("HIST")
        else:
            eff.Draw("hist same")
        index =+1
        leg.AddEntry(eff, "%s"%(eff.GetName()), "PL")
    
    #Draw CMS, Lumi, channel
    extraText  = "Preliminary"
    lumi_13TeV = getLumiLabel(year)
    CMS_lumi(lumi_13TeV, canvas, iPeriod, iPosX, extraText)
    leg.Draw()
    
    #Ratio lots
    if len(forRatio)>0: 
        canvas.cd(2)
        gPad.SetTopMargin(padGap); 
        gPad.SetBottomMargin(0.30); 
        gPad.SetRightMargin(0.03);
        #gPad.SetTickx(0);
        gPad.SetPad(xPadRange[0],yPadRange[0],xPadRange[1],yPadRange[2]);
        gPad.RedrawAxis();
        index_ = 0
        for two in forRatio:
            ys1 = forOverlay[two[0]]
            ys2 = forOverlay[two[1]]
            inHistDir1  = "/eos/uscms/%s/%s/merged"%(outDir, ys1[0])
            inFile1 = TFile("%s/%s.root"%(inHistDir1, ys1[1]), "read")
            inHistDir2  = "/eos/uscms/%s/%s/merged"%(outDir, ys2[0])
            inFile2 = TFile("%s/%s.root"%(inHistDir2, ys2[1]), "read")
            hEff1   = getEff(inFile1, var, "aa")
            hEff2   = getEff(inFile2, var, "bb")
            hRatio = hEff1.Clone("Ratio")
            hRatio.Divide(hEff2)
            decoHistRatio(hRatio, xTitle, "Ratio", index_+1)
            hRatio.GetYaxis().SetRangeUser(0.7, 1.3)
            if index_==0:
                hRatio.Draw("HIST")
            else:
                hRatio.Draw("hist same")
            index_=+1
        baseLine = TF1("baseLine","1", -100, 10000);
        baseLine.SetLineColor(3);
        baseLine.Draw("SAME");
    pdf = "/eos/uscms/%s/eff_%s.pdf"%(outPlotDir, var)
    canvas.SaveAs(pdf)
