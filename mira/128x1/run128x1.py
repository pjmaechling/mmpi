#/usr/bin/env python
import os

if __name__ == "__main__":
  os.system('qsub -A Equake_SS -n 128 -t 30 --mode c1 mmpi')
