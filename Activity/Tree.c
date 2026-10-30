#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int data;
    struct ListNode *next;
} ListNode;

typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

long comparisons = 0;

ListNode *listInsertTail(ListNode *head, int value)
{
    ListNode *node = malloc(sizeof(ListNode));
    ListNode *p;
    if (node == NULL) return head;
    node->data = value;
    node->next = NULL;
    if (head == NULL) return node;
    p = head;
    while (p->next != NULL) p = p->next;
    p->next = node;
    return head;
}

void listPrint(ListNode *head)
{
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

void listFree(ListNode *head)
{
    while (head != NULL) {
        ListNode *next = head->next;
        free(head);
        head = next;
    }
}

TreeNode *newTreeNode(int value)
{
    TreeNode *node = malloc(sizeof(TreeNode));
    if (node == NULL) return NULL;
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreeNode *insert(TreeNode *root, int value)
{
    if (root == NULL) return newTreeNode(value);
    if (value < root->data) root->left = insert(root->left, value);
    else root->right = insert(root->right, value);
    return root;
}

void inorder(TreeNode *root)
{
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

void preorder(TreeNode *root)
{
    if (root == NULL) return;
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

int height(TreeNode *root)
{
    int left, right;
    if (root == NULL) return 0;
    left = height(root->left);
    right = height(root->right);
    return 1 + (left > right ? left : right);
}

void freeTree(TreeNode *root)
{
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void writeInorder(TreeNode *root, ListNode **cursor)
{
    if (root == NULL) return;
    writeInorder(root->left, cursor);
    if (*cursor != NULL) {
        (*cursor)->data = root->data;
        *cursor = (*cursor)->next;
    }
    writeInorder(root->right, cursor);
}

ListNode *treeSortList(ListNode *head)
{
    TreeNode *root = NULL;
    ListNode *p, *cursor;
    for (p = head; p != NULL; p = p->next) root = insert(root, p->data);
    cursor = head;
    writeInorder(root, &cursor);
    freeTree(root);
    return head;
}

ListNode *insertionSortList(ListNode *head)
{
    ListNode *sorted = NULL;
    while (head != NULL) {
        ListNode *node = head;
        ListNode **place = &sorted;
        head = head->next;
        while (*place != NULL) {
            comparisons++;
            if ((*place)->data >= node->data) break;
            place = &(*place)->next;
        }
        node->next = *place;
        *place = node;
    }
    return sorted;
}

int isSorted(ListNode *head)
{
    while (head != NULL && head->next != NULL) {
        if (head->data > head->next->data) return 0;
        head = head->next;
    }
    return 1;
}

ListNode *findMiddle(ListNode *head)
{
    ListNode *slow = head, *fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

TreeNode *buildBalanced(ListNode *head)
{
    ListNode *middle, *p;
    TreeNode *root;
    if (head == NULL) return NULL;
    if (head->next == NULL) return newTreeNode(head->data);
    middle = findMiddle(head);
    p = head;
    while (p->next != middle) p = p->next;
    p->next = NULL;
    root = newTreeNode(middle->data);
    if (root == NULL) return NULL;
    root->left = buildBalanced(head);
    root->right = buildBalanced(middle->next);
    return root;
}

ListNode *makeList(int count, int mode)
{
    ListNode *head = NULL;
    int i;
    for (i = 0; i < count; i++) {
        int value;
        if (mode == 0) value = rand();
        else if (mode == 1) value = i;
        else value = count - i;
        head = listInsertTail(head, value);
    }
    return head;
}

void runRow(const char *name, int mode)
{
    ListNode *list;
    TreeNode *root = NULL;
    int i, treeHeight;
    long insertionCount, treeCount = 0;

    list = makeList(2000, mode);
    if (list == NULL) return;
    comparisons = 0;
    list = insertionSortList(list);
    insertionCount = comparisons;
    printf("%s insertion sort: sorted=%s, comparisons=%ld\n", name,
           isSorted(list) ? "yes" : "NO", insertionCount);
    listFree(list);

    if (mode == 0) srand(1);
    list = makeList(2000, mode);
    if (list == NULL) return;
    /* Use the same values for both sorts in the random row. */
    comparisons = 0;
    for (ListNode *p = list; p != NULL; p = p->next) {
        TreeNode **place = &root;
        while (*place != NULL) {
            comparisons++;
            if (p->data < (*place)->data) place = &(*place)->left;
            else place = &(*place)->right;
        }
        *place = newTreeNode(p->data);
    }
    treeCount = comparisons;
    treeHeight = height(root);
    /* Inorder puts the values back into the original list nodes. */
    {
        ListNode *cursor = list;
        writeInorder(root, &cursor);
    }
    printf("%s tree sort: sorted=%s, comparisons=%ld, height=%d\n", name,
           isSorted(list) ? "yes" : "NO", treeCount, treeHeight);
    listFree(list);
    freeTree(root);
}

int main(void)
{
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    int i;
    ListNode *list = NULL;
    TreeNode *root = NULL;

    printf("PART A: linked list\n");
    for (i = 0; i < 7; i++) list = listInsertTail(list, values[i]);
    listPrint(list);

    printf("\nPART B: binary search tree\nInorder: ");
    for (i = 0; i < 7; i++) root = insert(root, values[i]);
    inorder(root);
    printf("\nPreorder: ");
    preorder(root);
    printf("\nHeight: %d\n", height(root));
    freeTree(root);
    root = NULL;
    for (i = 10; i <= 50; i += 10) root = insert(root, i);
    printf("Ascending 10..50 tree height: %d (a linked list)\n", height(root));
    freeTree(root);
    listFree(list);

    printf("\nPART C: sorts of 2000 integers (fixed seed 1)\n");
    printf("Input                 Insertion comparisons  Tree comparisons  Height\n");
    srand(1);
    runRow("Random", 0);
    runRow("Already ascending", 1);
    runRow("Descending", 2);

    printf("\nPART D: balanced tree\n");
    list = NULL;
    for (i = 20; i <= 80; i += 10) list = listInsertTail(list, i);
    root = buildBalanced(list);
    printf("Test data balanced tree height: %d\n", height(root));
    freeTree(root);
    listFree(list);
    list = makeList(2000, 1);
    root = buildBalanced(list);
    printf("2000-item balanced tree height: %d\n", height(root));
    freeTree(root);
    listFree(list);

    printf("\nPART E: written answers\n");
    printf("1. Ascending input makes each new value go right, so the tree becomes a chain like a linked list.\n");
    printf("2. This resembles quicksort: the first value divides smaller and larger values. Already sorted input is a worst case for both this tree sort and quicksort with a first-item pivot.\n");
    printf("3. Merge sort also splits in the middle. Halving each time keeps the tree height near log2(n); for 2000 nodes it is 11.\n");
    printf("4. Tree sort uses O(n) extra memory for tree nodes. Linked-list insertion sort rearranges existing nodes and uses O(1) extra memory.\n");
    printf("5. Tree sort takes O(n log n) on random input on average, and O(n^2) on already sorted input.\n");
    return 0;
}
