/*
  Program: Measure MPI (mmpi.h)
  Author: Philip Maechling
  Purpose: Measure communication time between MPI processes
*/

/* Define limits of Msgs and Repeats */

#define MAX_MSG_LEN 10000 /* Max msg len in bytes */
#define MAX_REPEAT_NUMBER 100
#define MAX_PROCS 100
#define MAX_CHARS_IN_PROCNAME 100
#define USECS_IN_SEC 1000000
#define MAXHOSTNAMELEN 100

/* Define structures used to collect information */

typedef struct
{
  char name[MAX_CHARS_IN_PROCNAME];
  int rank;
} proc;

/* Define globally accessible variables */

proc procs[MAX_PROCS];
int maxrank;
float results[2]; /* avg comm time, comm time sqrd */
int myid; /* global access to current rank */
int rep_count; /* Counts which repeat we are on in the total repeatNum */
int total_path_count; /* Count of the number of comm paths being measured */
int cur_path_count; /* Count of the number of comm paths being measured */
int len;
char hostname[MAXHOSTNAMELEN];

/* Define the global values for msgLen and repeatNum */
/* If these change frequently, these can be put into a config file */
int msgLen = 64;
int repeatNum = 100;

/* Define methods used in main program */

void stay_in_sync();
void p2p_msgs(int rlo, int rhi,float* results);
void tx_msgs(int rlo, int rhi,float* results);
void rx_msgs(int rlo, int rhi);
void summarize_results(int count,void* msginfo);
void collect_measures(int lorank,int hirank,float* results);
