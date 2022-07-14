#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<signal.h>
#include<sys/time.h>

static struct timespec rtc,request,remain;

static struct timespec ot,nt;
long T;

void * sam_func(void *arg) {
    
       clock_gettime(CLOCK_MONOTONIC, &request);

        while(1) {

                request.tv_nsec += T; /* Sleep for 20 seconds from now */
                clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &request, NULL);
                clock_gettime(CLOCK_MONOTONIC, &rtc);
        }
   return NULL;

}
void * in_func(void *arg) {
        FILE *fp;
       while(1) {
        
        
        fp = fopen("freq.txt","r");
        char buff[100];
        fgets(buff,sizeof(buff),fp);
        char *eptr;
        T = strtol(buff,&eptr,10);

        fclose(fp);

       }
        return NULL;

}
void    *log_func(void* arg) {
         FILE *file;
        while(1) {
               
        if(rtc.tv_nsec != ot.tv_nsec ||rtc.tv_sec != ot.tv_sec ) {
        


        file = fopen("time_and_interval.txt","a+");

        if(file == NULL) {
        printf("ko mo dc file\n");
        return NULL;
        }

        long diff_sec = (long)rtc.tv_sec - (long)ot.tv_sec ;
        long diff_nsec;
        
        if(rtc.tv_nsec > ot.tv_nsec) {
        diff_nsec = rtc.tv_nsec - ot.tv_nsec;
        }

        else {
        diff_nsec = ot.tv_nsec - rtc.tv_nsec;
        diff_sec = diff_sec - 1;
        }

        
        if(ot.tv_nsec != 0) {
                fprintf(file, "%ld.%09ld %ld.%09ld\n",rtc.tv_sec,rtc.tv_nsec,diff_sec,diff_nsec);
        }
     
        //     fprintf(file, "\nnew val : %ld.%09ld\nold val : %ld.%09ld\n", nt->tv_sec, nt->tv_nsec, ot.tv_sec , ot.tv_nsec );
                fclose(file);
                ot.tv_nsec = rtc.tv_nsec;
                ot.tv_sec = rtc.tv_sec;
        } 
        }
        return NULL;

}
int main(int argc, char** argv)
{
pthread_t SAMPLE,INPUT,LOGIN;
request.tv_sec = 0;
request.tv_nsec = 0;

pthread_create(&SAMPLE, NULL, &sam_func, NULL); //Tạo luồng sample
pthread_create(&INPUT, NULL, &in_func, NULL); //Tạo luồng input
pthread_create(&LOGIN, NULL, &log_func, NULL); //Tạo luồng login


pthread_join(SAMPLE,NULL);
pthread_join(INPUT,NULL);
pthread_join(LOGIN,NULL);
return 0;

}