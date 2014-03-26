/*
  Program: Measure MPI (mmpi.c)
  Author: Philip Maechling
  Purpose: Define max measurements for mmpi.h program
*/

/* Assume MAX Procs is 100,then the number of comm paths we measure is */
/* (defined below) is 100+99+98... */
/* I believe this is a triangular number (n2 + n)/2 */ 
/* Working this out for MAX proc = 100 gives us 5050 */

/* #define MAX_COMM_PATHS ((MAX_PROCS * MAX_PROCS) + MAX_PROCS)/2 */
#define MAX_COMM_PATHS 5050

/* This measure will assume that all msgs are the same length (msgLen) */

struct comm_path_measures
{
  int start_rank;
  int end_rank;
  int msgLen;
  int totalMsgs;
  double totalTime;
  double totalTime2;
} comm_path_info[MAX_COMM_PATHS];
