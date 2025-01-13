//
//  Node.hpp
//  Graphs
//
//  Created by Tigran on 02.01.24.
//

#ifndef Node_hpp
#define Node_hpp

#include <iostream>
#include "stdio.h"
#include "Globals.h"

template <typename NData>
class Node {
public:
    Node(const NData& data, Identifier id);
    Node(const NData&& data, Identifier id);
    inline Identifier getId() const;
    inline NData& getData();
    inline const NData& getData() const;

private:
    NData data_;
    Identifier id_;
};

template <typename NData>
Node<NData>::Node(const NData& data, Identifier id):data_(data), id_(id) {};

template <typename NData>
Node<NData>::Node(const NData&& data, Identifier id):data_(std::move(data)), id_(id) {};

template <typename NData>
const NData& Node<NData>::getData() const {
    return data_;
}

template <typename NData>
NData& Node<NData>::getData() {
    return data_;
}


template <typename NData>
Identifier Node<NData>::getId() const {
    return id_;
}

template <typename NData>
std::ostream& operator<<(std::ostream& stream, const Node<NData>& node) {
    stream << "node (" << node.getId() << " {" << node.getData() << "})";
    return stream;
}
#endif /* Node_hpp */
