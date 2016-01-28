#!/usr/bin/env python

from math import *

asimov={
's':  17209., 'e_s': 919.,
'b':  39432., 'e_b': 931.,
'eff_s':  0.753, 'e_eff_s': 0.038,
'eff_b':  0.1085, 'e_eff_b': 0.0072
}

zup={
's':  16741., 'e_s': 727.,
'b':  43881., 'e_b': 743.,
'eff_s':  0.776, 'e_eff_s': 0.031,
'eff_b':  0.1128, 'e_eff_b': 0.0069
}

zdown={
's':  17672., 'e_s': 887.,
'b':  34989., 'e_b': 896.,
'eff_s':  0.732, 'e_eff_s': 0.035,
'eff_b':  0.1031, 'e_eff_b': 0.0079
}

STup={
's':  17290., 'e_s': 927.,
'b':  39478., 'e_b': 940.,
'eff_s':  0.752, 'e_eff_s': 0.038,
'eff_b':  0.1085, 'e_eff_b': 0.0072
}

STdown={
's':  17124., 'e_s': 934.,
'b':  39389., 'e_b': 946.,
'eff_s':  0.754, 'e_eff_s': 0.039,
'eff_b':  0.1086, 'e_eff_b': 0.0072
}

data={
's':  18978., 'e_s': 845.,
'b':  28614., 'e_b': 851.,
'eff_s':  0.595, 'e_eff_s':  0.026,
'eff_b':  0.1025, 'e_eff_b': 0.0089
}


def error(parameter):
  stat=data['e_'+parameter]
  zfrac_perc  = max(abs(zup[parameter]-asimov[parameter]), abs(zdown[parameter]-asimov[parameter]))/asimov[parameter]  
  STfrac_perc = max(abs(STup[parameter]-asimov[parameter]), abs(STdown[parameter]-asimov[parameter]))/asimov[parameter] 
  zfrac = zfrac_perc*data[parameter]
  STfrac = STfrac_perc*data[parameter]
  theError = sqrt(stat**2 + zfrac**2 + STfrac**2)
  return theError


variables = ['s', "b", "eff_s", "eff_b"]

for var in variables:
  print var, data[var], "+/-", error(var), " MC:", asimov[var], "  SF:", data[var]/asimov[var], "+/-", error(var)/asimov[var] 
