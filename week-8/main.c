/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_WORKERS 3

FILE *shared_file_ptr = NULL;
pthread_mutex_t file_lock = PTHREAD_MUTEX_INITIALIZER;

void* Worker_Save_Task(void* arg) {
    int worker_id = *(int*)arg;
    
    printf("[WORKER %d] Attempting to acquire lock...\n", worker_id);
    pthread_mutex_lock(&file_lock);
    printf("[WORKER %d] Lock aquired – now saving...\n", worker_id);
    
    fprintf(shared_file_ptr, "[WORKER %d] Starting save at %ld\n", worker_id, time(NULL));
    fflush(shared_file_ptr);
    sleep(5);  // Simulate 5-second save
    fprintf(shared_file_ptr, "[WORKER %d] Finished saving at %ld\n\n", worker_id, time(NULL));
    fflush(shared_file_ptr);
    
    printf("Worker %d releasing lock\n", worker_id);
    pthread_mutex_unlock(&file_lock);
}

int main()
{
    
    shared_file_ptr = fopen("shared_doc.txt", "a");
    
    if(shared_file_ptr == NULL){
        printf("Failed opening file.");
        exit(1);
    }
    
    fprintf(shared_file_ptr, "Initial document state\n");
    
    int workers[NUM_WORKERS];
    
    for (int i = 0; i < NUM_WORKERS; i++) {
        workers[i] = i;
    }
    
    pthread_t threads[NUM_WORKERS];
    
    for (int i = 0; i < NUM_WORKERS; i++) {
        void* worker_id = (void*) &workers[i];
        pthread_create(&threads[i], NULL, Worker_Save_Task, worker_id);
    }
    
    for (int i = 0; i < NUM_WORKERS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("All workers finished. Document is consistent!");
    fclose(shared_file_ptr);
    
    return 0;
}