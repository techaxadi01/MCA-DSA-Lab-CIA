"""
Lab Exercise 6:
Shortest Routes with Dijkstra's Algorithm (RouteFinder)
"""


# Custom Min-Heap Implementation
class MinHeap:
    def __init__(self):
        self.heap = []

    def push(self, item):
        # Add item to the end and shift up to restore heap order
        self.heap.append(item)
        self._shift_up(len(self.heap) - 1)

    def pop(self):
        # Empty heap check
        if not self.heap:
            return None
        min_item = self.heap[0]
        last_item = self.heap.pop()
        # Move last item to root and shift down
        if self.heap:
            self.heap[0] = last_item
            self._shift_down(0)
        return min_item

    def is_empty(self):
        return len(self.heap) == 0

    def _shift_up(self, idx):
        # Move node up if its distance is smaller than its parent
        parent = (idx - 1) // 2
        while idx > 0 and self.heap[idx] < self.heap[parent]:
            self.heap[idx], self.heap[parent] = self.heap[parent], self.heap[idx]
            idx = parent
            parent = (idx - 1) // 2

    def _shift_down(self, idx):
        # Move node down by swapping with the smaller of its two children
        n = len(self.heap)
        while True:
            smallest = idx
            left = 2 * idx + 1
            right = 2 * idx + 2

            # Check if left child has smaller distance
            if left < n and self.heap[left] < self.heap[smallest]:
                smallest = left
            # Check if right child has smaller distance
            if right < n and self.heap[right] < self.heap[smallest]:
                smallest = right

            # If a smaller child exists, swap and continue down
            if smallest != idx:
                self.heap[idx], self.heap[smallest] = self.heap[smallest], self.heap[idx]
                idx = smallest
            else:
                break


# Graph Construction (using Adjacency List)
def create_graph(V):
    return [[] for _ in range(V)]


def add_road(adj, u, v, travel_time):
    # Road travel times cannot be negative
    if travel_time < 0:
        raise ValueError(f"Negative road time detected on road ({u}, {v}) with time {travel_time}.")
    # Two-way road between junctions u and v
    adj[u].append((v, travel_time))
    if u != v:
        adj[v].append((u, travel_time))


# Dijkstra's Algorithm
def find_shortest_routes(V, adj, kitchen):
    # Initialize distances to infinity and kitchen to 0
    dist = [float('inf')] * V
    parent = [None] * V
    dist[kitchen] = 0

    # Min-Heap stores (distance, vertex) tuples
    pq = MinHeap()
    pq.push((0, kitchen))

    while not pq.is_empty():
        curr_dist, u = pq.pop()

        # Ignore outdated entries with larger distances
        if curr_dist > dist[u]:
            continue

        # Relax all adjacent roads
        for v, weight in adj[u]:
            new_dist = dist[u] + weight
            if new_dist < dist[v]:
                dist[v] = new_dist
                parent[v] = u
                pq.push((new_dist, v))

    # Format output for each junction
    results = []
    for j in range(V):
        if dist[j] == float('inf'):
            results.append(f"{j} unreachable")
        else:
            # Reconstruct route iteratively using parent pointers
            path = []
            curr = j
            while curr is not None:
                path.append(curr)
                if curr == kitchen:
                    break
                curr = parent[curr]

            path.reverse()
            route_str = "->".join(map(str, path))
            results.append(f"{j} {dist[j]} {route_str}")

    return results


# Take Input from the User
def user_input():
    try:
        print("Enter V, E:")
        line = input().split()
        while not line:
            line = input().split()
        V, E = int(line[0]), int(line[1])

        adj = create_graph(V)
        print("Enter Road Details (From, To, Travel Time):")
        for _ in range(E):
            edge = input().split()
            while not edge:
                edge = input().split()
            u, v, t = int(edge[0]), int(edge[1]), int(edge[2])
            add_road(adj, u, v, t)

        print("Enter Kitchen Junction:")
        kitchen_line = input().split()
        while not kitchen_line:
            kitchen_line = input().split()
        kitchen = int(kitchen_line[0])

        routes = find_shortest_routes(V, adj, kitchen)
        print("\nShortest Routes:")
        for r in routes:
            print(f"  {r}")
    except ValueError as e:
        print(f"Error: {e}")
    except EOFError:
        pass


# Test Case Helper Function
def run_test(test_num, name, V, edges, kitchen):
    print("-" * 60)
    print(f"Test Case {test_num}: {name}")
    print(f"Junctions: {V}, Roads: {len(edges)}, Kitchen: {kitchen}")
    print(f"Edges: {edges}")

    adj = create_graph(V)
    try:
        for u, v, t in edges:
            add_road(adj, u, v, t)

        routes = find_shortest_routes(V, adj, kitchen)
        print("Output:")
        for r in routes:
            print(f"  {r}")
    except ValueError as e:
        print(f"Output: {e}")
    print()


if __name__ == "__main__":
    # Test Case 1: Sample Map from Lab Sheet
    edges_tc1 = [
        (0, 1, 7), (0, 2, 9), (0, 5, 14),
        (1, 2, 10), (1, 3, 15),
        (2, 3, 11), (2, 5, 2),
        (3, 4, 6),
        (4, 5, 9)
    ]
    run_test(1, "Sample City Map", 6, edges_tc1, 0)

    # Test Case 2: Single Junction Map
    run_test(2, "Single Junction Map", 1, [], 0)

    # Test Case 3: Disconnected Map (Cut-off Junctions)
    edges_tc3 = [(0, 1, 4), (1, 2, 3), (3, 4, 5)]
    run_test(3, "Disconnected Map", 5, edges_tc3, 0)

    # Test Case 4: Parallel Roads & Self Loops
    edges_tc4 = [(0, 1, 12), (0, 1, 5), (1, 1, 3), (1, 2, 4)]
    run_test(4, "Parallel Roads and Self Loops", 3, edges_tc4, 0)

    # Test Case 5: Zero-Minute Road
    edges_tc5 = [(0, 1, 0), (1, 2, 6), (0, 2, 10)]
    run_test(5, "Zero-Minute Road", 3, edges_tc5, 0)

    # Test Case 6: Negative Road Time (Validation)
    edges_tc6 = [(0, 1, 4), (1, 2, -3), (0, 2, 8)]
    run_test(6, "Negative Road Time", 3, edges_tc6, 0)

    if input("Do you want to try Custom inputs (Y/N): ").upper() == 'Y':
        user_input()