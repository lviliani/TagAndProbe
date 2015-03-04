from __future__ import division
from ROOT import *
from math import *
import numpy as n

#JPM = 0.545
#JPT = 0.790
JPM = 1.4
JPT = 0.5 

LumiW = 19.468


#outFile = TFile("TPtree_LooseTag_sig_bkg.root","recreate")
outFile = TFile("QUESTA_E_DAVVERO_DAVVERO_LA_VOLTA_BUONA.root","recreate")
treeData = TTree("Data","Tree with data_pp and data_fp")
treeTTbar = TTree("TTbar","Tree with ttbar_pp and ttbar_fp")
treeOtherBkg = TTree("OtherBkg","Tree with otherbkg_pp and otherbkg_fp")


pjetpt_pp = n.zeros(1, dtype=float)
pjetpt_fp = n.zeros(1, dtype=float)
weight_pp = n.zeros(1, dtype=float)
weight_fp = n.zeros(1, dtype=float)
btagp_pp = n.zeros(1, dtype=float)
btagp_fp = n.zeros(1, dtype=float)
btagt_pp = n.zeros(1, dtype=float)
btagt_fp = n.zeros(1, dtype=float)
mll_pp = n.zeros(1, dtype=float)
mll_fp = n.zeros(1, dtype=float)
mth_pp = n.zeros(1, dtype=float)
mth_fp = n.zeros(1, dtype=float)
isb = n.zeros(1, dtype=float)

treeData.Branch('isb', isb, 'isb/D')
treeData.Branch('pjetpt_pp', pjetpt_pp, 'pjetpt_pp/D')
treeData.Branch('pjetpt_fp', pjetpt_fp, 'pjetpt_fp/D')
treeData.Branch('btagp_pp', btagp_pp, 'btagp_pp/D')
treeData.Branch('btagp_fp', btagp_fp, 'btagp_fp/D')
treeData.Branch('btagt_pp', btagt_pp, 'btagt_pp/D')
treeData.Branch('btagt_fp', btagt_fp, 'btagt_fp/D')
treeData.Branch('mll_pp', mll_pp, 'mll_pp/D')
treeData.Branch('mll_fp', mll_fp, 'mll_fp/D')
treeData.Branch('mth_pp', mth_pp, 'mth_pp/D')
treeData.Branch('mth_fp', mth_fp, 'mth_fp/D')

treeTTbar.Branch('isb', isb, 'isb/D')
treeTTbar.Branch('pjetpt_pp', pjetpt_pp, 'pjetpt_pp/D')
treeTTbar.Branch('pjetpt_fp', pjetpt_fp, 'pjetpt_fp/D')
treeTTbar.Branch('weight_pp', weight_pp, 'weight_pp/D')
treeTTbar.Branch('weight_fp', weight_fp, 'weight_fp/D')
treeTTbar.Branch('btagp_pp', btagp_pp, 'btagp_pp/D')
treeTTbar.Branch('btagp_fp', btagp_fp, 'btagp_fp/D')
treeTTbar.Branch('btagt_pp', btagt_pp, 'btagt_pp/D')
treeTTbar.Branch('btagt_fp', btagt_fp, 'btagt_fp/D')
treeTTbar.Branch('mll_pp', mll_pp, 'mll_pp/D')
treeTTbar.Branch('mll_fp', mll_fp, 'mll_fp/D')
treeTTbar.Branch('mth_pp', mth_pp, 'mth_pp/D')
treeTTbar.Branch('mth_fp', mth_fp, 'mth_fp/D')

treeOtherBkg.Branch('isb', isb, 'isb/D')
treeOtherBkg.Branch('pjetpt_pp', pjetpt_pp, 'pjetpt_pp/D')
treeOtherBkg.Branch('pjetpt_fp', pjetpt_fp, 'pjetpt_fp/D')
treeOtherBkg.Branch('weight_pp', weight_pp, 'weight_pp/D')
treeOtherBkg.Branch('weight_fp', weight_fp, 'weight_fp/D')
treeOtherBkg.Branch('btagp_pp', btagp_pp, 'btagp_pp/D')
treeOtherBkg.Branch('btagp_fp', btagp_fp, 'btagp_fp/D')
treeOtherBkg.Branch('btagt_pp', btagt_pp, 'btagt_pp/D')
treeOtherBkg.Branch('btagt_fp', btagt_fp, 'btagt_fp/D')
treeOtherBkg.Branch('mll_pp', mll_pp, 'mll_pp/D')
treeOtherBkg.Branch('mll_fp', mll_fp, 'mll_fp/D')
treeOtherBkg.Branch('mth_pp', mth_pp, 'mth_pp/D')
treeOtherBkg.Branch('mth_fp', mth_fp, 'mth_fp/D')



# open the file
Top = TChain("latino") 
Top.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_019_TTTo2L2Nu2B_GEN.root")

Data = TChain("latino")
Data.Add("/data/lviliani/tree_latino_nosel/Data_TightTight/4L/latino_RunA_892pbinv.root")
Data.Add("/data/lviliani/tree_latino_nosel/Data_TightTight/4L/latino_RunB_4404pbinv.root")
Data.Add("/data/lviliani/tree_latino_nosel/Data_TightTight/4L/latino_RunC_7032pbinv.root")
Data.Add("/data/lviliani/tree_latino_nosel/Data_TightTight/4L/latino_RunD_7274pbinv.root")

Bkg = TChain("latino")
Bkg.Add("/data/lviliani/tree_latino_nosel/wjets/latino_LooseLoose_19.5fb.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_000_WWJets2LMad.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_001_GluGluToWWTo4L.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_082_WGstarToElNuMad.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_083_WGstarToMuNuMad.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_084_WGstarToTauNuMad.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_085_WgammaToLNuG.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_086_ZgammaToLLuG.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_011_TtWFullDR.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_012_TbartWFullDR.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_DYtt_19.5fb.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_036_DY10toLLMad.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_037_DY50toLLMad.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_074_WZJetsMad.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_078_WZTo2L2QMad.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_075_ZZJetsMad.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_079_ZZTo2L2QMad.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_088_WWGJets.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_089_WZZJets.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_090_ZZZJets.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_091_WWZJets.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_092_WWWJets.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_093_TTWJets.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_094_TTZJets.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_095_TTWWJets.root")
Bkg.Add("/data/lviliani/tree_latino_nosel/MC_TightTight_DABCABC/latino_096_TTGJets.root")


ntopentries=Top.GetEntries()
ndataentries=Data.GetEntries()
nbkgentries=Bkg.GetEntries()

Top.SetBranchStatus("*",0)
Top.SetBranchStatus("sameflav",1)
Top.SetBranchStatus("trigger",1)
Top.SetBranchStatus("mll",1)
Top.SetBranchStatus("mth",1)
Top.SetBranchStatus("dataset",1)
Top.SetBranchStatus("nextra",1)
Top.SetBranchStatus("pt1",1)
Top.SetBranchStatus("pt2",1)
Top.SetBranchStatus("ch1",1)
Top.SetBranchStatus("ch2",1)
Top.SetBranchStatus("zveto",1)
Top.SetBranchStatus("njet",1)
Top.SetBranchStatus("dphilljetjet",1)
Top.SetBranchStatus("jetbjpb*",1)
Top.SetBranchStatus("baseW",1)
Top.SetBranchStatus("puW",1)
Top.SetBranchStatus("effW",1)
Top.SetBranchStatus("triggW",1)
Top.SetBranchStatus("jetpt*",1)
Top.SetBranchStatus("jeteta*",1)
Top.SetBranchStatus("jetphi*",1)
Top.SetBranchStatus("jetpt*",1)
Top.SetBranchStatus("jetGenPartonBeta*",1)
Top.SetBranchStatus("jetGenPartonBphi*",1)

Data.SetBranchStatus("*",0)
Data.SetBranchStatus("sameflav",1)
Data.SetBranchStatus("trigger",1)
Data.SetBranchStatus("mll",1)
Data.SetBranchStatus("mth",1)
Data.SetBranchStatus("dataset",1)
Data.SetBranchStatus("nextra",1)
Data.SetBranchStatus("pt1",1)
Data.SetBranchStatus("pt2",1)
Data.SetBranchStatus("ch1",1)
Data.SetBranchStatus("ch2",1)
Data.SetBranchStatus("zveto",1)
Data.SetBranchStatus("njet",1)
Data.SetBranchStatus("dphilljetjet",1)
Data.SetBranchStatus("jetbjpb*",1)
Data.SetBranchStatus("baseW",1)
Data.SetBranchStatus("puW",1)
Data.SetBranchStatus("effW",1)
Data.SetBranchStatus("triggW",1)
Data.SetBranchStatus("jetpt*",1)
Data.SetBranchStatus("jeteta*",1)
Data.SetBranchStatus("jetphi*",1)
Data.SetBranchStatus("jetpt*",1)

Bkg.SetBranchStatus("*",0)
Bkg.SetBranchStatus("sameflav",1)
Bkg.SetBranchStatus("trigger",1)
Bkg.SetBranchStatus("mll",1)
Bkg.SetBranchStatus("mth",1)
Bkg.SetBranchStatus("dataset",1)
Bkg.SetBranchStatus("nextra",1)
Bkg.SetBranchStatus("pt1",1)
Bkg.SetBranchStatus("pt2",1)
Bkg.SetBranchStatus("ch1",1)
Bkg.SetBranchStatus("ch2",1)
Bkg.SetBranchStatus("zveto",1)
Bkg.SetBranchStatus("njet",1)
Bkg.SetBranchStatus("dphilljetjet",1)
Bkg.SetBranchStatus("jetbjpb*",1)
Bkg.SetBranchStatus("baseW",1)
Bkg.SetBranchStatus("puW",1)
Bkg.SetBranchStatus("effW",1)
Bkg.SetBranchStatus("triggW",1)
Bkg.SetBranchStatus("jetpt*",1)
Bkg.SetBranchStatus("jeteta*",1)
Bkg.SetBranchStatus("jetphi*",1)
Bkg.SetBranchStatus("jetpt*",1)


def match(jeteta, jetphi, beta, bphi):
  flag=false

  deltaEta = jeteta - beta
  deltaPhi=fabs(jetphi - bphi)

  if deltaPhi>pi : deltaPhi = 2*pi - deltaPhi

  if (sqrt( pow(deltaEta,2) + pow(deltaPhi,2) )<0.5) :
    flag=true

  return flag



############### TOP

iEvt=0
EvtMax=100000
step=1000000
tag=0
probe=0
pass_probe=0
fail_probe=0
p=0
for e in Top:
  iEvt+=1
  if iEvt > 0 and iEvt%step == 0: print iEvt,'/',str(ntopentries),' events processed.'
#  if iEvt == EvtMax : break
#  print "Event%d" % iEvt
  
  if not ( (e.sameflav!=1) and  e.trigger==1. and e.mll>12 and e.nextra==0 and e.pt1>20 and e.pt2>10 and ( (e.ch1<0 and e.ch2>0) or ( e.ch1>0 and e.ch2<0 ) ) and (e.zveto==1 or (e.sameflav!=1)) and (e.njet==0 or e.njet==1 or (e.dphilljetjet<3.1415/180.*165. or (e.sameflav!=1) ) )  ) : continue
 
  if ( e.jetbjpb3>JPM or e.jetbjpb4>JPM ) : continue

  w = e.baseW*e.puW*e.effW*e.triggW*LumiW
 
  isb[0] = 0
    
  if ( e.jetpt1>30 and e.jetbjpb1>JPT ) :
    tag = tag + w
     
    if ( e.jetpt2>30 ) :
      probe = probe + w
  
      if( match(e.jeteta2,e.jetphi2,e.jetGenPartonBeta1,e.jetGenPartonBphi1) or match(e.jeteta2,e.jetphi2,e.jetGenPartonBeta2,e.jetGenPartonBphi2)  ): 
        isb[0] = 1   

      if ( e.jetbjpb2>JPM ):
        pass_probe = pass_probe +w
	pjetpt_pp[0] = e.jetpt2
        btagp_pp[0] = e.jetbjpb2
        btagt_pp[0] = e.jetbjpb1
        mll_pp[0] = e.mll
        mth_pp[0] = e.mth
	weight_pp[0] = w
        pjetpt_fp[0] = -9999
        btagp_fp[0] = -9999
        btagt_fp[0] = -9999
        mll_fp[0] = -9999
        mth_fp[0] = -9999
        weight_fp[0] = -9999
		
      else :
        fail_probe = fail_probe +w
	pjetpt_fp[0] = e.jetpt2
        btagp_fp[0] = e.jetbjpb2
        btagt_fp[0] = e.jetbjpb1
        mll_fp[0] = e.mll
        mth_fp[0] = e.mth
	weight_fp[0] = w
        pjetpt_pp[0] = -9999
        btagp_pp[0] = -9999
        btagt_pp[0] = -9999
        mll_pp[0] = -9999
        mth_pp[0] = -9999
        weight_pp[0] = -9999

      treeTTbar.Fill()

  isb[0]=0

  if ( e.jetpt2>30 and e.jetbjpb2>JPT ) :
    tag = tag + w

    if ( e.jetpt1>30 ) :
      probe = probe + w

      if( match(e.jeteta1,e.jetphi1,e.jetGenPartonBeta1,e.jetGenPartonBphi1) or match(e.jeteta1,e.jetphi1,e.jetGenPartonBeta2,e.jetGenPartonBphi2)  ):
        isb[0] = 1

      if ( e.jetbjpb1>JPM ):
        pass_probe = pass_probe +w
        pjetpt_pp[0] = e.jetpt1
        btagp_pp[0] = e.jetbjpb1
        btagt_pp[0] = e.jetbjpb2
        mll_pp[0] = e.mll
        mth_pp[0] = e.mth
	weight_pp[0] = w
        pjetpt_fp[0] = -9999
        btagp_fp[0] = -9999
        btagt_fp[0] = -9999
        mll_fp[0] = -9999
        mth_fp[0] = -9999
        weight_fp[0] = -9999

      else:
        fail_probe = fail_probe +w
        pjetpt_fp[0] = e.jetpt1
        btagp_fp[0] = e.jetbjpb1
        btagt_fp[0] = e.jetbjpb2
        mll_fp[0] = e.mll
        mth_fp[0] = e.mth
	weight_fp[0] = w
        pjetpt_pp[0] = -9999
        btagp_pp[0] = -9999
        btagt_pp[0] = -9999
        mll_pp[0] = -9999
        mth_pp[0] = -9999
        weight_pp[0] = -9999


      treeTTbar.Fill()

  
print "####### TOP ######"
print "TAG = ", tag
print "PROBE = ", probe
print "PASS_PROBE = ", pass_probe
print "FAIL_PROBE = ", fail_probe
print "b-tagging efficiency = ",pass_probe/probe
print "##################"





###################### BACKGROUND
iEvt=0
EvtMax=100000
step=1000000
tag=0
probe=0
pass_probe=0
fail_probe=0
p=0
for e in Bkg:
  iEvt+=1
  if iEvt > 0 and iEvt%step == 0: print iEvt,'/',str(nbkgentries),' events processed.'
#  if iEvt == EvtMax : break
#  print "Event%d" % iEvt
  
  if not ( (e.sameflav!=1) and  e.trigger==1. and e.mll>12 and e.nextra==0 and e.pt1>20 and e.pt2>10 and ( (e.ch1<0 and e.ch2>0) or ( e.ch1>0 and e.ch2<0 ) ) and (e.zveto==1 or (e.sameflav!=1)) and (e.njet==0 or e.njet==1 or (e.dphilljetjet<3.1415/180.*165. or (e.sameflav!=1) ) ) ) : continue


  current_file = Bkg.GetFile().GetName()

  if ( e.jetbjpb3>JPM or e.jetbjpb4>JPM ) : continue

  isb[0] = 0  

  if ( e.jetpt1>30 and e.jetbjpb1>JPT ) :
    if "latino_LooseLoose_19.5fb" in current_file : 
      w = e.fakeW*(e.run!=201191)
    elif "074" or "075" or "078" or "079" or "088" or "089" or "090" or "091" or "092" or "093" or "094" or "095" or "096" in current_file :
      w = e.puW*e.baseW*e.effW*e.triggW*(1+0.5*(e.dataset>=82 and e.dataset<=84))*LumiW
    else:
      w = e.baseW*e.puW*e.effW*e.triggW*LumiW

    tag = tag + w

    if ( e.jetpt2>30 ) :
      probe = probe + w
        
      if ( e.jetbjpb2>JPM ):
        pass_probe = pass_probe+w
        pjetpt_pp[0] = e.jetpt2
	btagp_pp[0] = e.jetbjpb2
        btagt_pp[0] = e.jetbjpb1
        mll_pp[0] = e.mll
        mth_pp[0] = e.mth
        weight_pp[0] = w
        pjetpt_fp[0] = -9999
        btagp_fp[0] = -9999
        btagt_fp[0] = -9999
        mll_fp[0] = -9999
        mth_fp[0] = -9999
        weight_fp[0] = -9999
      else :
        fail_probe = fail_probe+w
        pjetpt_fp[0] = e.jetpt2
        btagp_fp[0] = e.jetbjpb2
        btagt_fp[0] = e.jetbjpb1
        mll_fp[0] = e.mll
        mth_fp[0] = e.mth
        weight_fp[0] = w 
        pjetpt_pp[0] = -9999
        btagp_pp[0] = -9999
        btagt_pp[0] = -9999
        mll_pp[0] = -9999
        mth_pp[0] = -9999
        weight_pp[0] = -9999

      treeOtherBkg.Fill()


  if ( e.jetpt2>30 and e.jetbjpb2>JPT ) :
    if "latino_LooseLoose_19.5fb" in current_file : 
      w = e.fakeW*(e.run!=201191)
    elif "074" or "075" or "078" or "079" or "088" or "089" or "090" or "091" or "092" or "093" or "094" or "095" or "096" in current_file :
      w = e.puW*e.baseW*e.effW*e.triggW*(1+0.5*(e.dataset>=82 and e.dataset<=84))*LumiW
    else:
      w = e.baseW*e.puW*e.effW*e.triggW*LumiW

    tag = tag + w

    if ( e.jetpt1>30 ) :
      probe = probe + w

      if ( e.jetbjpb1>JPM ):
        pass_probe = pass_probe +w
	pjetpt_pp[0] = e.jetpt1
        btagp_pp[0] = e.jetbjpb1
        btagt_pp[0] = e.jetbjpb2
        mll_pp[0] = e.mll
        mth_pp[0] = e.mth
       	weight_pp[0] = w 
        pjetpt_fp[0] = -9999
        btagp_fp[0] = -9999
        btagt_fp[0] = -9999
        mll_fp[0] = -9999
        mth_fp[0] = -9999
        weight_fp[0] = -9999

      else:
        fail_probe = fail_probe +w
        pjetpt_fp[0] = e.jetpt1
        btagp_fp[0] = e.jetbjpb1
        btagt_fp[0] = e.jetbjpb2
        mll_fp[0] = e.mll
        mth_fp[0] = e.mth
        weight_fp[0] = w
        pjetpt_pp[0] = -9999
        btagp_pp[0] = -9999
        btagt_pp[0] = -9999
        mll_pp[0] = -9999
        mth_pp[0] = -9999
        weight_pp[0] = -9999

      treeOtherBkg.Fill()  

print "####### BACKGROUND ######"
print "TAG = ", tag
print "PROBE = ", probe
print "PASS_PROBE = ", pass_probe
print "FAIL_PROBE = ", fail_probe
#print "b-tagging efficiency = ",pass_probe/probe
print "##################"


###################### DATA

iEvt=0
EvtMax=100000
step=1000000
tag=0
probe=0
pass_probe=0
fail_probe=0

for e in Data:
  iEvt+=1
  if iEvt > 0 and iEvt%step == 0: print iEvt,'/',str(ndataentries),' events processed.'
#  if iEvt == EvtMax : break
#  print "Event%d" % iEvt

  if not ( (e.sameflav!=1) and  e.trigger==1. and e.mll>12 and e.nextra==0 and e.pt1>20 and e.pt2>10 and ( (e.ch1<0 and e.ch2>0) or ( e.ch1>0 and e.ch2<0 ) ) and (e.zveto==1 or (e.sameflav!=1)) and (e.njet==0 or e.njet==1 or (e.dphilljetjet<3.1415/180.*165. or (e.sameflav!=1) ) ) ) : continue
    
  if ( e.jetbjpb3>JPM or e.jetbjpb4>JPM ) : continue
  if ( e.run==201191 ) : continue
  
  isb[0] = 0

  if ( e.jetpt1>30 and e.jetbjpb1>JPT ) :
    tag = tag + 1
     
    if ( e.jetpt2>30 ) :
      probe = probe + 1
       
      if ( e.jetbjpb2>JPM ):
        pass_probe = pass_probe +1
        pjetpt_pp[0] = e.jetpt2
        btagp_pp[0] = e.jetbjpb2
        btagt_pp[0] = e.jetbjpb1
        mll_pp[0] = e.mll
        mth_pp[0] = e.mth
        pjetpt_fp[0] = -9999
        btagp_fp[0] = -9999
        btagt_fp[0] = -9999
        mll_fp[0] = -9999
        mth_fp[0] = -9999

      else :
        fail_probe = fail_probe +1
        pjetpt_fp[0] = e.jetpt2
        btagp_fp[0] = e.jetbjpb2
        btagt_fp[0] = e.jetbjpb1
        mll_fp[0] = e.mll
        mth_fp[0] = e.mth
        pjetpt_pp[0] = -9999
        btagp_pp[0] = -9999
        btagt_pp[0] = -9999
        mll_pp[0] = -9999
        mth_pp[0] = -9999

      treeData.Fill()

  if ( e.jetpt2>30 and e.jetbjpb2>JPT ) :
    tag = tag + 1

    if ( e.jetpt1>30 ) :
      probe = probe + 1

      if ( e.jetbjpb1>JPM ):
        pass_probe = pass_probe +1
        pjetpt_pp[0] = e.jetpt1
        btagp_pp[0] = e.jetbjpb1
        btagt_pp[0] = e.jetbjpb2
        mll_pp[0] = e.mll
        mth_pp[0] = e.mth
        pjetpt_fp[0] = -9999
        btagp_fp[0] = -9999
        btagt_fp[0] = -9999
        mll_fp[0] = -9999
        mth_fp[0] = -9999

      else :
        fail_probe = fail_probe +1
        pjetpt_fp[0] = e.jetpt1
        btagp_fp[0] = e.jetbjpb1
        btagt_fp[0] = e.jetbjpb2
        mll_fp[0] = e.mll
        mth_fp[0] = e.mth
        pjetpt_pp[0] = -9999
        btagp_pp[0] = -9999
        btagt_pp[0] = -9999
        mll_pp[0] = -9999
        mth_pp[0] = -9999


      treeData.Fill()

tttw_ratio_p = 0.051
tttw_ratio_pp = 0.0370

print "####### DATA ######"
print "TAG = ", tag
print "PROBE = ", probe
print "PASS_PROBE = ", pass_probe
print "FAIL_PROBE = ", fail_probe
print "b-tagging efficiency = ", (pass_probe*(1-tttw_ratio_pp))/(probe*(1-tttw_ratio_p))
print "##################"

outFile.Write()
outFile.Close()

