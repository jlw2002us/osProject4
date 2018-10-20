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
   int NoTerminated;
   int NextProcess;
   int processID;
   int HighStackQueue[150];
   int HighStackPointer;
   int HighStackNo;
//   long long int x;
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
 int shmid;
  sem_t *sem;
 struct Memory *shmPTR;
  void  sigtermhandler(int sig)
 {
   write (STDOUT_FILENO,"Process terminated\n",16);
   shmdt (shmPTR);
   shmctl (shmid, IPC_RMID, 0);
   sem_unlink ("pSem8");
   sem_unlink("pSem503");
   //sem_unlink("sem102");
    sem_close(sem);

 exit(0);


 }
int main(){
    long int getrand = getpid();
   key_t shmkey;
   int i = 0; //alarm(2);
    int j = 0;   
   int seconds = 0; long long int nanoseconds = 0;
   signal(SIGTERM,sigtermhandler);
    int processesTerminated = 0;   
     int value = 0;
     sem_t *sem2;
     sem_t *sem3;   
      shmkey = ftok (".", 'x');       /* valid directory name and a number */

     shmid = shmget (shmkey, sizeof (struct Memory), 0666 | IPC_CREAT);
     if (shmid < 0){                           /* shared memory error check */
                    perror ("shmget\n");
                             exit (1);


}
shmPTR  = (struct Memory *) shmat (shmid, NULL, 0);   /* attach p to shared memory */
     sem = sem_open ("pSem8", O_CREAT | O_EXCL, 0644, 1);
      
     sem_close(sem);
     shmPTR->LowStackPointer = 0;
     shmPTR->lowStackNo = 0;
     
     shmPTR->seconds = 0;
     shmPTR->queueNo = 0;
     shmPTR->nanoseconds = 0;
     pid_t pid; int noChildProcesses = 0;
    for(i = 0; i < 15; i++)

    {    noChildProcesses++;
         srand(getrand++);
               value =  (rand()%100);
          
        if(fork()  == 0)
           {
//               fprintf(stderr,"%s", "hello from child");               
               
             

              shmPTR->processID = i;
                char *args[]={"./user", NULL};
                execvp(args[0],args); printf("Exec error");

          }
           else{ 
               long long int x = 0;  while(x < 10000000){ x++;}   sem2 = sem_open("pSem503", 1); sem_post(sem2);sem_close(sem2);}
              
        }
   //      while(shmPTR->NoTerminated < 15)
     //      {
       //         noChildProcesses++; if (noChildProcesses == 15) break;
         //       if(fork() == 0){
           //       shmPTR->processID = i;
             //   char *args[]={"./user", NULL};
              //  execvp(args[0],args); printf("Exec error");
                     
               // }
                 //  else{ sleep(1);}                     
                    //shmPTR->NextProcess = shmPTR->LowStackQueue[shmPTR->LowStackPointer];
                 //   seconds = 0; nanoseconds = 0;
                   //  while(nanoseconds < 100000000){
                     //    shmPTR->nanoseconds = shmPTR->nanoseconds + 5;
                       //  nanoseconds = nanoseconds + 5;
                         //if(nanoseconds >= 1000000000){
                           // seconds++;
                            //nanoseconds = nanoseconds - 1000000000;
                         // }
                         //if(shmPTR->nanoseconds >= 1000000000){
                            
                           // shmPTR->seconds++;
                           // shmPTR->nanoseconds= shmPTR->nanoseconds - 1000000000;
                         // }

                    // }
                                     
       
             // }}
//           sleep(4);
     killpg(getpgid(getpid()), SIGTERM);

exit(0);
}

