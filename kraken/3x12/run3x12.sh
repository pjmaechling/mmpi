#!/bin/bash
#PBS -l walltime=00:10:00
#PBS -A TG-MCA03S012 
#PBS -l size=36
#PBS -e $PBS_JOBID.err
#PBS -o $PBS_JOBID.out
##PBS -q normal
##PBS -A jmz
## aprun -n 4 (Width as in number of PE)
## aprun -N 1 (Number of PEs per node)
cd $SCRATCHDIR
echo $SCRATCHDIR
date
aprun -n 32 -N 12 ./mmpi
