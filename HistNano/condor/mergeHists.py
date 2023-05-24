import os
import sys
import itertools
sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("HistNano/condor", ""))
from Inputs import *

def runCmd(cmd):
    print("\n\033[01;32m Excecuting: %s \033[00m"%cmd)
    os.system(cmd)

mergeDir = "%s/merged"%(outDir)
runCmd("eos root://cmseos.fnal.gov rm -r %s"%mergeDir)
runCmd("eos root://cmseos.fnal.gov mkdir -p %s"%mergeDir)
#Merge for each sample
for s in list(samples.keys()):
    haddOut = "root://cmseos.fnal.gov/%s/%s_Hist.root"%(mergeDir, s)
    haddIn  = "`xrdfs root://cmseos.fnal.gov ls -u %s | grep \'%s.*root\'`"%(outDir, s)
    runCmd("hadd -f  %s %s"%(haddOut, haddIn))

for name, samps in combSamp.items():
    haddOut = "root://cmseos.fnal.gov/%s/%s_Hist.root"%(mergeDir, name)
    haddIns = " "
    for samp in samps:
        In = "root://cmseos.fnal.gov/%s/%s_Hist.root"%(mergeDir, samp)
        haddIns = "%s %s"%(haddIns, In)
    print(haddIns)
    runCmd("hadd -f  %s %s"%(haddOut, haddIns))
print(runCmd(("eos root://cmseos.fnal.gov find --size %s")%mergeDir))

