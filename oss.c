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
   int priority; //1 for high, 0 for low
   
      };

struct Memory{
  long long int nanoseconds;
  long long int seconds;
  struct ProcessBlock processBlock[];
  
};
struct Memory *shmPTR;
bool signal_interrupt = false;
int shmid;
sem_t *sem;
bool ChildExceeded = false;


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
  sem_unlink ("pSem3");   
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
    int i;                        /*      loop variables          */
    key_t shmkey;
    
    pid_t pid;                    /*      fork pid                */
    

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
    shmPTR->processBlock[0].priority = 0;                                            
    
    sem = sem_open ("pSem3", O_CREAT | O_EXCL, 0644, 1); 
     printf ("semaphores initialized.\n\n");
    sem_close(sem);
   
        pid = fork ();
        
        if (pid < 0) {
            sem_unlink ("pSem3");   
            sem_close(sem); 
            printf ("Fork error.\n");
        }
        else if (pid == 0){
             char *args[]={"./user",NULL}; 
             execvp(args[0],args); printf("exec error");}
    
    
    
    else if (pid != 0){
          wait(NULL);
         sem = sem_open("pSem3",0);
        sem_wait(sem);
        if (shmPTR->processBlock[0].priority == 1){
           printf("received");
                           }
           
  
           sem_post(sem);
           sem_close(sem);
           sem_unlink("pSem3");}      
        //killpg(getpgid(getpid()), SIGTERM);

        exit (0);

    
}
