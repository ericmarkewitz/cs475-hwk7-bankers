#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"


//Ensure that the currently allocated resources do not exceed the total number of resources.
int currentlyExceeded(int* totals, int** allocation){
    for(int i=0; i<NUM_RESOURCES; i++){
        int currResourceCount = 0;
        for(int j=0; j<NUM_THREADS; j++){
            currResourceCount += allocation[j][i];
        }
        if(currResourceCount > totals[i]) {
            printf("Integrity test failed: allocated resources exceed the total number of resources\n");
            return 1;
        }
    }
    return 0;
}

//Ensure each thread’s needs do not exceed its max demands for each resource type.
int isAboveDemand(int** max, int** allocation){
    for(int i=0; i<NUM_THREADS; i++){
        for(int j=0; j<NUM_RESOURCES; j++){
            int diff = max[i][j] - allocation[i][j] ;
            if(diff < 0){
               printf("Integrity test failed: allocated resources exceed the total number of resources\n");
               printf("Need %d instances of resource %d\n", diff, j);
               return 1;
            }
        }
    }
    return 0;
}


//Safety Algorithm (Banker's)
void isSafe(int* available, int** alloc, int** need){
    int numThreadsFinished = 0;
    int doneThreads[NUM_THREADS];
    int* work = deepCopyVector(available);
    for(int i=0; i<NUM_RESOURCES;i++){
        for(int j=0; j<NUM_THREADS; j++){
            work[i] -= alloc[j][i];
        }
    }
    int* workCopy = deepCopyVector(work);
    int** needMore = deepCopyMatrix(need);
    subtractMatricies(needMore, alloc);

    //Set finish (create an 0ed out array)
    int finish[NUM_THREADS];
    for(int i=0; i<NUM_THREADS; i++){
        finish[i] = 0;
    }

    //Meat & Potatoes of Banker's loops trying to reclaim the resources of different threads
    int i=existsUnfinThread(work, needMore, finish);
    while(i != -1){
        addVectors(work, alloc[i]);
        finish[i] = 1;
        doneThreads[numThreadsFinished] = i;
        numThreadsFinished++;
        i = existsUnfinThread(work, needMore, finish);
    }

    //If any of the threads havent finished print out the order we tried and that it's unsafe
    for(int i=0; i<NUM_THREADS; i++){
        if(finish[i] == 0) {
            printf("UNSAFE: ");
            for(int j=0; j<numThreadsFinished; j++){
                printf("T%d ", doneThreads[j]);
            }
            printf("can't finish\n");
            return;
        }
    }
    
    
    //Regular
    /*
    printf("SAFE: ");
    for(int i=0; i<NUM_THREADS; i++){
         printf("T%d ", doneThreads[i]);
    }
    printf("\n");*/
    
    
    //Extra credit
    int allThreads[NUM_THREADS];
    for(int i=0; i<NUM_THREADS; i++){
        allThreads[i] = i;
    }
    
    permutations(allThreads, NUM_THREADS, workCopy, alloc, need); 
    //Option 2 for the extra credit please!

    free(work);
    free(workCopy);

    for(int i=0; i<NUM_THREADS; i++){
        free(needMore[i]);
    }

    free(needMore);
    
    return;
}

//Check for is safe's while loop, checks to see if theres a unfinished thread we can run given the current state of resources
int existsUnfinThread(int* work, int** needMore, int* finish){
    for(int i=0; i<NUM_THREADS; i++){
        if(finish[i] == 0){
            int* threadResourcesNeeded = needMore[i];
            if(compareVectors(work, threadResourcesNeeded)){
                subtractVectors(needMore[i],threadResourcesNeeded);
                return i;
            }
        }
    }
    return -1;
}

//Poorly named - should be along the lines of 'try one' of the permutations
void tryAll(int* order, int* work, int** alloc, int** need){
    int* localWork = deepCopyVector(work);

    for(int i=0; i<NUM_THREADS; i++){
        int currThread = order[i];
        int* currAlloc = alloc[currThread];
        int* currNeed = need[currThread];
        for(int j=0; j<NUM_RESOURCES; j++){
            int has = currAlloc[j];
            int max = currNeed[j];
            if(localWork[j] < max-has) {
                free(localWork);
                return;
            }
            else{
                localWork[j] += has;
            }
        }
    }
    free(localWork);
    printf("SAFE: ");
    for(int i=0; i<NUM_THREADS; i++){
        printf("T%d ", order[i]);
    }
    printf("\n");
    
}

//Implementation of Heap's algorithm to get all possible permutations
void permutations(int* nums, int count, int* work, int** alloc, int** need){
    //Base call
    if(count==1){
        tryAll(nums, work, alloc, need);

    }
    else{
        permutations(nums, count-1, work, alloc, need);

        for(int i=0; i<count-1; i++){    
            //If even swap ahead
            if(count % 2 == 0) {
                int temp = nums[count-1];
                nums[count-1] = nums[i];
                nums[i] = temp;
            } 
            else { //If odd swap w/ 0
                int temp = nums[0];
                nums[0] = nums[count-1];
                nums[count-1] = temp;
            }
            permutations(nums, count-1, work, alloc, need); //recursive call
        }
        
    }
}