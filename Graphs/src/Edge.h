//
//  Edge.hpp
//  Graphs
//
//  Created by Tigran on 02.01.24.
//

#ifndef Edge_hpp
#define Edge_hpp

#include <stdio.h>
#include <iostream>
#include "Globals.h"

template <typename EData>
class Edge {
public:
    Edge(const EData& data, Identifier source, Identifier target, Identifier id);
    Edge(EData&& data, Identifier source, Identifier target, Identifier id);
    inline EData& getData();
    inline const EData& getData() const;
    inline Identifier getId() const;
    inline Identifier getSource() const;
    inline Identifier getTarget() const;
private:
    EData data_;
    Identifier source_;
    Identifier target_;
    Identifier id_;
};

template <typename EData>
Edge<EData>::Edge(const EData& data, Identifier source, Identifier target, Identifier id):data_(data), source_(source), target_(target), id_(id) {}

template <typename EData>
Edge<EData>::Edge(EData&& data, Identifier source, Identifier target, Identifier id):data_(data), source_(source), target_(target), id_(id) {}

template <typename EData>
EData& Edge<EData>::getData() {
    return data_;
}

template <typename EData>
const EData& Edge<EData>::getData() const {
    return data_;
}

template <typename EData>
Identifier Edge<EData>::getId() const {
    return id_;
}

template <typename EData>
Identifier Edge<EData>::getSource() const {
    return source_;
}

template <typename EData>
Identifier Edge<EData>::getTarget() const {
    return target_;
}

template <typename EData>
std::ostream& operator<<(std::ostream& stream, const Edge<EData>& edge) {
    stream << "edge (" << edge.getSource() << ")-[" << edge.getId() << " {" << edge.getData() << "}]->(" << edge.getTarget() << ")";
    return stream;
}

#endif /* Edge_hpp */
