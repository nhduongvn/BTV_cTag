import os,sys

def write_condor_config(workDir, preName_rootMacro, name_output_subdir, nJob):
  f = open(workDir + '/condor_config.script', 'w')
  f.write('universe = vanilla \n')
  f.write('Executable = condor_executable.sh \n')
  f.write('Arguments = $(Process) ' + preName_rootMacro + ' ' + name_output_subdir + ' \n') 
  f.write('Should_Transfer_Files = YES \n')
  f.write('WhenToTransferOutput = ON_EXIT \n')
  f.write('Transfer_Input_Files = input.tar \n')
  f.write('Output = ctagana_$(Cluster)_$(Process).stdout \n')
  f.write('Error = ctagana_$(Cluster)_$(Process).stderr \n')
  f.write('Log = ctagana_$(Cluster)_$(Process).log \n')
  f.write('notify_user = ${LOGNAME}@FNAL.GOV \n')
  f.write('+LENGTH="SHORT" \n')
  f.write('x509userproxy = /tmp/x509up_u12772 \n')
  tmp = 'Queue ' + str(nJob) + '\n'
  f.write(tmp)
  f.close()

  f = open(workDir + '/condor_executable.sh', 'w')
  f.write('#!/bin/bash \n')
  f.write('echo $SHELL \n')
  f.write('source /cvmfs/cms.cern.ch/cmsset_default.sh \n')
  f.write('cd /uscms_data/d3/duong/CMSSW/CMSSW_7_6_5/src \n')
  f.write('eval `scramv1 runtime -sh` \n')
  f.write('cd ${_CONDOR_SCRATCH_DIR} \n')
  f.write('tar -xvf input.tar \n')
  f.write('root -b -q $2_$1.C \n')
  #f.write('xrdcp *.root /uscmst1b_scratch/lpc1/lpctrig/duong/Output/BTV_cTag/MC/QCD/$3/ \n')
  #f.write('rm *.root \n')
  f.close()
  

def make_input_file_list(nFile, outDir_file_list, file_list_name):
  
  lines = open(file_list_name).readlines()
  nJob = len(lines)/nFile + 1

  iFile = 0
  for line in range(0, len(lines), nFile):
    tmp = file_list_name.split('/')
    #newFile = open(outDir_file_list + '/' + tmp[len(tmp)-1] + '_' + str(iFile) + '.txt', 'w')  
    newFile = open(outDir_file_list + '/sampleList_' + str(iFile) + '.txt', 'w')  
    
    tmpFiles = lines[line:line+nFile]
    for i in range(0, len(tmpFiles)):
      newFile.write(tmpFiles[i])
    
    iFile = iFile + 1
  
  return nJob

def write_root_macro(is_data, workDir, nJob):
  for i in range(0, nJob):
    f = open(workDir + '/run_' + str(i) + '.C', 'w')
    f.write('{ \n') ;
    f.write('gROOT->ProcessLine(".L HistogramManager_C.so") ; \n')
    f.write('gROOT->ProcessLine(".L JetsAna2D_C.so") ; \n')
    f.write('gROOT->ProcessLine(".L Run2D_MC_C.so") ; \n')
    if not is_data : f.write('Run2D_MC(false, "sampleList_' + str(i) + '.txt", "All", "All", false, "2015", true) ; \n')
    else : f.write('Run2D_MC(true, "sampleList_' + str(i) + '.txt", "All", "All", false, "2015", true) ; \n')
    f.write('} \n')


#///////////////////////////////////////////////////////////////////
runMode = 1 #0: submit, 1: check output and hadd output file

lines = open("../dataList.txt").readlines()
#lines = open("dataListData.txt").readlines()
nFile = 5
dir_file_list = '../FileLists'


#os.system('root -l compileLibs.C')

is_data = False

for line in lines:
  line = line.replace("\n","")
  if line.find('#') != -1: continue
  print line
  data_name = ''
  dir_file_list_tmp = ''
  if line.find("JetHT") != -1:
    is_data = True
    data_name = line.split('/')[2]
    work_dir = 'condor_run_' + data_name 
    dir_file_list_tmp = dir_file_list + '/Data/JetHT' 
  else:
    is_data = False
    data_name = line.split('/')[1]
    work_dir = 'condor_run_' + data_name
    dir_file_list_tmp = dir_file_list + '/MC/QCD' 

#####################################################
  if runMode == 0:
    os.system('mkdir ' + work_dir)
    os.system('rm -r ' + work_dir + '/*')
    
    if not is_data: file_list_preName = 'Pt-' + line.split('_')[2] 
    else : file_list_preName = data_name
    file_list_name = dir_file_list_tmp + '/' + file_list_preName + '.txt' 
  
    nJob = make_input_file_list(nFile, work_dir, file_list_name)
    name_output_subdir = file_list_preName
    write_root_macro(is_data, work_dir, nJob)
    write_condor_config(work_dir, "run", name_output_subdir, nJob)
  
    os.system('cp ../HistogramManager_C.so ' + work_dir)
    os.system('cp ../HistogramManager_C_ACLiC_dict_rdict.pcm ' + work_dir)
    os.system('cp ../HistogramManager.* ' + work_dir)
    os.system('cp ../JetsAna2D_C.so ' + work_dir)
    os.system('cp ../JetsAna2D_C_ACLiC_dict_rdict.pcm ' + work_dir)
    os.system('cp ../JetsAna2D.* ' + work_dir)
    os.system('cp ../Run2D_MC_C.so ' + work_dir)
    os.system('cp ../Run2D_MC_C_ACLiC_dict_rdict.pcm ' + work_dir)
    os.system('cp ../Run2D_MC.* ' + work_dir)
    if not is_data : os.system('cp -r ../Weights ' + work_dir)
   #os.system('mkdir /uscmst1b_scratch/lpc1/lpctrig/duong/Output/BTV_cTag/MC/QCD/' + name_output_subdir + '/')
   #os.system('rm -rf /uscmst1b_scratch/lpc1/lpctrig/duong/Output/BTV_cTag/MC/QCD/' + name_output_subdir + '/*')

    os.chdir(work_dir)
    if not is_data : os.system('tar -cvf input.tar *.txt *.C *.h *.pcm Run2D_MC_C.so HistogramManager_C.so JetsAna2D_C.so Weights')
    else: os.system('tar -cvf input.tar *.txt *.C *.h *.pcm Run2D_MC_C.so HistogramManager_C.so JetsAna2D_C.so')
    os.system('condor_submit condor_config.script')

    os.chdir('../')

########################################################
  if runMode == 1:
    os.chdir(work_dir)
    os.system('ls run_*.C > tmp.txt')
    lines_tmp = open('tmp.txt', 'r').readlines()
    nJob = len(lines_tmp)
    
    taggers = ['JP', 'CSVv2', 'cMVAv2', 'CvsB', 'CvsL']
    triggers = ['0','40' ,'60' ,'80','140','200','260','320','400','450','500']
    weights = ['NW']
    preName_rootFile = ''
    dir_final_rootFile = ''
    if not is_data:
      dir_final_rootFile = '../../Output/MC/QCD/' + data_name
      preName_rootFile = 'JetTree_mc_FatJets_Subjets_sampleList_'
    else:
      dir_final_rootFile = '../../Output/Data/JetHT/' + data_name
      preName_rootFile = 'JetTree_data_FatJets_Subjets_sampleList_'

    os.system('mkdir ' + dir_final_rootFile)
    os.system('rm -rf ' + dir_final_rootFile + '/*')
    
    for tag in taggers:
      for trig in triggers:
        for wei in weights:
          wildcard = preName_rootFile + '*_' + wei + '_' + trig + '_' + tag + '.root'
          preName_out = wildcard.replace('_*_', '_all_')
          preName_out = preName_out.replace('.root', '')
          print preName_out
          os.system('ls ' + wildcard + ' > ' + preName_out + '.txt')
          ############check total number of root file###################
          lines_tmp = open(preName_out + '.txt').readlines()
          nJob_current = len(lines_tmp)
          if nJob_current != nJob:
            print '============================================'
            print 'Warning: MISSING OUTPUT ROOT FILES, found ' + nJob_current + ' files but ' + nJob + ' files expected'
            print work_dir + '  ' + preName_out

          #############hadd############################################## 
          cmd_hadd = 'hadd ' + dir_final_rootFile + '/' + preName_out + '.root @' + preName_out + '.txt' 
          print cmd_hadd
          os.system(cmd_hadd)     
     
    os.chdir('../')
