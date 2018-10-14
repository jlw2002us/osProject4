//Jennifer Wasson
 //Operating Systems
 //Project 4
 

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
   int pidd;
   int HighStackQueue[20];
     int LowStackQueue[20];
  int j;
  int k;
  int highStackNo;
  int LowStackNo;
   int processID;
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
   sem_unlink ("pSem2");   
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
     int i = 0; //int j = 0;int k = 0;
     int value = 0;
     bool high = false;
     bool low = false;
     key_t shmkey;
     pid_t pid;
//     int HighStackQueue[20];
  //   int LowStackQueue[20];
     //int highStackNo = 0;
     //int LowStackNo = 0;
     int childProcesses = 0;
     
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
     
                                                 
     
     sem = sem_open ("pSem2", O_CREAT | O_EXCL, 0644, 1); 
      printf ("semaphores initialized.\n\n");
     sem_close(sem);
     for(i=0; i < 2; i++){
    
        
          if ((pid = fork()) == 0){
               //char str[50];
               //snprintf(str, sizeof(str), "%d", i);

               shmPTR->pidd = i; //process ID
               
               
               srand(i);
               value =  (rand()%100);  
             
    //         process IDs in queues- low or high
               if (value <  0){
                 shmPTR->processBlock.queueNo = 1;
                 shmPTR->HighStackQueue[shmPTR->j] = i; shmPTR->j++;
                 shmPTR->highStackNo++;
               }
              else{
                  shmPTR->processBlock.queueNo = 0;
                  shmPTR->LowStackQueue[shmPTR->k] = i;shmPTR->k++;
                  shmPTR->LowStackNo++;
                  
               }    
                        
                       char *args[]={"./user",NULL}; 
                       execvp(args[0],args); 
//                     execlp("./user", str, NULL, NULL); 
            }
          
          else{  sleep(2);
               
              sem = sem_open("pSem17",0);
              sem_wait(sem);
              if(shmPTR->highStackNo != 0){high = true;
               shmPTR->processID = shmPTR->HighStackQueue[shmPTR->j - shmPTR->highStackNo];}
              else if(shmPTR->LowStackNo != 0){ 
                low = true; 
                shmPTR->processID = shmPTR->LowStackQueue[shmPTR->k - shmPTR->LowStackNo];
                fprintf(stderr,"%d",shmPTR->processID);
              }
             sem_post(sem);
             sem_close(sem);
            sleep(2);
            if(shmPTR->terminated == 1){
              processesTerminated++;
               if(high == true){
                 shmPTR->highStackNo--; 
                 high = false;}
               if(low == true){
                 shmPTR->LowStackNo--;
                 low = false;}
               
               }}}
            
         //if(pid != 0){
           //printf("hi");         
           //sem = sem_open("pSem15",0);
           //sem_wait(sem);
           //if(shmPTR->highStackNo != 0){high = true;
             // shmPTR->processID = shmPTR->HighStackQueue[shmPTR->j - shmPTR->highStackNo];}
              //else if(shmPTR->LowStackNo != 0){ low = true;
                //shmPTR->processID = shmPTR->LowStackQueue[shmPTR->k - shmPTR->LowStackNo];
                // fprintf(stderr,"%d",shmPTR->processID);
               // }
              //sem_post(sem);
              //sem_close(sem);
              
              //sleep(2);
             
              //if(shmPTR->terminated == 1){  //fprintf(stderr,"%d",shmPTR->processID);
                //if(high == true){
                  //shmPTR->highStackNo--; 
                  //high = false;}
                //if(low == true){
                  //shmPTR->LowStackNo--;
                  //low = false;}
                //processesTerminated++;}
              // else{
                // if(high == true){
                  //shmPTR->HighStackQueue[shmPTR->j] = shmPTR->processID;
                  //high = false; shmPTR->j++;}
                 //if(low == true){
                   //shmPTR->LowStackQueue[shmPTR->k] = shmPTR->processID;
                   //low = false; shmPTR->k++;}}
          // }  
         //}
         sleep(2);
         killpg(getpgid(getpid()), SIGTERM);
       
         exit (0);

     }
