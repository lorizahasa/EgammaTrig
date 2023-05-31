import sys
sys.dont_write_bytecode = True
eosDir  ="/store/user/lpctop/Output/EgammaNanoTnP"

#---------------
# Histogramming
#---------------
outDir  = "%s/%s"%(eosDir, "HistNano_Pt29_DQM")
# for condor jobs
samples = {}
#samples["EGamma_Run2022G"]  = "/EGamma/Run2022G-PromptNanoAODv11_v1-v2/NANOAOD"
samples["EGamma0_Run2023C"] = "/EGamma0/Run2023C-PromptNanoAODv11p9_v1-v1/NANOAOD"
samples["EGamma1_Run2023C"] = "/EGamma1/Run2023C-PromptNanoAODv11p9_v1-v1/NANOAOD"

# for combination
combSamp = {}
combSamp["EGamma_Run2023C"] = ["EGamma0_Run2023C", "EGamma1_Run2023C"]


#---------------
# Plotting 
#---------------
outPlotDir  = "%s/%s"%(eosDir, "PlotHist")
#For plotting efficiency
forOverlay = []
#forOverlay.append("EGamma_Run2022G")
#forOverlay.append("EGamma0_Run2023C")
#forOverlay.append("EGamma1_Run2023C")
forOverlay.append("HistNano_Pt29_DQM/EGamma_Run2023C")
forOverlay.append("HistNano_Pt31_DQM/EGamma_Run2023C")

forRatio = []
#forRatio.append(["EGamma_Run2023C", "EGamma_Run2022G"])
#forRatio.append(["EGamma0_Run2023C", "EGamma1_Run2023C"])
forRatio.append(["HistNano_Pt29_DQM/EGamma_Run2023C", "HistNano_Pt31_DQM/EGamma_Run2023C"])

xVars = []
xVars.append("Pt")
xVars.append("Eta")

