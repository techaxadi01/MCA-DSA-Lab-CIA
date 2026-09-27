#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* head = NULL;
Node* tail = NULL;

void insertAtEnd(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;
    if (head == NULL) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
}

void deleteFirstNode() {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    Node* temp = head;
    int value = temp->data;
    head = temp->next;
    if (head == NULL) {
        tail = NULL;
    }
    free(temp);
    printf("Deleted value: %d\n", value);
}

void displayList() {
    Node* curr = head;
    while (curr != NULL) {
        printf("%d -> ", curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}

void reverseList() {
    Node* prev = NULL;
    Node* current = head;
    Node* next = NULL;
    tail = head;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
}

int main() {
    int choice, value;
    while (1) {
        printf("1. Insert at end\n");
        printf("2. Delete first node\n");
        printf("3. Display list\n");
        printf("4. Reverse list\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Enter value to insert: ");
                scanf("%d", &value);
                insertAtEnd(value);
                break;
            case 2:
                deleteFirstNode();
                break;
            case 3:
                displayList();
                break;
            case 4:
                reverseList();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice\n");
        }
    }
    return 0;
}