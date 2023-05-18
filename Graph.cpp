#include "Graph.h"
#include <iostream>

using MyGraph = GraphMatrix<int, double>; // Ϊ GraphMatrix ����һ������

int main() {
    MyGraph g;
    int v1 = g.insert(1);
    int v2 = g.insert(2);
    int v3 = g.insert(3);
    g.insert(0.5, 10, v1, v2);
    g.insert(0.8, 20, v2, v3);
    std::cout << g.inDegree(v2) << std::endl; // �������2����ȣ�Ӧ��Ϊ1
    g.remove(2);           // ɾ������2���������
    std::cout << g.outDegree(v1) << std::endl;  // �������1�ĳ��ȣ�Ӧ��Ϊ0
    return 0;
}