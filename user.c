
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
   int  NoTerminated;
   int NextProcess;
   int processID;
   int HighStackQueue[150];
   int HighStackPointer;
   int HighStackNo;
   int LowStackQueue[150];
   int LowStackPointer;
   int lowStackNo;
   long long int nanoseconds;
   long int seconds;
   int ReadyProcessID;
   int totalWaitTime;//seconds
   long long int totalWaitnano;
   int totalcpuseconds;
  int readyProcessID;
   long long int totalCPU;



};

   int ShmID;


int main(){
   sem_t *sem ;
   sem_t *sem2;
   sem_t *sem3;
   long int getrand =  getpid();
   struct Memory *shmPTR;
   key_t ShmKEY;
   int quantum = 2000;
   int terminated = 0;
   int value = 0;
     sem2 = sem_open ("pSem503", O_CREAT | O_EXCL, 0644, 0);
          sem_close(sem2);
   //long long int x = 0;
   int flag = 0;
   //int NextProcess = 0;
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
     sem  = sem_open("pSem8", 0); sem_wait(sem);
     shmPTR->LowStackQueue[shmPTR->lowStackNo] = shmPTR->processID;
     shmPTR->lowStackNo++;
     processID = shmPTR->processID; sem_post(sem); sem_close(sem);
     fprintf(stderr,"My process ID is %d\n", shmPTR->processID);
     if( shmPTR->queueNo == 1)
      quantum = 1000;
        
    while(terminated == 0){ 
         shmPTR->NextProcess = shmPTR->LowStackQueue[shmPTR->LowStackPointer];
              
         if(processID  == shmPTR->NextProcess){ 
                       
         printf("Process %d  is dispatched ...\n",processID);
         srand(getrand++); 
         value =  (rand()%100);
         //printf("%d",value);
        if (value <= 5)//terminate, don't run
            flag = 1;
         if((value > 5) && ( value <= 40))
            flag = 2; //run for quantume and then finish
         if((value >  40)&& ( value <= 70))
            flag = 3; //wait for an event
         if(value >70)
            flag = 4; //get preempted before finishing quantum
         
       sem = sem_open("pSem8", 0);  sem_wait(sem);

       switch(flag){
      case 1:
          shmPTR->NoTerminated++;
          terminated = 1;
          shmPTR->LowStackPointer++;
         printf("Process %d terminated .. \n", processID);
         
         break;
      case 2:
          shmPTR->NoTerminated++;
//         shmPTR->nanoseconds = shmPTR->nanoseconds + quantum;
         terminated = 1;
         shmPTR->LowStackPointer++;
         printf("Process %d ran for %d nanoseconds and terminated.. \n",processID,quantum);
  //       shmPTR->totalCPU = shmPTR->totalCPU + quantum;
         
         break;
      case 3:

      srand(getrand++);
          int r = (rand()%5);
          srand(getrand++);
          int s = (rand()%1000);
    //      shmPTR->seconds = shmPTR->seconds + r;
      //    shmPTR->nanoseconds = shmPTR->nanoseconds + s;
          shmPTR->LowStackPointer++;
          shmPTR->LowStackQueue[shmPTR->lowStackNo] = processID;
          shmPTR->lowStackNo++;
          printf("Process %d waited for event for %d seconds, %d nanoseconds..\n", processID, r, s);
//          shmPTR->finished = 0;
          terminated = 0;
          
          break;
     case 4:
          srand(getrand++);
          int p = 1 + (rand()%99);
          float percentage = (float)p/100;
        //  shmPTR->nanoseconds = shmPTR->nanoseconds + (long long int)(percentage*quantum);
         // shmPTR->totalCPU = shmPTR->totalCPU + (long long int)percentage*quantum;
          shmPTR->LowStackPointer++;
          shmPTR->LowStackQueue[shmPTR->lowStackNo] = processID;
          shmPTR->lowStackNo++;
          printf("Process %d ran for %lld nanoseconds ..\n", processID, (long long int)(percentage*quantum));
          
          terminated = 0;
          
          break;
      default:
        
         printf("Error");
         break;
     }    sem_post(sem); sem_close(sem);   sem2 = sem_open("pSem503", 0); sem_wait(sem2); 
        }
         
    

     }
    shmdt((void *) shmPTR);
    exit(0);

}
