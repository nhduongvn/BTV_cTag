This package is used to analyze the BTagNtupler to derive the scale factors for heavy flavor taggers. It is developed by Pierre Van Hove with some modifications by Duong Nguyen to run on the FNAL LPC condor farm to significantly improve processing time. 
The packages include:

1. Root macros running on BTagNtupler to make raw plots
  Run2D_MC.C
  JetsAna2D.h
  JetsAna2D.C
  HistogramManager.h
  HistogramManager.C

2. Utility PYTHON scripts
  createFileLists.py
  compileLibs.C
  SubmitToCondor/launch.py

3. Text file for data set to be run on 
  dataList.txt

4. Text files for BTagNtupler file names. Created by createFileLists.py 
  FileLists/Data/JetHT/Run2015C_25ns-16Dec2015-v1.txt
  FileLists/Data/JetHT/Run2015D-16Dec2015-v1.txt
  FileLists/MC/QCD/Pt-15to30.txt
  FileLists/MC/QCD/Pt-30to50.txt
  FileLists/MC/QCD/Pt-50to80.txt
  FileLists/MC/QCD/Pt-80to120.txt
  FileLists/MC/QCD/Pt-120to170.txt
  FileLists/MC/QCD/Pt-170to300.txt
  FileLists/MC/QCD/Pt-300to470.txt
  FileLists/MC/QCD/Pt-470to600.txt
  FileLists/MC/QCD/Pt-600to800.txt
  FileLists/MC/QCD/Pt-800to1000.txt
  FileLists/MC/QCD/Pt-1000to1400.txt
  FileLists/MC/QCD/Pt-1400to1800.txt
  FileLists/MC/QCD/Pt-1800to2400.txt
  FileLists/MC/QCD/Pt-2400to3200.txt
  FileLists/MC/QCD/Pt-3200toInf.txt

5. Weights

6. Root macros to make tagger control plots and SF plots
macros/plot_tagger_all.C
macros/2DSubJets_plot_tagger.C
macros/plot_SF.C
macros/2DSubJets_mistag.C

How to run:
1. List data set you want to run in dataList.txt
2. python createFileLists.py (run once if BTagNtupler doesn't change)
3. compileLibs.C (run it every time you modify the codes in 1.)
4. Go to SubmitToCondor and launch python launch.py (you need to chose the runmode to submit it condor)
5. After condor jobs complete, run launch.py again with runmode set to hadd the outputs
6. Go to macros folder and run root plot_tagger_all.C and root plot_SF.C. They are independent scripts
