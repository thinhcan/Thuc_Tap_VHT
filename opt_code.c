#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sched.h>
#include<time.h>



static struct timespec rtc,request;

static struct timespec ot;

 FILE *file;
 FILE *fp;


long T =100000;

void * sam_func(void *arg) {
        //chờ
        struct sched_param sp;
        sp.sched_priority = 70;
        pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);

        clock_gettime(CLOCK_MONOTONIC, &request);
        
        while(1) {
      
        request.tv_nsec += T;
       
        if(request.tv_nsec > 1000*1000*1000) {
                request.tv_nsec -= 1000*1000*1000;
                request.tv_sec++;
        }

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &request, NULL);
        
        clock_gettime(CLOCK_MONOTONIC, &rtc);
        }
        
   return NULL;

}
void * in_func(void *arg) {
        struct sched_param sp;
        sp.sched_priority = 10;
        pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);

        fp = fopen("freq.txt","r");
       while(1) {
     
        char buff[100];
        fgets(buff,sizeof(buff),fp);
        char *eptr;
        T = strtol(buff,&eptr,10);  

       }
        fclose(fp);
        return NULL;

}
void    *log_func(void* arg) {    
        struct sched_param sp;
        sp.sched_priority = 40;
        pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);


         file = fopen("time_and_interval.txt","a+");
        while(1) {

                if(rtc.tv_nsec != ot.tv_nsec ||rtc.tv_sec != ot.tv_sec ) {
        
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
                                diff_nsec = 1000000000 - ot.tv_nsec + rtc.tv_nsec;
                                diff_sec = diff_sec - 1;
                        }        
                        
                        if(ot.tv_nsec != 0) {

                                fprintf(file, "%ld\n",diff_nsec);
                        }
                
                        ot.tv_nsec = rtc.tv_nsec;
                        ot.tv_sec = rtc.tv_sec;
                }
        }
        fclose(file);
        return NULL;
}
int main(int argc, char** argv)
{

pthread_t SAMPLE,INPUT,LOGIN;

request.tv_sec = 0;
request.tv_nsec = 0;


pthread_create(&INPUT, NULL, &in_func, NULL); //Tạo luồng input
pthread_create(&SAMPLE,  NULL,&sam_func, NULL); //Tạo luồng sample
pthread_create(&LOGIN, NULL, &log_func, NULL); //Tạo luồng login

pthread_join(INPUT,NULL);
pthread_join(SAMPLE,NULL);
pthread_join(LOGIN,NULL);

return 0;

}