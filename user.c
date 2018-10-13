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


 struct ProcessBlock{
  int queueNo;
   
   int ready;
 };

 struct Memory{
   long long int nanoseconds;
   int seconds;
   int totalWaitTime;
   long long int totalCPU;
   int terminated; //tell parent that it's done
   struct ProcessBlock processBlock;   
 };
 struct Memory *shmPTR;
  sem_t* sem;



 int main(int argc, char* argv[]){
    long long int y = 0;   
    key_t  ShmKEY;
    int ShmID;
    int value = 0;
    int flag = 0;
    long int getrand = getpid();
    int processID = atoi(argv[1]);
    int quantum = 2000;
    long long int CPUused = 0;     
    long long int x;
    long long int arrival_nanoseconds = 0;
    int arrival_seconds = 0;
   
   
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
   
   if( shmPTR->processBlock.queueNo == 1)
     quantum = 1000;    
   while(shmPTR->terminated == 0){               
    while(x < 100000){
       printf("hi");       
      sem = sem_open("pSem15",0);
      sem_wait(sem);
      
      if(shmPTR->processBlock.ready == 1)
        { printf("Process %d  is dispatched ...\n", processID);
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
            break;}                                    
         sem_post(sem);                                                                  
         sem_close(sem);
//        printf("%d",flag);
  
          x++; }
          
    switch(flag){
      case 0: 
         shmPTR->terminated = 1;
         printf("Process %d terminated .. \n", processID);
         break;
      case 1:
         shmPTR->nanoseconds = shmPTR->nanoseconds + quantum;
         shmPTR->terminated = 1;
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
          break;
      case 3:
          srand(getrand++);
          int p = 1 + (rand()%99);
          float percentage = (float)p/100;
          shmPTR->nanoseconds = shmPTR->nanoseconds + (long long int)(percentage*quantum);
          shmPTR->totalCPU = shmPTR->totalCPU + (long long int)percentage*quantum;
          printf("Process %d ran for %lld nanoseconds ..\n", processID, (long long int)(percentage*quantum));
          shmPTR->terminated = 0;
          break;
      default:
         printf("Error");
         break;          
     }
    sem_post(sem);
    sem_close(sem);
     if(shmPTR->terminated == 1)
       break;
     }
 
         shmdt((void *) shmPTR);
   
                                                                                                            
    exit(0);
    }
              
