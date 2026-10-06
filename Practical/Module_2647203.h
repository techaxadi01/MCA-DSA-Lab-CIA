#ifndef Module_2647203_H
#define Module_2647203_H

#include <stdio.h>
#include <stdlib.h>


// Order ID (if in future we want to change the structure of order eg we can add the customer name if we want)
typedef struct Order {
    int id;
} Order;


// Singly Linked List Queue
typedef struct Node {
    Order order;
    struct Node *next;
} Node;


// Defining the Front & Rear of Queue
typedef struct Queue {
    Node *front;
    Node *rear;
    int size;
} Queue;


// Function Prototypes
Queue *create_queue();
int isEmpty(Queue *q);
int enqueue(Queue *q, Order order);
int dequeue(Queue *q, Order *out);
int peek(Queue *q, Order *out);
void del_queue(Queue *q);


#endif
