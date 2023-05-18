#include <iostream>
#include <cstdlib>
#include <cstdio>
template<typename T>
class Queue {
private:
    T* queue;     // 队列的数组
    int front;    // 队头指针
    int rear;     // 队尾指针
    int capacity; // 队列容量
public:

    // 构造函数
    Queue(int cap) : front(0), rear(0), capacity(cap) {
        queue = new T[cap]; // 动态分配数组空间
    }

    // 析构函数
    ~Queue() {
        delete[] queue; // 释放数组空间
    }

    // 判断队列是否为空
    bool isEmpty() {
        return front == rear;
    }

    // 判断队列是否满
    bool isFull() {
        return (rear + 1) % capacity == front;
    }

    // 获取队头元素
    T& Front() {
        if (isEmpty()) {
            std::cout<<"Queue is empty"<< std::endl;
            exit(-1);
        }
        return queue[front];
    }

    // 获取队尾元素
    T& back() {
        if (isEmpty()) {
            std::cout << "Queue is empty" << std::endl;
            std::exit(-1);
        }
        return queue[rear-1];
    }

    // 入队一个元素
    void enqueue(T element) {
        if (isFull()) {
            std::cout << "Queue is full" << std::endl;
            exit(-1);
        }
        queue[rear] = element; // 将元素赋值给数组中队尾指针所指向的位置
        rear = (rear + 1) % capacity; // 将队尾指针加一后对容量取模
    }

    // 出队一个元素
    T dequeue() {
        if (isEmpty()) {
            std::cout << "Queue is empty" << std::endl;
            std::exit(-1);
        }
        T front_value = queue[front]; // 获取数组中队头指针所指向的元素
        front = (front + 1) % capacity; // 将队头指针加一后对容量取模
        return front_value; // 返回出队元素
    }

};
