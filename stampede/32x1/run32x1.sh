#!/bin/bash
#SBATCH -J mmpi           # job name
#SBATCH -o myMPI.o%j      # output and error file name (%j expands to jobID)
#SBATCH -n 32             # total number of mpi tasks requested
#SBATCH -N 32             # Total number of nodes
#SBATCH -p normal         # queue (partition) -- normal, development, etc.
#SBATCH -t 00:10:00       # run time (hh:mm:ss) - 1.5 hours
ibrun ./mmpi              # run the MPI executable named a.out
