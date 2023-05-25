import sys
sys.dont_write_bytecode = True

outDir="/store/user/lpctop/Output/EgammaNanoTnP/HistNano2"


# for condor jobs
samples = {}
samples["EGamma_Run2022G"]  = "/EGamma/Run2022G-PromptNanoAODv11_v1-v2/NANOAOD"
samples["EGamma0_Run2023C"] = "/EGamma0/Run2023C-PromptNanoAODv11p9_v1-v1/NANOAOD"
samples["EGamma1_Run2023C"] = "/EGamma1/Run2023C-PromptNanoAODv11p9_v1-v1/NANOAOD"

# for combination
combSamp = {}
combSamp["EGamma_Run2023C"] = ["EGamma0_Run2023C", "EGamma1_Run2023C"]


#For plotting efficiency
outPlotDir ="/store/user/lpctop/Output/EgammaNanoTnP/PlotHist2"
forOverlay = []
forOverlay.append("EGamma_Run2022G")
#forOverlay.append("EGamma0_Run2023C")
#forOverlay.append("EGamma1_Run2023C")
forOverlay.append("EGamma_Run2023C")

forRatio = []
forRatio.append(["EGamma_Run2023C", "EGamma_Run2022G"])
#forRatio.append(["EGamma0_Run2023C", "EGamma1_Run2023C"])

xVars = []
xVars.append("Pt")
xVars.append("Eta")

