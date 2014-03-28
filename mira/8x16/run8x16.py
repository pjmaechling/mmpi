#/usr/bin/env python
import os

# Use a NodesXCores convention
# run2X16 gives 32 cores on 2 nodes with 16 cores per node.
#
if __name__ == "__main__":
  os.system('qsub -A Equake_SS -n 8 -t 30 --mode c16 mmpi')
