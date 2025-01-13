//
// Created by Tigran on 07.01.24.
//

#ifndef GRAPHS_CLION_GRAPH_H
#define GRAPHS_CLION_GRAPH_H
#include "Node.h"
#include "Edge.h"
#include "Exception.h"
#include <fstream>
#include <regex>
#include <sstream>
#include "Globals.h"

const Identifier EMPTY_NODE = -1;

template <typename NData, typename EData>
class Graph {
public:
    class Nodes;
    class Edges;

    Graph();
    Graph(const Graph<NData, EData>& other) noexcept;
    Graph(Graph<NData, EData>&& other) noexcept;

    const Nodes& nodes() const;
    Nodes& nodes();

    const Edges& edges() const;
    Edges& edges();

    Node<NData>& add(Identifier id, const NData& data);
    Node<NData>& add(Identifier id, NData&& data);
    Node<NData>& add(const NData& data);
    Node<NData>& add(NData&& data);

    Edge<EData>& add(Identifier id,
                             Identifier source,
                             Identifier target,
                             const EData& data);
    Edge<EData>& add(Identifier id,
                             Identifier source,
                             Identifier target,
                             EData&& data);
    Edge<EData>& add(Identifier source,
                             Identifier target,
                             const EData& data);
    Edge<EData>& add(Identifier source,
                             Identifier target,
                             EData&& data);
    virtual void addEdgeToAdjacencyMatrix(Identifier id, Identifier source, Identifier target) = 0;

    Graph& operator=(const Graph& other) noexcept;
    Graph& operator=(Graph&& other) noexcept;

    void printMatrix(std::ostream& stream = std::cout) const;
    void print(std::ostream& stream = std::cout) const;
    void print(const std::string& filename) const;
    void import(std::istream& stream = std::cin);
    void import(const std::string& filename);
    EData getEdgeDataFromField(const std::vector<std::string>& fields, size_t index);
    NData getNodeDataFromField(const std::vector<std::string>& fields, size_t index);
    Identifier getIdFromField(const std::vector<std::string>& fields, size_t index);
    std::vector<std::string> smatchToVector(const std::smatch& matches);
    std::vector<std::vector<Identifier>> adjacencyMatrix_;

    virtual ~Graph() = default;
private:
    Nodes nodes_;
    Edges edges_;
    Identifier getId(const std::string& value);
    class iterator;
    class const_iterator;

    Node<NData>& insertNodeInBack(Node<NData> node);
    void validateAddNodes(Identifier id, const NData& data);
    Edge<EData>& insertEdgeInBack(Identifier id, Identifier source, Identifier target,Edge<EData> edge);
    void validateAddEdges(Identifier id, Identifier source, Identifier target);
};

/* --- INITIALIZATION ---- */
template <typename NData, typename EData>
Graph<NData, EData>::Graph() {
    nodes_.setGraph(this);
    edges_.setGraph(this);
}
template <typename NData, typename EData>
Graph<NData, EData>::Graph(const Graph<NData, EData>& other) noexcept {
    adjacencyMatrix_ = other.adjacencyMatrix_;
    nodes_ = other.nodes_;
    edges_ = other.edges_;
    nodes_.setGraph(this);
    edges_.setGraph(this);
}

template <typename NData, typename EData>
Graph<NData, EData>::Graph(Graph<NData, EData>&& other) noexcept {
    std::swap(adjacencyMatrix_, other.adjacencyMatrix_);
    std::swap(nodes_, other.nodes_);
    std::swap(edges_, other.edges_);
    nodes_.setGraph(this);
    edges_.setGraph(this);
}

/* copy assignments */
template <typename NData, typename EData>
Graph<NData, EData>& Graph<NData, EData>::operator=(const Graph& other) noexcept {
    adjacencyMatrix_ = other.adjacencyMatrix_;
    nodes_ = other.nodes_;
    edges_ = other.edges_;
    nodes_.setGraph(this);
    edges_.setGraph(this);
    return *this;
}

template <typename NData, typename EData>
Graph<NData, EData>& Graph<NData, EData>::operator=(Graph&& other) noexcept {
    std::swap(adjacencyMatrix_, other.adjacencyMatrix_);
    std::swap(nodes_, other.nodes_);
    std::swap(edges_, other.edges_);
    nodes_.setGraph(this);
    edges_.setGraph(this);
    return *this;
}

/* --- NODES COMPONENT --- */
template <typename NData, typename EData>
class Graph<NData, EData>::Nodes {
public:
    Nodes() {};
    Nodes(const Nodes& other) noexcept {
        nodes = other.nodes;
    };
    Nodes(Nodes&& other) noexcept {
        std::swap(nodes, other.nodes);
    };

    Nodes& operator=(const Nodes& other) noexcept {
        nodes = other.nodes;
        return *this;
    };
    Nodes& operator=(Nodes&& other) noexcept {
        std::swap(nodes, other.nodes);
        return *this;
    };

    Graph<NData, EData>* graph;
    std::vector<Node<NData>> nodes;
    inline size_t size() const;
    bool exists(Identifier id) const;

    Node<NData>& get(Identifier id);
    const Node<NData>& get(Identifier id) const;

    Node<NData>& operator[](size_t id);
    const Node<NData>& operator[](size_t id) const;
    void print(std::ostream& stream = std::cout) const;
    std::vector<Node<NData>>::iterator begin();
    std::vector<Node<NData>>::iterator end();

    std::vector<Node<NData>>::const_iterator begin() const;
    std::vector<Node<NData>>::const_iterator end() const;

    Node<NData>& add(Identifier id, const NData& data);
    Node<NData>& add(Identifier id, NData&& data);
    Node<NData>& add(const NData& data);
    Node<NData>& add(NData&& data);


    void setGraph(Graph<NData, EData>* graph_) {
        graph = graph_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const typename Graph<NData, EData>::Nodes& nodesToPrint) {
        nodesToPrint.print(stream);
        return stream;
    }
};

template <typename NData, typename EData>
size_t Graph<NData, EData>::Nodes::size() const {
    return nodes.size();
}

template <typename NData, typename EData>
bool Graph<NData, EData>::Nodes::exists(Identifier id) const {
    return id < nodes.size();
}

template <typename NData, typename EData>
Node<NData>& Graph<NData, EData>::Nodes::get(Identifier id) {
    if (id >= nodes.size()) {
        throw ElementException("Node with identifier " + std::to_string(id) + " does not exist");
    }
    return nodes[id];
}

template <typename NData, typename EData>
const Node<NData>& Graph<NData, EData>::Nodes::get(Identifier id) const {
    if (id >= nodes.size()) {
        throw ElementException("Node with identifier " + std::to_string(id) + " does not exist");
    }
    return nodes[id];
}

template <typename NData, typename EData>
Node<NData>& Graph<NData, EData>::Nodes::operator[](size_t id) {
    return get(id);
}

template <typename NData, typename EData>
const Node<NData>& Graph<NData, EData>::Nodes::operator[](size_t id) const {
    return get(id);
}

template <typename NData, typename EData>
void Graph<NData, EData>::Nodes::print(std::ostream& stream) const {
    for (size_t i = 0; i < nodes.size(); ++i) {
        stream << (nodes[i]);
        if (i < nodes.size() - 1) {
            stream << std::endl;
        }
    }
}

/* --- EDGES COMPONENT --- */
template <typename NData, typename EData>
class Graph<NData, EData>::Edges {
private:
    class Request;
    
public:
    Edges() {};
    Edges(const Edges& other) noexcept {
        edges = other.edges;
    };
    Edges(Edges&& other) noexcept {
        std::swap(edges, other.edges);
    };

    Edges& operator=(const Edges& other) noexcept {
        edges = other.edges;
        return *this;
    };
    Edges& operator=(Edges&& other) noexcept {
        std::swap(edges, other.edges);
        return *this;
    };

    Graph<NData, EData>* graph;
    std::vector<Edge<EData>> edges;
    inline size_t size() const;
    bool exists(Identifier id) const;
    bool exists(Identifier source, Identifier target) const;

    Edge<EData>& get(Identifier id);
    const Edge<EData>& get(Identifier id) const;

    Edge<EData>& get(Identifier source, Identifier target);
    const Edge<EData>& get(Identifier source, Identifier target) const;
    void print(std::ostream& stream = std::cout) const;
    std::vector<Edge<EData>>::iterator begin();
    std::vector<Edge<EData>>::iterator end();

    std::vector<Edge<EData>>::const_iterator begin() const;
    std::vector<Edge<EData>>::const_iterator end() const;

    Edge<EData>& add(Identifier id,
                     Identifier source,
                     Identifier target,
                     const EData& data);
    Edge<EData>& add(Identifier id,
                     Identifier source,
                     Identifier target,
                     EData&& data);
    Edge<EData>& add(Identifier source,
                     Identifier target,
                     const EData& data);
    Edge<EData>& add(Identifier source,
                     Identifier target,
                     EData&& data);
    void printMatrix(std::ostream& stream = std::cout) const;

    void setGraph(Graph<NData, EData>* graph_) {
        graph = graph_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const typename Graph<NData, EData>::Edges& edgesToPrint) {
        edgesToPrint.print(stream);
        return stream;
    }

    Request operator[](size_t index);
    const Request operator[](size_t index) const;
};

/* --- REQUEST for handling double [] --- */
template <typename NData, typename EData>
class Graph<NData, EData>::Edges::Request {
public:
    friend Graph<NData, EData>::Edges;
    Edge<EData>& operator[](size_t index);
    const Edge<EData>& operator[](size_t index) const;
private:
    Request(const Graph<NData, EData>::Edges& edges, size_t row);
    const Graph<NData, EData>::Edges& edges_;
    size_t row_;
};

template <typename NData, typename EData>
Graph<NData, EData>::Edges::Request::Request(const Graph<NData, EData>::Edges& edges, size_t row): edges_(edges), row_(row){};

template <typename NData, typename EData>
const typename Graph<NData, EData>::Edges::Request Graph<NData, EData>::Edges::operator[](size_t index) const {
    return Graph::Edges::Request(*this, index);
}

template <typename NData, typename EData>
typename Graph<NData, EData>::Edges::Request Graph<NData, EData>::Edges::operator[](size_t index) {
    return Graph::Edges::Request(*this, index);
}

template <typename NData, typename EData>
Edge<EData>& Graph<NData, EData>::Edges::Request::operator[](size_t index) {
    Edge<EData>& edge = const_cast<Edge<EData>&>(edges_.get(row_, index));
    return edge;
}

template <typename NData, typename EData>
const Edge<EData>& Graph<NData, EData>::Edges::Request::operator[](size_t index) const {
    return edges_.get(row_, index);
}

template <typename NData, typename EData>
size_t Graph<NData, EData>::Edges::size() const {
    return edges.size();
}

template <typename NData, typename EData>
bool Graph<NData, EData>::Edges::exists(Identifier id) const {
    return id < edges.size();
}

template <typename NData, typename EData>
bool Graph<NData, EData>::Edges::exists(Identifier source, Identifier target) const {
    if (!graph->nodes().exists(target) && graph->nodes().exists(source)) {
       throw ElementException("Target node with identifier " + std::to_string(target) + " does not exist");
    }

    if (!graph->nodes().exists(source) && graph->nodes().exists(target)) {
        throw ElementException("Source node with identifier " + std::to_string(source) + " does not exist");
    }

    return graph->adjacencyMatrix_[source][target] != EMPTY_NODE;
}

template <typename NData, typename EData>
Edge<EData>& Graph<NData, EData>::Edges::get(Identifier id) {
    if (id >= edges.size()) {
        throw ElementException("Edge with identifier " + std::to_string(id) + " does not exist");
    }
    return edges[id];
}

template <typename NData, typename EData>
const Edge<EData>& Graph<NData, EData>::Edges::get(Identifier id) const {
    if (id >= edges.size()) {
        throw ElementException("Edge with identifier " + std::to_string(id) + " does not exist");
    }
    return edges[id];
}

template <typename NData, typename EData>
Edge<EData>& Graph<NData, EData>::Edges::get(Identifier source, Identifier target) {
    if (!graph->nodes().exists(target) && graph->nodes().exists(source)) {
       throw ElementException("Target node with identifier " + std::to_string(target) + " does not exist");
    }

    if (!graph->nodes().exists(source) && graph->nodes().exists(target)) {
        throw ElementException("Source node with identifier " + std::to_string(source) + " does not exist");
    }


    for (size_t i = 0; i < edges.size(); ++i) {
        auto& edge = edges[i];
        if (edge.getSource() == source && edge.getTarget() == target) {
            return edge;
        }
    }

    throw ElementException("Edge between nodes " + std::to_string(source) + " and " + std::to_string(target) + " does not exist");
}

template <typename NData, typename EData>
const Edge<EData>& Graph<NData, EData>::Edges::get(Identifier source, Identifier target) const {
    if (!graph->nodes().exists(target) && graph->nodes().exists(source)) {
       throw ElementException("Target node with identifier " + std::to_string(target) + " does not exist");
    }

    if (!graph->nodes().exists(source) && graph->nodes().exists(target)) {
        throw ElementException("Source node with identifier " + std::to_string(source) + " does not exist");
    }

    for (size_t i = 0; i < edges.size(); ++i) {
        const auto& edge = edges[i];
        if (edge.getSource() == source && edge.getTarget() == target) {
            return edge;
        }
    }
    throw ElementException("Edge between nodes " + std::to_string(source) + " and " + std::to_string(target) + " does not exist");
}

template <typename NData, typename EData>
void Graph<NData, EData>::Edges::print(std::ostream& stream) const {
    for (size_t i = 0; i < edges.size(); ++i) {
        stream << (edges[i]);
        if (i < edges.size() - 1) {
            stream << std::endl;
        }
    }
}

template <typename NData, typename EData>
class UndirectedGraph: public Graph<NData, EData> {
public:
    using Graph<NData, EData>::Graph;
    void addEdgeToAdjacencyMatrix(Identifier id, Identifier source, Identifier target) override;
};

template <typename NData, typename EData>
class DirectedGraph: public Graph<NData, EData> {
public:
    using Graph<NData, EData>::Graph;
    void addEdgeToAdjacencyMatrix(Identifier id, Identifier source, Identifier target) override;
};

template <typename NData, typename EData>
void DirectedGraph<NData, EData>::addEdgeToAdjacencyMatrix(Identifier id,
                                 Identifier source,
                                 Identifier target) {
    this->adjacencyMatrix_[source][target] = id;
}

template <typename NData, typename EData>
void UndirectedGraph<NData, EData>::addEdgeToAdjacencyMatrix(Identifier id,
                                 Identifier source,
                                 Identifier target) {
    this->adjacencyMatrix_[source][target] = id;
    this->adjacencyMatrix_[target][source] = id;
}


/* ----  ADDING NODES ----  */
template <typename NData, typename EData>
void Graph<NData, EData>::validateAddNodes(Identifier id, const NData& data) {
    if (id > nodes_.nodes.size()) {
        throw IdentifierException("Invalid node identifier " + std::to_string(id) + " requested");
    }

    if (id < nodes_.size()) {
        throw ConflictException("Node with identifier " + std::to_string(id) + " already exists");
    }
}

template <typename NData, typename EData>
Node<NData>& Graph<NData, EData>::insertNodeInBack(Node<NData> node) {
    try {
        nodes_.nodes.push_back(node);
    } catch (const std::bad_alloc&) {
        throw MemoryException("Unavailable memory for a new node in the nodes container");
    }

    try {
        for (auto& row: adjacencyMatrix_) {
            row.resize(nodes_.size(), EMPTY_NODE);
        }
    } catch (const std::bad_alloc&) {
        MemoryException("Unavailable memory for the adjacency matrix extension");
    }


    adjacencyMatrix_.push_back(std::vector(nodes_.size(), EMPTY_NODE));
    Node<NData>& nodeRef = nodes_.nodes.back();
    return nodeRef;
}

template <typename NData, typename EData>
Node<NData>& Graph<NData, EData>::add(Identifier id, const NData& data) {
    validateAddNodes(id, data);
    auto newNode = Node<NData>(data, id);
    return insertNodeInBack(newNode);
}

template <typename NData, typename EData>
Node<NData>& Graph<NData, EData>::add(Identifier id, NData&& data) {
    validateAddNodes(id, data);
    auto newNode = Node<NData>(data, id);
    return insertNodeInBack(newNode);
}

template <typename NData, typename EData>
Node<NData>& Graph<NData, EData>::add(NData&& data) {
    return add(nodes_.size(), data);
}

template <typename NData, typename EData>
Node<NData>& Graph<NData, EData>::add(const NData& data) {
    return add(nodes_.size(), data);
}

/* --- Adding nodes PROXY --- */
template <typename NData, typename EData>
Node<NData>& Graph<NData, EData>::Nodes::add(Identifier id, const NData& data) {
    return graph->add(id, data);
}

template <typename NData, typename EData>
Node<NData>& Graph<NData, EData>::Nodes::add(Identifier id, NData&& data) {
    return graph->add(id, data);
}

template <typename NData, typename EData>
Node<NData>& Graph<NData, EData>::Nodes::add(const NData& data) {
    return graph->add(data);
}

template <typename NData, typename EData>
Node<NData>& Graph<NData, EData>::Nodes::add(NData&& data) {
    return graph->add(data);
}

/* ----  ADDING EDGES ----  */
template <typename NData, typename EData>
void Graph<NData, EData>::validateAddEdges(Identifier id, Identifier source, Identifier target) {
    if (id > edges_.size()) {
        throw IdentifierException("Invalid edge identifier " + std::to_string(id) + " requested");
    }

    if (id < edges_.size()) {
        throw ConflictException("Edge with identifier " + std::to_string(id) + " already exists");
    }

    if (source >= nodes_.size()) {
        throw ElementException("Source node with identifier " + std::to_string(source) + " does not exist");
    }

    if (target >= nodes_.size()) {
        throw ElementException("Target node with identifier " + std::to_string(target) + " does not exist");
    }

    if (adjacencyMatrix_[source][target] != EMPTY_NODE) {
        throw ConflictException("Edge between nodes " + std::to_string(source) + " and " + std::to_string(target) + " already exists");
    }
}

template <typename NData, typename EData>
Edge<EData>& Graph<NData, EData>::insertEdgeInBack(Identifier id, Identifier source, Identifier target,Edge<EData> edge) {
    try {
        edges_.edges.push_back(edge);
    } catch (const std::bad_alloc&) {
        throw MemoryException("Unavailable memory for a new edge in the edges container");
    }
    addEdgeToAdjacencyMatrix(id, source, target);

    return edges_.edges.back();
}

template <typename NData, typename EData>
Edge<EData>& Graph<NData, EData>::add(
        Identifier id,
        Identifier source,
        Identifier target,
        const EData& data) {
    validateAddEdges(id, source, target);
    auto newEdge = Edge<EData>(data, source, target, id);
    return insertEdgeInBack(id, source, target, newEdge);
}

template <typename NData, typename EData>
Edge<EData>& Graph<NData, EData>::add(
        Identifier id,
        Identifier source,
        Identifier target,
        EData&& data) {
    validateAddEdges(id, source, target);
    auto newEdge = Edge<EData>(data, source, target, id);
    return insertEdgeInBack(id, source, target, newEdge);
}

template <typename NData, typename EData>
Edge<EData>& Graph<NData, EData>::add(
        Identifier source,
        Identifier target,
        const EData& data) {
    return add(edges_.size(), source, target, data);
}

template <typename NData, typename EData>
Edge<EData>& Graph<NData, EData>::add(
        Identifier source,
        Identifier target,
        EData&& data) {
    return add(edges_.size(), source, target, data);
}

/* --- Ading edges from PROXY ---- */
template <typename NData, typename EData>
Edge<EData>& Graph<NData, EData>::Edges::add(
        Identifier id,
        Identifier source,
        Identifier target,
        EData&& data) {
    return graph->add(id, source, target, data);
}

template <typename NData, typename EData>
Edge<EData>& Graph<NData, EData>::Edges::add(
        Identifier id,
        Identifier source,
        Identifier target,
        const EData& data) {
    return graph->add(id, source, target, data);
}

template <typename NData, typename EData>
Edge<EData>& Graph<NData, EData>::Edges::add(
        Identifier source,
        Identifier target,
        const EData& data) {
    return graph->add(source, target, data);
}

template <typename NData, typename EData>
Edge<EData>& Graph<NData, EData>::Edges::add(
        Identifier source,
        Identifier target,
        EData&& data) {
    return graph->add(source, target, data);
}
/* ---- GETTERS ----- */

template <typename NData, typename EData>
Graph<NData, EData>::Nodes& Graph<NData, EData>::nodes() {
    return nodes_;
}

template <typename NData, typename EData>
const Graph<NData, EData>::Nodes& Graph<NData, EData>::nodes() const {
    return nodes_;
}

template <typename NData, typename EData>
Graph<NData, EData>::Edges& Graph<NData, EData>::edges() {
    return edges_;
}

template <typename NData, typename EData>
const Graph<NData, EData>::Edges& Graph<NData, EData>::edges() const {
    return edges_;
}

/* ---- IMPORT ----- */
template <typename NData, typename EData>
void Graph<NData, EData>::import(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw FileException("Unable to open input file " + filename);
    }

    import(file);
    file.close();
}

template <typename NData, typename EData>
void Graph<NData, EData>::import(std::istream& stream) {
    std::string line;
    
    std::regex nodePattern(R"(^node \((\d+)\s*\{\s*(.+?)\s*\}\s*\)$)");
    std::regex edgePattern(R"(^edge \((\d+)\)-\[(\d+)\s*\{\s*(.+?)\s*\}\]\s*->\s*\((\d+)\)$)");

    while (std::getline(stream, line)) {
        if (line.empty()) continue;

        std::smatch matches;
        if (std::regex_match(line, matches, nodePattern)) {
            std::vector<std::string> vec = smatchToVector(matches);

            Identifier id;
            std::istringstream(matches[1]) >> id;

            NData ndata;
            std::istringstream(matches[2]) >> ndata;
            this->add(id, ndata);
        } else if (std::regex_match(line, matches, edgePattern)) {
            std::vector<std::string> vec = smatchToVector(matches);

            Identifier sourceId;

            std::istringstream(matches[1]) >> sourceId;

            Identifier id;
            std::istringstream(matches[2]) >> id;

            EData edata;
            std::istringstream(matches[3]) >> edata;

            Identifier targetId;
            std::istringstream(matches[4]) >> targetId;
            this->add(id, sourceId, targetId, edata);
        } else {
            // Handle error or unmatched line
        }
    }
}
/* ---- DEBUG METHODS ----- */
template <typename NData, typename EData>
void Graph<NData, EData>::printMatrix(std::ostream& stream) const {
    for (size_t i = 0; i < adjacencyMatrix_.size(); ++i) {
        for (size_t j = 0; j < adjacencyMatrix_[i].size(); ++j) {
            Identifier cell = adjacencyMatrix_[i][j];
            if (cell == EMPTY_NODE) {
                stream << "-";
                if (j < adjacencyMatrix_[i].size() - 1) {
                    stream << "|";
                }
            } else {
                stream << cell;
                if (j < adjacencyMatrix_[i].size() - 1) {
                    stream << "|";
                }
            }
        }
        stream << std::endl;
    }
}

template <typename NData, typename EData>
void Graph<NData, EData>::Edges::printMatrix(std::ostream& stream) const {
    graph->printMatrix(stream);
}

template <typename NData, typename EData>
void Graph<NData, EData>::print(std::ostream& stream) const {
    nodes_.print(stream);
    stream << std::endl;
    edges_.print(stream);
    stream << std::endl;
}

template <typename NData, typename EData>
void Graph<NData, EData>::print(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        throw FileException("Unable to open output file " + filename);
    }

    print(file);
    file.close();
}

template <typename NData, typename EData>
Identifier Graph<NData, EData>::getId(const std::string& value) {
    Identifier id;
    std::istringstream(value) >> id;
    return id;
}

template <typename NData, typename EData>
EData Graph<NData, EData>::getEdgeDataFromField(const std::vector<std::string>& fields, size_t index) {
    std::string fieldString = fields.at(index);
    EData value;
    std::istringstream(fieldString) >> value;
    return value;
}

template <typename NData, typename EData>
NData Graph<NData, EData>::getNodeDataFromField(const std::vector<std::string>& fields, size_t index) {
    std::string fieldString = fields.at(index);
    NData value;
    std::istringstream(fieldString) >> value;
    return value;
}

template <typename NData, typename EData>
Identifier Graph<NData, EData>::getIdFromField(const std::vector<std::string>& fields, size_t index) {
    return getId(fields.at(index));
}

template <typename NData, typename EData>
std::vector<std::string> Graph<NData, EData>::smatchToVector(const std::smatch& matches) {
    std::vector<std::string> result;
    for (const auto& match : matches) {
        result.push_back(match.str());
    }
    return result;
}

/* --- ITERATOR --- */

template <typename NData, typename EData>
std::vector<Node<NData>>::iterator Graph<NData, EData>::Nodes::begin() {
    return nodes.begin();
}

template <typename NData, typename EData>
std::vector<Node<NData>>::iterator Graph<NData, EData>::Nodes::end() {
    return nodes.end();
}

template <typename NData, typename EData>
std::vector<Edge<EData>>::iterator Graph<NData, EData>::Edges::begin() {
    return edges.begin();
}

template <typename NData, typename EData>
std::vector<Edge<EData>>::iterator Graph<NData, EData>::Edges::end() {
    return edges.end();
}

template <typename NData, typename EData>
std::vector<Node<NData>>::const_iterator Graph<NData, EData>::Nodes::begin() const {
    return nodes.begin();
}

template <typename NData, typename EData>
std::vector<Node<NData>>::const_iterator Graph<NData, EData>::Nodes::end() const {
    return nodes.end();
}

template <typename NData, typename EData>
std::vector<Edge<EData>>::const_iterator Graph<NData, EData>::Edges::begin() const {
    return edges.begin();
}

template <typename NData, typename EData>
std::vector<Edge<EData>>::const_iterator Graph<NData, EData>::Edges::end() const {
    return edges.end();
}

template <typename NData, typename EData>
std::ostream& operator<<(std::ostream& stream, const Graph<NData, EData> graph) {
    graph.print(stream);
    return stream;
}

template <typename NData, typename EData>
std::ostream& operator<<(std::ostream& stream, const UndirectedGraph<NData, EData> graph) {
    graph.print(stream);
    return stream;
}

template <typename NData, typename EData>
std::ostream& operator<<(std::ostream& stream, const DirectedGraph<NData, EData> graph) {
    graph.print(stream);
    return stream;
}
#endif //GRAPHS_CLION_GRAPH_H

