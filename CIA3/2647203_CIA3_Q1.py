SIZE = 31      # Default Size of the BST Array


# Custom Stack Structure
class Stack:
    def __init__(self):
        self.items = []

    def push(self, item):
        self.items.append(item)

    def pop(self):
        if not self.items:
            return None
        return self.items.pop()

    def is_empty(self):
        return len(self.items) == 0


# Insert Element in BST and Raise Flag if overflow / duplicate
def insert_bst(T, key):
    idx = 0
    while idx < SIZE:
        if T[idx] == -1:
            T[idx] = key
            return "INSERTED"
        if key == T[idx]:
            return "DUPLICATE"
        elif key < T[idx]:
            idx = 2 * idx + 1
        else:
            idx = 2 * idx + 2
    return "OVERFLOW"


# Inorder Traversal: Left Root Right
def iterative_inorder(T):
    stack = Stack()
    result = []
    curr = 0

    while (curr < SIZE and T[curr] != -1) or not stack.is_empty():
        while curr < SIZE and T[curr] != -1:
            stack.push(curr)
            curr = 2 * curr + 1
        curr = stack.pop()
        result.append(T[curr])
        curr = 2 * curr + 2

    return result


# BST is vlaid if inorder is in Assending Order
def is_bst_valid(T):
    inorder_vals = iterative_inorder(T)
    total_nodes = sum(1 for val in T if val != -1)
    if len(inorder_vals) != total_nodes:
        return inorder_vals, False
    for i in range(len(inorder_vals) - 1):
        if inorder_vals[i] >= inorder_vals[i + 1]:
            return inorder_vals, False
    return inorder_vals, True


# Helper Function to rearrange the elements for deleting elements in the BST
def move_subtree(T, src, dst):
    stack = Stack()
    stack.push((src, dst))
    moved = []

    while not stack.is_empty():
        s, d = stack.pop()
        if s < SIZE and T[s] != -1:
            moved.append((d, T[s]))
            T[s] = -1
            stack.push((2 * s + 1, 2 * d + 1))
            stack.push((2 * s + 2, 2 * d + 2))

    for d, val in moved:
        if d < SIZE:
            T[d] = val


# Deleting elements in the BST
def delete_bst(T, key):
    idx = 0
    while idx < SIZE and T[idx] != -1:
        if key == T[idx]:
            break
        elif key < T[idx]:
            idx = 2 * idx + 1
        else:
            idx = 2 * idx + 2

    if idx >= SIZE or T[idx] == -1:
        return False

    left = 2 * idx + 1
    right = 2 * idx + 2
    has_left = left < SIZE and T[left] != -1
    has_right = right < SIZE and T[right] != -1

    if not has_left and not has_right:
        T[idx] = -1
    elif has_left and not has_right:
        move_subtree(T, left, idx)
    elif has_right and not has_left:
        move_subtree(T, right, idx)
    else:
        # Two children: find inorder successor (leftmost node in right subtree)
        succ = right
        while 2 * succ + 1 < SIZE and T[2 * succ + 1] != -1:
            succ = 2 * succ + 1
        T[idx] = T[succ]
        succ_right = 2 * succ + 2
        if succ_right < SIZE and T[succ_right] != -1:
            move_subtree(T, succ_right, succ)
        else:
            T[succ] = -1

    return True


# Calculate BST Height
def tree_height(T):
    if T[0] == -1:
        return -1

    stack = Stack()
    stack.push((0, 0))
    max_depth = 0

    while not stack.is_empty():
        idx, depth = stack.pop()
        if depth > max_depth:
            max_depth = depth
        left = 2 * idx + 1
        right = 2 * idx + 2
        if left < SIZE and T[left] != -1:
            stack.push((left, depth + 1))
        if right < SIZE and T[right] != -1:
            stack.push((right, depth + 1))

    return max_depth


# Count empty slots in the BST Array (by counting -1)
def empty_slots_until_last(T):
    last_used = -1
    for i in range(SIZE):
        if T[i] != -1:
            last_used = i
    if last_used == -1:
        return 0
    return sum(1 for i in range(last_used + 1) if T[i] == -1)


if __name__ == "__main__":
    R = int(input("Enter R: "))
    T = [-1] * SIZE

    # (a) Insert keys k(i) = (R * (i + 7)) % 89 + 10 for i = 0..11
    print("\n--- (a) Insertions ---")
    keys = [(R * (i + 7)) % 89 + 10 for i in range(12)]
    print("Generated keys:", keys)
    for key in keys:
        status = insert_bst(T, key)
        if status == "OVERFLOW":
            print(f"OVERFLOW {key}")
        elif status == "DUPLICATE":
            print(f"Skipped duplicate {key}")
        else:
            print(f"After inserting {key}: {T}")

    # (b) Iterative inorder traversal and BST validity check
    print("\n--- (b) Iterative Inorder & Validity ---")
    inorder_vals, valid = is_bst_valid(T)
    print("Inorder traversal:", inorder_vals)
    print("BST VALID" if valid else "INVALID")

    # (c) Delete a key and move subtrees
    print("\n--- (c) Deletion ---")
    del_key = int(input("Enter key to delete: "))
    print("T before deletion:", T)
    deleted = delete_bst(T, del_key)
    if not deleted:
        print(f"Key {del_key} not found.")
    print("T after deletion: ", T)

    # (d) Height and empty slots up to last used index
    print("\n--- (d) Height & Empty Slots ---")
    h = tree_height(T)
    empty_count = empty_slots_until_last(T)
    print("Height:", h)
    print("Empty slots up to last used index:", empty_count)
