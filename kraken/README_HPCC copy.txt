README.txt

HPCC has a complex number of processors types. Tom Jordan’s NBNS queue is AMD Optereons but these are known to be slower than the new cores. The main queue seems to share both AMD and XEONS. I believe the 32x1_main queue shows use of multiple types of cores AMD and XEONS in same run.
Also, we tried comparing the myrinet versus infinitband comm, using Jordan’s queue, but only the myrinet ran. It is possible these nodes do not have inifiniband.