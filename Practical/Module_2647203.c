#include "Module_2647203.h"


// Function to create and initialize the queue
Queue *create_queue(){
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (!q){
        printf("\n ERROR : Memory Allocation Failed !");
        return NULL;
    }
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}


// Function to check if queue is empty
int isEmpty(Queue *q) {
    return (q == NULL || q->front == NULL);
}


// ENQUEUE Operation: Adds order to rear in O(1) worst-case time
int enqueue(Queue *q, Order order){
    if (!q) return 0;

    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode){
        printf("\n ERROR : Memory Allocation Failed !");
        return 0;
    }

    newNode->order = order;
    newNode->next = NULL;

    if (q->rear == NULL){
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
    return 1;
}


// DEQUEUE Operation: Removes order from front in O(1) worst-case time and frees node memory
int dequeue(Queue *q, Order *out){
    if (isEmpty(q)){
        printf("\n ERROR : Queue Underflow !");
        return 0;
    }

    Node *temp = q->front;
    if (out != NULL){
        *out = temp->order;
    }

    q->front = q->front->next;
    if (q->front == NULL){
        q->rear = NULL;
    }
    q->size--;

    free(temp); // Free dynamically Allocated Memory
    return 1;
}


// PEEK Operation: Views front order in O(1) worst-case time
int peek(Queue *q, Order *out){
    if (isEmpty(q)){
        printf("\n ERROR : Queue is empty !");
        return 0;
    }

    if (out != NULL){
        *out = q->front->order;
    }
    return 1;
}


// Free all nodes and queue container (no memory leaks)
void del_queue(Queue *q){
    if (!q) return;

    Node *curr = q->front;
    while (curr != NULL){
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }

    free(q);
    printf("\n All Dynamically Allocated Space Cleared \n");
}
