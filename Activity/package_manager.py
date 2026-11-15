"""
Package Manager Core: BFS, DFS, and Topological Sort
A simple, minimal implementation answering real package manager questions.
"""

from collections import deque

# Dependency Graph: An edge A -> B means "A must be installed before B"
graph = {
    "libc": ["zlib", "openssl"],
    "zlib": ["libpng", "git"],
    "openssl": ["curl", "python"],
    "curl": ["git"],
    "python": ["pip", "numpy"],
    "pip": ["numpy"],
    "libpng": ["pillow"],
    "cmake": ["numpy"],
    "git": [],
    "numpy": [],
    "pillow": [],
}


# =====================================================================
# Part 1: BFS - Dependency Depth
# Question: How many dependency levels away is each package?
# =====================================================================

def bfs(graph, start):
    """Return {package: number_of_hops_from_start} for every reachable package."""
    distances = {start: 0}
    queue = deque([start])

    while queue:
        current = queue.popleft()
        for neighbor in graph.get(current, []):
            # Record distance when FIRST discovered (ensures shortest path)
            if neighbor not in distances:
                distances[neighbor] = distances[current] + 1
                queue.append(neighbor)

    return distances


# =====================================================================
# Part 2: DFS - Blast Radius
# Question: If a package breaks or has a bug, what else is affected?
# =====================================================================

def dfs_recursive(graph, start):
    """Return packages in the order they are first visited (recursive)."""
    visited = []

    def explore(node):
        visited.append(node)
        for neighbor in graph.get(node, []):
            if neighbor not in visited:
                explore(neighbor)

    explore(start)
    return visited


def dfs_iterative(graph, start):
    """
    Same result as recursive DFS, but using an explicit stack.
    Push neighbors in reverse so the first neighbor is popped first.
    """
    visited = []
    visited_set = set()
    stack = [start]

    while stack:
        node = stack.pop()
        if node not in visited_set:
            visited_set.add(node)
            visited.append(node)
            # Push in reverse so stack pops them in the original left-to-right order
            for neighbor in reversed(graph.get(node, [])):
                if neighbor not in visited_set:
                    stack.append(neighbor)

    return visited


# =====================================================================
# Part 3: Topological Sort - Safe Install Order & Cycle Detection
# Question: In what order should everything be installed?
# =====================================================================

def topo_dfs(graph):
    """
    Install order using DFS finish times.
    States: 0 = unvisited, 1 = in progress (visiting), 2 = finished
    Meeting an in-progress node means a circular dependency (cycle).
    """
    state = {node: 0 for node in graph}
    finish_order = []

    def dfs(node):
        state[node] = 1  # Mark in-progress
        for neighbor in graph.get(node, []):
            if state.get(neighbor, 0) == 1:
                raise ValueError(f"Cycle detected: {node} -> {neighbor}")
            if state.get(neighbor, 0) == 0:
                dfs(neighbor)
        state[node] = 2  # Mark finished
        finish_order.append(node)

    for node in graph:
        if state[node] == 0:
            dfs(node)

    # Reverse: things that finish last have no prerequisites waiting, so install first
    return finish_order[::-1]


def topo_kahn(graph):
    """
    Install order using Kahn's algorithm (in-degrees and a queue).
    In-degree = how many prerequisites must be installed before this package.
    """
    # Step 1: Count in-degrees
    in_degree = {node: 0 for node in graph}
    for u in graph:
        for v in graph[u]:
            in_degree[v] = in_degree.get(v, 0) + 1

    # Step 2: Queue all packages with 0 prerequisites
    queue = deque([node for node in graph if in_degree[node] == 0])
    install_order = []

    # Step 3: Repeatedly install ready packages and unlock their neighbors
    while queue:
        node = queue.popleft()
        install_order.append(node)
        for neighbor in graph.get(node, []):
            in_degree[neighbor] -= 1
            if in_degree[neighbor] == 0:
                queue.append(neighbor)

    # If some packages couldn't be installed, there is a cycle
    if len(install_order) < len(graph):
        raise ValueError("Cycle detected: circular dependency prevents install")

    return install_order


# =====================================================================
# Verification and Demonstration
# =====================================================================

def is_valid(order, g):
    """Checks that for every dependency u -> v, u comes before v in order."""
    pos = {n: i for i, n in enumerate(order)}
    return len(order) == len(g) and all(pos[u] < pos[v] for u in g for v in g[u])


def main():
    print("=" * 60)
    print("  PACKAGE MANAGER GRAPH ALGORITHMS (BFS, DFS, TOPO SORT)  ")
    print("=" * 60)

    # --- Part 1: BFS ---
    print("\n[Part 1] BFS: Layers of dependencies starting from 'libc':")
    depths = bfs(graph, "libc")
    for pkg, d in sorted(depths.items(), key=lambda x: x[1]):
        print(f"  - {pkg:<10}: {d} hop(s)")

    # Assertions from exercise
    assert depths["openssl"] == 1
    assert depths["git"] == 2
    assert depths["numpy"] == 3
    assert depths["pillow"] == 3
    assert "cmake" not in depths
    print("  -> Part 1 assertions passed!")

    # --- Part 2: DFS ---
    print("\n[Part 2] DFS: Blast radius if 'openssl' has a security bug:")
    affected_rec = dfs_recursive(graph, "openssl")
    affected_itr = dfs_iterative(graph, "openssl")
    print(f"  - Packages affected: {affected_rec}")

    # Assertions from exercise
    expected_affected = {"openssl", "curl", "git", "python", "pip", "numpy"}
    assert set(affected_rec) == expected_affected
    assert set(affected_itr) == expected_affected

    # Check that recursive and iterative produce identical order from libc
    order_rec = dfs_recursive(graph, "libc")
    order_itr = dfs_iterative(graph, "libc")
    assert order_rec == order_itr
    print(f"  - Visit order from 'libc': {order_rec}")
    print("  -> Part 2 assertions passed!")

    # --- Part 3: Topological Sort ---
    print("\n[Part 3] Topological Sort: Safe installation orders:")
    order_dfs = topo_dfs(graph)
    order_kahn = topo_kahn(graph)
    print(f"  - DFS-based install order : {order_dfs}")
    print(f"  - Kahn-based install order: {order_kahn}")

    assert is_valid(order_dfs, graph)
    assert is_valid(order_kahn, graph)
    print("  -> Both installation orders are verified valid!")

    # Cycle Detection check
    print("\n[Part 4] Cycle Detection (Adding circular dependency numpy -> libc):")
    cyclic_graph = {**graph, "numpy": ["libc"]}
    cycle_detected_count = 0
    for name, algo in [("topo_dfs", topo_dfs), ("topo_kahn", topo_kahn)]:
        try:
            algo(cyclic_graph)
            assert False, "Should have raised ValueError"
        except ValueError as err:
            cycle_detected_count += 1
            print(f"  - {name} correctly detected cycle: {err}")

    assert cycle_detected_count == 2
    print("  -> Cycle detection assertions passed!")

    print("\n" + "=" * 60)
    print("all checks passed")
    print("=" * 60)


if __name__ == "__main__":
    main()

