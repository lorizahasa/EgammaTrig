import os
import sys
import subprocess
import numpy as np
sys.dont_write_bytecode = True
from Inputs import *

#Function to fetch the name of all files in one string
def getFileList(sample):
    std_output, std_error = subprocess.Popen("dasgoclient --query='file dataset=%s status=*'"%sample,shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()
    names = std_output.decode("ascii").replace('\n',' ')
    return names

#Function to print the total events in nice format 
def getEvents(sample):
    std_output, std_error = subprocess.Popen("dasgoclient --query='summary dataset=%s' | sed  's/null/None/g'"%sample,shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()
    num = eval(std_output)[0]['nevents']
    return num

if __name__=="__main__":
    #Store the ouputs in two separate files
    f1 = open("FilesNano_cff.sh", "w")
    f2 = open("JobsNano_cff.py", "w")

    allJobs = 0
    for year in Years: 
    #for year in ['2017']: 
        splitJobs = {}
        print('---------------------------------------')
        print(year)
        print("nFiles\t  nJobs\t nEvents\t Samples")
        print('---------------------------------------')
        line = ""
        jobs = 0
        for sampleName, sample in samples.items():
            line += '%s_FileList_%s="'%(sampleName,year)
            line += "xrootd "
            fileList = getFileList(sample)
            line += fileList 
            line += '"\n\n'
            if fileList=='':
                print ("PROBLEM: %s \n did you do voms?"%sample)
                continue
            nFiles = len(fileList.split(" "))
            evt     = getEvents(sample)
            evtPerJob = 1e6 #5 million
            nJob = int(np.ceil(evt/evtPerJob))
            if nFiles<nJob: 
                nJob = nFiles
            #evt = "NA" 
            splitJobs[sampleName] = [nJob, evt, nFiles]
            jobs += nJob
            print("%i\t %i\t %s"%(nFiles, nJob, sampleName))
        f1.write(line)
        f2.write("Samples_%s = %s \n"%(str(year), str(splitJobs)))
        f2.write("AllJobs_%s = %s \n"%(str(year), str(jobs)))
        print('==================')
        print("AllJobs_%s = %i"%(year, jobs))
        print('==================')
        allJobs += jobs
    f2.write("AllJobs_AllYears = %s \n"%str(allJobs))
 
