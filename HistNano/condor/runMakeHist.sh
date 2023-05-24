#!/bin/bash
#To be run on remote machine
#Take input arguments as an array
myArray=( "$@" )
#Array: Size=$#, an element=$1, all element = $@

printf "Start skimming at ";/bin/date
printf "Worker node hostname ";/bin/hostname

if [ -z ${_CONDOR_SCRATCH_DIR} ] ; then 
    echo "Running Interactively" ; 
else
    echo "Running In Batch"
    echo ${_CONDOR_SCRATCH_DIR}
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    export SCRAM_ARCH=slc7_amd64_gcc700
    scramv1 project CMSSW CMSSW_13_0_0 
    cd CMSSW_13_0_0/src
    eval `scramv1 runtime -sh`
    cd ../..
	tar --strip-components=1 -zxf EgammaNanoTnP.tar.gz
fi

#Run for Base, Signal region
echo "All arguements: "$@
echo "Number of arguements: "$#
sample=$1
job=$2
nJobTotal=$3
outDir=$4
varname=${sample}_FileList
source FilesNano_cff.sh
cd HistNano
jobNum="${job}of${nJobTotal}"
echo "./makeHist ${jobNum} ${sample}_Hist.root ${!varname}"
./makeHist 2010 $jobNum ${sample}_Hist.root ${!varname}

printf "Done skimming at ";/bin/date
#---------------------------------------------
#Copy the ouput root files
#---------------------------------------------
if [ -z ${_CONDOR_SCRATCH_DIR} ] ; then
    echo "Running Interactively" ;
else
    xrdcp -f ${sample}_Hist_${jobNum}.root root://cmseos.fnal.gov/${outDir}
    echo "Cleanup"
    rm -rf CMSSW*
    rm *.root 
fi
printf "Done ";/bin/date
