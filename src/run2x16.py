#/usr/bin/env python
import os

#
# script Name Convention is NodeXCores
# EG. run2x16.py will run on 32 cores on 2 nodes with 16 cores per node.
# run32x1.py will run on 32 cores on 32 nodes with 1 core per node.
#
if __name__ == "__main__":
  os.system('qsub -A Equake_SS -n 2 -t 10 --mode c16 mmpi')
