#!/usr/bin/env python
"""
Description: Convert mmpi.c output.dat into matlab matrix file for plotting
Author: Philip Maechling
Date: 20 March 2008
"""
import os
import sys
from core import core
from matrix import Matrix
#
# The processor names are reported differently by each site
# Here, we define a subroutine for each site so that we can
# parse their report. The main code then must be changed to 
# call the correct subroutine when we are running at that site
#
def split_lines_hpcc(mlist):
  for x in mlist:
    nlist = []
    flist = []
    plist = []
    blist = []
    alist = []
    if x.startswith("#"):
      continue
    else:
      # split line into 6 pieces
      #print "Spliting" 
      #print x
      nlist = x.split()
      # save rank 0
      #print nlist
      plist.append(nlist[1])
      # append rank to plist
      # split to remove usc.edu
      blist = nlist[0].split(".")
      #print blist
      # append node name plist
      plist.append(blist[0])
      # save rank hi
      plist.append(nlist[3])
      # split name to remove usc.edu
      alist = nlist[2].split(".")
      # append nodename to plist
      #print alist
      plist.append(alist[0])
      # append commtime
      plist.append(nlist[4])
      #apend stdev
      plist.append(nlist[5])
      # put this list into tlist
      tlist.append(plist)
  return tlist

def split_lines_bluegene(lines):  
  for x in mlist:
    nlist = []
    flist = []
    plist = []
    blist = []
    alist = []
    if x.startswith("#"):
      continue
    else:
      # split line into 6 pieces
      #print "Spliting" 
      #print x
      ranklo = x[67:70]
      rankhi = x[139:142]
      lat = x[147:153]
      var = x[155:162]
      print "lo" + ranklo
      print "hi" + rankhi
      print "lat" + lat
      print "var" + var
      #sys.exit(0)
      # save rank 0
      #print nlist
      plist.append(ranklo)
      # append rank to plist
      # split to remove usc.edu
      #blist = nlist[0].split(".")
      #print blist
      # append node name plist
      plist.append("var")
      # save rank hi
      plist.append(rankhi)
      # split name to remove usc.edu
      #alist = nlist[2].split(".")
      # append nodename to plist
      #print alist
      plist.append("var")
      # append commtime
      plist.append(lat)
      #apend stdev
      plist.append(var)
      # put this list into tlist
      tlist.append(plist)
  return tlist

def split_lines_mira(lines):  
  for x in mlist:
    nlist = []
    flist = []
    plist = []
    blist = []
    alist = []
    if x.startswith("#"):
      continue
    else:
      # split line into 6 pieces
      #print "Spliting" 
      #print x
      vallist = x.split()
      ranklo = vallist[6]
      rankhi = vallist[13]
      lat = vallist[14]
      var = vallist[15]
      print "lo: " + ranklo
      print "hi: " + rankhi
      print "lat: " + lat
      print "var: " + var
      #sys.exit(0)
      # save rank 0
      #print nlist
      plist.append(ranklo)
      # append rank to plist
      # split to remove usc.edu
      #blist = nlist[0].split(".")
      #print blist
      # append node name plist
      plist.append("var")
      # save rank hi
      plist.append(rankhi)
      # split name to remove usc.edu
      #alist = nlist[2].split(".")
      # append nodename to plist
      #print alist
      plist.append("var")
      # append commtime
      plist.append(lat)
      #apend stdev
      plist.append(var)
      # put this list into tlist
      tlist.append(plist)
  return tlist


#
# Start of main routine
#
if not len(sys.argv) == 2:
  print "mmpi2mat.py cores"
  sys.exit(-1)

#
# Use cores from cmd line
# Add sub to read mmpi.h and extract msgLen and repeatMsg
#
cores = sys.argv[1]
mlen = 64
reps = 100

ifname = "../mira/ammpi_%s_%s_%s.dat"%(cores,mlen,reps)
opfname = "../mira/ammpi_%s_%s_%s_comm.dat"%(cores,mlen,reps)
ovfname = "../mira/ammpi_%s_%s_%s_var.dat"%(cores,mlen,reps)

#
# Read the report from the mmpi program
#
try:
  f = open(ifname,"r")
except IOError:
  print "Unable to find file %s - exiting"%(ifname)
  sys.exit(-1)

mlist = f.readlines()
f.close()

#
# Split the lines into fields we need
#
tlist = []
tlist = split_lines_mira(mlist)

#
# First Create the list of nodes
# There should be no duplicates in this list
#

nodelist = {}
for c in tlist:
  # use rank as key
  key = c[0]
  mcore = core()
  mcore.lo_rank = int(c[0])
  mcore.lo_name = c[1]
  mcore.hi_rank = int(c[2])
  mcore.hi_name = c[3]
  mcore.commtime = float(c[4])
  mcore.stddev = float(c[5])
  nodelist[key] = mcore


#
# The nodelist eliminates distrition from 0 to 0
# This gives us N-1. We increment matrix by 1 to fill this
#
matsize = len(nodelist)
matsize = matsize
mat = Matrix(matsize,matsize)

arclist = []
for c in tlist:
  mcore = core()
  mcore.lo_rank = int(c[0])
  mcore.lo_name = c[1]
  mcore.hi_rank = int(c[2])
  mcore.hi_name = c[3]
  mcore.commtime = float(c[4])
  mcore.stddev = float(c[5])
  arclist.append(mcore)

nf = open(opfname,"w")
for a in arclist:
  mat.setitem(a.lo_rank+1,a.hi_rank+1,a.commtime)
  mat.setitem(a.hi_rank+1,a.lo_rank+1,a.commtime)
mstr = "%s"%(mat)
nf.write(mstr) 
nf.close()

nf = open(ovfname,"w")
mat = Matrix(matsize,matsize)

for a in arclist:
  mat.setitem(a.lo_rank+1,a.hi_rank+1,a.stddev)
  mat.setitem(a.hi_rank+1,a.lo_rank+1,a.stddev)
mstr = "%s"%(mat)
nf.write(mstr) 
nf.close()
sys.exit(0)
