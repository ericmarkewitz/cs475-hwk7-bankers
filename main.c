#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"

int main(int argc, char *argv[])
{
  // TODO: attempt to open scenario file and scan data into allocated structures
  
  //First test number of arguments
  if(argc < 2){
    printf("Usage: ./bankers <state file>\n");
		return -1;
  }
  else if(argc > 2){
    printf("Usage: ./bankers <state file>\n");
		return -1;
  }
  char* fileName = argv[1];
  
  FILE * fp  = fopen(fileName, "r");

  //If the file doesn't exist or can't open
  if(fp == NULL){
    printf("Usage: ./bankers <state file>\n");
    printf("Error opening state file\n");
    return -1;
  }
  
  //Read out the file line by line
  fscanf(fp, "%d", &NUM_RESOURCES);
  fscanf(fp, "%d", &NUM_THREADS);
  
  int* resourceInstances = (int*) malloc(sizeof(int) * NUM_RESOURCES);
  for(int i=0; i<NUM_RESOURCES; i++){
    fscanf(fp, "%d", &resourceInstances[i]);
  }
  
  int** maxMatrix = (int**) malloc(sizeof(int*) * NUM_THREADS);
  for(int i=0; i<NUM_THREADS; i++){
    int* currVect = (int*) malloc(sizeof(int) * NUM_RESOURCES);
    for(int j=0; j<NUM_RESOURCES; j++){
      fscanf(fp, "%d", &currVect[j]);
    }
    maxMatrix[i] = currVect;
  }

  int** allocMatrix = (int**) malloc(sizeof(int*) * NUM_THREADS);
  for(int i=0; i<NUM_THREADS; i++){
    int* currVect = (int*) malloc(sizeof(int) * NUM_RESOURCES);
    for(int j=0; j<NUM_RESOURCES; j++){
      fscanf(fp, "%d", &currVect[j]);
    }
    allocMatrix[i] = currVect;
  }

  //Print out file contents (Mostly to check the file scanning worked)
  printf("%d\n", NUM_RESOURCES);
  printf("%d\n\n", NUM_THREADS);
  printVector(resourceInstances);
  printf("\n");
  printMatrix(maxMatrix);
  printf("\n");
  printMatrix(allocMatrix);
  

  // Do sanity checks then run Banker's if neither are an issue
  if(currentlyExceeded(resourceInstances, allocMatrix)) return -1;
  else if(isAboveDemand(maxMatrix, allocMatrix)) return -1;

  isSafe(resourceInstances, allocMatrix, maxMatrix);


  free(resourceInstances);

  for(int i=0; i<NUM_THREADS; i++){
    free(maxMatrix[i]);
    free(allocMatrix[i]);
  }
  free(maxMatrix);
  
  free(allocMatrix);
  
  fclose(fp);
  return 0;
  
}