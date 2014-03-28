#!/bin/bash
#PBS -l walltime=00:10:00
#PBS -l nodes=128:ppn=1:xe
#PBS -e $PBS_JOBID_mmpi.err
#PBS -o $PBS_JOBID_mmpi.out
#PBS -q debug
#PBS -A jmz
cd $PBS_O_WORKDIR
echo $PBS_O_WORKDIR
## aprun -n 4 (Width as in number of PE)
## aprun -N 1 (Number of PEs per node)
date
aprun -n 128 -N 1 ./mmpi
