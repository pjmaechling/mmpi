#!/bin/bash
#PBS -l walltime=00:10:00
## min run time on bw 30 min
#PBS -l nodes=1:ppn=32:xe
#PBS -e $PBS_JOBID.err
#PBS -o $PBS_JOBID.out
#PBS -q normal
#PBS -A jmz
cd $PBS_O_WORKDIR
echo $PBS_O_WORKDIR
## aprun -n 4 (Width as in number of PE)
## aprun -N 1 (Number of PEs per node)
date
aprun -n 32 -N 32 ./mmpi
