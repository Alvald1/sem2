import random
import string

def generate_random_string(length=8):
    return ''.join(random.choices(string.ascii_letters, k=length))

def generate_graph_file(num_vertices, num_edges,filename):
    if num_edges > num_vertices * (num_vertices - 1) // 2:
        raise ValueError("Too many edges for the given number of vertices to maintain acyclicity")

    # Generate unique vertex names
    vertex_names = set()
    while len(vertex_names) < num_vertices:
        vertex_names.add(generate_random_string())
    vertex_names = list(vertex_names)

    # Generate unique edges ensuring no cycles (DAG)
    edges = set()
    possible_edges = [(vertex_names[i], vertex_names[j]) for i in range(num_vertices) for j in range(i+1, num_vertices)]
    while len(edges) < num_edges:
        v1, v2 = random.choice(possible_edges)
        weight = random.randint(-10, 10)
        edges.add((v1, v2, weight))
        possible_edges.remove((v1, v2))

    # Write to file
    with open(filename, "w") as f:
        f.write(f"{num_vertices}\n")
        for vertex in vertex_names:
            f.write(f"{vertex}\n")
        for v1, v2, weight in edges:
            f.write(f"{v1}\n{v2}\n{weight}\n")

# Set the number of vertices and edges
num_vertices = 1000  # Example number of vertices
num_edges = 1000     # Example number of edges

# Generate the graph data file
generate_graph_file(num_vertices, num_edges, f"graph_data_{num_vertices}_{num_edges}")

