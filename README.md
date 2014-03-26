mmpi
====

Simple MPI Communications Time and Variability Measurements

mmpi.c is the top level program

Measure MPI

This does an all to all tx and rx and times the 
message delivery time.

The output files are post processed into plots showing core to core 
travel time

author:
Philip James Maechling
pjmaechling@yahoo.com


Example Call:
%qsub -A Equake_SS -n 2 -t 10 --mode c16 mmpi # Measures 32 cores on 2 nodes
%qsub -A Equake_SS -n 32 -t 10 —mode c1 mmpi # Measures 32 cores on 32 nodes


Please note the program limits defined in the mmpi.h amd mmsg.h 
configuration files. These can be changed
but then the program must be recompiled

Note MAX_Processors defined in mmsg.h
Max processors defines to be 100
Max comm paths 5050
