import os
import sys
import itertools
sys.dont_write_bytecode = True

#IMPORT MODULES FROM OTHER DIR
sys.path.insert(0, os.getcwd().replace("condor",""))
sys.path.insert(0, os.getcwd().replace("HistNano/condor",""))
from Inputs import *
from JobsNano_cff import * 

if os.path.exists("tmpSub"):
    os.system("rm -r tmpSub")
    print("Deleted dir: tmpSub")
os.system("mkdir -p tmpSub/log")
print("Created dir: tmpSub")
condorLogDir = "log"
tarFile = "tmpSub/EgammaNanoTnP.tar.gz"
if os.path.exists(tarFile):
	os.system("rm %s"%tarFile)
os.system("tar -zcvf %s  ../../../EgammaNanoTnP --exclude=.git --exclude=condor"%tarFile)
os.system("cp runMakeHist.sh tmpSub/")
common_command = \
'Universe   = vanilla\n\
should_transfer_files = YES\n\
when_to_transfer_output = ON_EXIT\n\
Transfer_Input_Files = EgammaNanoTnP.tar.gz, runMakeHist.sh\n\
x509userproxy        = /uscms/home/rverma/x509up_u56634\n\
Output = %s/log_$(cluster)_$(process).stdout\n\
Error  = %s/log_$(cluster)_$(process).stderr\n\
Log    = %s/log_$(cluster)_$(process).condor\n\n'%(condorLogDir, condorLogDir, condorLogDir)
#use_x509userproxy = true\n\

#----------------------------------------
#Create jdl files
#----------------------------------------
jdlName = 'submitJobs.jdl'
jdlFile = open('tmpSub/%s'%jdlName,'w')
jdlFile.write('Executable =  runMakeHist.sh \n')
jdlFile.write(common_command)
os.system("eos root://cmseos.fnal.gov mkdir -p %s"%outDir) 
jdlFile.write("X=$(step)+1\n")

for name in samples.keys():
    samp = eval("Sample_%s"%name)
    nJob = samp[name][0]
    args =  'Arguments  = %s $INT(X) %i %s\n' %(name, nJob, outDir)
    args += "Queue %i\n\n"%nJob
    jdlFile.write(args)
jdlFile.close() 
