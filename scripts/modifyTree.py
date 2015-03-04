from __future__ import division
from ROOT import *
import numpy as n

#JPM = 0.545
#JPT = 0.790
JPM = 1.4
JPT = 2 

LumiW = 19.468

# open the file
Data = TChain("Data") 
Data.Add("/home/lviliani/Documenti/TagAndProbe/looseTag_volta_buona_davvero_davvero/QUESTA_E_DAVVERO_DAVVERO_LA_VOLTA_BUONA.root")

############################  DATA PP ############################
jetpt_probe = n.zeros(1, dtype=float)
btag_probe = n.zeros(1, dtype=float)
btag_tag = n.zeros(1, dtype=float)
mll = n.zeros(1, dtype=float)
mth = n.zeros(1, dtype=float)
nbin = n.zeros(1, dtype=float)

outFile_pp = TFile("data_pp.root","recreate")
outTreeData_pp = TTree("Data","Tree with data_pp and data_fp")
outTreeData_pp.Branch('jetpt_probe', jetpt_probe, 'jetpt_probe/D')
outTreeData_pp.Branch('btag_probe', btag_probe, 'btag_probe/D')
outTreeData_pp.Branch('btag_tag', btag_tag, 'btag_tag/D')
outTreeData_pp.Branch('mll', mll, 'mll/D')
outTreeData_pp.Branch('mth', mth, 'mth/D')
outTreeData_pp.Branch('nbin', nbin, 'nbin/D')


iEvt=0
for e in Data:
  iEvt+=1
  if e.mll_pp < 0 : continue

  nbin[0] += 1
  jetpt_probe[0]=e.pjetpt_pp
  btag_probe[0]=e.btagp_pp
  btag_tag[0]=e.btagt_pp
  mll[0]=e.mll_pp
  mth[0]=e.mth_pp
  
  outTreeData_pp.Fill()

outFile_pp.Write()
outFile_pp.Close()

############################  DATA FP ############################
pjetpt = n.zeros(1, dtype=float)
btag_probe = n.zeros(1, dtype=float)
btag_tag = n.zeros(1, dtype=float)
mll = n.zeros(1, dtype=float)
mth = n.zeros(1, dtype=float)
nbin = n.zeros(1, dtype=float)

outFile_fp = TFile("data_fp.root","recreate")
outTreeData_fp = TTree("Data","Tree with data_pp and data_fp")
outTreeData_fp.Branch('jetpt_probe', jetpt_probe, 'jetpt_probe/D')
outTreeData_fp.Branch('btag_probe', btag_probe, 'btag_probe/D')
outTreeData_fp.Branch('btag_tag', btag_tag, 'btag_tag/D')
outTreeData_fp.Branch('mll', mll, 'mll/D')
outTreeData_fp.Branch('mth', mth, 'mth/D')
outTreeData_fp.Branch('nbin', nbin, 'nbin/D')


iEvt=0
for e in Data:
  iEvt+=1
  if e.mll_fp < 0 : continue

  nbin[0] += 1
  pjetpt[0]=e.pjetpt_fp
  jetpt_probe[0]=e.pjetpt_fp
  btag_probe[0]=e.btagp_fp
  btag_tag[0]=e.btagt_fp
  mll[0]=e.mll_fp
  mth[0]=e.mth_fp

  outTreeData_fp.Fill()


outFile_fp.Write()
outFile_fp.Close()


TTbar = TChain("TTbar")
TTbar.Add("/home/lviliani/Documenti/TagAndProbe/looseTag_volta_buona_davvero_davvero/QUESTA_E_DAVVERO_DAVVERO_LA_VOLTA_BUONA.root")
############################  TTBAR PP ############################
pjetpt = n.zeros(1, dtype=float)
weight = n.zeros(1, dtype=float)
btag_probe = n.zeros(1, dtype=float)
btag_tag = n.zeros(1, dtype=float)
mll = n.zeros(1, dtype=float)
mth = n.zeros(1, dtype=float)
nbin = n.zeros(1, dtype=float)

outFile_ttbar_pp = TFile("ttbar_pp.root","recreate")
outTreeTTbar_pp = TTree("TTbar","")
outTreeTTbar_pp.Branch('jetpt_probe', jetpt_probe, 'jetpt_probe/D')
outTreeTTbar_pp.Branch('weight', weight, 'weight/D')
outTreeTTbar_pp.Branch('btag_probe', btag_probe, 'btag_probe/D')
outTreeTTbar_pp.Branch('btag_tag', btag_tag, 'btag_tag/D')
outTreeTTbar_pp.Branch('mll', mll, 'mll/D')
outTreeTTbar_pp.Branch('mth', mth, 'mth/D')
outTreeTTbar_pp.Branch('nbin', nbin, 'nbin/D')


iEvt=0
for e in TTbar:
  iEvt+=1
  if e.mll_pp < 0 : continue

  if e.isb < 1 : continue

  nbin[0] += 1
  weight[0]=e.weight_pp
  jetpt_probe[0]=e.pjetpt_pp
  btag_probe[0]=e.btagp_pp
  btag_tag[0]=e.btagt_pp
  mll[0]=e.mll_pp
  mth[0]=e.mth_pp

  outTreeTTbar_pp.Fill()

outFile_ttbar_pp.Write()
outFile_ttbar_pp.Close()

############################ TTBAR FP  ############################
pjetpt = n.zeros(1, dtype=float)
weight = n.zeros(1, dtype=float)
btag_probe = n.zeros(1, dtype=float)
btag_tag = n.zeros(1, dtype=float)
mll = n.zeros(1, dtype=float)
mth = n.zeros(1, dtype=float)
nbin = n.zeros(1, dtype=float)

outFile_ttbar_fp = TFile("ttbar_fp.root","recreate")
outTreeTTbar_fp = TTree("TTbar","")
outTreeTTbar_fp.Branch('jetpt_probe', jetpt_probe, 'jetpt_probe/D')
outTreeTTbar_fp.Branch('weight', weight, 'weight/D')
outTreeTTbar_fp.Branch('btag_probe', btag_probe, 'btag_probe/D')
outTreeTTbar_fp.Branch('btag_tag', btag_tag, 'btag_tag/D')
outTreeTTbar_fp.Branch('mll', mll, 'mll/D')
outTreeTTbar_fp.Branch('mth', mth, 'mth/D')
outTreeTTbar_fp.Branch('nbin', nbin, 'nbin/D')

iEvt=0
for e in TTbar:
  iEvt+=1
  if e.mll_fp < 0 : continue

  if e.isb < 1 : continue

  nbin[0] += 1
  weight[0]=e.weight_fp
  jetpt_probe[0]=e.pjetpt_fp
  btag_probe[0]=e.btagp_fp
  btag_tag[0]=e.btagt_fp
  mll[0]=e.mll_fp
  mth[0]=e.mth_fp

  outTreeTTbar_fp.Fill()

outFile_ttbar_fp.Write()
outFile_ttbar_fp.Close()


Bkg = TChain("OtherBkg")
Bkg.Add("/home/lviliani/Documenti/TagAndProbe/looseTag_volta_buona_davvero_davvero/QUESTA_E_DAVVERO_DAVVERO_LA_VOLTA_BUONA.root")

TTbar_bkg = TChain("TTbar")
TTbar_bkg.Add("/home/lviliani/Documenti/TagAndProbe/looseTag_volta_buona_davvero_davvero/QUESTA_E_DAVVERO_DAVVERO_LA_VOLTA_BUONA.root")
############################ BKG PP  ############################
pjetpt = n.zeros(1, dtype=float)
weight = n.zeros(1, dtype=float)
btag_probe = n.zeros(1, dtype=float)
btag_tag = n.zeros(1, dtype=float)
mll = n.zeros(1, dtype=float)
mth = n.zeros(1, dtype=float)
nbin = n.zeros(1, dtype=float)

outFile_bkg_pp = TFile("bkg_pp.root","recreate")
outTreeBkg_pp = TTree("OtherBkg","")
outTreeBkg_pp.Branch('jetpt_probe', jetpt_probe, 'jetpt_probe/D')
outTreeBkg_pp.Branch('weight', weight, 'weight/D')
outTreeBkg_pp.Branch('btag_probe', btag_probe, 'btag_probe/D')
outTreeBkg_pp.Branch('btag_tag', btag_tag, 'btag_tag/D')
outTreeBkg_pp.Branch('mll', mll, 'mll/D')
outTreeBkg_pp.Branch('mth', mth, 'mth/D')
outTreeBkg_pp.Branch('nbin', nbin, 'nbin/D')

iEvt=0
for e in Bkg:
  iEvt+=1
  if e.mll_pp < 0 : continue

  nbin[0] += 1
  weight[0]=e.weight_pp
  jetpt_probe[0]=e.pjetpt_pp
  btag_probe[0]=e.btagp_pp
  btag_tag[0]=e.btagt_pp
  mll[0]=e.mll_pp
  mth[0]=e.mth_pp

  outTreeBkg_pp.Fill()

iEvt=0
for e in TTbar_bkg:
  print 'ciao'
  iEvt+=1
  if e.mll_pp < 0 : continue

  if e.isb==1 : continue

  nbin[0] += 1
  weight[0]=e.weight_pp
  jetpt_probe[0]=e.pjetpt_pp
  btag_probe[0]=e.btagp_pp
  btag_tag[0]=e.btagt_pp
  mll[0]=e.mll_pp
  mth[0]=e.mth_pp

  outTreeBkg_pp.Fill()


outFile_bkg_pp.Write()
outFile_bkg_pp.Close()

############################ BKG FP  ############################
pjetpt = n.zeros(1, dtype=float)
weight = n.zeros(1, dtype=float)
btag_probe = n.zeros(1, dtype=float)
btag_tag = n.zeros(1, dtype=float)
mll = n.zeros(1, dtype=float)
mth = n.zeros(1, dtype=float)
nbin = n.zeros(1, dtype=float)

outFile_bkg_fp = TFile("bkg_fp.root","recreate")
outTreeBkg_fp = TTree("OtherBkg","")
outTreeBkg_fp.Branch('jetpt_probe', jetpt_probe, 'jetpt_probe/D')
outTreeBkg_fp.Branch('weight', weight, 'weight/D')
outTreeBkg_fp.Branch('btag_probe', btag_probe, 'btag_probe/D')
outTreeBkg_fp.Branch('btag_tag', btag_tag, 'btag_tag/D')
outTreeBkg_fp.Branch('mll', mll, 'mll/D')
outTreeBkg_fp.Branch('mth', mth, 'mth/D')
outTreeBkg_fp.Branch('nbin', nbin, 'nbin/D')

iEvt=0
for e in Bkg:
  iEvt+=1
  if e.mll_fp < 0 : continue

  nbin[0] += 1
  weight[0]=e.weight_fp
  jetpt_probe[0]=e.pjetpt_fp
  btag_probe[0]=e.btagp_fp
  btag_tag[0]=e.btagt_fp
  mll[0]=e.mll_fp
  mth[0]=e.mth_fp

  outTreeBkg_fp.Fill()

iEvt=0
for e in TTbar_bkg:
  iEvt+=1
  if e.mll_pp < 0 : continue

  if e.isb==1 : continue

  nbin[0] += 1
  weight[0]=e.weight_pp
  jetpt_probe[0]=e.pjetpt_pp
  btag_probe[0]=e.btagp_pp
  btag_tag[0]=e.btagt_pp
  mll[0]=e.mll_pp
  mth[0]=e.mth_pp

  outTreeBkg_fp.Fill()


outFile_bkg_fp.Write()
outFile_bkg_fp.Close()
