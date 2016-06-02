/*-------------------------------------------------------------------------------
*
* Parameter "Type" can be
*   -"DATA12A", "DATA12B" ...
*   -"JetTree_Pt-15to30", "JetTree_Pt-30to50"...
*   -"JetTree_Pt-15to30_TP", "JetTree_Pt-30to50_TP"...
*
* Parameter "tTagger" can be "All", "CSV", "CSVJP"...
*   if it is set to all, it will loop on all defined taggers.
*
*-------------------------------------------------------------------------------*/
#include "JetsAna2D.h"
#include "TROOT.h"
#include "TChain.h"
#include <iostream>
#include "TSystem.h"
#include "TFileCollection.h"

//samplePt can be input file in the format: file.txt
//runCondor : if run on condor place the output to current directory

void Run2D_MC(bool isData=false, TString samplePt="All", TString tTrigger="All", TString tTagger="All", bool WeightTracks = false, TString TrigType = "2015", bool runCondor=false)
{

 bool useInputFileList = false ;
 if (samplePt.Contains(".txt")) useInputFileList = true ; 
   
 gROOT->Reset() ; 

 //gROOT->ProcessLine(".L HistogramManager.C+g") ;
 //gROOT->ProcessLine(".L JetsAna2D.C+g") ;
 
 //gSystem->Load("HistogramManager_C.so") ;
 //gSystem->Load("JetsAna2D_C.so") ;

  vector<TString> Cuts;
  Cuts.push_back("JP");
  Cuts.push_back("CSVv2");
  Cuts.push_back("cMVAv2");
  Cuts.push_back("CvsB") ;
  Cuts.push_back("CvsL") ;

// -- General directory where we can find the list of ntuple
  TString ntDir="FileLists";
// -- General directory for the outputs.
  TString oDir = "/uscmst1b_scratch/lpc1/lpctrig/duong/BTagAnalyzer_withCTag/Output/";
  if (runCondor) oDir = "./" ;
  TString iDir = "MC/" ;  // First the CMSSW version, then below the full path.
          iDir       += "QCD/";
  if (isData) iDir = "Data/JetHT/" ;
  TString weightPU_file    = "QCD_TuneCUETP8M1_13TeV_pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_Data2015D_16Dec2015_v1_nPV.weightPU";
  TString weightPthat_file = "QCD_TuneCUETP8M1_13TeV_pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12.weightPthat";
  TString JSONFile = "one";
  if (isData) {
    weightPU_file = "one" ;
    weightPthat_file = "one" ;
  }


  vector<TString> PthatRanges;
  if (samplePt == "All" || samplePt == "15to30") PthatRanges.push_back("/Pt-15to30/");
  if (samplePt == "All" || samplePt == "30to50") PthatRanges.push_back("/Pt-30to50/");
  if (samplePt == "All" || samplePt == "50to80") PthatRanges.push_back("/Pt-50to80/");
  if (samplePt == "All" || samplePt == "80to120") PthatRanges.push_back("/Pt-80to120/");
  if (samplePt == "All" || samplePt == "120to170") PthatRanges.push_back("/Pt-120to170/");
  if (samplePt == "All" || samplePt == "170to300") PthatRanges.push_back("/Pt-170to300/");
  if (samplePt == "All" || samplePt == "300to470") PthatRanges.push_back("/Pt-300to470/");
  if (samplePt == "All" || samplePt == "470to600") PthatRanges.push_back("/Pt-470to600/");
  if (samplePt == "All" || samplePt == "600to800") PthatRanges.push_back("/Pt-600to800/");
  if (samplePt == "All" || samplePt == "800to1000") PthatRanges.push_back("/Pt-800to1000/");
  if (samplePt == "All" || samplePt == "1000to1400") PthatRanges.push_back("/Pt-1000to1400/");
  if (samplePt == "All" || samplePt == "1400to1800") PthatRanges.push_back("/Pt-1400to1800/");
  if (samplePt == "All" || samplePt == "1800to2400") PthatRanges.push_back("/Pt-1800to2400/");
  if (samplePt == "All" || samplePt == "2400to3200") PthatRanges.push_back("/Pt-2400to3200/");
  if (samplePt == "All" || samplePt == "3200toInf") PthatRanges.push_back("/Pt-3200toInf/");

/////////////for data PthatRanges is used as looping over data set period///////////////
  if (isData) {
    PthatRanges.clear() ;
    if (samplePt == "All" || samplePt == "Run2015C_25ns-16Dec2015-v1") PthatRanges.push_back("/Run2015C_25ns-16Dec2015-v1/") ;
    if (samplePt == "All" || samplePt == "Run2015D-16Dec2015-v1") PthatRanges.push_back("/Run2015D-16Dec2015-v1/") ;
  }

  bool truePU = false;

  truePU = false;

  float maxCutJetPtMax = 1000000.;
  
  int TrigVal_11[9] =      {  0,   30,    60,    80,    110,   150,   190,   240,   300 };
  TString STrigVal_11[9] = { "0", "30",  "60",  "80",  "110", "150", "190", "240", "300"};
  float TrigCut_11[9] =    {  0.,  40.,   70.,   90.,   130.,  170.,  210.,  260,   320 };

  int TrigVal_12[11] =      {  0,   40 ,   60 ,  80 ,   140 ,   200 ,  260 ,  320 ,  400 ,  450 ,  500 };
  TString STrigVal_12[11] = { "0", "40",  "60", "80",  "140",  "200", "260", "320", "400", "450", "500"};
  float TrigCut_12[11] =    {  0.,  50.,   70., 100.,   160.,   220.,  310.,  360.,  440.,  500.,  550.};

  int NTriggers = 11;
  int *TrigVal = TrigVal_12;
  TString *STrigVal = STrigVal_12;
  float *TrigCut = TrigCut_12;

  int iTrigMin = 0; // 0 for MC, 1 for DATA
  int iTrigMax = NTriggers;

 // Compile user's analysis class //
 

  if (gROOT->GetClass("JetTree")==0) return;
 

// //-------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////

// std::vector<int>::iterator it = fifth.begin(); it != fifth.end(); ++it
  for( int iCut = 0; iCut < Cuts.size(); ++iCut )
  {
    for( int iTrig = iTrigMin ; iTrig < iTrigMax; ++iTrig )
    {   
      int nPtHat = PthatRanges.size() ;
      if (useInputFileList) nPtHat = 1 ; 
      for( int iPthat = 0; iPthat < nPtHat; ++iPthat )
      {
        
        TChain c("btagana/ttree");
        // Add RootTuple to the chain.
        c.Reset();  // But first reset it otherwise, you accumulate loop after loop.

        //////////////configure input sample//////////////////
        //c.Add( ntDir + iDir + PthatRanges.at(iPthat) + "JetTree_mc_FatJets_Subjets_*.root" );
        TString fileList = "" ;
        if (!useInputFileList) {
          TString tmp = PthatRanges.at(iPthat) ;
          tmp = tmp.ReplaceAll("/", "") ;
          fileList = ntDir + "/" + iDir + tmp + ".txt" ;
        }
        else { fileList = samplePt ; }
        cout << "\n File list is: " << fileList << endl ;

        TFileCollection fc("fc","list of input root files", fileList) ;
        c.AddFileInfoList((TCollection*)fc.GetList()) ;
        
        /////////////configure output///////////////////////////
        TString oFileBase = oDir;
        if (!runCondor) {
          oFileBase += iDir;  // use the same path as from the input directory
          oFileBase += "/2D/TrigType_" + TrigType;  // Add a subdir to differentiate various TrigType
          if (!useInputFileList) oFileBase += PthatRanges.at(iPthat);      // Add subdir for pthat ranges
          else oFileBase += "_" + samplePt ;
          gSystem->mkdir(oFileBase,kTRUE);	// Create output Directory if it does not exist
          cout << "Created dir" << endl;
        }
        if (!isData) {
          oFileBase += "/JetTree_mc_FatJets_Subjets_";
        }
        else {
          oFileBase += "/JetTree_data_FatJets_Subjets_";
        }
        if (runCondor) {
          TString tmp = samplePt ;
          tmp = tmp.ReplaceAll(".txt", "") ;
          oFileBase += tmp + "_" ;
        }
        if( weightPU_file.Contains("_Run2015C_") ) oFileBase += "2015C_";
        if( weightPU_file.Contains("_Run2015D_05Oct2015_v1_SilverNov03_") ) oFileBase += "Run2015D_05Oct2015_v1_SilverNov03_";
        else if( weightPU_file.Contains("_Run2015D_PromptReco_v4_SilverNov13_") ) oFileBase += "Run2015D_PromptReco_v4_SilverNov13_";
        else if( weightPU_file.Contains("_Run2015D_PromptReco_v4_Silver_") ) oFileBase += "Run2015D_PromptReco_v4_Silver_";
        else if( weightPU_file.Contains("_Run2015D_05Oct2015_v1_") ) oFileBase += "Run2015D_05Oct2015_v1_";
        else if( weightPU_file.Contains("_Run2015D_") ) oFileBase += "2015D_All_";
        else if( weightPU_file.Contains("_Data2015D_16Dec2015_v1") ) oFileBase += "Data2015D_16Dec2015_v1_";


        JetTree* t = new JetTree(&c);

        TString oFile = oFileBase;
        if( WeightTracks ) oFile += "TW_";	// For TrackWeight
        else oFile += "NW_";			// For NoWeight
        oFile += STrigVal[iTrig] + "_";	// Add the HLTCut in the fileName
        oFile = oFile + Cuts.at(iCut) + ".root";	// Add tagger type
        cout << "Test Tagger " << endl;
        cout << "Cuts.at(iCut) = " << Cuts.at(iCut) << endl;
        cout << "tTagger = " << tTagger << endl;

        cout << "Test Trigger " << endl;
        cout << "STrigVal[iTrig] = " << STrigVal[iTrig] << endl;
        cout << "tTrigger = " << tTrigger << endl;
        if( (Cuts.at(iCut) == tTagger || tTagger == "All") &&
            (STrigVal[iTrig] == tTrigger || tTrigger == "All") )
        {
          cout << "Trying to call loop with following parameters " << endl;
          cout << TrigVal[iTrig] << "\t" << TrigCut[iTrig] << endl;
          cout << oFile << endl;
          cout << weightPU_file << "\t" << weightPthat_file << "\t" << truePU << endl;
          //
          t->Loop(Cuts.at(iCut),20.,1000.,
             0., TrigVal[iTrig], TrigCut[iTrig], maxCutJetPtMax, oFile,
             weightPU_file, weightPthat_file, JSONFile, truePU, WeightTracks, TrigType);
          //
        }
      }
    }
  }
}


//#if defined(__ROOTCLING__)
//#pragma link C++ class JetTree;
//#endif
