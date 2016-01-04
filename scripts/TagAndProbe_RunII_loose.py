from __future__ import division
from ROOT import *
from math import *
import numpy as n

#JPM = 0.545
#JPT = 0.790
veryloose = 0.
loose = 0.605
medium = 0.89
tight = 0.97

LumiW = 1.27

outFile = TFile("tandp_mediumT_looseP_loose.root","recreate")

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
weightUp = n.zeros(1, dtype=float)
weightDown = n.zeros(1, dtype=float)
weightST = n.zeros(1, dtype=float)
weightSTUp = n.zeros(1, dtype=float)
weightSTDown = n.zeros(1, dtype=float)
btagprobe = n.zeros(1, dtype=float)
btagtag = n.zeros(1, dtype=float)
mll = n.zeros(1, dtype=float)
mth = n.zeros(1, dtype=float)
isb = n.zeros(1, dtype=float)
ptll = n.zeros(1, dtype=float)
ht = n.zeros(1, dtype=float)
ht2 = n.zeros(1, dtype=float)
dataset = n.zeros(1, dtype=float)

for key in trees.keys():
  trees[key].Branch('isb', isb, 'isb/D')
  trees[key].Branch('jetptprobe', jetptprobe, 'jetptprobe/D')
  trees[key].Branch('jetpttag', jetpttag, 'jetpttag/D')
  trees[key].Branch('jetptsum', jetptsum, 'jetptsum/D')
  trees[key].Branch('jetdphi', jetdphi, 'jetdphi/D')
  trees[key].Branch('passfail', passfail, 'passfail/I')
  trees[key].Branch('weight', weight, 'weight/D')
  trees[key].Branch('weightUp', weightUp, 'weightUp/D')
  trees[key].Branch('weightDown', weightDown, 'weightDown/D')
  trees[key].Branch('weightST', weightST, 'weightST/D')
  trees[key].Branch('weightSTUp', weightSTUp, 'weightSTUp/D')
  trees[key].Branch('weightSTDown', weightSTDown, 'weightSTDown/D')
  trees[key].Branch('btagprobe', btagprobe, 'btagprobe/D')
  trees[key].Branch('btagtag', btagtag, 'btagtad/D')
  trees[key].Branch('mll', mll, 'mll/D')
  trees[key].Branch('mth', mth, 'mth/D')
  trees[key].Branch('ptll', ptll, 'ptll/D')
  trees[key].Branch('ht', ht, 'ht/D')
  trees[key].Branch('ht2', ht2, 'ht2/D')
  trees[key].Branch('dataset', dataset, 'dataset/D')


Dir_mc = "../eos/cms/store/caf/user/lenzip/ww2016/21Oct_25ns_MC/mcwghtcount__MC__l2sel__bPogSF__hadd/"
#Dir_mc = "../eos/user/x/xjanssen/HWW2015/21Oct_25ns_MC/mcwghtcount__MC__l2sel__hadd/"
Dir_data = "../eosuser/user/x/xjanssen/HWW2015/21Oct_Run2015D_05Oct2015/l2sel__hadd/"
Dir_data2 = "../eosuser/user/x/xjanssen/HWW2015/21Oct_Run2015D_PromptReco/l2sel__hadd/"


# open the file
Top = TChain("latino") 
Top.Add(Dir_mc+"latino_TTTo2L2Nu.root")

Data = TChain("latino")
Data.Add(Dir_data2+"latino_Run2015D_PromptReco_25ns_MuonEG.root")
Data.Add(Dir_data2+"latino_Run2015D_PromptReco_25ns_DoubleEG.root")
Data.Add(Dir_data2+"latino_Run2015D_PromptReco_25ns_DoubleMuon.root")
Data.Add(Dir_data2+"latino_Run2015D_PromptReco_25ns_SingleElectron.root")
Data.Add(Dir_data2+"latino_Run2015D_PromptReco_25ns_SingleMuon.root")
Data.Add(Dir_data+"latino_Run2015D_05Oct2015_DoubleEG.root")
Data.Add(Dir_data+"latino_Run2015D_05Oct2015_DoubleMuon.root")
Data.Add(Dir_data+"latino_Run2015D_05Oct2015_MuonEG.root")
Data.Add(Dir_data+"latino_Run2015D_05Oct2015_SingleElectron.root")
Data.Add(Dir_data+"latino_Run2015D_05Oct2015_SingleMuon.root")


Bkg = TChain("latino")
Bkg.Add(Dir_mc+"latino_Wg.root");
Bkg.Add(Dir_mc+"latino_ST_s-channel.root")
Bkg.Add(Dir_mc+"latino_ST_tW_antitop.root")
Bkg.Add(Dir_mc+"latino_ST_tW_top.root")
Bkg.Add(Dir_mc+"latino_ST_t-channel_antitop.root")
Bkg.Add(Dir_mc+"latino_ST_t-channel_top.root")
Bkg.Add(Dir_mc+"latino_DYJetsToLL_M-10to50.root")
Bkg.Add(Dir_mc+"latino_DYJetsToLL_M-50.root")
Bkg.Add(Dir_mc+"latino_WJetsToLNu.root")
Bkg.Add(Dir_mc+"latino_WWTo2L2Nu.root")
Bkg.Add(Dir_mc+"latino_WWZ.root")
Bkg.Add(Dir_mc+"latino_WZZ.root")
Bkg.Add(Dir_mc+"latino_ZZ.root")
Bkg.Add(Dir_mc+"latino_ZZZ.root")
Bkg.Add(Dir_mc+"latino_ZZTo4L.root")
#Bkg.Add(Dir_mc+"latino_GluGluHToWWTo2L2Nu_M125_skim.root")


#chains=[(Data, "Data")]
chains=[(Data, "Data") , (Top, "Top"), (Bkg, "Bkg")]
#chains=[(Top, "Top"), (Bkg, "Bkg")]

ntopentries=Top.GetEntries()
ndataentries=Data.GetEntries()
nbkgentries=Bkg.GetEntries()

for chain in chains:
  print chain[1]
  chain[0].SetBranchStatus("*",0)
  chain[0].SetBranchStatus("std_vector_lepton_flavour*",1)
  chain[0].SetBranchStatus("trigger",1)
  chain[0].SetBranchStatus("mll",1)
  chain[0].SetBranchStatus("mth",1)
  chain[0].SetBranchStatus("ptll",1)
  chain[0].SetBranchStatus("pfType1Met",1)
  chain[0].SetBranchStatus("dataset",1)
  chain[0].SetBranchStatus("std_vector_lepton_pt*",1)
  chain[0].SetBranchStatus("std_vector_lepton_ch*",1)
  chain[0].SetBranchStatus("njet",1)
  chain[0].SetBranchStatus("std_vector_jet_pt*",1)
  chain[0].SetBranchStatus("std_vector_jet_eta*",1)
  chain[0].SetBranchStatus("std_vector_jet_phi*",1)
  chain[0].SetBranchStatus("std_vector_jet_csvv2ivf*",1)
  chain[0].SetBranchStatus("baseW",1)
  chain[0].SetBranchStatus("puW",1)
  chain[0].SetBranchStatus("effW",1)
  chain[0].SetBranchStatus("bPogSF2Jet",1)
  chain[0].SetBranchStatus("bPogSF2JetUp",1)
  chain[0].SetBranchStatus("bPogSF2JetDown",1)
  chain[0].SetBranchStatus("triggW",1)
  chain[0].SetBranchStatus("ppfMet",1)
  chain[0].SetBranchStatus("std_vector_jet_PartonFlavour*",1)
  chain[0].SetBranchStatus("std_vector_jet_HadronFlavour*",1)
  chain[0].SetBranchStatus("std_vector_jetGen*",1)

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
  EvtMax=1000000000
  step=10000
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
   
    if not ( (e.std_vector_lepton_flavour[0] * e.std_vector_lepton_flavour[1] == -11*13 ) and  e.mll>12 and e.std_vector_lepton_pt[0]>20. and e.std_vector_lepton_pt[1]>15. and e.std_vector_lepton_pt[2]<10. and e.pfType1Met>20. and  e.trigger == 1) : continue
    #if not ( (e.std_vector_lepton_flavour[0] * e.std_vector_lepton_flavour[1] == -11*13) and  e.mll>12 and e.std_vector_lepton_pt[2]<0 and e.std_vector_lepton_pt[0]>20 and e.std_vector_lepton_pt[1]>10 and ( (e.std_vector_lepton_ch[0]<0 and e.std_vector_lepton_ch[1]>0) or ( e.std_vector_lepton_ch[0]>0 and e.std_vector_lepton_ch[1]<0 ) ) and e.ppfMet>20. ) : continue
    #if not ( e.mll>12 and e.std_vector_lepton_pt[0]>20 and e.std_vector_lepton_pt[1]>15 and e.ppfMet>20. and e.trigger == 1) : continue

    if ( e.std_vector_jet_csvv2ivf[2]>medium or e.std_vector_jet_csvv2ivf[3]>medium ) : 
      continue
    if chain[1] == "Top":
      weight[0] = e.baseW*e.puW*e.bPogSF2Jet*LumiW
      weightUp[0] = e.baseW*e.puW*e.bPogSF2JetUp*LumiW
      weightDown[0] = e.baseW*e.puW*e.bPogSF2JetDown*LumiW
      weightST[0]=weight[0]
      weightSTUp[0]=weight[0]
      weightSTDown[0]=weight[0]
    elif chain[1] == "Bkg":
      weight[0] = e.baseW*e.puW*e.bPogSF2Jet*LumiW
      weightUp[0] = e.baseW*e.puW*e.bPogSF2JetUp*LumiW
      weightDown[0] = e.baseW*e.puW*e.bPogSF2JetDown*LumiW
      weightST[0]=weight[0]
      weightSTUp[0]=weight[0]
      weightSTDown[0]=weight[0]
      #single top variations
      if ( e.dataset >=12 and e.dataset <=16 )  or e.dataset == 22:
        weightSTUp[0]=weight[0]*1.2
        weightSTDown[0]=weight[0]*0.8
    elif chain[1] == "Data":
      weight[0] = e.trigger 
   
    dataset[0] =  e.dataset    
    isb[0] = 0
    ht[0] = 0.
    ht2[0] = 0.
    for i in range(10):
      if (e.std_vector_jet_pt[i] > 0.):
        ht[0] += e.std_vector_jet_pt[i]
        if (i < 2):
          ht2[0] += e.std_vector_jet_pt[i]
      
    #if ( e.std_vector_jet_pt[0]>30 and e.std_vector_jet_csvv2ivf[0]>medium ) :
    if ( e.std_vector_jet_pt[0]>15 and e.std_vector_jet_csvv2ivf[0]>veryloose ) :
      tag += weight[0]
       
      if ( e.std_vector_jet_pt[1]>15 ) :
        probe += weight[0]
    
        if chain[1] != "Data":
          #if  match(e.jeteta2,e.jetphi2,e.jetGenPartonBeta1,e.jetGenPartonBphi1) or match(e.jeteta2,e.jetphi2,e.jetGenPartonBeta2,e.jetGenPartonBphi2)  : 
          if abs(e.std_vector_jet_HadronFlavour[1])==5:
            isb[0] = 1   
          #elif (abs(e.std_vector_jet_HadronFlavour[1])==0 and abs(e.std_vector_jet_PartonFlavour[1])==0): continue #let's keep these as non b

        jetptsum[0] = e.std_vector_jet_pt[0] +  e.std_vector_jet_pt[1]
        jetdphi[0] = deltaPhi(e.std_vector_jet_phi[0], e.std_vector_jet_phi[1])
        jetptprobe[0] = e.std_vector_jet_pt[1]
        jetpttag[0] = e.std_vector_jet_pt[0]
        btagprobe[0] = e.std_vector_jet_csvv2ivf[1]
        btagtag[0] = e.std_vector_jet_csvv2ivf[0]
        mll[0]=e.mll
        mth[0]=e.mth
        ptll[0]=e.ptll
        if ( e.std_vector_jet_csvv2ivf[1]>loose ):
          pass_probe += weight[0]
          passfail[0] = 1
        else:
          fail_probe += weight[0]
          passfail[0] = 0   
          

        trees[chain[1]].Fill()

    isb[0]=0

    #if ( e.std_vector_jet_pt[1]>30 and e.std_vector_jet_csvv2ivf[1]>medium ) :
    if ( e.std_vector_jet_pt[1]>15 and e.std_vector_jet_csvv2ivf[1]>veryloose ) :
      tag += weight[0]

      if ( e.std_vector_jet_pt[0]>15 ) :
        probe += weight[0]

        if chain[1] != "Data": 
          #if match(e.jeteta1,e.jetphi1,e.jetGenPartonBeta1,e.jetGenPartonBphi1) or match(e.jeteta1,e.jetphi1,e.jetGenPartonBeta2,e.jetGenPartonBphi2)  :
          if abs(e.std_vector_jet_HadronFlavour[0])==5:
            isb[0] = 1
          #elif (abs(e.std_vector_jet_HadronFlavour[0])==0 and abs(e.std_vector_jet_PartonFlavour[0])==0): continue
 
        jetptsum[0] = e.std_vector_jet_pt[0] +  e.std_vector_jet_pt[1]
        jetdphi[0] = deltaPhi(e.std_vector_jet_phi[0], e.std_vector_jet_phi[1])
        jetptprobe[0] = e.std_vector_jet_pt[0]
        jetpttag[0] = e.std_vector_jet_pt[1]
        btagprobe[0] = e.std_vector_jet_csvv2ivf[0]
        btagtag[0] = e.std_vector_jet_csvv2ivf[1]
        mll[0]=e.mll
        mth[0]=e.mth
        ptll[0]=e.ptll
        if ( e.std_vector_jet_csvv2ivf[0]>loose ):
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

