{
  //gSystem->Exec("rm /uscms/home/duong/BTV_cTag/HistogramManager_C.so") ;
  //gSystem->Exec("rm /uscms/home/duong/BTV_cTag/JetsAna2D_C.so");
  //gSystem->Unload("/uscms/home/duong/BTV_cTag/HistogramManager_C.so");
  //gSystem->Unload("/uscms/home/duong/BTV_cTag/JetsAna2D_C.so");
  gROOT->ProcessLine(".L HistogramManager.C++") ;
  gROOT->ProcessLine(".L JetsAna2D.C++") ;
  gROOT->ProcessLine(".L Run2D_MC.C++")
}
