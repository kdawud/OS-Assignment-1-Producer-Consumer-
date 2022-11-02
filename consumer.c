#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <pthread.h>

#define MaxItems 5 
#define BufferSize 5 


//initializing variables and semaphores
sem_t *empty;
sem_t *full;
sem_t *mutex;
int in = 0;
int out = 0;
int buffer[BufferSize];

int main(){

    //creating shared memory
     int shm_fd;
     int * table;
    
  
     shm_fd = shm_open("name",O_CREAT | O_RDWR, 0666);

     if(shm_fd = -1){
        exit(1);
    }
     ftruncate(shm_fd, sizeof(int));
     table = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);


    empty = sem_open("sema1", O_CREAT,0666,0);
    full = sem_open("sema2", O_CREAT, 0666, 3);
    mutex = sem_open("sema3",O_CREAT,0666,1);


//for loop that waits until it should consume, then decrements mutex and consumes
    for(int i = 0; i < 5; ++i){
        sem_wait(&full); 
        sleep(1);
        sem_wait(&mutex);
        --*table;   
        sem_post(&mutex);
        printf("consumed");
        sem_post(&empty);
    }
//closes, unlinks and destroys semaphores.
    sem_close(&full);
    sem_close(&empty);
    sem_close(&mutex);

    sem_unlink("sema1");
    sem_unlink("sema2");
    sem_unlink("sema3");


    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
//ends shared memory
    munmap(table, sizeof(int));
    close("shm_fd");
    shm_unlink("name");
    return 0;
}