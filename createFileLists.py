import os,sys

def findSubFolders(path, fileList):
  if path.find('failed') != -1: return 1
  os.system('rm tmp.txt')
  os.system('xrdfs root://cmseos.fnal.gov ls -u ' + path + '/ > tmp.txt')
  f = open('tmp.txt')
  lines = f.readlines()
  f.close()
  #print lines
  if len(lines) == 0:
    return 0
  
  for line in lines:
    line = line.replace('\n','')
    if line.find('log.tar.gz') != -1: 
      continue
    if line.find('.root') != -1: 
      os.system('echo \'' + line + '\' >> ' + fileList)
    else:
      line = '/store' + line.split('/store')[1]
      findSubFolders(line,fileList)
     
  return 1


lines = open("dataList.txt").readlines()
fileList_name = ''
os.system('mkdir -p FileLists/Data/JetHT/')
os.system('rm FileLists/Data/JetHT/*')
os.system('mkdir -p FileLists/MC/QCD_TuneCUETP8M1_13TeV_pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12/')
os.system('rm FileLists/MC/QCD_TuneCUETP8M1_13TeV_pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12/*')

dir_name = ''
eos_path = ''
for line in lines:
  line = line.replace("\n","")
  print line
  data_name = ''
  if line.find("JetHT") != -1:
    is_data = True
    data_name = line.split('/')[2]
    dir_name = "FileLists/Data/JetHT/"
    fileList_name =  dir_name + data_name + '.txt' 
    eos_path = '/store/user/lpcphys/noreplica/duong/BTagAnalyzer_withCTag/Data/JetHT/crab_' + line.split('/')[2]
    type_str = 'data'
  else:
    is_data = False
    data_name = line.split('_')[1] + '-' + line.split('_')[2] #Pt bin
    dir_name = "FileLists/MC/QCD_TuneCUETP8M1_13TeV_pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12/"
    fileList_name =  dir_name + data_name + '.txt'
    eos_path = '/store/user/lpcphys/noreplica/duong/BTagAnalyzer_withCTag/MC/' + line.split('/')[1]
    type_str = "MC"
  
  print dir_name
  print fileList_name
  print eos_path
  findSubFolders(eos_path, fileList_name)
