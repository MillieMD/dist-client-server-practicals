/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_NODES 5
#define VOTE_OK 1
#define VOTE_FAIL 0
#define VOTE_CRASH -1

#define QUORUM ((NUM_NODES / 2) + 1)  // 3 out of 5
#define GLOBAL_OK 1
#define GLOBAL_ABORT 0

void* Get_Vote(void* arg){
    int node_id = *(int*) arg;
    
    printf("[NODE %d] Performing health check...\n", node_id);
    
    if(node_id == 4){
        pthread_exit(NULL);
    }
    
    int chance = rand() % 100;
    
    if(chance >= 70){
        return (void*)(intptr_t) VOTE_OK;
    }
    
    return (void*)(intptr_t) VOTE_FAIL;
}

int Decide_Consensus(int ok, int fail, int crash){
    if(ok >= QUORUM){
        return GLOBAL_OK;
    }else {
        return GLOBAL_ABORT;
    }
}

int main()
{
    
    int nodes[NUM_NODES];
    
    for (int i = 0; i < NUM_NODES; i++) {
        nodes[i] = i;
    }
    
    pthread_t threads[NUM_NODES];
    
    for (int i = 0; i < NUM_NODES; i++) {
        void* node_id = (void*) &nodes[i];
        pthread_create(&threads[i], NULL, Get_Vote, node_id);
    }
    
    int ok_votes = 0, fail_votes = 0, crash_count = 0;
    
    void* result;
    for (int i = 0; i < NUM_NODES; i++) {
        
        int join_status = pthread_join(threads[i], &result);
        
        if(join_status != 0 || result == PTHREAD_CANCELED){
            printf("Node %d crashed\n", i);
            crash_count++;
            
        } else {
            int vote = (intptr_t) result;
            
            if(vote == VOTE_OK){
                ok_votes++;
            }else {
                fail_votes++;
            }
            
        }
        
    }
    
    int outcome = Decide_Consensus(ok_votes, fail_votes, crash_count);
    
    if(outcome == GLOBAL_OK){
        printf("QUORUM ACHIEVED: %d OK VOTES -> PROCEED\n", ok_votes);
    }else {
                printf("NO QUORUM: Only %d OK VOTES. ABORTING!\n", ok_votes);
    }
    
    printf("FINAL TALLY:");
    printf("OK: %d\nFAIL: %d\nCRASH: %d\n", ok_votes, fail_votes, crash_count);
    
    
    return 0;
}