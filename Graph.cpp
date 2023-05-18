#include "Graph.h"
#include <iostream>

using MyGraph = GraphMatrix<int, double>; // 为 GraphMatrix 类起一个别名

int main() {
    MyGraph g;
    int v1 = g.insert(1);
    int v2 = g.insert(2);
    int v3 = g.insert(3);
    g.insert(0.5, 10, v1, v2);
    g.insert(0.8, 20, v2, v3);
    std::cout << g.inDegree(v2) << std::endl; // 输出顶点2的入度，应该为1
    g.remove(2);           // 删除顶点2及其关联边
    std::cout << g.outDegree(v1) << std::endl;  // 输出顶点1的出度，应该为0
    return 0;
}