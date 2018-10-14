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
 int shmid;
  sem_t *sem;
 struct Memory *shmPTR;
  void  sigtermhandler(int sig)
 { 
   write (STDOUT_FILENO,"Process terminated\n",16);
   shmdt (shmPTR);
   shmctl (shmid, IPC_RMID, 0);
   sem_unlink ("pSem30");   
    sem_close(sem);  

 exit(0);


 }

int main(){
   
   key_t shmkey;
   int i = 0;
   signal(SIGTERM,sigtermhandler);
    
     int value = 0;
     bool high = false;
     bool low = false;

  int processesTerminated = 0;
      shmkey = ftok (".", 'x');       /* valid directory name and a number */

     shmid = shmget (shmkey, sizeof (struct Memory), 0666 | IPC_CREAT);
     if (shmid < 0){                           /* shared memory error check */
                    perror ("shmget\n");
                             exit (1);
                                 }
     //
     shmPTR  = (struct Memory *) shmat (shmid, NULL, 0);   /* attach p to shared memory */
     sem = sem_open ("pSem30", O_CREAT | O_EXCL, 0644, 1); 
      printf ("semaphores initialized.\n\n");
     sem_close(sem);
     shmPTR->highStackNo = 0;
     shmPTR->terminated = 0;
     shmPTR->LowStackNo = 0;
    for(i = 0; i < 2; i++)
    {
        if(fork() == 0)
           {
               srand(i);
               value =  (rand()%100);

             //process IDs in queues- low or high
             if (value <=  0){
               shmPTR->queueNo = 1;
               shmPTR->HighStackQueue[shmPTR->j] = i; shmPTR->j++;
               shmPTR->highStackNo++;
              }
             else{
               sem = sem_open("pSem30",0); sem_wait(sem);
               shmPTR->queueNo = 0;
               shmPTR->LowStackQueue[shmPTR->k] = i;shmPTR->k++;
               shmPTR->LowStackNo++;sem_post(sem);sem_close(sem);
                }    

              shmPTR->LoopVar = i;
                char *args[]={".user", NULL}; 
                execvp(args[0],args); 
           }
       else{
             sleep(2);
             sem = sem_open("pSem30",0);
              sem_wait(sem); 
              if(shmPTR->highStackNo != 0){high = true;
                shmPTR->processID = shmPTR->HighStackQueue[shmPTR->j - shmPTR->highStackNo];
                 sem_post(sem); sem_close(sem);
           }
              
             else if(shmPTR->LowStackNo != 0){ 
               low = true; 
               shmPTR->processID = shmPTR->LowStackQueue[shmPTR->k - shmPTR->LowStackNo];
               fprintf(stderr,"%d",shmPTR->processID);
               sem_post(sem); sem_close(sem);
              }
            else{ fprintf(stderr, "%s", "Both queues empty");
                 sem_post(sem);
                 sem_close(sem);}
                 sleep(2);
            sem_open("pSem30", 0);sem_wait(sem);
            if(shmPTR->terminated == 1){
               processesTerminated++;
              if(high == true){
                shmPTR->highStackNo--; 
                high = false;sem_post(sem); sem_close(sem);}
              
               if(low == true){
                shmPTR->LowStackNo--;
                 low = false;
                 sem_post(sem); sem_close(sem);}}
              
            else{

               if(high == true){
                 shmPTR->HighStackQueue[shmPTR->j] = shmPTR->processID;
                 high = false; shmPTR->j++; sem_post(sem); sem_close(sem);}
               if(low == true){ 
                shmPTR->LowStackQueue[shmPTR->k] = shmPTR->processID;
                low = false; shmPTR->k++;sem_post(sem);sem_close(sem);}

            }}    
            }
             while(processesTerminated < 2){
              sem = sem_open("pSem30",0);
              sem_wait(sem);
              if(shmPTR->highStackNo != 0){high = true;
                 shmPTR->processID = shmPTR->HighStackQueue[shmPTR->j - shmPTR->highStackNo];
                 sem_post(sem); sem_close(sem);}
              else if(shmPTR->LowStackNo != 0){ low = true;
                 shmPTR->processID = shmPTR->LowStackQueue[shmPTR->k - shmPTR->LowStackNo];
                 sem_post(sem); sem_close(sem);
              }
              else{ fprintf(stderr, "%s", "Both queues empty");
                sem_post(sem);
                sem_close(sem);}

              sleep(2);
              sem_open("pSem30", 0); sem_wait(sem);

              if(shmPTR->terminated == 1){  //fprintf(stderr,"%d",shmPTR->processID);
                if(high == true){
                  shmPTR->highStackNo--; 
                  high = false;
                  sem_post(sem); sem_close(sem);}
                if(low == true){
                  shmPTR->LowStackNo--;
                  low = false;
                  sem_post(sem); sem_close(sem);}
                  processesTerminated++;}
                else{
                  if(high == true){
                    shmPTR->HighStackQueue[shmPTR->j] = shmPTR->processID;
                    high = false; shmPTR->j++;
                    sem_post(sem); sem_close(sem);}
                else if(low == true){
                   shmPTR->LowStackQueue[shmPTR->k] = shmPTR->processID;
                   low = false; shmPTR->k++;
                  sem_post(sem); sem_close(sem);}
                
                else{
                     fprintf(stderr,"%s", "error");
                     sem_post(sem); sem_close(sem);}}
             }

              


 
     sleep(2);
     killpg(getpgid(getpid()), SIGTERM);
 
exit(0);
}
