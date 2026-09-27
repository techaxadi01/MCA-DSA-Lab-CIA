#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

int bin_ser(int ele, int *arr, int n){
    int st = 0, en = n - 1;
    while (st <= en){
        int mid = (st +(en-st)/2);
        if (arr[mid] == ele){
            return mid;
        }
        else if (arr[mid] < ele){
            st = mid + 1;
        }
        else if (arr[mid] > ele){
            en = mid - 1;
        }
        else{
            printf("ERROR");
            break;
        }
    }
    return -1;
}

int main(){
    int arr[100];
    int a,n;

    printf("Enter size of array :");
    scanf("%d",&n);

    printf("enter the elements (in sorted order) :\n");
    for (int i = 0; i<n; i++){
        scanf("%d", &arr[i]);
    }

    printf("Enter element to search ;");
    scanf("%d",&a);

    int result = bin_ser(a, arr, n);

    if (result != -1){
        printf("Element found at index %d\n", result);
    }
    else{
        printf("-1 \n");
    }

    return 0;
}
