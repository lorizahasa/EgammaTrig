


# EgammaNanoTnP
Egamma HLT efficiency from NanoAOD using Tag and Probe


* cmsrel CMSSW_13_0_0
* cd CMSSW_13_0_0
* cmsenv
* git clone git@github.com:ravindkv/EgammaNanoTnP.git
* cd EgammaNanoTnP

Get the NanoAOD files from DAS for the datasets
* python getFiles.py

(here we also decide number of condor jobs for a given dataset, each will have
1 million events to run)

Next we make one TnP histogram from the NanoAOD files
* source FilesNano_cff.sh
* cd HistNano
* make
* ./makeHist 2023C 1of30 EGamma1_Run2023C.root $EGamma1_Run2023C_FileList_2023C

Next we submit condor jobs to make all histograms for all dataset
* cd condor
* python createJdlFiles.py
* cd tmpSub
* condor_submit submitJobs_2023C.jdl
Once the jobs are finished, we merge the histograms
* python mergeHists.py

Finally, we make the efficiency plots from the histograms
* cd ../../PlotHist/
* python plotHist.py

