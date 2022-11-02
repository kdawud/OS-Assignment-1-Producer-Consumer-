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

sem_t *empty;
sem_t *full;
sem_t *mutex;
int in = 0;
int out = 0;
int buffer[BufferSize];


int main(){
     int shm_fd;
     int * table;
   
  
     shm_fd = shm_open("tbl",O_CREAT | O_RDWR, 0666);

     if(shm_fd = -1){
        exit(1);
    }
     ftruncate(shm_fd, sizeof(int));
     table = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);


    empty = sem_open("s1", O_CREAT,0666,0);
    full = sem_open("s2", O_CREAT, 0666, 3);
    mutex = sem_open("s3",O_CREAT,0666,1);

    for(int i = 0; i < 5; ++i){
        sem_wait(&empty); 
        sleep(1);
        sem_wait(&mutex);
        ++*table;   
        sem_post(&mutex);
        sem_post(&full);
    }

    sem_close(&full);
    sem_close(&empty);
    sem_close(&mutex);

    sem_unlink("s1");
    sem_unlink("s2");
    sem_unlink("s3");


    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    munmap(table, sizeof(int));
    close(shm_fd);
    shm_unlink("tbl");
    return 0;
}

