import os
import sys
import itertools
sys.dont_write_bytecode = True

#IMPORT MODULES FROM OTHER DIR
sys.path.insert(0, os.getcwd().replace("condor",""))
sys.path.insert(0, os.getcwd().replace("HistNano/condor",""))
from SkimInputs import *
from JobsNano_cff import Samples_2016Pre, Samples_2016Post,  Samples_2017, Samples_2018  

if os.path.exists("tmpSub"):
    os.system("rm -r tmpSub")
    print("Deleted dir: tmpSub")
os.system("mkdir -p tmpSub/log")
print("Created dir: tmpSub")
condorLogDir = "log"
tarFile = "tmpSub/EgammaTrig.tar.gz"
exDir = '../../../EgammaTrig/HistNano'
ex = '--exclude=%s/plots --exclude=%s/condor --exclude=%s/.git --exclude=%s/*.root'%(exDir, exDir, exDir, exDir)
os.system("tar %s -zcvf %s  ../../../EgammaTrig"%(ex, tarFile))
os.system("cp runMakeSkims.sh tmpSub/")
common_command = \
'Universe   = vanilla\n\
should_transfer_files = YES\n\
when_to_transfer_output = ON_EXIT\n\
Transfer_Input_Files = EgammaTrig.tar.gz, runMakeSkims.sh\n\
x509userproxy        = /uscms/home/lhasa/x509up_u54210\n\
Output = %s/log_$(cluster)_$(process).stdout\n\
Error  = %s/log_$(cluster)_$(process).stderr\n\
Log    = %s/log_$(cluster)_$(process).condor\n\n'%(condorLogDir, condorLogDir, condorLogDir)
#use_x509userproxy = true\n\

#----------------------------------------
#Create jdl files
#----------------------------------------
subFile = open('tmpSub/condorSubmit.sh','w')
for year in Years:
    samples = eval("Samples_%s"%year)
    jdlName = 'submitJobs_%s.jdl'%(year)
    jdlFile = open('tmpSub/%s'%jdlName,'w')
    jdlFile.write('Executable =  runMakeSkims.sh \n')
    jdlFile.write(common_command)
    outDir="%s/%s"%(outSkimDir, year)
    os.system("eos root://cmseos.fnal.gov mkdir -p %s"%outDir)
    print("Created out dir: %s"%outDir)
    jdlFile.write("X=$(step)+1\n")

    for sampleName, nJobEvt in samples.items():
        if "Data_SingleMu" in sampleName:
            nJob = nJobEvt[0]
            args =  'Arguments  = %s %s $INT(X) %i %s\n' %(year, sampleName, nJob, outDir)
            args += "Queue %i\n\n"%nJob
            jdlFile.write(args)
        if "TTbarPowheg_Dilepton" in sampleName:
            nJob = nJobEvt[0]
            args =  'Arguments  = %s %s $INT(X) %i %s\n' %(year, sampleName, nJob, outDir)
            args += "Queue %i\n\n"%nJob
            jdlFile.write(args)
        else: continue
    subFile.write("condor_submit %s\n"%jdlName)
	#print "condor_submit jdl/%s"%jdlFile
    jdlFile.close()
subFile.close()
