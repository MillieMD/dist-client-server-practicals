/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t balance_lock = PTHREAD_MUTEX_INITIALIZER;
int balance = 1000;

const int LOOP_NUM = 50000;

void* Deposit (void* arg) {
    pthread_mutex_lock(&balance_lock);
	for(int i = 0; i < LOOP_NUM; i++) {
		balance = balance + 1;
	}
	pthread_mutex_unlock(&balance_lock);
}

void* Withdrawal(void* args) {
	pthread_mutex_lock(&balance_lock);
	for(int i = 0; i < LOOP_NUM; i++) {
		balance = balance - 1;
	}
	pthread_mutex_unlock(&balance_lock);
}

int main()
{
    printf("Initial Balance: %i\n", balance);
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
	pthread_t withdrawal_thread;
	pthread_create(&withdrawal_thread, NULL, Withdrawal, NULL);
	
	pthread_t deposit_thread;
	pthread_create(&deposit_thread, NULL, Deposit, NULL);
	
	pthread_join(withdrawal_thread, NULL);
	pthread_join(deposit_thread, NULL);
	
	clock_gettime(CLOCK_MONOTONIC, &end);
    int time_taken = end.tv_nsec - start.tv_nsec;
	
	printf("Final Balace: %i\n", balance);
	printf("Time taken: %i\n", time_taken);
}