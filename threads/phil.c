 #include <pthread.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <semaphore.h>
 #include <time.h>

 #define NUM_PHIL     5
 #define NUM_FORK     5 
 #define NUM_CHAIRS   5/2

sem_t forks[NUM_FORK];
sem_t chairs[NUM_CHAIRS];
int getRand(float max){
  return 1 + (rand() * max) / RAND_MAX;
}

void eating(void){
  sleep(getRand(5.0));
}

void thinking(void){
  sleep(getRand(5.0));
}

void *philLive(void *threadid){
  long tid;
  tid = (long)threadid;
  while(1)
  {
    printf("I am phil %ld, thinking \n", tid);
    thinking();
    sem_wait(&chairs[tid]);
    sem_wait(&forks[tid]);
    sem_wait(&forks[(tid + 1) % NUM_PHIL]);
    printf("I am phil %ld, eating \n", tid);
    eating();
    sem_post(&chairs[tid]);
    sem_post(&forks[tid]);
    sem_post(&forks[(tid + 1) % NUM_PHIL]);
  }
  pthread_exit(NULL);
}

 int main (int argc, char *argv[])
 {
    pthread_t philosophers[NUM_PHIL];
    int rc;
    long t;
    srand(time(NULL));
    for(t=0; t<NUM_FORK; t++){
      sem_init(&forks[t],0,1);
    }

    for(t=0; t<NUM_PHIL; t++){
       rc = pthread_create(&philosophers[t], NULL, philLive, (void *)t);
       if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
       }
    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
 }