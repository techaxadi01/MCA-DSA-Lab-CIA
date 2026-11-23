from collections import deque


# Queue Data Structure
class Queue:
    """Simple FIFO Queue implementation for Kahn's algorithm."""
    def __init__(self):
        self.items = deque()

    def enqueue(self, item):
        self.items.append(item)

    def dequeue(self):
        if not self.is_empty():
            return self.items.popleft()
        return None

    def is_empty(self):
        return len(self.items) == 0

    def size(self):
        return len(self.items)

    def clear(self):
        self.items.clear()


# Graph Construction & In-Degree Computation

def create_graph(V):
    """Initializes an empty adjacency list for V vertices (0 to V-1)."""
    return [[] for _ in range(V)]


def add_edge(adj, u, v):
    """
    Adds a directed edge u -> v.
    Meaning: Course u must be completed before Course v.
    """
    adj[u].append(v)


def compute_indegrees(V, adj):
    """
    Computes the in-degree of every vertex.
    In-degree represents the count of prerequisites for that course.
    """
    indegree = [0] * V
    for u in range(V):
        for v in adj[u]:
            indegree[v] += 1
    return indegree


# Kahn's Algorithm

def topological_sort_kahn(V, adj):
    """
    Implements Kahn's Algorithm to find a valid topological order:
    1. Computes in-degree for all vertices.
    2. Enqueues all vertices with in-degree 0 (courses with no prerequisites).
    3. Repeatedly dequeues a vertex, adds it to the order, and decrements
       in-degrees of its adjacent vertices.
    4. Enqueues adjacent vertices as soon as their in-degree becomes 0.
    5. If all V vertices are processed, returns (True, order).
       Otherwise, a cycle exists, returns (False, None).
    """
    # Step 1: Compute in-degrees
    indegree = compute_indegrees(V, adj)

    # Step 2: Initialize Queue and enqueue vertices with in-degree 0
    q = Queue()
    for i in range(V):
        if indegree[i] == 0:
            q.enqueue(i)

    topological_order = []

    # Step 3: Process the queue
    while not q.is_empty():
        curr = q.dequeue()
        topological_order.append(curr)

        # Reduce in-degree of all adjacent vertices
        for neighbor in adj[curr]:
            indegree[neighbor] -= 1
            # If in-degree becomes 0, add to queue
            if indegree[neighbor] == 0:
                q.enqueue(neighbor)

    # Step 4: Check if all vertices were processed
    if len(topological_order) == V:
        return True, topological_order
    else:
        # Cycle detected (some vertices could never reach in-degree 0)
        return False, None


# DFS-based Topological Sort

def topological_sort_dfs(V, adj):
    """
    Implements Topological Sort using Depth First Search (DFS):
    States for each vertex:
      0 = Unvisited
      1 = Visiting (Currently in active recursion stack)
      2 = Visited (Finished processing all descendants)
    If a vertex in state 1 is visited again, a cycle exists.
    """
    state = [0] * V
    finish_stack = []
    has_cycle = False

    def dfs(node):
        nonlocal has_cycle
        if has_cycle:
            return

        state[node] = 1  # Mark as visiting

        for neighbor in adj[node]:
            if state[neighbor] == 1:
                # Back-edge found: Cycle detected!
                has_cycle = True
                return
            elif state[neighbor] == 0:
                dfs(neighbor)

        state[node] = 2  # Mark as visited / finished
        finish_stack.append(node)

    # Run DFS from every unvisited vertex (handles disconnected graphs)
    for i in range(V):
        if state[i] == 0:
            dfs(i)
            if has_cycle:
                return False, None

    # Reverse the finish stack to get topological ordering
    finish_stack.reverse()
    return True, finish_stack


# Memory Cleanup

def free_graph(adj):
    """
    Cleans up graph adjacency list references to ensure
    no lingering references remain in memory.
    """
    if adj is not None:
        for neighbors in adj:
            neighbors.clear()
        adj.clear()


def run_case(name, V, edges):
    """Helper function to build graph, run topological sort, and display output."""
    print("-" * 55)
    print(f" Test Case: {name}")
    print(f" Number of Vertices (V) : {V}")
    print(f" Number of Edges    (E) : {len(edges)}")
    print(f" Edges (u -> v)         : {edges}")

    # Build graph
    adj = create_graph(V)
    for u, v in edges:
        add_edge(adj, u, v)

    # Run Kahn's Algorithm
    success_kahn, order_kahn = topological_sort_kahn(V, adj)

    print("\n [Kahn's Algorithm Result]:")
    if success_kahn:
        print(" Topological Order:", " ".join(map(str, order_kahn)))
    else:
        print(" CYCLE DETECTED")

    # Run DFS-based Topological Sort
    success_dfs, order_dfs = topological_sort_dfs(V, adj)
    print(" [DFS-based Result]:")
    if success_dfs:
        print(" Topological Order:", " ".join(map(str, order_dfs)))
    else:
        print(" CYCLE DETECTED")

    # Release memory
    free_graph(adj)
    print()


if __name__ == "__main__":

    # -------------------------------------------------------------------
    # Example 1 from Lab Sheet:
    # 6 vertices, 6 edges (Valid DAG)
    # Dependencies:
    #   5 -> 2, 5 -> 0, 4 -> 0, 4 -> 1, 2 -> 3, 3 -> 1
    # -------------------------------------------------------------------
    edges_ex1 = [(5, 2), (5, 0), (4, 0), (4, 1), (2, 3), (3, 1)]
    run_case("Example 1 (Valid Course Prerequisite Graph)", 6, edges_ex1)

    # -------------------------------------------------------------------
    # Example 2 from Lab Sheet:
    # 4 vertices, 4 edges (Contains Cycle: 1 -> 2 -> 3 -> 1)
    # Dependencies:
    #   0 -> 1, 1 -> 2, 2 -> 3, 3 -> 1
    # -------------------------------------------------------------------
    edges_ex2 = [(0, 1), (1, 2), (2, 3), (3, 1)]
    run_case("Example 2 (Circular Dependency Graph)", 4, edges_ex2)

    # -------------------------------------------------------------------
    # Example 3: Disconnected Graph Edge Case
    # Graph has multiple independent components (e.g. elective tracks)
    # -------------------------------------------------------------------
    edges_ex3 = [(0, 1), (2, 3), (4, 5)]
    run_case("Example 3 (Disconnected Graph / Independent Components)", 6, edges_ex3)
