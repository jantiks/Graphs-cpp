//
//  main.cpp
//  Graphs
//
//  Created by Tigran on 30.12.23.
//

#include <iostream>
#include "Array.h"
#include "Graph.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "C01: Copy Constructor" << std::endl << std::endl;
    std::cout << "UndirectedGraph<std::string, std::string> gSource1; ..." << std::endl;
    UndirectedGraph<std::string, std::string> gSource1;
    std::cout << "gSource1.import(\"Input-Graph-6.txt\"); ..." << std::endl;
//    gSource1.import("/Users/tigran/Desktop/Classes/cpp/Graphs/Graphs/input.txt");
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
