#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct Stack{
    char arr[100];
    int size;
    int top;
} Stack;


int main(){
    int n;
    char arr[100];

    printf("Enter the size of the stack :");
    scanf("%d",&n);

    printf("Enter the brackets : \n");
    for(int i= 0 ; i < n; i++){
        scanf(" %c", &arr[i]);
    }

    Stack *st = (Stack *)malloc(sizeof(Stack));
    st->top = -1;
    st->size = n;

    int isBalanced = 1;
    for (int i=0 ; i< n; i++){
        if (arr[i] == '(' || arr[i] == '[' || arr[i] == '{'){
            st->arr[++st->top] = arr[i];
        }
        else if (arr[i] == ')' || arr[i] == ']' || arr[i] == '}'){
            if (st->top == -1 ||
                (arr[i] == ')' && st->arr[st->top] != '(') ||
                (arr[i] == ']' && st->arr[st->top] != '[') ||
                (arr[i] == '}' && st->arr[st->top] != '{')){
                isBalanced = 0;
                break;
            }
            st->top--;
        }
    }

    if (st->top != -1){
        isBalanced = 0;
    }

    if (isBalanced){
        printf("\n Brackets are Balanced");
    }
    else{
        printf("\n Brackets are NOT Balanced");
    }

    free(st);
    return 0;
}


