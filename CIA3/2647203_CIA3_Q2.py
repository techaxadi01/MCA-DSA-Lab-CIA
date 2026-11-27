# Custom made Heap Structure
class MinHeap:
    def __init__(self):
        self.heap = []

    def is_empty(self):
        return len(self.heap) == 0

    def push(self, item):
        self.heap.append(item)
        self._sift_up(len(self.heap) - 1)

    def pop(self):
        if not self.heap:
            return None
        top = self.heap[0]
        last = self.heap.pop()
        if self.heap:
            self.heap[0] = last
            self._sift_down(0)
        return top

    def _sift_up(self, idx):
        while idx > 0:
            parent = (idx - 1) // 2
            if self.heap[idx] < self.heap[parent]:
                self.heap[idx], self.heap[parent] = self.heap[parent], self.heap[idx]
                idx = parent
            else:
                break

    def _sift_down(self, idx):
        n = len(self.heap)
        while True:
            smallest = idx
            left = 2 * idx + 1
            right = 2 * idx + 2

            if left < n and self.heap[left] < self.heap[smallest]:
                smallest = left
            if right < n and self.heap[right] < self.heap[smallest]:
                smallest = right

            if smallest != idx:
                self.heap[idx], self.heap[smallest] = self.heap[smallest], self.heap[idx]
                idx = smallest
            else:
                break


# Create Graph Using Adjacency List
def create_graph(V):
    return [[] for _ in range(V)]


# Add Edges and fill the Adjacency List for the Graph
def add_edge(adj, u, v, cost):
    adj[u].append((v, cost))
    # Avoid Duplicate entries for the Self Loops
    if u != v:
        adj[v].append((u, cost))


# Prim's Algorith for Minimum Spanning Tree (MST)
def prims_mst(V, adj, start=0):
    if V == 0:
        return 0, [], 0

    inf = float('inf')
    key = [inf] * V         # Default Weight of Vertices = inf
    parent = [-1] * V       # Default Parent of Vertices = -1
    in_mst = [False] * V    # Is Visited List

    key[start] = 0
    pq = MinHeap()
    pq.push((0, start))

    total_cost = 0
    visited_count = 0

    while not pq.is_empty():
        cost, u = pq.pop()
        if in_mst[u]:
            continue    # Skips the Vertex which is already covered

        in_mst[u] = True
        total_cost += cost
        visited_count += 1

        # Check the Adjacent Vertices of the Current Vertex and Update the Min Weight and Parent if Not Visited
        for v, weight in adj[u]:
            if not in_mst[v] and weight < key[v]:
                key[v] = weight
                parent[v] = u
                pq.push((weight, v))

    return total_cost, parent, visited_count


def switch_on_order(V, parent):
    dag = [[] for _ in range(V)]
    indegree = [0] * V

    # Parent -> child dependencies from Prim's MST
    for v in range(V):
        if parent[v] != -1:
            dag[parent[v]].append(v)
            indegree[v] += 1

    # Always pick the smallest-numbered ready village
    ready = MinHeap()
    for i in range(V):
        if indegree[i] == 0:
            ready.push(i)

    order = []
    while not ready.is_empty():
        u = ready.pop()
        order.append(u)
        for nxt in dag[u]:
            indegree[nxt] -= 1
            if indegree[nxt] == 0:
                ready.push(nxt)

    if len(order) != V:
        return None
    return order


def read_non_empty_ints(message = "INPUT \n"):
    while True:
        line = input(message).strip()
        if line:
            return [int(x) for x in line.split()]


if __name__ == "__main__":
    size = read_non_empty_ints("Enter n & m: ")
    n = size[0]
    m = size[1]

    adj = create_graph(n)
    print("Enter Graph Edges:")
    for _ in range(m):
        u, v, cost = read_non_empty_ints("    u v cost:  ")
        add_edge(adj, u, v, cost)

    total_cost, parent, visited_count = prims_mst(n, adj, start=0)
    print(total_cost)

    if n == 0:
        print("Edge Case 'n = 0'")

    if visited_count != n:
        print("IMPOSSIBLE")

    order = switch_on_order(n, parent)
    if order is None:
        print("IMPOSSIBLE")
    else:
        print(" ".join(str(v) for v in order))
