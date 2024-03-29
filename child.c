
 #include<string.h>
 #include <stdio.h>          /* printf()                 */
 #include <stdlib.h>         /* exit(), malloc(), free() */
 #include <sys/types.h>      /* key_t, sem_t, pid_t      */
 #include <sys/shm.h>        /* shmat(), IPC_RMID        */
 #include <errno.h>          /* errno, ECHILD            */
 #include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
 #include <fcntl.h>          /* O_CREAT, O_EXEC          */
 #include <time.h>
 #include <unistd.h>
 #include <wait.h>

struct Memory{
   int queueNo;
   long long int nanoseconds;
   int seconds;   
   int totalWaitTime;
   int HighStackQueue[20];
   int LowStackQueue[20];
  int j;
  int k;
  int LoopVar;
  int highStackNo;
  int LowStackNo;
  int processID;
   long long int totalCPU;
   int terminated; //tell parent that it's done
   
};

int ShmID;


int main(){
   sem_t *sem;
   long int getrand = 0;
   struct Memory *shmPTR;
    printf("hello from exec");   
   key_t ShmKEY;
   int quantum = 2000;
   int terminated = 0;
   int value = 0;
   long long int x;
   int flag = 0;
   int processID = 0;
   ShmKEY = ftok(".",'x');
   ShmID = shmget(ShmKEY, sizeof(struct Memory), 0666);
   if (ShmID < 0){
     printf("*** shmget error (client) ***\n");
    exit(1);
    }
    shmPTR = (struct Memory *) shmat(ShmID, NULL, 0);
   if(ShmID == -1){
     printf("*** shmat error(client) ***\n");
     exit(1);
    }
     processID = shmPTR->LoopVar;
     printf("%d", shmPTR->LoopVar);
     if( shmPTR->queueNo == 1)
     quantum = 1000; 
    while(x < 100000){
      
      sem = sem_open("pSem30",0);
      sem_wait(sem);

      if(processID  == shmPTR->processID){
         printf("Process %d  is dispatched ...\n",processID);
         srand(getrand++);
         value =  (rand()%100);
         printf("%d", value);
         if (value <= 10)//terminate, don't run
            flag = 0;
         if((value > 10) && ( value <= 50))
            flag = 1; //run for quantume and then finish
         if((value > 50)&& ( value <= 70))
            flag = 2; //wait for an event
         if(value >70)
            flag = 3; //get preempted before finishing quantum
           sem_post(sem); sem_close(sem);break;}
        
         sem_post(sem);
         sem_close(sem);  
          x++; }
      switch(flag){
      case 0: 
  //       shmPTR->terminated = 1;
         terminated = 1;
         printf("Process %d terminated .. \n", processID);
         break;
      case 1:
         shmPTR->nanoseconds = shmPTR->nanoseconds + quantum;
//         shmPTR->terminated = 1;
         terminated = 1;
         printf("Process %d ran for %d nanoseconds and terminated.. \n",processID,quantum);
         shmPTR->totalCPU = shmPTR->totalCPU + quantum;
         break;
      case 2:
          srand(getrand++);
          int r = (rand()%5);
          srand(getrand++);
          int s = (rand()%1000);
          shmPTR->seconds = shmPTR->seconds + r;
          shmPTR->nanoseconds = shmPTR->nanoseconds + s;
          printf("Process %d waited for event for %d seconds, %d nanoseconds..\n", processID, r, s);
          shmPTR->terminated = 0;

  //        terminated = 0;
          break;
      case 3:
          srand(getrand++);
          int p = 1 + (rand()%99);
             float percentage = (float)p/100;
          shmPTR->nanoseconds = shmPTR->nanoseconds + (long long int)(percentage*quantum);
          shmPTR->totalCPU = shmPTR->totalCPU + (long long int)percentage*quantum;
          printf("Process %ld ran for %lld nanoseconds ..\n", processID, (long long int)(percentage*quantum));
          shmPTR->terminated = 0;

          terminated = 0;
          break;
      default:
         printf("Error");
         break;          
     }

    shmdt((void *) shmPTR);
    exit(0);
}
