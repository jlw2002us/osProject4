#include <string.h>
 #include <ctype.h>
 #include <signal.h>
 #include <stdbool.h>
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
   int HighStackQueue[200];
   int HighStackPointer;
   int HighStackNo;
//   long long int x;
   int LowStackQueue[200];
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
 int shmid;
  sem_t *sem;
 struct Memory *shmPTR;
  void  sigtermhandler(int sig)
 {
   write (STDOUT_FILENO,"Process terminated\n",16);
   shmdt (shmPTR);
   shmctl (shmid, IPC_RMID, 0);
   sem_unlink ("Sem811");
  // sem_unlink("sem10");
    sem_close(sem);

 exit(0);


 }
int main(){
    long int getrand = getpid();
   key_t shmkey;
   int i = 0; //alarm(2);
   long long int x;
   signal(SIGTERM,sigtermhandler);
    int processesTerminated = 0;   
     int value = 0;
     sem_t *sem2;
        
      shmkey = ftok (".", 'x');       /* valid directory name and a number */

     shmid = shmget (shmkey, sizeof (struct Memory), 0666 | IPC_CREAT);
     if (shmid < 0){                           /* shared memory error check */
                    perror ("shmget\n");
                             exit (1);
  
    }
shmPTR  = (struct Memory *) shmat (shmid, NULL, 0);   /* attach p to shared memory */
//     sem = sem_open ("pSem81", O_CREAT | O_EXCL, 0644, 0);
      printf ("semaphores initialized.\n\n");
     sem_close(sem);
     shmPTR->LowStackPointer = 0;
     shmPTR->lowStackNo = 0;
     int nochildProcesses = 0;
     shmPTR->NextProcess = 0;
    for(i = 0; i < 11; i++)

    {    nochildProcesses++;
        if(fork() == 0)
           {
               srand(getrand++);
               value =  (rand()%100);

             //process IDs in queues- low or high
     if (value <   0){
         
         shmPTR->queueNo = 1; 
         
         
         fprintf(stderr,"Process %d is added to high priority queue\n",i);
         }
     else{
//               LowStackQueue[lowStackNo] = i;
  //q             lowStackNo++;
          
               shmPTR->queueNo = 0;
               fprintf(stderr,"Process %d is added to low prioirty queue\n",i);
                }

              shmPTR->processID = i;
                char *args[]={"./user", NULL};
                execvp(args[0],args); printf("Exec error");

          }
           else{ //sleep(1);
//              while(processesTerminated < 4){
                         
                 
                 if(shmPTR->finished ==1){
                   shmPTR->LowStackPointer++;
                   processesTerminated++;
                   
                  }      
                //  sleep(1);          
                  else{
                   shmPTR->LowStackQueue[shmPTR->lowStackNo] = shmPTR->ReadyProcessID;
                    
                   shmPTR->lowStackNo++;
                   shmPTR->LowStackPointer++;
                    fprintf(stderr, "Process %d is added to low priority queue\n", shmPTR->ReadyProcessID);
                    //shmPTR->NextProcess = shmPTR->LowStackQueue[shmPTR->LowStackPointer];
                 } //sleep(1);
                     for(x = 0; x < 10000000; x++){ }
                    //for( j = shmPTR->lowStackNo-1; j>=0; j--)
                      //   fprintf(stderr,"%d",shmPTR->LowStackQueue[j]);
                  
                   sem = sem_open("pSem811",1); sem_post(sem); sem_close(sem);

  //                 if(nochildProcesses  < 4)
                      //break;  }                
       
       }
     }
     fprintf(stderr,"low stack pointer is %d\n", shmPTR->LowStackPointer);
     killpg(getpgid(getpid()), SIGTERM);

exit(0);
}
