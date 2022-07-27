#include<stdio.h>
#include<stdint.h>
#include<pthread.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>


long cycle=10000;

struct timespec now,old;
struct timespec timecheck, timecheck2, request;



u_int8_t check_loop = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  


void *currently_time(void *time)
{  
  clock_gettime(CLOCK_REALTIME, &request);
    while(1)
    {

        request.tv_nsec += cycle;
        
        if(request.tv_nsec > 1000*1000*1000) {
                request.tv_nsec -= 1000*1000*1000;
                request.tv_sec
                ++;
        }
        clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME, &request,NULL);

        clock_gettime(CLOCK_REALTIME,&now);  

  }
      return NULL;
}


//main of thread input
void *check_time(void *time)
{ 
  while(1)
  {
      
     
      // struct timespec timecheck;
      FILE *file;
      file = fopen("freq.txt","r");
      char buff[100];
      fgets(buff,sizeof(buff),file);
      //convert from string to long
      char *eptr;
     
      cycle = strtol(buff,&eptr,10);
      fclose(file);

        return NULL;
}
}

//main of thread logging
void *save_time(void *time)
{
    FILE *file;
    FILE *file1;
    FILE *fp;
  while(1){

    
      file1 = fopen("time_and_interval.txt","a+"); // save all of offset values in file

    if(now.tv_nsec != old.tv_nsec || now.tv_sec != old.tv_sec) {
      long interval_sec = ((long)now.tv_sec) - (long)old.tv_sec ;
      long interval_nsec;

      if(now.tv_nsec > old.tv_nsec)
        {
          interval_nsec = now.tv_nsec - old.tv_nsec;
        }
      else 
        {
          interval_nsec = 1000000000 + now.tv_nsec - old.tv_nsec;
          interval_sec = interval_sec - 1;
        }

        old.tv_nsec = now.tv_nsec;
        old.tv_sec = now.tv_sec;

      fprintf(file1,"%ld \n",interval_nsec);
      

     
      fclose(file1);
    }
    

  }

  return NULL;
}
int main(){

    


    int* ptr;
    pthread_t sample;
    pthread_t input;
    pthread_t logging;

      pthread_create(&input,NULL,check_time,NULL);
      pthread_create(&sample, NULL, currently_time,NULL);
      pthread_create(&logging,NULL,save_time,NULL);

      pthread_join(input,NULL);
      pthread_join(logging,NULL);
      pthread_join(sample, NULL);  

   return 0;
}