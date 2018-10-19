
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
   int finished;
   int NextProcess;
   int processID;
   int HighStackQueue[50];
   int HighStackPointer;
   int HighStackNo;
   int LowStackQueue[50];
   int LowStackPointer;
   int lowStackNo;
   long long int nanoseconds;
   int seconds;
   int ReadyProcessID;
   int totalWaitTime;//seconds
   long long int totalWaitnano;
   int totalcpuseconds;
  int readyProcessID;
   long long int totalCPU;



};

   int ShmID;
int j;

int main(){
   sem_t *sem ;
   sem_t *sem2;
   long int getrand =  getpid();
   struct Memory *shmPTR;
   key_t ShmKEY;
   int quantum = 2000;
   int terminated = 0;
   int value = 0;
      sem2 = sem_open ("sem24", O_CREAT | O_EXCL, 0644, 0);
   
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
     shmPTR->LowStackQueue[shmPTR->lowStackNo] = shmPTR->processID;
     shmPTR->lowStackNo++;
     processID = shmPTR->processID;
//     printf("My process ID is %d\n", shmPTR->processID);
     if( shmPTR->queueNo == 1)
      quantum = 1000;
        
    while(terminated == 0){ //printf("hi");
             //if(shmPTR->lowStackNo != shmPTR->LowStackPointer)                   
              shmPTR->NextProcess = shmPTR->LowStackQueue[shmPTR->LowStackPointer];
              //else break;
        
//         fprintf(stderr,"Next process is %d\n", shmPTR->NextProcess);
         if(processID  == shmPTR->NextProcess){ 
         shmPTR->ReadyProcessID = processID;
         
                       
         printf("Process %d  is dispatched ...\n",processID);
         srand(getrand++); 
         value =  (rand()%100);
        //printf("%d",value);
        if (value <= 10)//terminate, don't run
            flag = 1;
         if((value > 10) && ( value <= 50))
            flag = 2; //run for quantume and then finish
         if((value > 50)&& ( value <= 70))
            flag = 3; //wait for an event
         if(value >70)
            flag = 4; //get preempted before finishing quantum
         

       switch(flag){
      case 1:
  
          terminated = 1;
          shmPTR->finished = 1;

         printf("Process %d terminated .. \n", processID);
         
         break;
      case 2:
//         shmPTR->ProcessesTerminated++;
         shmPTR->nanoseconds = shmPTR->nanoseconds + quantum;
         terminated = 1;
         shmPTR->finished = 1;
          //printf("Low stack pointer is  %d\n", shmPTR->LowStackPointer);
         printf("Process %d ran for %d nanoseconds and terminated.. \n",processID,quantum);
         shmPTR->totalCPU = shmPTR->totalCPU + quantum;
         
         break;
      case 3:

      srand(getrand++);
          int r = (rand()%5);
          srand(getrand++);
          int s = (rand()%1000);
          shmPTR->seconds = shmPTR->seconds + r;
          shmPTR->nanoseconds = shmPTR->nanoseconds + s;
          printf("Process %d waited for event for %d seconds, %d nanoseconds..\n", processID, r, s);
          shmPTR->finished = 0;
          terminated = 0;
          
          break;
     case 4:
          srand(getrand++);
          int p = 1 + (rand()%99);
          float percentage = (float)p/100;
          shmPTR->nanoseconds = shmPTR->nanoseconds + (long long int)(percentage*quantum);
          shmPTR->totalCPU = shmPTR->totalCPU + (long long int)percentage*quantum;
          printf("Process %d ran for %lld nanoseconds ..\n", processID, (long long int)(percentage*quantum));
          shmPTR->finished = 0;
          terminated = 0;
          
          break;
      default:
        
         printf("Error");
         break;
     }    sem = sem_open("pSem95",1);sem_post(sem); sem_close(sem);
        sem2 = sem_open("sem24",0); sem_wait(sem2);
       }
    
    

     }
    shmdt((void *) shmPTR);
    exit(0);

}
