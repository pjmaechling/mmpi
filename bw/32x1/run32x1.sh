#!/bin/bash
#PBS -l walltime=00:10:00
#PBS -l nodes=32:ppn=1:xe
#PBS -e err
#PBS -o out
#PBS -q normal
#PBS -A jmz
##
## aprun -n 4 (Width as in number of PE)
## qsub -l mppwidth=4
##
## aprun -N 1 (Number of PEs per node)
## qsub -l mppnppn=1
## cd $PBS_O_WORKDIR
## echo $PBS_O_WORKDIR
date
aprun -n 32  -N 1 ./mmpi
