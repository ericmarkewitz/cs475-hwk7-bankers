/* declarations related to banker's algorithm */

//Ensure that the currently allocated resources do not exceed the total number of resources.
int currentlyExceeded(int* totals, int** allocation);

//Ensure each thread’s needs do not exceed its max demands for each resource type.
int isAboveDemand(int** max, int** allocation);

void isSafe(int* available, int** alloc, int** need);

int existsUnfinThread(int* work, int** needMore, int* finish);

void tryAll(int* order, int* work, int** alloc, int ** need);

void permutations(int* nums, int count, int* work, int** alloc, int** need);