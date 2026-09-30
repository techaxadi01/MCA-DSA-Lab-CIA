/*
Lab Exercise 2:
Searching and Hashing (Hash Table with Kept-Sorted Arrays & Separate Chaining)
*/

/*
Time and Space Complexity :-

1. INSERT (dept, rollNo):
    Average Time Complexity : O(k)
    Worst Time Complexity   : O(D + k)

2. SEARCH (dept, rollNo):
    Average Time Complexity : O(log k)
    Worst Time Complexity   : O(D + log k)

3. RANK (dept, rollNo):
    Average Time Complexity : O(log k)
    Worst Time Complexity   : O(D + log k)

4. DELETE (dept, rollNo):
    Average Time Complexity : O(k)
    Worst Time Complexity   : O(D + k)

Space Complexity :
    Overall Space Complexity : O(M + N)

Where:
    M = Number of buckets in the Hash Table
    N = Total number of student records across all departments
    D = Number of colliding departments in a bucket chain
    k = Number of roll numbers in a specific department
*/

/*
Memory Leak Testing :-

Tested by reviewing all dynamically allocated memory (HashTable, bucket pointers, DeptGroup nodes, and dynamic 'rolls' arrays) using Valgrind.
Verified that every malloc/calloc/realloc allocation is released using free().

All department roll arrays, department group nodes, bucket pointer array, and the HashTable structure itself are completely freed before program termination.
Ensuring zero memory leaks.

Valgrind Output :

'''
 All Dynamically Allocated Space Cleared
==270==
==270== HEAP SUMMARY:
==270==     in use at exit: 0 bytes in 0 blocks
==270==   total heap usage: 8 allocs, 8 frees, 2,224 bytes allocated
==270==
==270== All heap blocks were freed -- no leaks are possible
==270==
==270== For lists of detected and suppressed errors, rerun with: -s
==270== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
'''
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DEPT_LEN 6


// Structure for Department Group Node (Separate Chaining + Dynamic Sorted Array)
typedef struct DeptGroup {
    char dept[MAX_DEPT_LEN];  // Department code string (e.g., "CSE")
    int *rolls;               // Dynamically allocated sorted array of roll numbers
    int size;                 // Current number of roll numbers stored
    int capacity;             // Allocated capacity of rolls array
    struct DeptGroup *next;   // Pointer to next department group in bucket chain
} DeptGroup;


// Structure for Hash Table Container
typedef struct HashTable {
    DeptGroup **buckets;      // Array of bucket pointers (head of each bucket's linked list)
    int t_size;               // Number of buckets in the Hash Table
} HashTable;


// Function Prototypes
void upper(char str[]);
int hash(const char *dept, int t_size);
DeptGroup *find_grp(HashTable *HT, int bkt_idx, const char *dept);
DeptGroup *create_grp(const char *dept);
HashTable *create_table(int t_size);
void insert(HashTable *HT, const char *dept, int rollNo);
void search(HashTable *HT, const char *dept, int rollNo);
void rank(HashTable *HT, const char *dept, int rollNo);
void delete(HashTable *HT, const char *dept, int rollNo);
void free_space(HashTable *HT);



int main(){
    int t_size, no_op;
    int IP_chk, rollNo;
    char op[10], dept[MAX_DEPT_LEN];

    // Step 1:
    // Taking Input of Hash Table Size (No of Buckets) and Number of Operations
    printf("\n Enter the size of the Hash Table (No of Buckets) & the No of operations : ");
    scanf("%d %d", &t_size, &no_op);

    // Step 2:
    // Initialize the Hash Table with t_size buckets
    HashTable *HT = create_table(t_size);

    // Step 3:
    // Process each operation
    for (int i = 0; i < no_op; i++){
        printf("\n\t Enter the Operation and the Data : ");
        IP_chk = scanf("%s %s %d", op, dept, &rollNo);

        if (IP_chk != 3){
            printf("\n ERROR : Input Error, Try Again !");
            break;
        }

        // Convert operation and department code to uppercase for case-insensitivity
        upper(op);
        upper(dept);

        // Perform operation based on command
        if (strcmp(op, "INSERT") == 0){
            insert(HT, dept, rollNo); // Insert roll number in sorted order
        }
        else if (strcmp(op, "SEARCH") == 0){
            search(HT, dept, rollNo); // Binary search for roll number
        }
        else if (strcmp(op, "RANK") == 0){
            rank(HT, dept, rollNo);   // Find count of strictly smaller roll numbers
        }
        else if (strcmp(op, "DELETE") == 0){
            delete(HT, dept, rollNo); // Remove roll number and cleanup empty group
        }
        else{
            printf("\n ERROR : Invalid Operation !");
        }
    }

    // Step 4:
    // Free all dynamically allocated memory (for no leaks)
    free_space(HT);
    return 0;
}


// Function to convert string to uppercase
void upper(char str[]){
    for (int i = 0; str[i] != '\0'; i++){
        str[i] = toupper(str[i]);
    }
}


// Polynomial Rolling Hash Function: maps department string to bucket index: h = (h * 31 + c) % t_size
int hash(const char *dept, int t_size) {
    int h = 0;
    for (int i = 0; dept[i] != '\0'; i++) {
        h = (h * 31 + dept[i]) % t_size;
    }
    return h;
}


// Function to find department group in a bucket using strcmp (Separate Chaining)
DeptGroup *find_grp(HashTable *HT, int bkt_idx, const char *dept){
    DeptGroup *curr = HT->buckets[bkt_idx];
    while (curr != NULL){
        if (strcmp(curr->dept, dept) == 0){
            return curr; // Found exact department group
        }
        curr = curr->next; // Move to next department in bucket chain
    }
    return NULL; // Department not found
}


// Function to create and initialize a new department group node
DeptGroup *create_grp(const char *dept){
    DeptGroup *grp = (DeptGroup *)malloc(sizeof(DeptGroup));
    if (!grp){
        printf("\n ERROR : Memory Allocation Failed");
        return NULL;
    }

    strncpy(grp->dept, dept, MAX_DEPT_LEN-1);
    grp->dept[MAX_DEPT_LEN - 1] = '\0';
    grp->size = 0;
    grp->capacity = 5; // Initial capacity for roll numbers array
    grp->rolls = (int *)malloc(grp->capacity * sizeof(int));
    if (!grp->rolls){
        printf("\n ERROR : Memory Allocation Failed");
        free(grp);
        return NULL;
    }
    grp->next = NULL;
    return grp;
}


// Function to create and initialize the Hash Table container
HashTable *create_table(int t_size){
    HashTable *HT = (HashTable *)malloc(sizeof(HashTable));
    if (!HT){
        printf("\n ERROR : Memory Allocation Failed");
        return NULL;
    }
    HT->t_size = t_size;
    // Allocate bucket pointer array and initialize all bucket pointers to NULL using calloc
    HT->buckets = (DeptGroup **)calloc(t_size, sizeof(DeptGroup *));
    if (!HT->buckets){
        printf("\n ERROR : Memory Allocation Failed");
        free(HT);
        return NULL;
    }
    return HT;
}


// INSERT Operation: Binary search for sorted insertion position and shift-insert
void insert(HashTable *HT, const char *dept, int rollNo){
    int bkt = hash(dept, HT->t_size); // Hash dept to get bucket index
    DeptGroup *grp = find_grp(HT, bkt, dept); // Search for department group in bucket

    // If department is not present in bucket, create it and prepend to chain
    if (grp == NULL){
        grp = create_grp(dept);
        if (!grp){
            return;
        }
        grp->next = HT->buckets[bkt];
        HT->buckets[bkt] = grp;
    }

    // Dynamic array resizing: double capacity using realloc when array is full (amortized O(1))
    if (grp->size == grp->capacity){
        grp->capacity *= 2;
        grp->rolls = (int *)realloc(grp->rolls, grp->capacity * sizeof(int));
    }

    // Binary search to find sorted insertion position in O(log k)
    int low = 0, high = grp->size - 1;
    int pos = grp->size; // Default position at end of array
    while (low <= high){
        int mid = low + ((high - low) / 2);
        if (grp->rolls[mid] >= rollNo){
            pos = mid;
            high = mid - 1; // Continue checking left half
        }
        else{
            low = mid + 1;  // Continue checking right half
        }
    }

    // Shift elements to the right to open space at insertion position
    for (int i = grp->size; i > pos; i--){
        grp->rolls[i] = grp->rolls[i - 1];
    }
    grp->rolls[pos] = rollNo; // Place new roll number at sorted position
    grp->size++;
}


// SEARCH Operation: Binary search within department's sorted array in O(log k)
void search(HashTable *HT, const char *dept, int rollNo){
    int bkt = hash(dept, HT->t_size); // Hash dept to get bucket index
    DeptGroup *grp = find_grp(HT, bkt, dept); // Find department group in bucket

    // If department does not exist in hash table
    if (grp == NULL){
        printf("\n NOT FOUND");
        return;
    }

    // Binary search in sorted roll numbers array
    int low = 0, high = grp->size - 1;
    int found = 0;
    while (low <= high){
        int mid = low + ((high - low) / 2);
        if (grp->rolls[mid] == rollNo){
            found = 1; // Element found
            break;
        }
        else if (grp->rolls[mid] < rollNo){
            low = mid + 1; // Search right half
        }
        else{
            high = mid - 1; // Search left half
        }
    }

    // Output search result
    if (found){
        printf("\n FOUND");
    }
    else{
        printf("\n NOT FOUND");
    }
}


// RANK Operation: Count how many roll numbers in department are strictly smaller
void rank(HashTable *HT, const char *dept, int rollNo){
    int bkt = hash(dept, HT->t_size); // Hash dept to get bucket index
    DeptGroup *grp = find_grp(HT, bkt, dept); // Find department group in bucket

    // If department does not exist, rank is 0
    if (grp == NULL){
        printf("0\n");
        return;
    }

    // Binary search to find 0-indexed position
    int low = 0, high = grp->size - 1;
    int rank = -1;
    while (low <= high){
        int mid = low + ((high - low) / 2);
        if (grp->rolls[mid] == rollNo){
            rank = mid; // In 0-indexed sorted array, index equals count of strictly smaller elements
            break;
        }
        else if (grp->rolls[mid] < rollNo){
            low = mid + 1;
        }
        else{
            high = mid - 1;
        }
    }

    // Output rank
    if (rank != -1){
        printf("%d\n", rank);
    }
    else{
        // Fallback: low gives count of smaller elements if rollNo is absent
        printf("%d\n", low);
    }
}


// DELETE Operation: Removes roll number, shifts elements left, and cleans up empty group node
void delete(HashTable *HT, const char *dept, int rollNo){
    int bkt = hash(dept, HT->t_size); // Hash dept to get bucket index
    DeptGroup *curr = HT->buckets[bkt];
    DeptGroup *prev = NULL;

    // Search bucket chain for department while tracking previous pointer
    while (curr != NULL && strcmp(curr->dept, dept) != 0){
        prev = curr;
        curr = curr->next;
    }

    // Department not found
    if (curr == NULL){
        return;
    }

    // Binary search to find the roll number to delete
    int low = 0, high = curr->size - 1;
    int pos = -1;
    while (low <= high){
        int mid = low + ((high - low) / 2);
        if (curr->rolls[mid] == rollNo){
            pos = mid; // Found roll number index
            break;
        }
        else if (curr->rolls[mid] < rollNo){
            low = mid + 1;
        }
        else{
            high = mid - 1;
        }
    } 

    // Roll number not found in department
    if (pos == -1){
        return;
    }

    // Shift elements left to fill the gap
    for (int i = pos; i < curr->size - 1; i++){
        curr->rolls[i] = curr->rolls[i + 1];
    }
    curr->size--;

    // Group Cleanup: If department's array becomes empty, unlink node and free memory
    if (curr->size == 0){
        if (prev == NULL){
            HT->buckets[bkt] = curr->next; // Unlink first node in bucket
        }
        else{
            prev->next = curr->next;       // Unlink internal node in bucket
        }

        free(curr->rolls); // Free dynamic array
        free(curr);        // Free department group node
    }

    printf("\n Element Deleted \n");
}


// Free Table: Frees all dynamically allocated memory (no memory leaks)
void free_space(HashTable *HT){
    if (!HT){
        return;
    }

    // Traverse all buckets
    for (int i = 0; i < HT->t_size; i++){
        DeptGroup *curr = HT->buckets[i];
        while (curr != NULL){
            DeptGroup *temp = curr;
            curr = curr->next;
            free(temp->rolls); // Free dynamic array of roll numbers
            free(temp);        // Free department group node
        }
    }
    free(HT->buckets); // Free array of bucket pointers
    free(HT);          // Free hash table container

    printf("\n All Dynamically Allocated Space Cleared \n");
}
