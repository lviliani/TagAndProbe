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
#include <TMarker.h>
#include "TLegend.h"

#include <RooMinuit.h>
#include <RooChi2Var.h>
#include <RooRealVar.h>
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

int fit_data_jetptprobe() {

  double nbins = 10;

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
  TString dir = "../scripts/";
  TString mll_cut = "jetptprobe < 30";

  //TFile* filein = new TFile(dir+"tandpOF_loose.root"); 
  TFile* fileinData = new TFile(dir+"tandp_mediumT_looseP_loose.root");
  TFile* fileinMC = new TFile(dir+"tandp_mediumT_looseP_loose.root");
  TFile* fileout = new TFile("data_jetptprobe_0jet.root", "RECREATE");
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

  TTree* tree_data = (TTree*) fileinData->Get("Data");
  TTree* tree_ttbar = (TTree*) fileinMC->Get("TTbar");
  TTree* tree_bkg = (TTree*) fileinMC->Get("OtherBkg");

  RooCategory passprobe_cat("passfail","passfail");
  
  passprobe_cat.defineType("pass",  1);
  passprobe_cat.defineType("fail",  0);

  RooRealVar isb("isb", "isb", -1, 1);
  
  // Define the model
  RooRealVar jetptprobe("jetptprobe", "jetptprobe", 15, 30);
  //jetptprobe.setBins(nbins) ;
  double binning_a[4] = {15, 20, 25, 30};
  RooBinning binning(3, binning_a);
  jetptprobe.setBinning(binning) ;
  RooRealVar weight("weight", "weight", -1000, 1000);
  RooRealVar mll("mll", "mll", 0, 500);
  RooRealVar mth("mth", "mth", 0, 500); 

  RooRealVar efficiency_s("efficiency_s", "efficiency_s", 0.60, 0, 1);
  RooRealVar efficiency_b("efficiency_b", "efficiency_b", 0.03, 0, 1); 
  RooRealVar s("s","signal yield",500,10000);
  RooRealVar b("b","background yield",1000,10000);
  
  RooFormulaVar s_pass("s_pass","s*efficiency_s", RooArgList(s,efficiency_s));
  RooFormulaVar b_pass("b_pass","b*efficiency_b", RooArgList(b,efficiency_b));
  RooFormulaVar s_fail("s_fail","s*(1-efficiency_s)", RooArgList(s,efficiency_s));
  RooFormulaVar b_fail("b_fail","b*(1-efficiency_b)", RooArgList(b,efficiency_b)); 

  RooDataSet* data = new RooDataSet("data", "data",  RooArgSet(jetptprobe, weight, mll, passprobe_cat, isb), Import(*tree_data), WeightVar(weight), Cut(mll_cut+" && isb==0"));

  // Import TTbar and Bkg datasets
  //signal: the probe is a b
  RooDataSet* ttbar_pp = new RooDataSet("ttbar_pp", "ttbar_pp", RooArgSet(jetptprobe, weight, mll, passprobe_cat, isb), Import(*tree_ttbar), WeightVar(weight), Cut(mll_cut+" && passfail==1 && isb==1"));
  RooDataSet* ttbar_fp = new RooDataSet("ttbar_fp", "ttbar_fp", RooArgSet(jetptprobe, weight, mll, passprobe_cat, isb), Import(*tree_ttbar), WeightVar(weight), Cut(mll_cut+" && passfail==0 && isb==1"));

  RooDataSet* ttbar_pp2 = new RooDataSet("ttbar_pp2", "ttbar_pp", RooArgSet(jetptprobe, weight, mll, passprobe_cat, isb), Import(*tree_bkg), WeightVar(weight), Cut(mll_cut+" && passfail==1 && isb==1"));
  RooDataSet* ttbar_fp2 = new RooDataSet("ttbar_fp2", "ttbar_fp", RooArgSet(jetptprobe, weight, mll, passprobe_cat, isb), Import(*tree_bkg), WeightVar(weight), Cut(mll_cut+" && passfail==0 && isb==1"));

  ttbar_pp->append(*ttbar_pp2);
  ttbar_fp->append(*ttbar_fp2);
  
  //background: the probe is not a b
  RooDataSet* otherbkg_pp = new RooDataSet("otherbkg_pp", "otherbkg_pp", RooArgSet(jetptprobe, weight, mll, passprobe_cat, isb), Import(*tree_bkg), WeightVar(weight), Cut(mll_cut+" && passfail==1 && isb==0"));
  RooDataSet* otherbkg_fp = new RooDataSet("otherbkg_fp", "otherbkg_fp", RooArgSet(jetptprobe, weight, mll, passprobe_cat, isb), Import(*tree_bkg), WeightVar(weight), Cut(mll_cut+" && passfail==0 && isb==0"));
 
  RooDataSet* otherbkg_pp2 = new RooDataSet("otherbkg_pp2", "otherbkg_pp2", RooArgSet(jetptprobe, weight, mll, passprobe_cat, isb), Import(*tree_ttbar), WeightVar(weight), Cut(mll_cut+" && passfail==1 && isb==0"));
  RooDataSet* otherbkg_fp2 = new RooDataSet("otherbkg_fp2", "otherbkg_fp2", RooArgSet(jetptprobe, weight, mll, passprobe_cat, isb), Import(*tree_ttbar), WeightVar(weight), Cut(mll_cut+" && passfail==0 && isb==0"));
 
  otherbkg_pp->append(*otherbkg_pp2);
  otherbkg_fp->append(*otherbkg_fp2);  
/*
  Double_t entries_tt_pp = ttbar_pp->sumEntries();
  Double_t entries_tt_fp = ttbar_fp->sumEntries();
  Double_t entries_bkg_pp = otherbkg_pp->sumEntries();
  Double_t entries_bkg_fp = otherbkg_fp->sumEntries();

  Double_t tot_events = entries_tt_pp+entries_tt_fp+entries_bkg_pp+entries_bkg_fp; 
*/
  
   
  RooDataHist dttbar_pp("dttbar_pp","ttbar_pp",RooArgSet(jetptprobe),*ttbar_pp) ;
  RooDataHist dttbar_fp("dttbar_fp","ttbar_fp",RooArgSet(jetptprobe),*ttbar_fp) ;
  RooDataHist dotherbkg_pp("dotherbkg_pp","otherbkg_pp",RooArgSet(jetptprobe),*otherbkg_pp) ;
  RooDataHist dotherbkg_fp("dotherbkg_fp","otherbkg_fp",RooArgSet(jetptprobe),*otherbkg_fp) ;
  

  RooHistPdf pdf_ttbar_pp("pdf_ttbar_pp","ttbar_pp",RooArgSet(jetptprobe),dttbar_pp);
  RooHistPdf pdf_ttbar_fp("pdf_ttbar_fp","ttbar_fp",RooArgSet(jetptprobe),dttbar_fp);
  RooHistPdf pdf_otherbkg_pp("pdf_otherbkg_pp","otherbkg_pp",RooArgSet(jetptprobe),dotherbkg_pp);
  RooHistPdf pdf_otherbkg_fp("pdf_otherbkg_fp","otherbkg_fp",RooArgSet(jetptprobe),dotherbkg_fp);
  
  RooAddPdf model_pp("model_pp", "Model PP", RooArgList(pdf_ttbar_pp, pdf_otherbkg_pp), RooArgList(s_pass, b_pass));
  RooAddPdf model_fp("model_fp", "Model FP", RooArgList(pdf_ttbar_fp, pdf_otherbkg_fp), RooArgList(s_fail, b_fail));




  // Create the simultaneous fit
  RooSimultaneous total_fit("total_fit","total_fit", passprobe_cat);
  passprobe_cat.setLabel("pass");
  total_fit.addPdf(model_pp, passprobe_cat.getLabel());
  passprobe_cat.setLabel("fail");
  total_fit.addPdf(model_fp, passprobe_cat.getLabel());


  RooDataHist hdata("hdata", "hdata", RooArgSet(jetptprobe, passprobe_cat), *data);
  RooChi2Var chi2("chi2","chi2",total_fit,hdata, Extended(), DataError(1));
  RooMinuit m2(chi2) ;
  //m2.migrad() ;
  //m2.hesse() ;
  RooFitResult* fitresult = total_fit.fitTo(hdata, Extended(), Save(true));//m2.fit("r");//m2.save() ; 
  
  // Fit to MC templates
  //RooFitResult* fitresult = total_fit.fitTo(hdata,  RooFit::Save(true),  RooFit::PrintLevel(3), RooFit::Strategy(1), RooFit::NumCPU(6),  RooFit::Extended(kTRUE), RooFit::SumW2Error(true));

  /*RooChi2Var chi2("chi2","chi2",total_fit,hdata);
  RooMinuit m2(chi2) ;
  m2.migrad() ;
  m2.hesse() ;
  RooFitResult* fitresult = m2.save() ;
  */
/*
  // Create a Toy MC and fit again
  RooMCStudy mc(total_fit, RooArgSet(jetptprobe,passprobe_cat), FitModel(total_fit), Extended(true), FitOptions(Save(true),Minos(RooArgSet(efficiency_s))), FitOptions(NumCPU(4,0)));
  mc.generateAndFit(ntoys, tot_events, true);


  TH1D* h_pull_effs = new TH1D("pull_effs","pull_effs",100,-3,3);

  RooRealVar* effs_true = (RooRealVar*)fitresult->floatParsFinal().find("efficiency_s");

  for (int i=0; i<ntoys; i++){
    const RooFitResult* frSB = mc.fitResult(i);
    if (frSB->status()==0 && frSB->covQual()==3) {
      RooRealVar* effs_res = (RooRealVar*)frSB->floatParsFinal().find("efficiency_s");
      double pull_effs = (effs_res->getValV() - effs_true->getValV())/effs_res->getError();
//      cout << "Efficiency s = " << efficiency_s.getValV() << "+-" << efficiency_s.getError() << endl;
      h_pull_effs->Fill(pull_effs);       
    }
  }

  cout << "################################################" << endl;
  cout << "True Efficiency_s = " << effs_true->getValV() << " +- " << effs_true->getError() << endl;
  cout << "################################################" << endl;


  // plot results

  TCanvas* c_pull = new TCanvas();
  c_pull->cd();
  h_pull_effs->Draw();
*/
  RooPlot* frame = jetptprobe.frame(Title("tag-pass-probe"),Binning(binning));
  RooPlot* frame1 = jetptprobe.frame(Title("tag-fail-probe"),Binning(binning));

  total_fit.paramOn(frame,RooArgSet(s,b,efficiency_s,efficiency_b));
//  mc.genData(0)->plotOn(frame,Cut("passprobe==passprobe::pass")) ;
  data->plotOn(frame,Cut("passfail==passfail::pass"), Binning(binning)) ;
  total_fit.plotOn(frame,Slice(passprobe_cat,"pass"),ProjWData(passprobe_cat,*data), Binning(binning)) ;
  total_fit.plotOn(frame,Slice(passprobe_cat,"pass"),Components("pdf_otherbkg_pp"),ProjWData(passprobe_cat,*data),LineStyle(kDashed), Binning(binning)) ;
  total_fit.plotOn(frame,Slice(passprobe_cat,"pass"),Components("pdf_ttbar_pp"),ProjWData(passprobe_cat,*data),LineStyle(kDashed),LineColor(kRed), Binning(binning));
  data->plotOn(frame,Cut("passfail==passfail::pass"), Binning(binning)) ;

  //total_fit.paramOn(frame1,RooArgSet(s,b,efficiency_s,efficiency_b));
//  mc.genData(0)->plotOn(frame1,Cut("passfail==passfail::fail")) ;
  data->plotOn(frame1,Cut("passfail==passfail::fail"), Binning(binning)) ;
  total_fit.plotOn(frame1,Slice(passprobe_cat,"fail"),ProjWData(passprobe_cat,*data), Binning(binning)) ;
  total_fit.plotOn(frame1,Slice(passprobe_cat,"fail"),Components("pdf_otherbkg_fp"),ProjWData(passprobe_cat,*data),LineStyle(kDashed), Binning(binning)) ;
  total_fit.plotOn(frame1,Slice(passprobe_cat,"fail"),Components("pdf_ttbar_fp"),ProjWData(passprobe_cat,*data),LineStyle(kDashed),LineColor(kRed), Binning(binning)) ;
  data->plotOn(frame1,Cut("passfail==passfail::fail"), Binning(binning)) ;
  frame1->Print();
  TLegend * leg = new TLegend(0.4,0.6,0.89,0.89);
  leg->AddEntry(frame1->findObject("h_data_Cut[passfail==passfail::fail]"), "Data", "lp");
  leg->AddEntry(frame1->findObject("model_fp_Norm[jetptprobe]"), "total fit", "l");
  leg->AddEntry(frame1->findObject("model_fp_Norm[jetptprobe]_Comp[pdf_ttbar_fp]"), "t#bar{t} fit", "l");
  leg->AddEntry(frame1->findObject("model_fp_Norm[jetptprobe]_Comp[pdf_otherbkg_fp]"), "Other background fit", "l");
  

  TCanvas* c = new TCanvas("rf501_simultaneouspdf","rf403_simultaneouspdf",800,400) ;
  c->Divide(2) ;
  c->cd(1) ; gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.4) ; frame->Draw() ;
  c->cd(2) ; gPad->SetLeftMargin(0.15) ; frame1->GetYaxis()->SetTitleOffset(1.4) ; frame1->Draw(); leg->Draw("same");
  fileout->cd();
  c->Write();
  return 0;
}
