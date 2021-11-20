#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

sem_t sem;
int sem_val;
int count = 0;

pthread_mutex_t mutex;
pthread_cond_t cond;

struct arg {
  int id;
};

void printSemValue()
{
  sem_getvalue(&sem, &sem_val);
  printf("sem=%d\n", sem_val);
}

void* func(void* args)
{
  struct arg *tmp = (struct arg*) args;
  printf("thread %d waiting ...\n", tmp->id);
  pthread_mutex_lock(&mutex);
  printf("thread %d aquired mutex ...\n", tmp->id);
  int i = 0;
  for(; i < 100 ; ++i)
    count++;

  pthread_cond_wait(&cond, &mutex);
  printf("thread %d complete ...\n", tmp->id);
  pthread_mutex_unlock(&mutex);
   printf("thread %d released mutex ...\n", tmp->id);
  return 0;
}


int main(int argc, char** argv)
{
    
    sem_init(&sem, 0, count);    
    pthread_t t1, t2;

    struct arg arg1;
    struct arg arg2;
    arg1.id = 1;
    arg2.id = 2;

    pthread_create(&t1,0,func,(void*) &arg1);
    pthread_create(&t2,0,func,(void*) &arg2);
    

    sleep(3);
    pthread_cond_signal(&cond);
    sleep(3);
    pthread_cond_signal(&cond);
    

    pthread_join(t1, 0);
    pthread_join(t2, 0);
    sem_destroy(&sem);


    printf("count = %d\n", count);
    return 0;
}
