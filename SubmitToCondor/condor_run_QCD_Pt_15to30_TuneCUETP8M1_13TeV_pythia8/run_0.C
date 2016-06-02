{ 
gROOT->ProcessLine(".L HistogramManager_C.so") ; 
gROOT->ProcessLine(".L JetsAna2D_C.so") ; 
gROOT->ProcessLine(".L Run2D_MC_C.so") ; 
Run2D_MC("sampleList_0.txt", "All", "All", false, "2015", true) ; 
} 
