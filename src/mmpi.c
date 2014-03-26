/*
  Program: Measure MPI (mmpi.c)
  Author: Philip Maechling
  Purpose: Measure MPI comm time between cores in cluster
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "mpi.h"
#include "mmpi.h"
#include "mmsg.h"

int main(int argc, char *argv[]) 
{
  int i,curlorank,nextrank;
  int measuring_comm;
  double com,com1;
  int ranklo,rankhi;
  int reslen,rxlen;
  char procname[MAX_CHARS_IN_PROCNAME];
  char rxprocname[MAX_CHARS_IN_PROCNAME];
  MPI_Status status;
  struct hostent* thisHost;

  if(argc != 1)
  {
    printf("Usage: mmpi\n");
    exit(-1);
  }

  /* Initialize MPI */

  MPI_Init(&argc,&argv);

  /* Determine max and current rank */

  MPI_Comm_size(MPI_COMM_WORLD,&maxrank);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Get_processor_name(procname,&reslen);  

  /* Determine Host name*/
  gethostname(hostname,MAXHOSTNAMELEN);
  if(myid == 0)
  {
    printf("hostname - %s reslen - %d\n",hostname, reslen);
  }

  /* Collect processor information for rank on rank 0 */
  /* This is used to label the physical nodes on the output */
  /* This depends on a MPI message exchange from all nodes to rank 0 node */
  for(i=0;i<maxrank;i++)
  {
    if(myid == 0)
    {
      if(i==0)
      {
       memcpy(procs[0].name,procname,reslen+1);
       procs[0].rank = 0;
      }
      else
      {
        MPI_Recv(rxprocname, MAX_CHARS_IN_PROCNAME, MPI_CHAR, i,10, MPI_COMM_WORLD, &status);

        /* find length of bcast string */
        for(rxlen=0;rxlen<MAX_CHARS_IN_PROCNAME;rxlen++)
        {
          if(rxprocname[rxlen] == 0)
          {
            break;
          }
        }
        memcpy(procs[i].name,rxprocname,rxlen);
        procs[i].rank=i;
      }
    }
    else if(myid==i)
    {
      MPI_Send(procname, MAX_CHARS_IN_PROCNAME, MPI_CHAR,0,10, MPI_COMM_WORLD);
    }
  }

  /* Sync all Processes now that processer info has been exchanged by ranks */

  MPI_Barrier(MPI_COMM_WORLD);

  /* The path_count counter indicates how many processor to processor links we */
  /* test per repitition. It gets reset on each repitition but is correct at the */
  /* end of the last one */

  total_path_count = 0;
  for (curlorank=0;curlorank<maxrank;curlorank++)
  {
    for (nextrank=curlorank;nextrank<maxrank;nextrank++)
    {
      comm_path_info[total_path_count].totalMsgs = 0;
      comm_path_info[total_path_count].totalTime = 0.0;
      comm_path_info[total_path_count].totalTime2 = 0.0;
      ++total_path_count;
    }
  }
  if (myid == 0)
  {
    printf("Total paths under test: %d\n",total_path_count);
  }

  /* Top level repeat which will take use through the total number of repeat times */
  /* We run through the ranks then repeat in order to reduce buffering issues */

  for (rep_count=0;rep_count<repeatNum;rep_count++)
  {
    /* rep_count indicates which of the total number of repeat intervals */
    /* we are currently measuring. */

    cur_path_count = 0;

    for (curlorank=0;curlorank<maxrank;curlorank++)
    {
      for (nextrank=curlorank;nextrank<maxrank;nextrank++)
      {
        if ((myid == curlorank) && (myid == nextrank))
	    {
	       /* this is a process sending a message to itself */
	       /* We consider this an invalid case and set values to 0.0 */
	       p2p_msgs(curlorank,nextrank,results);
	    }
	    else if (myid == curlorank)
        {
	      /* We use the convention that the low rank always initiate the send */
	      /* the message traffic is round trip so upper never initiates msgs*/
	      tx_msgs(curlorank,nextrank,results);
	    }
	    else if (myid == nextrank)
	    {
	      /*This is the case where we are receiving message from the low rank */
	      rx_msgs(curlorank,nextrank);
	    }
	    else
        {
	      /* In some cases, our process is not sending or receiving */
	      /*In this case, we go to a barrier and wait for the other processes */
	      /* to do the exchange and then continue */
	      stay_in_sync();
        }

	    /* Collect measures is another MPI exchange. The data just collected */
	    /* is sent back to the rank 0 processes to help build reports */
	    /* Sync all processes, then the rank 0 and lo_rank processes */
	    /* send data to rank 0 for reporting */
        MPI_Barrier(MPI_COMM_WORLD);
		collect_measures(curlorank,nextrank,results);
        ++cur_path_count;
     } /* End receive rank loop */
    } /* End send rank loop */
  } /* end repeat interval */

  MPI_Finalize();
  if (myid== 0)
  {
    summarize_results(total_path_count,comm_path_info);
  }
  exit(0);
}

void collect_measures(int lo_rank,int hi_rank,float* ires)
{
  MPI_Status status;
  float res[2];
  float t,tt;
  if(myid == 0) 
  {
    if(lo_rank == 0)
    {
      comm_path_info[cur_path_count].start_rank = lo_rank;
      comm_path_info[cur_path_count].end_rank = hi_rank;
      comm_path_info[cur_path_count].msgLen = msgLen;
      comm_path_info[cur_path_count].totalMsgs += 1;
      comm_path_info[cur_path_count].totalTime += ires[0];
      comm_path_info[cur_path_count].totalTime2 += ires[1];
    }
    else
    { 
      MPI_Recv(res,2, MPI_FLOAT,lo_rank,10, MPI_COMM_WORLD, &status);
      comm_path_info[cur_path_count].start_rank = lo_rank;
      comm_path_info[cur_path_count].end_rank = hi_rank;
      comm_path_info[cur_path_count].msgLen = msgLen;
      comm_path_info[cur_path_count].totalMsgs += 1;
      comm_path_info[cur_path_count].totalTime += res[0];
      comm_path_info[cur_path_count].totalTime2 += res[1];
    }
  }
  else if (myid == lo_rank)
  {
    MPI_Send(ires,2, MPI_FLOAT,0,10, MPI_COMM_WORLD);
  }
}

void summarize_results(int count,void* msg_stats)
{
  int i;
  FILE *fp;
  int errnum;
  char fname[MAX_CHARS_IN_PROCNAME];
  int nMsgs;
  double stddev,sumT2,avgM;
  sprintf(fname,"/projects/Equake_SS/maechlin_work/ammpi_%d_%d_%d.dat",maxrank,msgLen,repeatNum);
  fp = fopen(fname,"w");
  if(fp == NULL)
  {
    errnum = errno;
    fprintf(stderr, "value of errno: %d\n",errno);
    perror("Error printed by perror");
    fprintf(stderr, "Error opening file: %s\n", strerror(errnum));
    return;
  }
  fprintf(fp,"# Testing Hostname: %s\n",hostname);
  fprintf(fp,"# Processors: %d MsgLen %d Repeats: %d\n",
    maxrank,msgLen,repeatNum);
  fprintf(fp,"# ProcName1:rank   ProcName2:rank   AvgCommTime(usecs) StdDev\n");
  for (i=0;i<count;i++)
  {
    sumT2 = comm_path_info[i].totalTime2;
    nMsgs = comm_path_info[i].totalMsgs;
    avgM = comm_path_info[i].totalTime/nMsgs;
    stddev = sqrt( ((sumT2/nMsgs)-(avgM * avgM)) / (nMsgs-1) );
    fprintf(fp,"%s %03d  %s %03d     %6.2f  %6.2f\n",
       procs[comm_path_info[i].start_rank].name,
       comm_path_info[i].start_rank,
       procs[comm_path_info[i].end_rank].name,
       comm_path_info[i].end_rank,avgM,stddev);
  }
  fflush(fp);
  fclose(fp);
  return;
}

void p2p_msgs(int rlo, int rhi,float results[])
{
  MPI_Barrier(MPI_COMM_WORLD);
  results[0] = 0.0;
  results[1] = 0.0;
  return;
}

void tx_msgs(int rlo, int rhi,float results[])
{

  double com1 = 0.0;
  double res = 0.0;
  int n[MAX_MSG_LEN];
  MPI_Status status;

  MPI_Barrier(MPI_COMM_WORLD);
  com1 = MPI_Wtime();
  MPI_Send(&n, msgLen, MPI_INT, rhi,10, MPI_COMM_WORLD);
  MPI_Recv(&n, msgLen, MPI_INT, rhi,10, MPI_COMM_WORLD, &status);
  res = MPI_Wtime() - com1;
  res = (res * USECS_IN_SEC);
  results[0]= (float)res;
  results[1] = ((float)res * (float)res);
  return;
}

/* Rx msg hits a barrier then receives and resends a message back to sending rank */
void rx_msgs(int rlo, int rhi)
{
  int n[MAX_MSG_LEN];
  MPI_Status status;

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Recv(&n, msgLen, MPI_INT, rlo,10, MPI_COMM_WORLD, &status);
  MPI_Send(&n, msgLen, MPI_INT, rlo,10, MPI_COMM_WORLD);
  return;
}


void stay_in_sync()
{
  MPI_Barrier(MPI_COMM_WORLD);
  return;
}
