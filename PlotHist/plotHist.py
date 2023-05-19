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
    index = 0
    for ys in list(forOverlay.keys()):
        print(ys)
        year = forOverlay[ys][0]
        samp = forOverlay[ys][1]
        inHistDir  = "/eos/uscms/%s/%s/merged"%(outDir, year)
        inFile = TFile("%s/%s.root"%(inHistDir, samp), "read")
        os.system("mkdir -p /eos/uscms/%s"%outPlotDir)

        hEff   = getEff(inFile, var)
        hEff.SetLineColor(index+1)

        plotLegend = TLegend(0.65,0.40,0.95,0.88); 
        decoLegend(plotLegend, 4, 0.035)
        plotLegend.AddEntry(hEff, "%s__%s"%(year, samp))
        if index==0:
            hEff.Draw("HIST")
        else:
            hEff.Draw("hist same")
        index =+1
        plotLegend.Draw()
        lumi_13TeV = getLumiLabel(year)
        xTitle = var
        yTitle = "Efficiency"
        hEff.SetMaximum(1.3*hEff.GetMaximum())
        hEff.GetXaxis().SetTitle(xTitle)
        hEff.GetYaxis().SetTitle(yTitle)
    
        #Draw CMS, Lumi, channel
        extraText  = "Preliminary"
        CMS_lumi(lumi_13TeV, canvas, iPeriod, iPosX, extraText)
    
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
            hEff1   = getEff(inFile1, var)
            hEff2   = getEff(inFile2, var)
            hRatio = hEff1.Clone("Ratio")
            hRatio.Divide(hEff2)
            decoHistRatio(hRatio, xTitle, "Ratio", index_+1)
            hRatio.GetYaxis().SetRangeUser(0.9, 1.1)
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
