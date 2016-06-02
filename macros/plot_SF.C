{
  gROOT->ProcessLine(".L 2DSubJets_mistag.C+g") ;
  TH1F retSF, retMistag ;
  TCanvas* tc = plot(retSF, retMistag, 3, true, true, 1, 24, "CvsL", "M", false, 20., 1000., 20., 1000., "Run2015D", 4., false, 0.0, "Jets") ;
  LoopPlot(retSF, retMistag, "CvsL", "2DSubjets",4.) ;
  LoopPlot(retSF, retMistag, "cMVAv2", "2DSubjets",4.) ;
  //LoopPlot(retSF, retMistag, "CvsB", "2DSubjets",4.) ;
}
