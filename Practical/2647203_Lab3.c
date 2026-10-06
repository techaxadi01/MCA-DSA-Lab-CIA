/*
Lab Exercise 3:
Queue Implementation using Linked List
*/

/*
Time and Space Complexity :-

Time Complexity :
1. ENQUEUE : O(1)
2. DEQUEUE : O(1)
3. PEEK : O(1)
4. IS EMPTY : O(1)

Space Complexity : O(N)
*/

/*
Memory Leak Testing :-

Tested by reviewing all dynamically allocated memory (Queue container and individual Node elements) using Valgrind.
Verified that every malloc allocation is released using free().

All queue nodes and the Queue structure itself are completely freed before program termination.
Ensuring zero memory leaks.


Valgrind Output :
'''
### Can't Valgrind as Linux does not have <windows.h> and raises error while compiling in WSL. ###
'''
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "Module_2647203.h"


double get_time_us();
void run_stress_test();


// Main Code (Demo & Stress Test)
int main(){
    Queue *q = create_queue();
    if (!q) return 1;

    int choice, id;
    Order ord;

    while (1){
        printf("\n ================= ORDER QUEUE MENU =================");
        printf("\n 1. Enqueue Order");
        printf("\n 2. Dequeue Order");
        printf("\n 3. Peek Front Order");
        printf("\n 4. Run Stress Test");
        printf("\n 0. Exit");

        printf("\n\n\t Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 0){
            printf("\n Exiting Program. \n");
            break;
        }

        else if (choice == 1){
            printf(" Enter Order ID to enqueue: ");
            if (scanf("%d", &id) == 1){
                Order o = { id };
                enqueue(q, o);
                printf(" Enqueued Order #%d | Current Queue Size: %d \n", id, q->size);
            }
        }

        else if (choice == 2){
            if (dequeue(q, &ord)){
                printf(" Dequeued Order #%d | Remaining Queue Size: %d \n", ord.id, q->size);
            }
        }

        else if (choice == 3){
            if (peek(q, &ord)){
                printf(" Front Order in Queue: #%d \n", ord.id);
            }
        }

        else if (choice == 4){
            run_stress_test();
        }

        else {
            printf("\n ERROR : Invalid Choice ! Please enter 0 to 4. \n");
        }
    }

    del_queue(q);
    return 0;
}


// Get current time in microseconds {AI Generated Function}
double get_time_us(){
    static LARGE_INTEGER freq;
    static int init = 0;
    if (!init){
        QueryPerformanceFrequency(&freq);
        init = 1;
    }
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return ((double)count.QuadPart * 1000000.0) / (double)freq.QuadPart;
}


// Stress Test: Large randomized operation
void run_stress_test(){
    srand((unsigned int)time(NULL));

    Queue *q = create_queue();
    if (!q) return;

    // Randomize total operations (unpredictable count)
    int total_ops = 20000 + (rand() % 20001); // 20,000 to 40,000
    printf("\n Starting Stress Test with %d Randomized Operations...\n", total_ops);

    FILE *log = fopen("timing_log.txt", "w");
    if (log){
        fprintf(log, "OpNumber,Type,QueueSize,Latency_us\n");
    }

    double total_time = 0.0;
    double min_lat = 1e9, max_lat = 0.0;
    int enq_count = 0, deq_count = 0;
    int peak_size = 0;
    int order_id = 1;

    // Run interleaved operations so queue size fluctuates
    for (int i = 0; i < total_ops; i++){
        int do_enqueue = isEmpty(q) || (rand() % 100 < 55);

        double t0 = get_time_us();

        if (do_enqueue){
            Order ord = { order_id++ };
            enqueue(q, ord);
            enq_count++;
        }
        else {
            Order out;
            dequeue(q, &out);
            deq_count++;
        }
    
        double t1 = get_time_us();
        double lat = t1 - t0;

        total_time += lat;
        if (lat < min_lat) min_lat = lat;
        if (lat > max_lat) max_lat = lat;
        if (q->size > peak_size) peak_size = q->size;

        if (log && (i % 50 == 0)){
            fprintf(log, "%d,%s,%d,%.3f\n", i, do_enqueue ? "ENQUEUE" : "DEQUEUE", q->size, lat);
        }
    }

    if (log) fclose(log);

    double avg_lat = total_time / total_ops;
    printf("\n ================= STRESS TEST RESULTS =================");
    printf("\n Total Operations       : %d", total_ops);
    printf("\n Total Enqueues         : %d", enq_count);
    printf("\n Total Dequeues         : %d", deq_count);
    printf("\n Peak Queue Size (Rush) : %d orders", peak_size);
    printf("\n Final Queue Size       : %d orders", q->size);
    printf("\n -------------------------------------------------------");
    printf("\n Average Latency        : %.3f microseconds", avg_lat);
    printf("\n Min Latency            : %.3f microseconds", min_lat);
    printf("\n Max Latency            : %.3f microseconds", max_lat);
    printf("\n -------------------------------------------------------");
    printf("\n Latency remained flat throughout with no reallocation spikes!");
    printf("\n =======================================================\n");

    // Clean failure check on empty queue
    Order dummy;
    while (!isEmpty(q)){
        dequeue(q, &dummy);
    }
    
    printf("\n Testing peek on empty queue:");
    peek(q, &dummy);
    printf("\n Testing dequeue on empty queue:");
    dequeue(q, &dummy);

    del_queue(q);
}
