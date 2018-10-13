//Jennifer Wasson
 //Operating Systems
 //Project 3
 //Sources used:https://stackoverflow.com/questions/16400820/how-to-use-posix-semaphores-on-forked-processes-in-c

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

 struct ProcessBlock{
    int queueNo; //1 - high, 0- low
  
  
  int ready; //if process is ready to be run = 1
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
 bool signal_interrupt = false;
 int shmid;
 sem_t *sem;
 

 void  ALARMhandler(int sig)
 { if(signal_interrupt == false)
   write (STDOUT_FILENO,"Alarm went off\n",16);
         signal(SIGALRM, SIG_IGN);
    signal_interrupt = true;
   signal(SIGALRM, ALARMhandler);
        
 }

 void  sigtermhandler(int sig)
 { 
   write (STDOUT_FILENO,"Process terminated\n",16);
   shmdt (shmPTR);
   shmctl (shmid, IPC_RMID, 0);
   sem_unlink ("pSem15");   
    sem_close(sem);  

 exit(0);
       

 }

 void helpoptions()
 {
   printf("options for running Project 3:  \n");
   printf("type ./oss -h for help options\n");
   printf("type ./oss -s x for value for how many processes you want to fork");
   printf("type ./oss -t z for value of seconds for alarm clock");
   printf("type ./oss -l filename with name of file that you want to write to");
 }

 int main (int argc, char **argv){
     int i = 0; 
     int value = 0;
     key_t shmkey;
     int childProcesses = 0;
     pid_t pid;                    /*      fork pid                */
     signal(SIGTERM, sigtermhandler);
     int processesTerminated = 0;
      shmkey = ftok (".", 'x');       /* valid directory name and a number */
     
     shmid = shmget (shmkey, sizeof (struct Memory), 0666 | IPC_CREAT);
     if (shmid < 0){                           /* shared memory error check */
                    perror ("shmget\n");
                             exit (1);
                                 }
     //
     shmPTR  = (struct Memory *) shmat (shmid, NULL, 0);   /* attach p to shared memory */
     shmPTR->seconds = 0;
     shmPTR->nanoseconds = 0;
     
                                                 
     
     sem = sem_open ("pSem15", O_CREAT | O_EXCL, 0644, 1); 
      printf ("semaphores initialized.\n\n");
     sem_close(sem);
     while(processesTerminated < 1){
     if(childProcesses < 1){       
        if (fork() == 0){
                //get random number for priority
             srand(i);
             value =  (rand()%100);  
             value =  (rand()%100);
             if (value <= 10){
               shmPTR->processBlock.queueNo = 1;
              }
             else{
               shmPTR->processBlock.queueNo = 0;}         
                        
                 
                execl("./user", (char*)i, NULL, NULL); 
           }
          
         else{    
            sem = sem_open("pSem15",0);
            sem_wait(sem);
            shmPTR->processBlock.ready = 1;
            sem_post(sem);
            sem_close(sem);
            sleep(2);
            if(shmPTR->terminated == 1){
               processesTerminated++;
               
               break;}}           
         childProcesses++;}
          sem = sem_open("pSem15",0);
            sem_wait(sem);
            shmPTR->processBlock.ready = 1;
            sem_post(sem);
            sem_close(sem);
            sleep(2);
             if(shmPTR->terminated == 1)
               processesTerminated++;
          }
         sleep(2);
         killpg(getpgid(getpid()), SIGTERM);
       
         exit (0);

     }
