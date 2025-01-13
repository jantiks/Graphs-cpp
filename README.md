# Assignment A02: Graphs Implementation

This project implements a robust graph data structure in C++ based on the requirements of **Assignment A02 - Graphs**. It supports directed and undirected graphs, provides flexible array-based storage for nodes and edges, and implements serialization, import, and manipulation functionalities.

## Key Features

### 1. Flexible Array
- A custom templated container `lib::Array<T>` is implemented for storing elements of any type.
- Guarantees immutability of element locations, ensuring references and pointers remain valid throughout their lifetime.
- Supports efficient memory management, avoiding internal storage reallocation.

### 2. Graph Representation
- **Graph Template:** `Graph<NData, EData>` is the base class for all graph representations.
- Derived classes:
  - `DirectedGraph<NData, EData>` for directed graphs.
  - `UndirectedGraph<NData, EData>` for undirected graphs.
- Nodes and edges are represented as `Node<NData>` and `Edge<EData>`, respectively.
- Unique identifiers for nodes and edges.

### 3. Components
- **Nodes:** Managed through `Nodes<NData, EData>` class.
- **Edges:** Managed through `Edges<NData, EData>` class.
- **Adjacency Matrix:** Efficient edge lookup using pointers to edge instances.

### 4. Serialization
- Nodes and edges can be serialized to streams or files.
- Format:
  - Node: `node (id {data})`
  - Edge: `edge (source)-[id {data}]->(target)`
- Adjacency matrix can be printed for debugging.

### 5. Graph Import and Export
- Import graph structure from text files or streams.
- Mixed order of nodes and edges is supported during import.

### 6. Error Handling
- Custom exception hierarchy for robust error management:
  - `MemoryException`
  - `IdentifierException`
  - `ElementException`
  - `ConflictException`
  - `FileException`

### 7. Iterators
- Iterator support for traversing nodes and edges.

### 8. Extensibility
- Custom data types can be associated with nodes and edges.
- Supports user-defined serialization and deserialization methods.

## Usage

### Adding Nodes and Edges
```cpp
// Adding a node
auto& node = graph.nodes().add(0, "Node Data");

// Adding an edge
auto& edge = graph.edges().add(0, 1, 2, "Edge Data");
```

### Overall Importing and node accessing
```cpp
#include <iostream>
#include "Array.h"
#include "Graph.h"

int main(int argc, const char * argv[]) {
    std::cout << "UndirectedGraph<std::string, std::string> gSource1; ..." << std::endl;
    UndirectedGraph<std::string, std::string> gSource1;
    std::cout << "gSource1.import(\"Input-Graph-6.txt\"); ..." << std::endl;
    gSource1.import("/Users/tigran/Desktop/Classes/cpp/Graphs/Graphs/test.txt");
    gSource1.import("/Users/tigran/Desktop/Classes/cpp/Graphs/Graphs/test2.txt");
    std::cout << "Graph modifications ..." << std::endl;
    std::cout << std::endl;
    std::cout << "auto gTarget1 = gSource1; ..." << std::endl;
    auto gTarget1 = gSource1;
    std::cout << std::endl;
    gSource1.print();
    gTarget1.nodes()[5];
    std::cout << std::endl;
    gTarget1.printMatrix();
}
```
