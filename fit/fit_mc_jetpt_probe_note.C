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

using namespace std;

using namespace RooFit;

//-------------------------------------------------------------------------

int fit_mc_jetptprobe(int ntoys=0) {

  double nbins = 10;
  //int ntoys=1;

  //TCanvas* c1 = new TCanvas();
// Set a bunch of parameters to make the plot look nice

 // c1->SetFillColor(0);
  //c1->UseCurrentStyle();
 // c1->SetBorderMode(0);       // still leaves red frame bottom and right
 // c1->SetFrameBorderMode(0);    // need this to turn off red hist frame!
  gROOT->SetStyle("Plain");
 // c1->UseCurrentStyle();
  gROOT->ForceStyle();

  gStyle->SetOptStat(0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleSize(0.04);
  gStyle->SetTitleFont(42, "hxy");      // for histogram and axis titles
  gStyle->SetLabelFont(42, "xyz");      // for axis labels (values)
  gROOT->ForceStyle();
  
  // Read in the data.  

  TString dir = "/afs/cern.ch/work/l/lviliani/TagAndProbeRunII/CMSSW_7_5_4/src/TagAndProbe/scripts/";
  TString mll_cut = "mth>0";

  //TFile* filein = new TFile(dir+"tandpOF_loose.root"); 
  TFile* filein = new TFile(dir+"tandp_mediumT_looseP.root");
  TFile* fileout = new TFile("mc_jetptprobe.root", "RECREATE");
  /*
  // Read in the ttbar MC
  TFile* ttbarFile_pp = new TFile(dir+"ttbar_pp.root");
  TFile* ttbarFile_fp = new TFile(dir+"ttbar_fp.root");
    
  TTree* tree_ttbar_pp = (TTree*)ttbarFile_pp->Get("TTbar");
  TTree* tree_ttbar_fp = (TTree*)ttbarFile_fp->Get("TTbar");
  
  // Read in the other MC bkgs

  TFile* bkgFile_pp = new TFile(dir+"bkg_pp.root");
  TFile* bkgFile_fp = new TFile(dir+"bkg_fp.root");
  
  TTree* tree_bkg_pp = (TTree*)bkgFile_pp->Get("OtherBkg");
  TTree* tree_bkg_fp = (TTree*)bkgFile_fp->Get("OtherBkg");
  */

  TTree* tree_ttbar = (TTree*) filein->Get("TTbar");
  TTree* tree_bkg = (TTree*) filein->Get("OtherBkg");

  RooCategory passprobe_cat("passfail","passfail");
  
  passprobe_cat.defineType("pass",  1);
  passprobe_cat.defineType("fail",  0);

  RooRealVar isb("isb", "isb", -1, 1);
  
  // Define the model

  RooRealVar jetptprobe("jetptprobe", "jetptprobe", 30, 100);
  jetptprobe.setBins(nbins) ;
  RooRealVar weight("weight", "weight", -1000, 1000);  
  RooRealVar mll("mll", "mll", 0, 500);
  RooRealVar mth("mth", "mth", 0, 500);
  

  RooRealVar efficiency_s("efficiency_s", "efficiency_s", 0, 1);
  RooRealVar efficiency_b("efficiency_b", "efficiency_b", 0, 1); 
  RooRealVar s("s","signal yield",0,1000000);
  RooRealVar b("b","background yield",0,500000);
  
  RooFormulaVar s_pass("s_pass","s*efficiency_s", RooArgList(s,efficiency_s));
  RooFormulaVar b_pass("b_pass","b*efficiency_b", RooArgList(b,efficiency_b));
  RooFormulaVar s_fail("s_fail","s*(1-efficiency_s)", RooArgList(s,efficiency_s));
  RooFormulaVar b_fail("b_fail","b*(1-efficiency_b)", RooArgList(b,efficiency_b)); 


  // Import TTbar and Bkg datasets
  
  RooDataSet* ttbar_pp = new RooDataSet("ttbar_pp", "ttbar_pp", RooArgSet(jetptprobe, weight, mth, passprobe_cat, isb), Import(*tree_ttbar), WeightVar(weight), Cut(mll_cut+" && passfail==1 && isb==1"));
  RooDataSet* ttbar_fp = new RooDataSet("ttbar_fp", "ttbar_fp", RooArgSet(jetptprobe, weight, mth, passprobe_cat, isb), Import(*tree_ttbar), WeightVar(weight), Cut(mll_cut+" && passfail==0 && isb==1"));
  RooDataSet* otherbkg_pp = new RooDataSet("otherbkg_pp", "otherbkg_pp", RooArgSet(jetptprobe, weight, mth, passprobe_cat, isb), Import(*tree_bkg), WeightVar(weight), Cut(mll_cut+" && passfail==1 && isb==0"));
  RooDataSet* otherbkg_fp = new RooDataSet("otherbkg_fp", "otherbkg_fp", RooArgSet(jetptprobe, weight, mth, passprobe_cat, isb), Import(*tree_bkg), WeightVar(weight), Cut(mll_cut+" && passfail==0 && isb==0"));
  
  RooDataSet* otherbkg_pp2 = new RooDataSet("otherbkg_pp2", "otherbkg_pp2", RooArgSet(jetptprobe, weight, mth, passprobe_cat, isb), Import(*tree_ttbar), WeightVar(weight), Cut(mll_cut+" && passfail==1 && isb==0"));
  RooDataSet* otherbkg_fp2 = new RooDataSet("otherbkg_fp2", "otherbkg_fp2", RooArgSet(jetptprobe, weight, mth, passprobe_cat, isb), Import(*tree_ttbar), WeightVar(weight), Cut(mll_cut+" && passfail==0 && isb==0"));
  
  otherbkg_pp->append(*otherbkg_pp2);
  otherbkg_fp->append(*otherbkg_fp2);
  

  Double_t entries_tt_pp = ttbar_pp->sumEntries();
  cout << "entries_tt_pp " << entries_tt_pp << endl;  
  Double_t entries_tt_fp = ttbar_fp->sumEntries();
  Double_t entries_bkg_pp = otherbkg_pp->sumEntries();
  Double_t entries_bkg_fp = otherbkg_fp->sumEntries();

  Double_t tot_events = entries_tt_pp+entries_tt_fp+entries_bkg_pp+entries_bkg_fp; 

  
   
  RooDataHist * dttbar_pp = new RooDataHist("dttbar_pp","ttbar_pp",RooArgSet(jetptprobe),*ttbar_pp) ;
  RooDataHist * dttbar_fp = new RooDataHist("dttbar_fp","ttbar_fp",RooArgSet(jetptprobe),*ttbar_fp) ;
  RooDataHist * dotherbkg_pp = new RooDataHist("dotherbkg_pp","otherbkg_pp",RooArgSet(jetptprobe),*otherbkg_pp) ;
  RooDataHist * dotherbkg_fp = new RooDataHist("dotherbkg_fp","otherbkg_fp",RooArgSet(jetptprobe),*otherbkg_fp) ;
  

  RooHistPdf pdf_ttbar_pp("pdf_ttbar_pp","ttbar_pp",RooArgSet(jetptprobe),*dttbar_pp);
  RooHistPdf pdf_ttbar_fp("pdf_ttbar_fp","ttbar_fp",RooArgSet(jetptprobe),*dttbar_fp);
  RooHistPdf pdf_otherbkg_pp("pdf_otherbkg_pp","otherbkg_pp",RooArgSet(jetptprobe),*dotherbkg_pp);
  RooHistPdf pdf_otherbkg_fp("pdf_otherbkg_fp","otherbkg_fp",RooArgSet(jetptprobe),*dotherbkg_fp);
  
  RooAddPdf model_pp("model_pp", "Model PP", RooArgList(pdf_ttbar_pp, pdf_otherbkg_pp), RooArgList(s_pass, b_pass));
  RooAddPdf model_fp("model_fp", "Model FP", RooArgList(pdf_ttbar_fp, pdf_otherbkg_fp), RooArgList(s_fail, b_fail));


  // Use MC templates as Data

  RooDataSet* comb_mc_tt= (RooDataSet*)ttbar_pp->Clone();
  comb_mc_tt->append(*ttbar_fp);
  RooDataHist d_model_tt("d_model_tt", "d_model_tt", RooArgSet(jetptprobe, passprobe_cat), *comb_mc_tt);
  d_model_tt.Print();
  RooHistPdf model_tt("model_tt", "model_tt", RooArgSet(jetptprobe, passprobe_cat), d_model_tt);

  TCanvas * stapuppa = new TCanvas();
  stapuppa->cd();
  RooPlot* cicia = jetptprobe.frame();
  //d_model_tt.plotOn(cicia);
  model_tt.plotOn(cicia);
  cicia->Draw();


  RooDataSet* comb_mc_otherbkg= (RooDataSet*)otherbkg_pp->Clone();
  comb_mc_otherbkg->append(*otherbkg_fp);
  RooDataHist d_model_otherbkg("d_model_otherbkg", "d_model_otherbkg", RooArgSet(jetptprobe, passprobe_cat), *comb_mc_otherbkg);
  RooHistPdf model_otherbkg("model_otherbkg", "model_otherbkg", RooArgSet(jetptprobe, passprobe_cat), d_model_otherbkg);

  TCanvas * stapuppa2 = new TCanvas();
  stapuppa2->cd();
  RooPlot* cicia2 = jetptprobe.frame();
  //d_model_tt.plotOn(cicia);
  //model_otherbkg.plotOn(cicia2, Slice(passprobe_cat,"pass"));
  //cicia2->Draw();  

  RooDataSet* comb_mc= (RooDataSet*)comb_mc_tt->Clone();
  comb_mc->append(*comb_mc_otherbkg);
 
  //RooDataSet combMC("combMC","combined MC", RooArgSet(jetptprobe,weight, passprobe_cat),Import(*comb_mc),WeightVar(weight)) ;


  // Create the simultaneous fit
  RooSimultaneous total_fit("total_fit","total_fit", passprobe_cat);
  passprobe_cat.setLabel("pass");
  total_fit.addPdf(model_pp, passprobe_cat.getLabel());
  passprobe_cat.setLabel("fail");
  total_fit.addPdf(model_fp, passprobe_cat.getLabel());

  RooDataHist hdata("hdata", "hdata", RooArgSet(jetptprobe, passprobe_cat), *comb_mc);
  RooChi2Var chi2("chi2","chi2",total_fit,hdata, Extended(), DataError(1));
  RooMinuit m2(chi2) ;
  //m2.migrad() ;
  //m2.hesse() ;
  RooFitResult* fitresult = m2.fit("r");//m2.save() ;
  
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

  for (int i=0; i<ntoys; i++){

    std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$ TOY " << i << " $$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
    cout << "tt  entries: " << entries_tt_pp+entries_tt_fp << endl;
    cout << "bkg entries: " << entries_bkg_pp+entries_bkg_fp << endl;
     
    RooDataSet* toy_tt = model_tt.generate(RooArgSet(jetptprobe, passprobe_cat), NumEvents(int(entries_tt_pp+entries_tt_fp)), AutoBinned(false), Extended());
    ttbar_pp = (RooDataSet*) toy_tt->reduce("passfail==passfail::pass");
    ttbar_pp->SetName("ttbar_pp");
    ttbar_fp = (RooDataSet*) toy_tt->reduce("passfail==passfail::fail");
    ttbar_fp->SetName("ttbar_fp");
    RooDataSet* toy_otherbkg = model_otherbkg.generate(RooArgSet(jetptprobe, passprobe_cat), NumEvents(int(entries_bkg_pp+entries_bkg_fp)), AutoBinned(false), Extended());
    otherbkg_pp = (RooDataSet*) toy_otherbkg->reduce("passfail==passfail::pass");
    otherbkg_pp->SetName("otherbkg_pp"); 
    otherbkg_fp = (RooDataSet*) toy_otherbkg->reduce("passfail==passfail::fail");
    otherbkg_fp->SetName("otherbkg_fp"); 
    RooDataSet* toy_comb = (RooDataSet*) toy_tt->Clone();
    toy_comb->append(*toy_otherbkg);
    comb_mc = toy_comb;
    comb_mc->SetName("ttbar_pp");

    RooDataHist * htoy= new RooDataHist("toy", "toy", RooArgSet(jetptprobe, passprobe_cat), *toy_comb);
    htoy->plotOn(cicia2);
    cicia2->Draw();
    RooChi2Var chi2toy("chi2toy","chi2toy",total_fit,*htoy, Extended());
    RooMinuit m2toy(chi2toy) ;
    RooFitResult* frSB = m2toy.fit("r");

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
    if (i != ntoys-1){
      delete toy_tt;
      delete toy_otherbkg;
      delete toy_comb;
      delete htoy;
      delete frSB;
      delete ttbar_pp;
      delete ttbar_fp;
      delete otherbkg_pp;
      delete otherbkg_fp;

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
  
  RooPlot* frame = jetptprobe.frame(Title("tag-pass-probe"),Bins(nbins),Range(30,100,1));
  RooPlot* frame1 = jetptprobe.frame(Title("tag-fail-probe"),Bins(nbins),Range(30,100,1));

  comb_mc->plotOn(frame,Cut("passfail==passfail::pass")) ;
  total_fit.paramOn(frame,RooArgSet(s,b,efficiency_s,efficiency_b));
//  mc.genData(0)->plotOn(frame,Cut("passprobe==passprobe::pass")) ;
  total_fit.plotOn(frame,Slice(passprobe_cat,"pass"),ProjWData(passprobe_cat,*comb_mc)) ;
  total_fit.plotOn(frame,Slice(passprobe_cat,"pass"),Components("pdf_otherbkg_pp"),ProjWData(passprobe_cat,*comb_mc),LineStyle(kDashed)) ;
  total_fit.plotOn(frame,Slice(passprobe_cat,"pass"),Components("pdf_ttbar_pp"),ProjWData(passprobe_cat,*comb_mc),LineStyle(kDashed),LineColor(kRed));
  comb_mc->plotOn(frame,Cut("passfail==passfail::pass")) ;
  ttbar_pp->plotOn(frame,MarkerColor(kRed), LineWidth(0));
  otherbkg_pp->plotOn(frame,MarkerColor(kBlue), LineWidth(0));

  //total_fit.paramOn(frame1,RooArgSet(s,b,efficiency_s,efficiency_b));
//  mc.genData(0)->plotOn(frame1,Cut("passfail==passfail::fail")) ;
  comb_mc->plotOn(frame1,Cut("passfail==passfail::fail")) ;
  total_fit.plotOn(frame1,Slice(passprobe_cat,"fail"),ProjWData(passprobe_cat,*comb_mc)) ;
  total_fit.plotOn(frame1,Slice(passprobe_cat,"fail"),Components("pdf_otherbkg_fp"),ProjWData(passprobe_cat,*comb_mc),LineStyle(kDashed)) ;
  total_fit.plotOn(frame1,Slice(passprobe_cat,"fail"),Components("pdf_ttbar_fp"),ProjWData(passprobe_cat,*comb_mc),LineStyle(kDashed),LineColor(kRed)) ;
  comb_mc->plotOn(frame1,Cut("passfail==passfail::fail")) ;
  ttbar_fp->plotOn(frame1,MarkerColor(kRed), LineWidth(0));
  otherbkg_fp->plotOn(frame1,MarkerColor(kBlue), LineWidth(0));
  frame1->Print();
    
  TLegend * leg = new TLegend(0.4,0.6,0.89,0.89);
  leg->AddEntry(frame1->findObject("h_ttbar_pp_Cut[passfail==passfail::fail]"), "MC t#bar{t}+Other backgrounds", "lp");
  leg->AddEntry(frame1->findObject("h_ttbar_fp"), "MC t#bar{t}", "lp");
  leg->AddEntry(frame1->findObject("h_otherbkg_fp"), "MC Other backgrounds", "lp");
  leg->AddEntry(frame1->findObject("model_fp_Norm[jetptprobe]"), "total fit", "l");
  leg->AddEntry(frame1->findObject("model_fp_Norm[jetptprobe]_Comp[pdf_ttbar_fp]"), "t#bar{t} fit", "l");
  leg->AddEntry(frame1->findObject("model_fp_Norm[jetptprobe]_Comp[pdf_otherbkg_fp]"), "Other background fit", "l");   
  //leg->SetFillColor(0);
  //leg->SetFillStyle(4000);
  //leg->SetBorderSize(0);

  TCanvas* c = new TCanvas("rf501_simultaneouspdf","rf403_simultaneouspdf",800,400) ;
  c->Divide(2) ;
  c->cd(1) ; gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.4) ; frame->Draw() ;
  c->cd(2) ; gPad->SetLeftMargin(0.15) ; frame1->GetYaxis()->SetTitleOffset(1.4) ; frame1->Draw(); leg->Draw("same");
  fileout->cd();
  c->Write();
  return 0;
}
