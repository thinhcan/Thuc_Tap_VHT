#include"stdio.h"
#include"stdint.h"
#include"pthread.h"
#include"time.h"
#include"string.h"
#include"stdlib.h"
#include "unistd.h"

struct timespec tp;
struct timespec ot = {{0,0}};

long freq;
 long old_freq = 0;
struct timespec time1, time2;
uint8_t kt = 0;


long get_freq() {
  long data;
  FILE *fp;
 fp = fopen("freq.txt","r");
 char buff[100];
 fgets(buff,sizeof(buff),fp);
 char *eptr;
 data = strtol(buff,&eptr,10);

 fclose(fp);
 return data;
 }

 void *getTime(void *args ) {  

  clock_gettime(CLOCK_REALTIME,&tp);

  printf("%09ld.%09ld\n",tp.tv_sec,tp.tv_nsec);
  return NULL;
}

void *getFreq(void *args) {

 //long x = (*(long*)args);
 long new_freq = get_freq();

    if(old_freq == new_freq) {
          return NULL;
    }
    else {
      // FILE *fp;
      // fp = fopen("freq.txt","w");
      // fprintf(fp,"%ld",x);
      // fclose(fp);
      freq = new_freq;
      time1.tv_sec = 0;
      time1.tv_nsec = freq;

       old_freq = new_freq;
    }
     return NULL;

}

void *save_time(void *args)
{
 struct timespec *nt = (struct timespec *)args;
 if(nt->tv_nsec != ot.tv_nsec ||nt->tv_sec != ot.tv_sec ) {
  FILE *file;


     file = fopen("time_and_interval.txt","a+");

    if(file == NULL) {
      printf("ko mo dc file\n");
      return NULL;
    }

    long diff_sec = (long)nt->tv_sec - (long)ot.tv_sec ;
    long diff_nsec;
    
    if(nt->tv_nsec > ot.tv_nsec) {
      diff_nsec = nt->tv_nsec - ot.tv_nsec;
    }

    else {
      diff_nsec = ot.tv_nsec - nt->tv_nsec;
      diff_sec = diff_sec - 1;
    }

 

   fprintf(file, "%ld.%09ld %ld.%09ld\n",nt->tv_sec,nt->tv_nsec,diff_sec,diff_nsec);
   //     fprintf(file, "\nnew val : %ld.%09ld\nold val : %ld.%09ld\n", nt->tv_sec, nt->tv_nsec, ot.tv_sec , ot.tv_nsec );
    fclose(file);
    ot.tv_nsec = nt->tv_nsec;
    ot.tv_sec = nt ->tv_sec;

 } else {
  return NULL;
 }
    



}

int main(int argc, char const *argv[])
{

    freq = get_freq();
    pthread_t SAMPLE;
    pthread_t INPUT;
    pthread_t LOGGING;

    int s,i,l;
    time1.tv_sec = 0;
    time1.tv_nsec = freq;

    printf("freq = %ld\n",freq);
    printf("tv_nsec = %ld\n",time1.tv_nsec);

    while(1)
    {
        if(nanosleep(&time1 , &time2) < 0 ) {
        printf("Nano sleep system call failed \n");
        return -1;
      }
      else {
        //       printf(" chay ma nhi thread\n");
          i = pthread_create(&INPUT,NULL,getFreq,NULL);
          s = pthread_create(&SAMPLE, NULL, getTime,&freq);
          l = pthread_create(&LOGGING,NULL,save_time,&tp);

          pthread_join(INPUT,NULL);
          pthread_join(SAMPLE,NULL);
          pthread_join(LOGGING,NULL);

      }
        }
     return 0;
}
                           