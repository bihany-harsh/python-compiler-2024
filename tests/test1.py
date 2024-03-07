class Graph:
    def __init__(self, size):
        self.size = size
        self.graph = [[float('infinity') for j in range(size)] for i in range(size)]
        for i in range(size):
            self.graph[i][i] = 0
        self.shortest_paths = [float('infinity')] * size
        self.previous_nodes = [-1] * size

    def add_edge(self, u, v, weight):
        self.graph[u][v] = weight

    def dijkstra(self, start):
        n = self.size
        for i in range(n):
            self.shortest_paths[i] = float('infinity')
            self.previous_nodes[i] = -1
        self.shortest_paths[start] = 0
        visited = [False] * n

        for _ in range(n):
            min_distance = float('infinity')
            current_node = -1
            for i in range(n):
                if not visited[i]:
                    if self.shortest_paths[i] < min_distance:
                        min_distance = self.shortest_paths[i]
                        current_node = i

            if current_node == -1:
                break

            visited[current_node] = True
            for neighbor in range(n):
                if self.graph[current_node][neighbor] != float('infinity'):
                    if not visited[neighbor]:
                        distance = self.graph[current_node][neighbor]
                        new_distance = self.shortest_paths[current_node] + distance
                        if new_distance < self.shortest_paths[neighbor]:
                            self.shortest_paths[neighbor] = new_distance
                            self.previous_nodes[neighbor] = current_node

    def reconstruct_path(self, start, end):
        path = []
        current = end
        while current != start:
            if current == -1:
                return []
            path.insert(0, current)
            current = self.previous_nodes[current]
        path.insert(0, start)
        return path

# Example usage
size = 4
graph = Graph(size)
graph.add_edge(0, 1, 1)
graph.add_edge(0, 2, 4)
graph.add_edge(1, 2, 2)
graph.add_edge(1, 3, 5)
graph.add_edge(2, 3, 1)

start_node = 0
graph.dijkstra(start_node)
print("Shortest paths from node", start_node, ":", graph.shortest_paths)
print("Previous nodes:", graph.previous_nodes)

target_node = 3
path = graph.reconstruct_path(start_node, target_node)
print("Shortest path from node", start_node, "to node", target_node, ":", path)
