class Node:
    def __init__(self,key):
        self.key = key
        self.right = None
        self.left = None

    def New_Node(self,key):
        return Node(key)

    def Insert(self, root, key):
        if root is None:
            return self.New_Node(key)
        if key < root.key:
            root.left = self.Insert(root.left, key)
        elif key > root.key:
            root.right = self.Insert(root.right, key)
        return root

    def Search(self, root, key):
        if root is None or root.key == key:
            return root
        if key < root.key:
            return self.Search(root.left, key)
        elif key > root.key:
            return self.Search(root.right, key)

    def Min_Node(self, root):
        current = root
        while(current.left is not None):
            current = current.left
        return current

    def Max_Node(self, root):
        current = root
        while(current.right is not None):
            current = current.right
        return current

    