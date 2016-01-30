#!/usr/bin/env python

from math import *
from ROOT import *

asimov={
  "file": "mc_jetptAndMll_probe_RunII_ntoys_0_Z0.000000_ST0.000000.root",
}

zup={
  "file": "mc_jetptAndMll_probe_RunII_ntoys_0_Z0.300000_ST0.000000.root",
}

zdown={
  "file": "mc_jetptAndMll_probe_RunII_ntoys_0_Z-0.300000_ST0.000000.root"
}

STup={
  "file": "mc_jetptAndMll_probe_RunII_ntoys_0_Z0.000000_ST0.200000.root"
}

STdown={
  "file": "mc_jetptAndMll_probe_RunII_ntoys_0_Z0.000000_ST-0.200000.root"
}

data={
  "file": "data_jetptAndMll_probe_RunII_above30.root"
}

fitpars = [
  "bTT",
  "bZ",
  "efficiency_b",
  "efficiency_s",
  "sTT",
  "sZ",
]


def extractFit(sample):
  filein = TFile(sample['file'])
  fitres = filein.Get("fitresult_total_fit_hdata")
  for par in fitpars:
    variable = fitres.floatParsFinal().find(par)
    value = variable.getValV()
    error = variable.getError()
    print sample['file'], par, value, "+/-", error
    sample[par] = value
    sample["e_"+par] = error

def error(parameter):
  stat=data['e_'+parameter]
  #zfrac_perc  = max(abs(zup[parameter]-asimov[parameter]), abs(zdown[parameter]-asimov[parameter]))/asimov[parameter]  
  STfrac_perc = max(abs(STup[parameter]-asimov[parameter]), abs(STdown[parameter]-asimov[parameter]))/asimov[parameter] 
  #zfrac = zfrac_perc*data[parameter]
  STfrac = STfrac_perc*data[parameter]
  #theError = sqrt(stat**2 + zfrac**2 + STfrac**2)
  theError = sqrt(stat**2 + STfrac**2)
  return theError

extractFit(asimov)
#extractFit(zup)
#extractFit(zdown)
extractFit(STup)
extractFit(STdown)
extractFit(data)

for var in fitpars:
  print var, data[var], "+/-", error(var), " MC:", asimov[var], "  SF:", data[var]/asimov[var], "+/-", error(var)/asimov[var] 
