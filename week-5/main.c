/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 50

char shared_buffer[BUFFER_SIZE];
pthread_mutex_t buffer_lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int player_id;
    float x_coord;
    char action[10];
} GameState;

void serialize(const GameState* state, char *buffer){
    memcpy(buffer, &state->player_id, sizeof(state->player_id));
    memcpy(buffer + 4, &state->x_coord, sizeof(state->x_coord));
    memcpy(buffer + 8, &state->action, sizeof(state->action));
    
}

void* sender_thread(void* arg){
    pthread_mutex_lock(&buffer_lock);
    GameState msg = {rand()%100, rand()%100, "jump"};
    serialize(&msg, shared_buffer);
    pthread_mutex_unlock(&buffer_lock);
    
    printf("Server sent update\n");
}

void deserialize(const char *buffer, GameState *state){
    memcpy(&state->player_id, buffer, sizeof(int));
    memcpy(&state->x_coord, buffer + 4, sizeof(int));
    memcpy(&state->action, buffer + 8, 10 * sizeof(char));
    
}

void* reciever_thread(void* arg){
    pthread_mutex_lock(&buffer_lock);
    
    char temp[50];
    memcpy(&temp, &shared_buffer, BUFFER_SIZE*sizeof(char));
    
    GameState recieved;
    deserialize(temp, &recieved);
    
    pthread_mutex_unlock(&buffer_lock);
    
    printf("Client recieved: \nID: %d\nX: %.1f\nAction:%s\n", 
    recieved.player_id, 
    recieved.x_coord, 
    recieved.action);
    
}

int main()
{
    pthread_t sender;
    pthread_create(&sender, NULL, sender_thread, NULL);
    
    usleep(10000);
    
    pthread_t reciever;
    pthread_create(&reciever, NULL, reciever_thread, NULL);
    
    pthread_join(sender, NULL);
    printf("sender joined");
    pthread_join(reciever, NULL);

    return 0;
}