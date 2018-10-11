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

  int priority;
};

struct Memory{
  long long int nanoseconds;
  long long int seconds;
  struct ProcessBlock processBlock[];
  
};
struct Memory *shmPTR;
 sem_t* sem;



int main(){
  
  key_t  ShmKEY;
  int ShmID;
  printf("hello");
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

    
                  
     sem = sem_open("pSem3",0);
     sem_wait(sem);
     shmPTR->processBlock[0].priority = 1;                                     
        sem_post(sem);                                                                  
        sem_close(sem);
        shmdt((void *) shmPTR);
  
                                                                                                           
   exit(0);
   }
                               
