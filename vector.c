#include <stdio.h>
#include <stdlib.h>
#include "vector.h"


//Creates a vector copy without side effects
int* deepCopyVector(int* vect){
    int* vectCopy = (int*) malloc(sizeof(int) * NUM_RESOURCES);
    for(int i=0; i<NUM_RESOURCES; i++){
        vectCopy[i] = vect[i];
    }
    return vectCopy;
}

//Creates a matrix copy without side effects (Using the vector copy for the assist)
int** deepCopyMatrix(int** matrix){
    int**  matrixCopy = (int**) malloc(sizeof(int*) * NUM_THREADS);
    for(int i=0; i<NUM_THREADS; i++){
        matrixCopy[i] = deepCopyVector(matrix[i]);
    }
    return matrixCopy;
}

//Returns 0 if any of the resources in v2 is higher than v1, returns 1 otherwise
int compareVectors(int* v1, int* v2){
    for(int i=0; i<NUM_RESOURCES; i++){
        if (v1[i] < v2[i]) return 0;
    }
    return 1;
}

//Adds the values of v2 into v1
void addVectors(int* v1, int* v2){
    //int* sumVect;
    for(int i=0; i<NUM_RESOURCES; i++){
        v1[i] = v1[i]+v2[i];
    }
}

//Subtracts the values of v2 from v1
void subtractVectors(int* v1, int* v2){
    for(int i=0; i<NUM_RESOURCES; i++){
        v1[i] = (v1[i])-(v2[i]);
    }
}

//Subtracts the values of m2 from m1
void subtractMatricies(int** m1, int** m2){
    for(int i=0; i<NUM_THREADS; i++){
        subtractVectors(m1[i], m2[i]);
    }
}

//Prints the vector with brackets surrounding
void printVector(int* vect){
    printf("[");
    for(int i=0; i<NUM_RESOURCES; i++){
        printf("%d ", vect[i]);
    }
    printf("]\n");
}

void printFinishVector(int* vect){
    printf("[");
    for(int i=0; i<NUM_THREADS; i++){
        printf("%d ", vect[i]);
    }
    printf("]\n");
}

void printMatrix(int** matrix){
    for(int i=0; i<NUM_THREADS; i++){
        int* vect = matrix[i];
        printVector(vect);
    }
    printf("\n");
}