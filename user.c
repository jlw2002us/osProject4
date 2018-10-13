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
   int flag;
   int ProcessID;
   int ready;
};

struct Memory{
  long long int nanoseconds;
  int seconds;
  
  struct ProcessBlock processBlock[];
   
};
struct Memory *shmPTR;
 sem_t* sem;



int main(int argc, char* argv[]){
  printf("hello");  
  key_t  ShmKEY;
  int ShmID;
   int n = atoi(argv[1]);
   int quantum = 2000;
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
   if(shmPTR->processBlock[n].queueNo == 1)
     quantum = 1000;

     long long int x =0;
     while(x < 10000000){
       
       sem = sem_open("pSem7",0);
       sem_wait(sem);
      
         if(shmPTR->processBlock[n].ready == 1)
          { printf("Process no %d is running ...\n", shmPTR->processBlock[n].ProcessID);
            int value = 0;
            value =  (rand()%100);
            if (value <= 10)//terminate, don't run
              shmPTR->processBlock[0].flag = 0;
            if((value > 10) && ( value <= 50))
              shmPTR->processBlock[0].flag = 1; //run for quantume and then finish
            if((value > 50)&& ( value <= 70))
               shmPTR->processBlock[0].flag = 2; //wait for an event
             if(value >70)
               shmPTR->processBlock[0].flag = 3; //get preempted before finishing quantum

           break;}                                    
        sem_post(sem);                                                                  
        sem_close(sem);
        
  
                 x++; }
   
    shmPTR->seconds = 2;
    sem_post(sem);
    sem_close(sem);

    shmdt((void *) shmPTR);
    exit(0);
   }
                               
