#include "Graph.h"
#include <iostream>

using MyGraph = GraphMatrix<int, double>; // 为 GraphMatrix 类起一个别名

int main() {
    MyGraph g;
    int v1 = g.insert(1);
    int v2 = g.insert(2);
    int v3 = g.insert(3);
    int v4 = g.insert(4);
    int v5 = g.insert(5);
    int v6 = g.insert(6);
    int v7 = g.insert(7);
    int v8 = g.insert(8);
    int v9 = g.insert(9);
    int v10 = g.insert(10);
    g.insert(0.5, 10, v1, v2);
    g.insert(0.6, 15, v2, v3);
    g.insert(0.1, 16, v3, v4);
    g.insert(0.4, 4, v4, v5);
    g.insert(0.9, 14, v5, v6);
    g.insert(0.2, 3, v6, v7);
    g.insert(0.3, 7, v7, v8);
    g.insert(0.7, 9, v8, v9);
    g.insert(0.5, 11, v9, v10);
    g.insert(0.7, 12, v10, v1);
    g.insert(0.6, 20, v1, v3);
    g.insert(0.8, 19, v3, v5);
    g.insert(0.3, 18, v5, v7);
    g.bfs(0);
    g.dfs(0);
    return 0;
}