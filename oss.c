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
   int HighStackQueue[50];
   int HighStackPointer;
   int HighStackNo;
//   long long int x;
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
 int shmid;
  sem_t *sem;
 struct Memory *shmPTR;
  void  sigtermhandler(int sig)
 {
   write (STDOUT_FILENO,"Process terminated\n",16);
   shmdt (shmPTR);
   shmctl (shmid, IPC_RMID, 0);
   sem_unlink ("pSem95");
   sem_unlink("sem24");
    sem_close(sem);

 exit(0);


 }
int main(){
    long int getrand = getpid();
   key_t shmkey;
   int i = 0; //alarm(2);
   long long int x;
   int seconds = 0; long long int nanoseconds = 0;
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
     sem = sem_open ("pSem95", O_CREAT | O_EXCL, 0644, 0);
      printf ("semaphores initialized.\n\n");
     sem_close(sem);
     shmPTR->LowStackPointer = 0;
     shmPTR->lowStackNo = 0;
     int nochildProcesses = 0;
     shmPTR->seconds = 0;
     shmPTR->nanoseconds = 0;
    for(i = 0; i < 18; i++)

    {    nochildProcesses++;
        if(fork() == 0)
           {
               srand(getrand++);
               value =  (rand()%100);

             //process IDs in queues- low or high
     if (value <=  0){
         
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
              while(processesTerminated < 18){
                         
                 sem_open("pSem95",0); sem_wait(sem);
                 if(shmPTR->finished ==1){
                   shmPTR->LowStackPointer++;
                   processesTerminated++;
                   //shmPTR->NextProcess = shmPTR->LowStackQueue[shmPTR->LowStackPointer];}
                  }      
                //  sleep(1);          
                  else{
                   shmPTR->LowStackQueue[shmPTR->lowStackNo] = shmPTR->ReadyProcessID;
                    
                   shmPTR->lowStackNo++;
                   shmPTR->LowStackPointer++;
                    fprintf(stderr, "Process %d is added to low priority queue\n", shmPTR->ReadyProcessID);
                    //shmPTR->NextProcess = shmPTR->LowStackQueue[shmPTR->LowStackPointer];
                 }   seconds = 0; nanoseconds = 0;
                     while(seconds < 1){
                         shmPTR->nanoseconds = shmPTR->nanoseconds + 1000;
                         nanoseconds = nanoseconds + 1000;
                         if(nanoseconds >= 1000000000){
                            seconds++;
                            nanoseconds = nanoseconds - 1000000000;
                          }
                         if(shmPTR->nanoseconds >= 1000000000){
                            
                            shmPTR->seconds++;
                            shmPTR->nanoseconds - shmPTR->nanoseconds - 1000000000;
                          }

                     }
                    //for( j = shmPTR->lowStackNo-1; j>=0; j--)
                      //   fprintf(stderr,"%d",shmPTR->LowStackQueue[j]);
                  
                   sem2 = sem_open("sem24",1); sem_post(sem2);sem_close(sem2);
                   if(nochildProcesses  < 18)
                      break; }                
       
       } }
//           sleep(4);
     killpg(getpgid(getpid()), SIGTERM);

exit(0);
}

