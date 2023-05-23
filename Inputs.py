import sys
sys.dont_write_bytecode = True

Years = ["2023C"]
outDir="/store/user/lpctop/Output/EgammaNanoTnP/HistNano"


samples = {}
samples["EGamma0_Run2023C"] = "/EGamma0/Run2023C-PromptNanoAODv11p9_v1-v1/NANOAOD"
samples["EGamma1_Run2023C"] = "/EGamma1/Run2023C-PromptNanoAODv11p9_v1-v1/NANOAOD"


#For plottin efficiency
outPlotDir="/store/user/lpctop/Output/EgammaNanoTnP/PlotHist"
forOverlay = {}
forOverlay["2023C_EGamma0"] = ['2023C', 'EGamma0_Run2023C']
forOverlay["2023C_EGamma1"] = ['2023C', 'EGamma1_Run2023C']

forRatio = []
forRatio.append(["2023C_EGamma0", "2023C_EGamma1"])

xVars = []
xVars.append("Pt")
#xVars.append("Eta")

