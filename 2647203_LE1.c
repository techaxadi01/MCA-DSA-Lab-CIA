/*
Lab Exercise 1:
Array, Linnked List & Stack
*/

/*
Time and Spcae Complexity :-

Approach 1 - Linked List with Stack
    Time Complexity : O(n)
    Space Complexcity : O(n)

Approach 2 - Linked List only by Reversing Half of the List
    Time Complexity : O(n)
    Space Complexcity : O(1)
*/

/*
Memory Leak Testing :-

Tested by reviewing all dynamically allocated memory (Node, Stack, Array) using Debuger.
Verified that every malloc allocation is released using free().

The input array, stack and licked list nodes are freed befre program termination.
*/


#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
    
} Node;

typedef struct Stack {
    int *data;
    int top;
    int capacity;
} Stack;

Node* create_node(int data);
Node* reverse_node(Node *head);
Stack* create_stack(int capacity);


int main(){
    int a, i, n;


    // Step 1:
    // Taking Input of the Number into an array
    printf("\n Enter the no of Elements : ");
    scanf("%d", &n);

    if (n < 0){
        printf("Invalid Input ! \n");
        return 0;
    }
    else if (n == 0){
        printf("Can't check number list of Size 0 for Palindrome ! \n"); // Edge Case Handeling (n=0)
        return 0;
    }
    
    int *arr = (int *)malloc(n * sizeof(int));

    printf("Enter the Elements : \n");
    for (i = 0; i < n; i++){
        scanf("%d", &arr[i]);
    }


    // Step 2:
    // Inserting the elements into the Linked List (Because we have to use Linked List (not Array))
    Node *head = NULL;
    Node *tail = NULL;
    
    for (i = 0; i < n; i++){
        Node *new_node = create_node(arr[i]); 

        if (head != NULL){
            tail->next = new_node; // Set the last node to point to the new Node
            tail = new_node; // Set the tail to the new Node
        }
        else{
            head = new_node; // Initially both head and tail are at NULL so we have to set it to the first node
            tail = new_node; // When the List has only one item its Tail and Head are same
        }
    }


    // Option Seletion for the 2 cases
    printf("\n Select Approach :");
    printf("\n\t 1. Approach 1 - Linked List with Stack (O(n) Space)");
    printf("\n\t 2. Approach 2 - Linked List only by Reversing Half of the List (O(1) Space)");
    printf("\n Enter Choicce : ");
    scanf("%d",&a);


    switch(a){

        // Approach 1 - Linked List with Stack (O(n) Space)
        case 1:
            printf("\n\t Approach 1 - Linked List with Stack \n");


            // Step 3:
            // Push all the elements into the Stack (making a reversed form of the Linked List)
            Stack *stack = create_stack(n);
            Node *curr = head; // curr pointer to traverse the Linked List

            // Loop runs till the pointer hit the Last Node (which points to NULL)
            // OR the Stack is Full (which will not be the case here as capacity is n (size of linked list / input array))
            while ((curr != NULL) && ((stack->top + 1) < stack->capacity)){
                stack->top++; // Increments the top fisrt
                stack->data[stack->top] = curr->data; // Push the data at the current Node into the Stack
                curr = curr->next; // Move the curr pointer to the Next Node in the List
            }


            // Step 4:
            // Second Traversal of the List and comparing it with the Stack elements
            curr = head;
            int is_Palinndrome = 1; // Edge Case Handeling (n=1) (By Default Assums the Input is Palindrome)

            // Loop runs till the pointer hit the Last Node (which points to NULL)
            // OR the Stack is Empty (which will not be the case here as stack has n elements (size of linked list / input array))
            while ((curr != NULL) && (stack->top >= 0)){
                if (curr->data != stack->data[stack->top]){
                    is_Palinndrome = 0; // If there is a mismatch then its not a Palindrome
                    break;
                }

                stack->top--; // Pop the elment form the stack
                curr = curr->next; 
            }


            // Step 5:
            // Print the result
            // The Edge Cases (n=1) are handled automatically by the comparision of the list and stack block
            if (is_Palinndrome){
                printf("\n\t PALINDROME \n");
            } 
            else{
                printf("\n\t NOT A PALINDROME \n");
            }


            // Step 6:
            // Free all dynamically allocated memory (for no leaks)
            free(stack->data); // Free up the stack
            free(stack);

            curr = head;
            while (curr != NULL){
                Node *temp = curr->next;
                free(curr); // Free all Nodes of Linked List
                curr = temp;
            }

            free(arr); // Free the input Array

            printf("\n All Dynamically Allocated Memory Freed \n");
            break;

        
        // Approach 2 - Linked List only by Reversing Half of the List (O(1) Space)
        case 2:
            printf("\n\t Approach 2 - Linked List only by Reversing Half of the List \n");


            // Step 3:
            // Traverse th List to find the Middle element (n/2 if n is even & n+1/2 is n is odd)
            Node *fast = head; // Set both pointers at Head
            Node *slow = head;

            while ((fast->next != NULL) && (fast->next->next != NULL)){
                fast = fast->next->next;
                slow = slow->next; // By the end of the loop the slow will point at the middle element of the list
            }


            // Step 4:
            // Reverse the second half of the list
            Node *mid = slow; // Split the List into two at middle element (n/2+n/2 if n is even & (n+1/2)+(n-1/2) if n is odd)
            Node *rev = mid->next;
            mid->next = NULL;
            Node *rev_head =  reverse_node(rev);


            //Step 5:
            // Compare the first elements of both Main List and Reversed List
            curr = head;
            Node *rev_curr = rev_head;
            is_Palinndrome = 1; // Edge Case Handeling (n=1) (By Default Assums the Input is Palindrome)

            // Loop runs till the pointer hit the Last Node of the reverse List (which points to NULL)
            while (rev_curr != NULL){
                if (curr->data != rev_curr->data){
                    is_Palinndrome = 0; // If there is a mismatch then its not a Palindrome
                    break;
                }

                curr = curr->next;
                rev_curr = rev_curr->next;
            }


            // Step 6:
            // Restore the Linked List
            rev = reverse_node(rev_head);
            mid->next = rev;


            // Step 7:
            // Print the result
            // The Edge Cases (n=1) are handled automatically by the comparision of the list and stack block
            if (is_Palinndrome){
                printf("\n\t PALINDROME \n");
            } 
            else{
                printf("\n\t NOT A PALINDROME \n");
            }


            // Step 8:
            // Free all dynamically allocated memory (for no leaks)
            curr = head;
            while (curr != NULL){
                Node *temp = curr->next;
                free(curr); // Free all Nodes of Linked List
                curr = temp;
            }

            free(arr); // Free the input Array

            printf("\n All Dynamically Allocated Memory Freed \n");
            break;


        default:
            printf("incorrect input. \n");
    }


    return 0;
}


Node* create_node(int data){
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = data; // Insert the data (number) into the new Node
    new_node->next = NULL; // Set the new Node of the List to point to NULL

    return new_node;
}


Node* reverse_node(Node *head){
    Node *curr = head; // Set the current pointer at headd 
    Node *rev_next = NULL;
    Node *rev_prev = NULL;

    while (curr != NULL){
        rev_prev = curr->next; // Storing the next element of llist so its not lost
        curr->next = rev_next; // Setting the pointer to the previous element
        rev_next = curr; // Saving the current Node to add to the next Node
        curr = rev_prev; // Moving the curr forward
    }
    Node *rev_head = rev_next;

    return rev_head;
}


Stack* create_stack(int capacity){
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->data = (int *)malloc(capacity * sizeof(int));
    stack->top = -1;
    stack->capacity = capacity;

    return stack;
}
