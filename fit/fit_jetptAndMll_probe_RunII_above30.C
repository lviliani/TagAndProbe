#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
//#include <vector>

#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TMarker.h>

#include <RooRealVar.h>
#include <RooMinuit.h>
#include <RooChi2Var.h>
#include <RooExponential.h>
#include <RooFormulaVar.h>
#include <RooAddPdf.h>
#include <RooDataSet.h>
#include <RooDataSet.h>
#include <RooArgSet.h>
#include <RooArgList.h>
#include <RooPlot.h>
#include <RooFitResult.h>
#include <RooKeysPdf.h>
#include <RooCategory.h>
#include <RooSimultaneous.h>
#include <RooDataHist.h>
#include <RooHistPdf.h>
#include <RooMCStudy.h>
#include <RooArgList.h>
#include <RooBinning.h>

using namespace std;

using namespace RooFit;

//-------------------------------------------------------------------------

int fit_mc_jetptprobe(int isMC=1, int ntoys=0, double scaleZ=0, double scaleST=0) {

  gROOT->SetStyle("Plain");
  gROOT->ForceStyle();

  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleSize(0.04);
  gStyle->SetTitleFont(42, "hxy");      // for histogram and axis titles
  gStyle->SetLabelFont(42, "xyz");      // for axis labels (values)
  // Read in the data.  

  TString dir = "../scripts/";
  TString mll_cut = "jetptprobe > 30 && jetpttag > 20 && btagtag>0.605";

  //TFile* fileinData = new TFile(dir+"tandp_mediumT_looseP_loose_test3.root");
  TFile* fileinMC = new TFile(dir+"tandp_mediumT_looseP_loose_test3.root");
  TFile* fileout = 0;
  if (isMC == 1)
    fileout = new TFile(Form("mc_jetptAndMll_probe_RunII_ntoys_%d_Z%f_ST%f.root", ntoys, scaleZ, scaleST), "RECREATE");
  else
    fileout = new TFile("data_jetptAndMll_probe_RunII_above30.root", "RECREATE");

  TTree* tree_ttbar = (TTree*) fileinMC->Get("TTbar");
  TTree* tree_bkg = (TTree*) fileinMC->Get("OtherBkg");
  TTree* tree_data = (TTree*) fileinMC->Get("Data");

  RooCategory passprobe_cat("passfail","passfail");
  passprobe_cat.defineType("pass", 1);
  passprobe_cat.defineType("fail", 0);

  RooRealVar isb("isb", "isb", -1, 1);
  RooRealVar jetpt2("jetpttag", "jetpttag", 15, 150);
  RooRealVar jetptprobe("jetptprobe", "jetptprobe", 30, 200);
  RooRealVar btagtag("btagtag", "btagtag", 0., 1.);
  RooRealVar dataset("dataset", "dataset", 0., 10000000.);
  double binning_a[9] = {30, 40, 50, 60, 80, 100, 120, 150, 200};
  RooBinning binning(8, binning_a);
  jetptprobe.setBinning(binning) ;
  RooRealVar weight("weight", "weight", -1000, 1000);  
  RooRealVar weightSTUp("weightSTUp", "weightSTUp", -1000, 1000);  
  RooRealVar weightSTDown("weightSTDown", "weightSTDown", -1000, 1000);  
  RooRealVar mll("mll", "mll", 0., 180.);
  mll.setBins(3);
  RooRealVar mth("mth", "mth", 0, 500);
  

  RooRealVar efficiency_s("efficiency_s", "efficiency_s", 0, 1);
  RooRealVar efficiency_b("efficiency_b", "efficiency_b", 0, 1); 
  RooRealVar sTT("sTT","signal yield TT",500,100000);
  RooRealVar sZ("sZ","signal yield Z",500,100000);
  RooRealVar bTT("bTT","background yield TT",1000,100000);
  RooRealVar bZ("bZ","background yieldZ",1000,100000);
  
  RooFormulaVar s_pass_TT("s_pass_TT","sTT*efficiency_s", RooArgList(sTT,efficiency_s));
  RooFormulaVar b_pass_TT("b_pass_TT","bTT*efficiency_b", RooArgList(bTT,efficiency_b));
  RooFormulaVar s_fail_TT("s_fail_TT","sTT*(1-efficiency_s)", RooArgList(sTT,efficiency_s));
  RooFormulaVar b_fail_TT("b_fail_TT","bTT*(1-efficiency_b)", RooArgList(bTT,efficiency_b));

  RooFormulaVar s_pass_Z("s_pass_Z","sZ*efficiency_s", RooArgList(sZ,efficiency_s));
  RooFormulaVar b_pass_Z("b_pass_Z","bZ*efficiency_b", RooArgList(bZ,efficiency_b));
  RooFormulaVar s_fail_Z("s_fail_Z","sZ*(1-efficiency_s)", RooArgList(sZ,efficiency_s));
  RooFormulaVar b_fail_Z("b_fail_Z","bZ*(1-efficiency_b)", RooArgList(bZ,efficiency_b));

  // Import Data
  RooArgSet variables_to_import(jetpt2, jetptprobe, weight, mll, passprobe_cat, isb, btagtag, dataset);
  RooDataSet* data = new RooDataSet("data", "data",  variables_to_import, Import(*tree_data), WeightVar(weight), Cut(mll_cut+" && isb==0"));

  // Import TTbar and Bkg datasets
  //signal: the probe is a b
  RooDataSet* b_pp = new RooDataSet("b_pp", "b_pp", variables_to_import, Import(*tree_ttbar), WeightVar(weight), Cut(mll_cut+" && passfail==1 && isb==1"));
  RooDataSet* b_fp = new RooDataSet("b_fp", "b_fp", variables_to_import, Import(*tree_ttbar), WeightVar(weight), Cut(mll_cut+" && passfail==0 && isb==1"));

  RooDataSet* b_pp2 = new RooDataSet("b_pp2", "b_pp", variables_to_import, Import(*tree_bkg), WeightVar(weight), Cut(mll_cut+" && passfail==1 && isb==1"));
  RooDataSet* b_fp2 = new RooDataSet("b_fp2", "b_fp", variables_to_import, Import(*tree_bkg), WeightVar(weight), Cut(mll_cut+" && passfail==0 && isb==1"));

  b_pp->append(*b_pp2);
  b_fp->append(*b_fp2);
  
  
  //background: the probe is not a b
  RooDataSet* l_pp = new RooDataSet("l_pp", "l_pp", variables_to_import, Import(*tree_bkg), WeightVar(weight), Cut(mll_cut+" && passfail==1 && isb==0"));
  RooDataSet* l_fp = new RooDataSet("l_fp", "l_fp", variables_to_import, Import(*tree_bkg), WeightVar(weight), Cut(mll_cut+" && passfail==0 && isb==0"));
  
  RooDataSet* l_pp2 = new RooDataSet("l_pp2", "l_pp2", variables_to_import, Import(*tree_ttbar), WeightVar(weight), Cut(mll_cut+" && passfail==1 && isb==0"));
  RooDataSet* l_fp2 = new RooDataSet("l_fp2", "l_fp2", variables_to_import, Import(*tree_ttbar), WeightVar(weight), Cut(mll_cut+" && passfail==0 && isb==0"));
  
  l_pp->append(*l_pp2);
  l_fp->append(*l_fp2);

  Double_t entries_b_pp = b_pp->sumEntries();
  Double_t entries_b_fp = b_fp->sumEntries();
  Double_t entries_l_pp = l_pp->sumEntries();
  Double_t entries_l_fp = l_fp->sumEntries();

  //Double_t tot_events = entries_b_pp+entries_b_fp+entries_l_pp+entries_l_fp; 

  
  //histd to build pdfs for:
  //true b passprobe not from Z
  RooDataHist * d_b_ppTT = new RooDataHist("d_b_ppTT","d_b_ppTT",RooArgSet(jetptprobe, mll),*(b_pp->reduce("dataset != 3"))) ;
  //true b failprobe not from Z
  RooDataHist * d_b_fpTT = new RooDataHist("d_b_fpTT","d_b_fpTT",RooArgSet(jetptprobe, mll),*(b_fp->reduce("dataset != 3"))) ;
  //true non-b passprobe not from Z
  RooDataHist * d_l_ppTT = new RooDataHist("d_l_ppTT","d_l_ppTT",RooArgSet(jetptprobe, mll),*(l_pp->reduce("dataset != 3"))) ;
  //true non-b failprobe not from Z
  RooDataHist * d_l_fpTT = new RooDataHist("d_l_fpTT","d_l_fpTT",RooArgSet(jetptprobe, mll),*(l_fp->reduce("dataset != 3"))) ;

  //same as above, but from Z
  RooDataHist * d_b_ppZ = new RooDataHist("d_b_ppZ","d_b_ppZ",RooArgSet(jetptprobe, mll),*(b_pp->reduce("dataset == 3"))) ;
  RooDataHist * d_b_fpZ = new RooDataHist("d_b_fpZ","d_b_fpZ",RooArgSet(jetptprobe, mll),*(b_fp->reduce("dataset == 3"))) ;
  RooDataHist * d_l_ppZ = new RooDataHist("d_l_ppZ","d_l_ppZ",RooArgSet(jetptprobe, mll),*(l_pp->reduce("dataset == 3"))) ;
  RooDataHist * d_l_fpZ = new RooDataHist("d_l_fpZ","d_l_fpZ",RooArgSet(jetptprobe, mll),*(l_fp->reduce("dataset == 3"))) ;
 
  
  //PDFS corresponding to the eight combinations above
  RooHistPdf pdf_b_ppTT("pdf_b_ppTT","pdf_b_ppTT",RooArgList(jetptprobe, mll),*d_b_ppTT);
  RooHistPdf pdf_b_fpTT("pdf_b_fpTT","pdf_b_fpTT",RooArgList(jetptprobe, mll),*d_b_fpTT);
  RooHistPdf pdf_l_ppTT("pdf_l_ppTT","pdf_l_ppTT",RooArgList(jetptprobe, mll),*d_l_ppTT);
  RooHistPdf pdf_l_fpTT("pdf_l_fpTT","pdf_l_fpTT",RooArgList(jetptprobe, mll),*d_l_fpTT);

  RooHistPdf pdf_b_ppZ("pdf_b_ppZ","pdf_b_ppZ",RooArgList(jetptprobe, mll),*d_b_ppZ);
  RooHistPdf pdf_b_fpZ("pdf_b_fpZ","pdf_b_fpZ",RooArgList(jetptprobe, mll),*d_b_fpZ);
  RooHistPdf pdf_l_ppZ("pdf_l_ppZ","pdf_l_ppZ",RooArgList(jetptprobe, mll),*d_l_ppZ);
  RooHistPdf pdf_l_fpZ("pdf_l_fpZ","pdf_l_fpZ",RooArgList(jetptprobe, mll),*d_l_fpZ);
  
  //model for pass probe-pairs, summing true b passprobes from Z and not and true non b from Z and not
  RooAddPdf model_pp("model_pp", "Model PP", RooArgList(pdf_b_ppTT, pdf_l_ppTT, pdf_b_ppZ, pdf_l_ppZ), RooArgList(s_pass_TT, b_pass_TT, s_pass_Z, b_pass_Z));
  //model for fail probe-pairs, summing true b passprobes from Z and not and true non b from Z and not
  RooAddPdf model_fp("model_fp", "Model FP", RooArgList(pdf_b_fpTT, pdf_l_fpTT, pdf_b_fpZ, pdf_l_fpZ), RooArgList(s_fail_TT, b_fail_TT, s_fail_Z, b_fail_Z));


  // Use MC templates as Data
  //comb_mc_b is an asimov for the b
  //model_b is used afterwards to make toys
  RooDataSet* comb_mc_b= (RooDataSet*)b_pp->Clone();
  comb_mc_b->append(*b_fp);
  RooDataHist d_model_b("d_model_tt", "d_model_tt", RooArgSet(jetptprobe, mll, passprobe_cat), *comb_mc_b);
  RooHistPdf model_b("model_b", "model_b", RooArgSet(jetptprobe, mll, passprobe_cat), d_model_b);

  //comb_mc_b is an asimov for the l
  //model_l is used afterwards to make toys
  RooDataSet* comb_mc_l= (RooDataSet*)l_pp->Clone();
  comb_mc_l->append(*l_fp);
  RooDataHist d_model_l("d_model_l", "d_model_l", RooArgSet(jetptprobe, mll, passprobe_cat), *comb_mc_l);
  RooHistPdf model_l("model_l", "model_l", RooArgSet(jetptprobe, mll, passprobe_cat), d_model_l);

  // possibly add a new weight
  // b_d and l_d below differ from comb_mc_b and comb_mc_l only for the weight. Some Gymnastic is needed to ass the new weight due
  // deprecation of setWeightVar
  RooArgSet variables(btagtag, dataset, jetpt2, jetptprobe, weight, mll, passprobe_cat);
  cout << Form("weight*(1+%f*(dataset==3)*(1+%f((dataset>=12 && dataset<=16)||dataset==22)))", scaleZ, scaleST) << endl;
  RooFormulaVar new_weight("weightNew", "weightNew", Form("weight*(1+%f*(dataset==3)*(1+%f*((dataset>=12 && dataset<=16)||dataset==22)))", scaleZ, scaleST), RooArgList(weight, dataset));
  //RooFormulaVar new_weight("weightNew", "weightNew", "weight*(1-0.17*((dataset>=12 && dataset<=16)||dataset==22))", RooArgList(weight, dataset));
  RooDataSet* b_d1 = new RooDataSet("b_d1", "ttbar_d1", variables, Import(*tree_ttbar), Cut(mll_cut));
  RooRealVar* wVar1 = (RooRealVar*) b_d1->addColumn(new_weight);
  RooDataSet* b_d = new RooDataSet("b_d", "b_d", RooArgSet(variables, *wVar1), Import(*b_d1), WeightVar(*wVar1));

  RooDataSet* l_d1 = new RooDataSet("l_d1", "l_d1", variables, Import(*tree_bkg), Cut(mll_cut));
  RooRealVar* wVar2 = (RooRealVar*) l_d1->addColumn(new_weight);
  RooDataSet* l_d = new RooDataSet("l_d", "l_d", RooArgSet(variables, *wVar2), Import(*l_d1),  WeightVar(*wVar2));


  RooDataSet* comb_mc = (RooDataSet*) b_d->Clone();
  comb_mc->append(*l_d);


  // Create the simultaneous fit
  RooSimultaneous total_fit("total_fit","total_fit", passprobe_cat);
  passprobe_cat.setLabel("pass");
  total_fit.addPdf(model_pp, passprobe_cat.getLabel());
  passprobe_cat.setLabel("fail");
  total_fit.addPdf(model_fp, passprobe_cat.getLabel());

  RooDataHist * hdata = 0;
  if (isMC==1)
    hdata = new RooDataHist("hdata", "hdata", RooArgSet(jetptprobe, mll, passprobe_cat), *comb_mc);
  else 
    hdata = new RooDataHist("hdata", "hdata", RooArgSet(jetptprobe, mll, passprobe_cat), *data);
  //m2.migrad() ;
  //m2.hesse() ;
  RooFitResult* fitresult = total_fit.fitTo(*hdata, Extended(), Save(true));//m2.fit("r");//m2.save() ;
   
  // Fit to MC templates
  //RooFitResult* fitresult = total_fit.fitTo(combMC,  RooFit::Save(true),  RooFit::PrintLevel(3), RooFit::Strategy(1), RooFit::NumCPU(6),  RooFit::Extended(kTRUE), RooFit::SumW2Error(true));

/*
  // Create a Toy MC and fit again
  RooMCStudy mc(total_fit, RooArgSet(jetptprobe,passprobe_cat), FitModel(total_fit), Extended(true), FitOptions(Save(true),Minos(RooArgSet(efficiency_s))), FitOptions(NumCPU(4,0)));
  mc.generateAndFit(ntoys, tot_events, true);

*/

  TH1D* h_pull_effs = new TH1D("pull_effs","pull_effs",100,-3,3);
  TH1D* h_pull_effb = new TH1D("pull_effb","pull_effb",100,-3,3);

  TH1D* h_effs = new TH1D("effs","effs",100,0,2);
  TH1D* h_effb = new TH1D("effb","effb",100,0,2);

  RooRealVar* effs_true = (RooRealVar*)fitresult->floatParsFinal().find("efficiency_s");
  RooRealVar* effb_true = (RooRealVar*)fitresult->floatParsFinal().find("efficiency_b");

  //TCanvas * stapuppa2 = new TCanvas();
  //stapuppa2->cd();
  ///RooPlot* cicia2 = jetptprobe.frame();

  for (int i=0; i<ntoys; i++){

    std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$ TOY " << i << " $$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
    cout << "tt  entries: " << entries_b_pp+entries_b_fp << endl;
    cout << "bkg entries: " << entries_l_pp+entries_l_fp << endl;
     
    RooDataSet* toy_b = model_b.generate(RooArgSet(jetptprobe, mll, passprobe_cat), NumEvents(int(entries_b_pp+entries_b_fp)), Extended());
    b_pp = (RooDataSet*) toy_b->reduce("passfail==passfail::pass");
    b_pp->SetName("b_pp");
    b_fp = (RooDataSet*) toy_b->reduce("passfail==passfail::fail");
    b_fp->SetName("b_fp");
    RooDataSet* toy_l = model_l.generate(RooArgSet(jetptprobe, mll, passprobe_cat), NumEvents(int(entries_l_pp+entries_l_fp)), Extended());
    l_pp = (RooDataSet*) toy_l->reduce("passfail==passfail::pass");
    l_pp->SetName("l_pp"); 
    l_fp = (RooDataSet*) toy_l->reduce("passfail==passfail::fail");
    l_fp->SetName("l_fp"); 
    RooDataSet* toy_comb = (RooDataSet*) toy_b->Clone();
    toy_comb->append(*toy_l);
    comb_mc = toy_comb;
    comb_mc->SetName("l_pp");
     

    RooDataHist * htoy= new RooDataHist("toy", "toy", RooArgSet(jetptprobe, mll, passprobe_cat), *toy_comb);
    //htoy->plotOn(cicia2);
    ///cicia2->Draw();
    RooChi2Var chi2toy("chi2toy","chi2toy",total_fit,*htoy, Extended());
    RooMinuit m2toy(chi2toy) ;
    RooFitResult* frSB = total_fit.fitTo(*htoy, Extended(), Save(true));//m2toy.fit("r");

    //const RooFitResult* frSB = mc.fitResult(i);
    if (frSB->status()==0 && frSB->covQual()==3) {
      RooRealVar* effs_res = (RooRealVar*)frSB->floatParsFinal().find("efficiency_s");
      RooRealVar* effb_res = (RooRealVar*)frSB->floatParsFinal().find("efficiency_b");
      double pull_effs = (effs_res->getValV() - effs_true->getValV())/effs_res->getError();
      double pull_effb = (effb_res->getValV() - effb_true->getValV())/effb_res->getError();
//      cout << "Efficiency s = " << efficiency_s.getValV() << "+-" << efficiency_s.getError() << endl;
      h_pull_effs->Fill(pull_effs);       
      h_pull_effb->Fill(pull_effb);       
      h_effs->Fill(effs_res->getValV());
      h_effb->Fill(effb_res->getValV());
    }
    frSB->Write();
    if (i != ntoys-1){
      delete toy_l;
      delete toy_b;
      delete toy_comb;
      delete htoy;
      delete frSB;
      delete l_pp;
      delete l_fp;
      delete b_pp;
      delete b_fp;

    }
  }

  cout << "################################################" << endl;
  cout << "True Efficiency_s = " << effs_true->getValV() << " +- " << effs_true->getError() << endl;
  cout << "True Efficiency_b = " << effb_true->getValV() << " +- " << effb_true->getError() << endl;
  cout << "################################################" << endl;


  // plot results

  TCanvas* c_pull = new TCanvas();
  c_pull->Divide(2);
  c_pull->cd(1);
  h_pull_effs->Draw();
  c_pull->cd(2);
  h_pull_effb->Draw();
  /*c_pull->cd(3);
  h_effs->Draw();
  c_pull->cd(4);
  h_effb->Draw();*/

  fileout->cd();
  c_pull->Write();
  h_pull_effs->Write();
  h_pull_effb->Write();
  
  //RooPlot* frame = jetptprobe.frame(Title("tag-pass-probe"),Binning(binning),Range(30,150,1));
  //RooPlot* frame1 = jetptprobe.frame(Title("tag-fail-probe"),Binning(binning),Range(30,150,1));

  RooPlot* frame = jetptprobe.frame(Title("tag-pass-probe"),Binning(binning));
  RooPlot* frame1 = jetptprobe.frame(Title("tag-fail-probe"),Binning(binning));

  //RooAbsPdf* fitclone = (RooAbsPdf*) total_fit.Clone();

  comb_mc->plotOn(frame,Cut("passfail==passfail::pass"), Binning(binning)) ;
  total_fit.paramOn(frame,RooArgSet(sTT,bTT,sZ,bZ,efficiency_s,efficiency_b));
  total_fit.plotOn(frame,Slice(passprobe_cat,"pass"),ProjWData(mll,*hdata));
  comb_mc->plotOn(frame,Cut("passfail==passfail::pass"), Binning(binning)) ;
  b_pp->plotOn(frame,MarkerColor(kRed), LineWidth(0), Binning(binning));
  l_pp->plotOn(frame,MarkerColor(kBlue), LineWidth(0), Binning(binning));
  //ttbar_pp->plotOn(frame,MarkerColor(kGreen), LineWidth(0), Cut("dataset == 3"), Binning(binning));
  //otherbkg_pp->plotOn(frame,MarkerColor(kMagenta), LineWidth(0), Cut("dataset == 3"), Binning(binning));
  //pdf_b_ppTT.plotOn(frame, ProjWData(passprobe_cat,*hdata), LineColor(kRed));
  //pdf_l_ppTT.plotOn(frame, ProjWData(mll,*hdata), LineColor(kBlue));
  //pdf_b_ppZ.plotOn(frame, ProjWData(mll,*hdata), LineColor(kGreen));
  //pdf_l_ppZ.plotOn(frame, ProjWData(mll,*hdata), LineColor(kMagenta));


  comb_mc->plotOn(frame1,Cut("passfail==passfail::fail"), Binning(binning)) ;
  total_fit.plotOn(frame1,Slice(passprobe_cat,"fail"),ProjWData(mll, *hdata)) ;
  comb_mc->plotOn(frame1,Cut("passfail==passfail::fail"),Binning(binning)) ;
  b_fp->plotOn(frame1,MarkerColor(kRed), LineWidth(0),Binning(binning));
  l_fp->plotOn(frame1,MarkerColor(kBlue), LineWidth(0),Binning(binning));
  frame1->Print();


  RooPlot* frame3 = mll.frame(Title("tag-pass-probe-mll"));
  RooPlot* frame4 = mll.frame(Title("tag-fail-probe-mll"));

  comb_mc->plotOn(frame3,Cut("passfail==passfail::pass"));
  total_fit.plotOn(frame3,Slice(passprobe_cat,"pass"),ProjWData(passprobe_cat,*hdata)) ;
  comb_mc->plotOn(frame3,Cut("passfail==passfail::pass")) ;
  b_pp->plotOn(frame3,MarkerColor(kRed), LineWidth(0));
  l_pp->plotOn(frame3,MarkerColor(kBlue), LineWidth(0));

  comb_mc->plotOn(frame4,Cut("passfail==passfail::fail"));                 
  total_fit.plotOn(frame4,Slice(passprobe_cat,"fail"),ProjWData(passprobe_cat,*hdata)) ;
  comb_mc->plotOn(frame4,Cut("passfail==passfail::fail")) ;
  b_fp->plotOn(frame4,MarkerColor(kRed), LineWidth(0));
  l_fp->plotOn(frame4,MarkerColor(kBlue), LineWidth(0));

    
  TLegend * leg = new TLegend(0.4,0.6,0.89,0.89);
  leg->AddEntry(frame1->findObject("h_b_pp_Cut[passfail==passfail::fail]"), "MC t#bar{t}+Other backgrounds", "lp");
  leg->AddEntry(frame1->findObject("h_b_fp"), "MC b-jet", "lp");
  leg->AddEntry(frame1->findObject("h_l_fp"), "MC non-b-jets", "lp");
  //leg->AddEntry(frame1->findObject("model_fp_Norm[jetptprobe]"), "total fit", "l");
  //leg->AddEntry(frame1->findObject("model_fp_Norm[jetptprobe]_Comp[pdf_b_fp]"), "b-jet fit", "l");
  //leg->AddEntry(frame1->findObject("model_fp_Norm[jetptprobe]_Comp[pdf_l_fp]"), "non-b-jet fit", "l");   
  //leg->SetFillColor(0);
  //leg->SetFillStyle(4000);
  //leg->SetBorderSize(0);

  TCanvas* c = new TCanvas("rf501_simultaneouspdf","rf403_simultaneouspdf",800,800) ;
  c->Divide(2,2) ;
  c->cd(1) ; gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.4) ; frame->Draw() ;
  c->cd(2) ; gPad->SetLeftMargin(0.15) ; frame1->GetYaxis()->SetTitleOffset(1.4) ; frame1->Draw(); leg->Draw("same");
  c->cd(3) ; gPad->SetLeftMargin(0.15) ; frame3->GetYaxis()->SetTitleOffset(1.4) ; frame3->Draw(); 
  c->cd(4) ; gPad->SetLeftMargin(0.15) ; frame4->GetYaxis()->SetTitleOffset(1.4) ; frame4->Draw(); 
  fileout->cd();
  c->Write();
  fitresult->Write();
  cout << "################################################" << endl;
  cout << "True Efficiency_s = " << effs_true->getValV() << " +- " << effs_true->getError() << endl;
  cout << "True Efficiency_b = " << effb_true->getValV() << " +- " << effb_true->getError() << endl;
  cout << "################################################" << endl;



  return 0;
  
}
