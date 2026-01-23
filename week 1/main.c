/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int request_id;
	char resource_path[50];
	int client_ip_hash;
	double timestamp;
} ClientRequest;

ClientRequest CreateRequest(int id, const char *path, int ip_hash) {

	ClientRequest client;
	client.request_id = id;
	client.timestamp = time(NULL);
	client.client_ip_hash = ip_hash;
	strcpy(client.resource_path, path);
	
	return client;
}

void DisplayRequest(const ClientRequest *req) {
	printf("Client Request ID: %d \n", req->request_id);
	printf("Resource Path: %s \n", req->resource_path);
	printf("Client IP Hash: %d \n", req->client_ip_hash);
	printf("Timestamp: %f \n", req->timestamp);
}

typedef struct {
    int request_id;
    int status_code;
} ServiceResponse;

ServiceResponse* ProcessRequest(const ClientRequest *req){
    
    ServiceResponse *response = malloc(sizeof(ServiceResponse));
    
    if (req == NULL){
        printf("Client Request is NULL. Could not fulfil request.\n");
        return NULL;
    }
    
    response->request_id = req->request_id;
    response->status_code = 200;
    
    printf("Processed request %d -> 200 OK\n", response->request_id);
    
    return response;
}

int main()
{
    const int NUM_REQ = 1000;
    
    ServiceResponse *responses[NUM_REQ];
    // memset used because const int NUM_REQ was throwing error 
    // for variable size initialisation of array
    memset(&responses, 0, sizeof(ServiceResponse)*NUM_REQ); 
    
    for(int i = 0; i < NUM_REQ; i++){
        ClientRequest req = CreateRequest(i+100, "/index.html", 12345678+i);
        DisplayRequest(&req);
        responses[i] = ProcessRequest(&req);
    }
    
    for(int i = 0; i < NUM_REQ; i++){
        if(responses[i] != NULL){
            printf("Freesing response for request %i\n", responses[i]->request_id);
            free(responses[i]);
            responses[i] = NULL;
        }
    }
    
}