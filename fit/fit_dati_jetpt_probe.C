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

using namespace std;

using namespace RooFit;

//-------------------------------------------------------------------------

int fit_dati_jetpt_probe() {


  double nbins=20;

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

  TString dir = "/home/lviliani/Documenti/TagAndProbe/looseTag_volta_buona_davvero_davvero/";
  TString mll_cut = "mll>0";

  TFile* dataFile_pp = new TFile(dir+"data_pp.root");
  TFile* dataFile_fp = new TFile(dir+"data_fp.root");
  
  TTree* tree_data_pp = (TTree*)dataFile_pp->Get("Data");
  TTree* tree_data_fp = (TTree*)dataFile_fp->Get("Data");
  
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

 
  RooCategory passprobe_cat("passprobe","passprobe");
  
  passprobe_cat.defineType("pass",  1);
  passprobe_cat.defineType("fail",  0);
  
  // Define the model

  RooRealVar jetpt_probe("jetpt_probe", "jetpt_probe", 30, 100);
  RooRealVar weight("weight", "weight", -1000, 1000);  
  RooRealVar mll("mll", "mll", 0, 500);
  
  RooRealVar efficiency_s("efficiency_s", "efficiency_s", 0, 1);
  RooRealVar efficiency_b("efficiency_b", "efficiency_b", 0, 1); 
  RooRealVar s("s","signal yield",0,40000);
  RooRealVar b("b","background yield", 0,20000);
  
  RooFormulaVar s_pass("s_pass","s*efficiency_s", RooArgList(s,efficiency_s));
  RooFormulaVar b_pass("b_pass","b*efficiency_b", RooArgList(b,efficiency_b));
  RooFormulaVar s_fail("s_fail","s*(1-efficiency_s)", RooArgList(s,efficiency_s));
  RooFormulaVar b_fail("b_fail","b*(1-efficiency_b)", RooArgList(b,efficiency_b)); 
  

  // Import DATA  
  RooDataSet* data_pp = new RooDataSet("data_pp","data_pp",RooArgSet(jetpt_probe,mll), Cut(mll_cut), Import(*tree_data_pp));
  RooDataSet* data_fp = new RooDataSet("data_fp","data_fp",RooArgSet(jetpt_probe,mll), Cut(mll_cut), Import(*tree_data_fp));
 
  // Combine PassProbe and FailProbe datasets
  RooDataSet combData("combData","combined data", jetpt_probe,Index(passprobe_cat),Import("pass",*data_pp),Import("fail",*data_fp)) ;
 

  // Import TTbar and Bkg datasets
  RooDataSet* ttbar_pp = new RooDataSet("ttbar_pp", "ttbar_pp", RooArgSet(jetpt_probe, mll, weight), Cut(mll_cut), Import(*tree_ttbar_pp), WeightVar(weight));
  RooDataSet* ttbar_fp = new RooDataSet("ttbar_fp", "ttbar_fp", RooArgSet(jetpt_probe, mll, weight), Cut(mll_cut), Import(*tree_ttbar_fp), WeightVar(weight));
  
  RooDataSet* otherbkg_pp = new RooDataSet("otherbkg_pp", "otherbkg_pp", RooArgSet(jetpt_probe, mll, weight), Cut(mll_cut), Import(*tree_bkg_pp), WeightVar(weight));
  RooDataSet* otherbkg_fp = new RooDataSet("otherbkg_fp", "otherbkg_fp", RooArgSet(jetpt_probe, mll, weight), Cut(mll_cut), Import(*tree_bkg_fp), WeightVar(weight));

  
  jetpt_probe.setBins(nbins) ;

  RooDataHist dttbar_pp("dttbar_pp","ttbar_pp",RooArgSet(jetpt_probe),*ttbar_pp) ;
  RooDataHist dttbar_fp("dttbar_fp","ttbar_fp",RooArgSet(jetpt_probe),*ttbar_fp) ;
  RooDataHist dotherbkg_pp("dotherbkg_pp","otherbkg_pp",RooArgSet(jetpt_probe),*otherbkg_pp) ;
  RooDataHist dotherbkg_fp("dotherbkg_fp","otherbkg_fp",RooArgSet(jetpt_probe),*otherbkg_fp) ;


  RooHistPdf pdf_ttbar_pp("pdf_ttbar_pp","ttbar_pp",RooArgSet(jetpt_probe),dttbar_pp);
  RooHistPdf pdf_ttbar_fp("pdf_ttbar_fp","ttbar_fp",RooArgSet(jetpt_probe),dttbar_fp);
  RooHistPdf pdf_otherbkg_pp("pdf_otherbkg_pp","otherbkg_pp",RooArgSet(jetpt_probe),dotherbkg_pp);
  RooHistPdf pdf_otherbkg_fp("pdf_otherbkg_fp","otherbkg_fp",RooArgSet(jetpt_probe),dotherbkg_fp);


  RooAddPdf model_pp("model_pp", "Model PP", RooArgList(pdf_ttbar_pp, pdf_otherbkg_pp), RooArgList(s_pass, b_pass));
  RooAddPdf model_fp("model_fp", "Model FP", RooArgList(pdf_ttbar_fp, pdf_otherbkg_fp), RooArgList(s_fail, b_fail));


  RooSimultaneous total_fit("total_fit","total_fit", passprobe_cat);
  passprobe_cat.setLabel("pass");
  total_fit.addPdf(model_pp, passprobe_cat.getLabel());
  passprobe_cat.setLabel("fail");
  total_fit.addPdf(model_fp, passprobe_cat.getLabel());

  RooFitResult* fitresult = total_fit.fitTo(combData, RooFit::Minos(RooArgSet(efficiency_s)), RooFit::Save(true),  RooFit::PrintLevel(3), RooFit::Strategy(1), RooFit::NumCPU(6),  RooFit::Extended(kTRUE), RooFit::SumW2Error(true));


  // plot results

  fitresult->SetName("Fit Results");

  fitresult->Write(); 

  RooPlot* frame = jetpt_probe.frame(Title("Pass Probe Events"),Bins(nbins),Range(30,100,1));
  RooPlot* frame1 = jetpt_probe.frame(Title("Fail Probe Events"),Bins(nbins),Range(30,100,1));

  total_fit.paramOn(frame,RooArgSet(s,b,efficiency_s,efficiency_b));
  total_fit.paramOn(frame1,RooArgSet(s,b,efficiency_s,efficiency_b));
 
  combData.plotOn(frame,Cut("passprobe==passprobe::pass")) ;
  total_fit.plotOn(frame,Slice(passprobe_cat,"pass"),ProjWData(passprobe_cat,combData)) ;
  total_fit.plotOn(frame,Slice(passprobe_cat,"pass"),Components("pdf_otherbkg_pp"),ProjWData(passprobe_cat,combData),LineStyle(kDashed)) ;
  total_fit.plotOn(frame,Slice(passprobe_cat,"pass"),Components("pdf_ttbar_pp"),ProjWData(passprobe_cat,combData),LineStyle(kDashed),LineColor(kRed));
  
  combData.plotOn(frame1,Cut("passprobe==passprobe::fail")) ;
  total_fit.plotOn(frame1,Slice(passprobe_cat,"fail"),ProjWData(passprobe_cat,combData)) ;
  total_fit.plotOn(frame1,Slice(passprobe_cat,"fail"),Components("pdf_otherbkg_fp"),ProjWData(passprobe_cat,combData),LineStyle(kDashed)) ;
  total_fit.plotOn(frame1,Slice(passprobe_cat,"fail"),Components("pdf_ttbar_fp"),ProjWData(passprobe_cat,combData),LineStyle(kDashed),LineColor(kRed)) ;

  TCanvas* c = new TCanvas("rf501_simultaneouspdf","rf403_simultaneouspdf",800,400) ;
  c->Divide(2) ;
  c->cd(1) ; gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.4) ; frame->Draw() ;
  c->cd(2) ; gPad->SetLeftMargin(0.15) ; frame1->GetYaxis()->SetTitleOffset(1.4) ; frame1->Draw();
  return 0;
}
