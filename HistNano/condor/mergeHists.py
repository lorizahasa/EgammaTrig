import os
import sys
import itertools
sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("HistNano/condor", ""))
from SkimInputs import *

def runCmd(cmd):
    print("\n\033[01;32m Excecuting: %s \033[00m"%cmd)
    os.system(cmd)

for year in Years:
    mergeDir = "%s/merged/%s"%(outSkimDir, year)
#runCmd("eos root://cmseos.fnal.gov rm -r %s"%mergeDir)
    runCmd("eos root://cmseos.fnal.gov mkdir -p %s"%mergeDir)
#Merge for each sample
    for s in Samp:
        haddOut = "root://cmseos.fnal.gov/%s/%s_Skim.root"%(mergeDir, s)
        haddIn  = "`xrdfs root://cmseos.fnal.gov ls -u %s/%s | grep \'%s.*root\'`"%(outSkimDir,year, s)
        runCmd("hadd -f  %s %s"%(haddOut, haddIn))

    #for name, samps in combSamp.items():
     #   haddOut = "root://cmseos.fnal.gov/%s/%s_Skim.root"%(mergeDir, name)
     #   haddIns = " "
     #   for samp in samps:
     #       In = "root://cmseos.fnal.gov/%s/%s_Skim.root"%(mergeDir, samp)
     #       haddIns = "%s %s"%(haddIns, In)
     #   print(haddIns)
     #   runCmd("hadd -f  %s %s"%(haddOut, haddIns))
    print(runCmd(("eos root://cmseos.fnal.gov find --size %s")%mergeDir))

