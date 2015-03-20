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
trees={"Data": treeData, "Top":treeTTbar, "Bkg":treeOtherBkg}

jetptprobe = n.zeros(1, dtype=float)
jetpttag = n.zeros(1, dtype=float)
jetptsum = n.zeros(1, dtype=float)
jetdphi = n.zeros(1, dtype=float)
passfail  = n.zeros(1, dtype=int)
weight = n.zeros(1, dtype=float)
btagprobe = n.zeros(1, dtype=float)
btagtag = n.zeros(1, dtype=float)
mll = n.zeros(1, dtype=float)
mth = n.zeros(1, dtype=float)
isb = n.zeros(1, dtype=float)

for key in trees.keys():
  trees[key].Branch('isb', isb, 'isb/D')
  trees[key].Branch('jetptprobe', jetptprobe, 'jetptprobe/D')
  trees[key].Branch('jetpttag', jetpttag, 'jetpttag/D')
  trees[key].Branch('jetptsum', jetptsum, 'jetptsum/D')
  trees[key].Branch('jetdphi', jetdphi, 'jetdphi/D')
  trees[key].Branch('passfail', passfail, 'passfail/I')
  trees[key].Branch('weight', weight, 'weight/D')
  trees[key].Branch('btagprobe', btagprobe, 'btagprobe/D')
  trees[key].Branch('btagtag', btagtag, 'btagtad/D')
  trees[key].Branch('mll', mll, 'mll/D')
  trees[key].Branch('mth', mth, 'mth/D')


# open the file
Top = TChain("latino") 
Top.Add("/data/lenzip/differential/tree_noskim/nominals//latino_019_TTTo2L2Nu2B_GEN.root")

Data = TChain("latino")
Data.Add("/data/lenzip/differential/tree_noskim/data/latino_RunA_892pbinv.root")
Data.Add("/data/lenzip/differential/tree_noskim/data/latino_RunB_4404pbinv.root")
Data.Add("/data/lenzip/differential/tree_noskim/data/latino_RunC_7032pbinv.root")
Data.Add("/data/lenzip/differential/tree_noskim/data/latino_RunD_7274pbinv.root")

Bkg = TChain("latino")
Bkg.Add("/data/lenzip/differential/tree_noskim/wjets/latino_RunA_892pbinv_LooseLoose.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/wjets/latino_RunB_4404pbinv_LooseLoose.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/wjets/latino_RunC_7032pbinv_LooseLoose.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/wjets/latino_RunD_7274pbinv_LooseLoose.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_000_WWJets2LMad.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_001_GluGluToWWTo4L.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/wjets/latino_082_WGstarToElNuMad.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/wjets/latino_083_WGstarToMuNuMad.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/wjets/latino_084_WGstarToTauNuMad.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/wjets/latino_085_WgammaToLNuG.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/wjets/latino_086_ZgammaToLLuG.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_011_TtWFullDR.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_012_TbartWFullDR.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_DYtt_19.5fb.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_036_DY10toLLMad.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_037_DY50toLLMad.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_074_WZJetsMad.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_078_WZTo2L2QMad.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_075_ZZJetsMad.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_079_ZZTo2L2QMad.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_088_WWGJets.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_089_WZZJets.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_090_ZZZJets.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_091_WWZJets.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_092_WWWJets.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_093_TTWJets.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_094_TTZJets.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_095_TTWWJets.root")
Bkg.Add("/data/lenzip/differential/tree_noskim/nominals//latino_096_TTGJets.root")

chains=[(Data, "Data") , (Top, "Top"), (Bkg, "Bkg")]

ntopentries=Top.GetEntries()
ndataentries=Data.GetEntries()
nbkgentries=Bkg.GetEntries()

for chain in chains:
  print chain[1]
  chain[0].SetBranchStatus("*",0)
  chain[0].SetBranchStatus("sameflav",1)
  chain[0].SetBranchStatus("trigger",1)
  chain[0].SetBranchStatus("mll",1)
  chain[0].SetBranchStatus("mth",1)
  chain[0].SetBranchStatus("dataset",1)
  chain[0].SetBranchStatus("nextra",1)
  chain[0].SetBranchStatus("pt1",1)
  chain[0].SetBranchStatus("pt2",1)
  chain[0].SetBranchStatus("ch1",1)
  chain[0].SetBranchStatus("ch2",1)
  chain[0].SetBranchStatus("zveto",1)
  chain[0].SetBranchStatus("njet",1)
  chain[0].SetBranchStatus("dphilljetjet",1)
  chain[0].SetBranchStatus("jetbjpb*",1)
  chain[0].SetBranchStatus("baseW",1)
  chain[0].SetBranchStatus("puW",1)
  chain[0].SetBranchStatus("effW",1)
  chain[0].SetBranchStatus("triggW",1)
  chain[0].SetBranchStatus("jetpt*",1)
  chain[0].SetBranchStatus("jeteta*",1)
  chain[0].SetBranchStatus("jetphi*",1)
  chain[0].SetBranchStatus("jetpt*",1)
  chain[0].SetBranchStatus("pfmet",1)
  chain[0].SetBranchStatus("mpmet",1)
  chain[0].SetBranchStatus("jetGenPartonBeta*",1)
  chain[0].SetBranchStatus("jetGenPartonBphi*",1)

def deltaPhi(jetphi, bphi):
  deltaPhi=fabs(jetphi - bphi)

  if deltaPhi>pi : deltaPhi = 2*pi - deltaPhi

  return deltaPhi

def match(jeteta, jetphi, beta, bphi):
  flag=false

  deltaEta = jeteta - beta
  deltaPhi=fabs(jetphi - bphi)

  if deltaPhi>pi : deltaPhi = 2*pi - deltaPhi

  if (sqrt( pow(deltaEta,2) + pow(deltaPhi,2) )<0.5) :
    flag=true

  return flag



for chain in chains:
  iEvt=0
  EvtMax=10000000000000
  step=1000000
  tag=0
  probe=0
  pass_probe=0
  fail_probe=0
  p=0 
  for e in chain[0]:
    iEvt+=1
    if iEvt > 0 and iEvt%step == 0: print iEvt,'/',chain[0].GetEntries(),' events processed.'
    if iEvt == EvtMax : break
  #  print "Event%d" % iEvt
    
   
    #if not ( (e.sameflav!=1) and  e.trigger==1. and e.mll>12 and e.nextra==0 and e.pt1>20 and e.pt2>10 and ( (e.ch1<0 and e.ch2>0) or ( e.ch1>0 and e.ch2<0 ) ) and (e.zveto==1 or (e.sameflav!=1)) and (e.njet==0 or e.njet==1 or (e.dphilljetjet<3.1415/180.*165. or (e.sameflav!=1) ) )  ) : continue
    if not ( (e.sameflav!=1) and  e.trigger==1. and e.mll>12 and e.nextra==0 and e.pt1>20 and e.pt2>10 and ( (e.ch1<0 and e.ch2>0) or ( e.ch1>0 and e.ch2<0 ) ) and e.pfmet>20. and e.mpmet>20. ) : continue

    if ( e.jetbjpb3>JPM or e.jetbjpb4>JPM ) : continue
    if chain[1] == "Top":
      weight[0] = e.baseW*e.puW*e.effW*e.triggW*LumiW
    elif chain[1] == "Bkg":
      current_file = Bkg.GetFile().GetName()
      if "Loose" in current_file :
        weight[0] = e.fakeW*(e.run!=201191)
      else:
        weight[0] = e.puW*e.baseW*e.effW*e.triggW*(1+0.5*(e.dataset>=82 and e.dataset<=84))*LumiW
    elif chain[1] == "Data":
      weight[0] = 1.
   
    isb[0] = 0
    
    if ( e.jetpt1>30 and e.jetbjpb1>JPT ) :
      tag += weight[0]
       
      if ( e.jetpt2>30 ) :
        probe += weight[0]
    
        if chain[1] == "Top":
          if  match(e.jeteta2,e.jetphi2,e.jetGenPartonBeta1,e.jetGenPartonBphi1) or match(e.jeteta2,e.jetphi2,e.jetGenPartonBeta2,e.jetGenPartonBphi2)  : 
            isb[0] = 1   

        jetptsum[0] = e.jetpt2 +  e.jetpt1
        jetdphi[0] = deltaPhi(e.jetphi1, e.jetphi2)
        jetptprobe[0] = e.jetpt2
        jetpttag[0] = e.jetpt1
        btagprobe[0] = e.jetbjpb2
        btagtag[0] = e.jetbjpb1
        mll[0]=e.mll
        mth[0]=e.mth
        if ( e.jetbjpb2>JPM ):
          pass_probe += weight[0]
          passfail[0] = 1
        else:
          fail_probe += weight[0]
          passfail[0] = 0   
          

        trees[chain[1]].Fill()

    isb[0]=0

    if ( e.jetpt2>30 and e.jetbjpb2>JPT ) :
      tag += weight[0]

      if ( e.jetpt1>30 ) :
        probe += weight[0]

        if chain[1] == "Top": 
          if match(e.jeteta1,e.jetphi1,e.jetGenPartonBeta1,e.jetGenPartonBphi1) or match(e.jeteta1,e.jetphi1,e.jetGenPartonBeta2,e.jetGenPartonBphi2)  :
           isb[0] = 1
        
        jetptsum[0] = e.jetpt2 +  e.jetpt1
        jetdphi[0] = deltaPhi(e.jetphi1, e.jetphi2)
        jetptprobe[0] = e.jetpt1
        jetpttag[0] = e.jetpt2
        btagprobe[0] = e.jetbjpb1
        btagtag[0] = e.jetbjpb2
        mll[0]=e.mll
        mth[0]=e.mth
        if ( e.jetbjpb1>JPM ):
          pass_probe += weight[0]
          passfail[0] = 1

        else:
          fail_probe += weight[0]
          passfail[0] = 0 

        trees[chain[1]].Fill()
        
    
  print "#######", chain[1], " ######"
  print "TAG = ", tag
  print "PROBE = ", probe
  print "PASS_PROBE = ", pass_probe
  print "FAIL_PROBE = ", fail_probe
  print "b-tagging efficiency = ",pass_probe/probe
  print "##################"


outFile.Write()
outFile.Close()

