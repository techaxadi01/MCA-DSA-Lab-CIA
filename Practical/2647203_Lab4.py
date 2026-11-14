import math
import random

# Node Definition
class Node: # BST Node contains an integer key and references to left and right node.
    def __init__(self,key):
        self.key = key
        self.right = None
        self.left = None


# Part A: Build the Tree

# A1: Create Node
def createNode(key):
    # Creates new Node
    return Node(key)

# A2: Insert New Node
def insert(root, key):
    # End of the Tree create the Node
    if root is None:
        return createNode(key)

    # Key is smaller: insert in left subtree
    elif key < root.key:
        root.left = insert(root.left,key)

    # Key is larger: insert in right subtree
    elif key > root.key:
        root.right = insert(root.right,key)

    # Duplicate Keys: ignored
    else:
        pass

    return root

# A3: Search in the Tree
def search(root, key):
    curr = root
    while curr is not None:
        if curr.key == key:
            return True
        elif key < curr.key:
            curr = curr.left
        else:
            curr = curr.right

    return False


# Part B: Traversals and Measurements

# B1a: Inorder Traversal
def inorder(root):
    if root is not None:
        inorder(root.left)
        print(root.key, end=" ")
        inorder(root.right)

#B1b: Preorder Traversal
def preorder(root):
    if root is not None:
        print(root.key, end=" ")
        preorder(root.left)
        preorder(root.right)

# B1c: Postorder Traversal
def postorder(root):
    if root is not None:
        postorder(root.left)
        postorder(root.right)
        print(root.key, end=" ")

# B3: Height of the Tree
def height(root):
    # Base Case Emplty Node height -1 and single Node 0
    if root is None:
        return -1
    return 1 + max(height(root.left), height(root.right)) 

# B4a: Count no. of Node in Tree
def countNodes(root):
    if root is None:
        return 0
    return 1 + countNodes(root.left) + countNodes(root.right)

# B4b: Count no. of Leaves in Tree
def countLeaves(root):
    if root is None:
        return 0
    if root.left is None and root.right is None:
        return 1
    return countLeaves(root.left) + countLeaves(root.right)

# B5a: Min Element of the Tree
def findMin(root):
    if root is None:
        return None

    curr = root
    while curr.left is not None:
        curr = curr.left
    return curr

# B5b: Max Element of the Tree
def findMax(root):
    if root is None:
        return None

    curr = root
    while curr.right is not None:
        curr = curr.right
    return curr


# Part C: Deletion
def deleteNode(root,key):
    # Edge Case Node not found
    if root is None:
        return None

    # Traverse the Tree to find the Node
    elif key < root.key:
        root.left = deleteNode(root.left,key)
    elif key > root.key:
        root.right = deleteNode(root.right,key)

    # Found: Delete the Node
    else:
        # Case 1: Leaf Node
        if root.left is None and root.right is None:
            return None

        # Case 2: One child
        elif root.left is None:
            return root.right
        elif root.right is None:
            return root.left

        # Case 3: Two children
        else:
            successor = findMin(root.right)
            root.key = successor.key
            root.right = deleteNode(root.right, successor.key)

    return root

def freeTree(root):
    # Set Every Node to None
    if root is not None:
        freeTree(root.left)
        freeTree(root.right)
        root.left = None
        root.right = None


# Part D: Why Balance Matters

# D1: Check if BST is valid
def isBST(root, min_val=-math.inf, max_val=math.inf):
    if root is None:
        return True

    # Check if current node violates global boundary
    if not (min_val < root.key < max_val):
        return False

    # Check subtrees with tightened bounds:
    # Left subtree keys must be < root.key
    # Right subtree keys must be > root.key
    return (isBST(root.left, min_val, root.key) and isBST(root.right, root.key, max_val))


if __name__ == "__main__":
    print("=" * 30)
    print(" LAB EXERCISE EXECUTION ")
    print("=" * 30)
    print()
    print()

    # Part A
    print(" Part A: Build the Tree ")
    keys_a = [50, 30, 70, 20, 40, 60, 80, 35]
    root = None
    for k in keys_a:
        root = insert(root, k)

    print(f"Search 60: {"Found" if search(root, 60) == 1 else "Not Found"}")
    print(f"Search 25: {"Found" if search(root, 25) == 1 else "Not Found"}")
    print(f"Search 90: {"Found" if search(root, 90) == 1 else "Not Found"}")
    print()
    print()

    # Part B
    print(" Part B: Traversals and Measurements ")
    print("Inorder:   ", end="")
    inorder(root)
    print()

    print("Preorder:  ", end="")
    preorder(root)
    print()

    print("Postorder: ", end="")
    postorder(root)
    print()
    print()

    print(f"Height: {height(root)}")
    print(f"No of Nodes: {countNodes(root)}")
    print(f"No of Leaves: {countLeaves(root)}")
    print()

    print(f"Min key: {findMin(root).key if findMin(root) is not None else 'Tree is empty'}")
    print(f"Max key: {findMax(root).key if findMax(root) is not None else 'Tree is empty'}")
    print()
    print()

    # Part C
    print(" Part C: Deletion ")
    print("Before Deletion:", end="  ")
    inorder(root)
    # reinserting the deleted element for testing other deletions
    print()
    print("Case 1: Leaf Node")
    print("Delete 35:", end="  ")
    inorder(deleteNode(root,35))
    insert(root,35)
    print()
    print("Case 2: One child")
    print("Delete 40:", end="  ")
    inorder(deleteNode(root,40))
    insert(root,40)
    print()
    print("Case 3: Two children")
    print("Delete 30:", end="  ")
    inorder(deleteNode(root,30))
    insert(root,30)
    print()

    freeTree(root)
    print("Tree memory released successfully via freeTree()")
    print()
    print()


    # Part D
    print(" Part D: Why Balance Matters ")
    valid_root = None
    for val in [50, 30, 70, 20, 40]:
        valid_root = insert(valid_root, val)
    print(f"isBST on Valid Tree:   {isBST(valid_root)} (Expected: 1)")

    # Creating Invalid BST
    invalid_root = Node(50)
    invalid_root.left = Node(30)
    invalid_root.right = Node(70)
    invalid_root.left.left = Node(20)
    invalid_root.left.right = Node(55)
    print(f"isBST on Invalid Tree: {isBST(invalid_root)} (Expected: 0)")
    print()

    # Insert 1 to 15 in sorted order
    sorted_keys = list(range(1, 16))
    skewed_tree = None
    for k in sorted_keys:
        skewed_tree = insert(skewed_tree, k)
    skewed_h = height(skewed_tree)
    print(f"Height of Sorted 1 to 15 Tree: {skewed_h:2d}")

    # Insert same 15 keys in balanced order
    balanced_keys = [8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 15]
    balanced_tree = None
    for k in balanced_keys:
        balanced_tree = insert(balanced_tree, k)
    balanced_h = height(balanced_tree)
    print(f"Height of Balanced 1 to 15 Tree: {balanced_h:2d}")
    print()
    print()

    # Part D Extention
    print(" Part D Extention Random Tree Experiment (1000 elements): ")
    rand_keys = random.sample(range(1, 100000), 1000)
    rand_root = None
    for k in rand_keys:
        rand_root = insert(rand_root, k)

    rand_h = height(rand_root)
    log2_1000 = math.log2(1000)
    print(f"Height of 1000 random keys BST : {rand_h}")
    print(f"Height of 1000 sorted keys BST : {1000 - 1} (Skewed)")
    print(f"Theoretical minimum log2(1000) : {log2_1000:.2f} (Floor: {math.floor(log2_1000)})")
    print("\n End \n")
