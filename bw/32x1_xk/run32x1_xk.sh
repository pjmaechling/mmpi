#!/bin/bash
#PBS -l walltime=00:10:00
#PBS -l nodes=32:ppn=1:xk
#PBS -e $PBS_JOBID.err
#PBS -o $PBS_JOBID.out
#PBS -q debug
#PBS -A jmz
##
## aprun -n 4 (Width as in number of PE)
## aprun -N 1 (Number of PEs per node)
cd $PBS_O_WORKDIR
echo $PBS_O_WORKDIR
date
aprun -n 32  -N 1 ./mmpi
