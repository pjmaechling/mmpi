#/usr/bin/env python
import os

#
#
if __name__ == "__main__":
  os.system('qsub -A Equake_SS -n 32 -t 10 --mode c1 --env RUNJOB_MAPPING=ABCDET mmpi')
