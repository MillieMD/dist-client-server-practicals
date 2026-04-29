/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 5

int event_buffer[BUFFER_SIZE];
int count = 0, in = 0, out = 0;

pthread_mutex_t buffer_lock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond_full  = PTHREAD_COND_INITIALIZER;  // producer waits on this
pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER;  // consumer waits on this

void* producer_task(void* arg){
    
    printf("[PRODUCER] Starting...\n");
    
    pthread_mutex_lock(&buffer_lock);
    printf("[PRODUCER] Lock obtained\n");
    
    pthread_cond_wait(&cond_full, &buffer_lock);
        
    while(count < BUFFER_SIZE){
        
        int new_event_id = rand();
        event_buffer[in] = new_event_id;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        
        printf("[PRODUCER] Produced event with id %d\n", new_event_id);
        
    }
    
    pthread_cond_signal(&cond_empty);
    
    pthread_mutex_unlock(&buffer_lock);
    printf("[PRODUCER] Lock released\n");

}

void* consumer_task(void* arg){
    
    printf("[CONSUMER] Starting...\n");
    
    pthread_mutex_lock(&buffer_lock);
    printf("[CONSUMER] Lock obtained\n");
    
    pthread_cond_wait(&cond_empty, &buffer_lock);
        
    while(count != 0){
        
        int event_id = event_buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        
        printf("[CONSUMER] Consumed event with id %d\n", event_id);
        
    }
    
    pthread_cond_signal(&cond_full);
    
    pthread_mutex_unlock(&buffer_lock);
    printf("[CONSUMER] Lock released\n");

}

int main()
{
    pthread_t producer_thread;
    pthread_create(&producer_thread, NULL, producer_task, NULL);
    
    pthread_t consumer_thread;
    pthread_create(&consumer_thread, NULL, consumer_task, NULL);
    
    sleep(1);
    pthread_cond_signal(&cond_full);
    sleep(10);
    
    pthread_join(producer_thread, NULL);
    printf("[MAIN] Joined producer thread\n");
    pthread_join(consumer_thread, NULL);
    printf("[MAIN] Joined consumer thread\n");
    
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d\n", event_buffer[i]);
    }
    
    printf("COUNT %d\nIN %d\nOUT %d\n", count, in, out);
    
    return 0;
}