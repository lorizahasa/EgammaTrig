import os
import sys
import itertools
sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("HistNano/condor", ""))
from Inputs import *

def runCmd(cmd):
    print("\n\033[01;32m Excecuting: %s \033[00m"%cmd)
    os.system(cmd)

for y in Years:
    mergeDir = "%s/%s/merged"%(outDir, y)
    runCmd("eos root://cmseos.fnal.gov rm -r %s"%mergeDir)
    runCmd("eos root://cmseos.fnal.gov mkdir -p %s"%mergeDir)
    #Merge for each sample
    for s in list(samples.keys()):
        haddOut = "root://cmseos.fnal.gov/%s/%s.root"%(mergeDir, s)
        haddIn  = "`xrdfs root://cmseos.fnal.gov ls -u %s/%s | grep \'%s.*root\'`"%(outDir, y, s)
        #haddIn  = "/eos/uscms/%s/%s*.root"%(outDir, s) 
        runCmd("hadd -f  %s %s"%(haddOut, haddIn))
    print(runCmd(("eos root://cmseos.fnal.gov find --size %s")%mergeDir))

