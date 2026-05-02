/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PARTICIPANTS 3
#define VOTE_READY   1
#define VOTE_ABORT   0
#define GLOBAL_COMMIT 2
#define GLOBAL_ABORT  3

void* Participant_Service(void* arg) {
	int service_id = *(int*) arg;

	printf("[SERVICE %d] Recieved VOTE_REQUEST (Phase 1)\n", service_id);

	int chance = rand() % 100;

	if(chance <= 80) {
		printf("[SREVICE %d] PREPARED\n", service_id);
		return (void*)(intptr_t) VOTE_READY;
	}

	printf("[SREVICE %d] CANNOT PREAPRE\n", service_id);
	return (void*)(intptr_t) VOTE_ABORT;
}

int main()
{
    
    srand(time(NULL));

	pthread_t threads[NUM_PARTICIPANTS];
	int services[NUM_PARTICIPANTS];

	for (int i = 0; i < NUM_PARTICIPANTS; i++) {
		services[i] = i;
	}

    printf("== PHASE 1: COLLECT VOTES ==\n");

	for (int i = 0; i < NUM_PARTICIPANTS; i++) {
		void* service_id = (void*) &services[i];
		pthread_create(&threads[i], NULL, Participant_Service, service_id);
	}

	int votes[NUM_PARTICIPANTS];
    void* result;

	for (int i = 0; i < NUM_PARTICIPANTS; i++) {
		pthread_join(threads[i], &result);

		votes[i] = (intptr_t) result;
	}

	int all_ready = 1;

	for (int i = 0; i < NUM_PARTICIPANTS; i++) {
		if(votes[i] == VOTE_ABORT) {
			all_ready = 0;
		}
	}

	int global_decision = all_ready ? GLOBAL_COMMIT : GLOBAL_ABORT;

	printf("== PHASE 2: BROADCASTING DECISION ==\n");

	if(global_decision == GLOBAL_COMMIT) {
		printf("GLOBAL COMMIT: Transaction successful\n");
	} else {
		printf("GLOBAL_ABORT: Transaction failed - rolling back all!\n");
	}

	for (int i = 0; i < NUM_PARTICIPANTS; i++) {
		if(global_decision == GLOBAL_COMMIT) {
			printf("[SERVICE %d] Finalized changes (commit)\n");
		} else {
			printf("[SERVICE %d] Rolled back (abort)\n");
		}
	}
	
	printf("\n2PC Complete. Atomicity Guaranteed.\n");

	return 0;
}