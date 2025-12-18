// CarPlateSystem.cpp
#include "CarPlateSystem.h"
#include <iostream>
using namespace std;

StaticList::StaticList() : length(0), head(-1), freeList(0) {
    for (int i = 0; i < MAX_SIZE - 1; ++i) next[i] = i + 1;
    next[MAX_SIZE - 1] = -1;
}

StaticList::~StaticList() {
    // 可选：clear(); 当前为静态分配，无需释放
}

void StaticList::clear() {
    for (int i = 0; i < MAX_SIZE - 1; ++i) next[i] = i + 1;
    next[MAX_SIZE - 1] = -1;
    freeList = 0;
    head = -1;
    length = 0;
}

int StaticList::getLength() const { return length; }
bool StaticList::isEmpty() const { return length == 0; }

bool StaticList::append(const CarPlate& cp) {
    if (freeList == -1) return false;  // 满了

    int node = freeList;          // 取一个空闲节点
    freeList = next[freeList];    // 空闲链表后移

    data[node] = cp;
    next[node] = -1;

    if (head == -1) {
        head = node;
    } else {
        int p = head;
        while (next[p] != -1) p = next[p];
        next[p] = node;
    }
    ++length;
    return true;
}

CarPlate StaticList::get(int index) const {
    if (index < 0 || index >= MAX_SIZE) return CarPlate();
    return data[index];
}

bool StaticList::set(int index, const CarPlate& cp) {
    if (index < 0 || index >= MAX_SIZE) return false;
    data[index] = cp;
    return true;
}

int StaticList::find(const CarPlate& cp) const {
    int p = head;
    while (p != -1) {
        if (data[p].plate == cp.plate) return p;
        p = next[p];
    }
    return -1;
}

bool StaticList::remove(int index) {
    if (head == -1 || index < 0 || index >= MAX_SIZE) return false;

    // 删除头节点
    if (head == index) {
        head = next[head];
    } else {
        int p = head;
        while (p != -1 && next[p] != index) p = next[p];
        if (p == -1) return false;       // 未找到
        next[p] = next[index];           // 跨过 index
    }

    // 回收节点
    next[index] = freeList;
    freeList = index;
    --length;
    return true;
}

const CarPlate* StaticList::getDataArray() const { return data; }

int StaticList::getNext(int index) const {
    if (index < 0 || index >= MAX_SIZE) return -1;
    return next[index];
}

int StaticList::getHead() const { return head; }

void StaticList::setList(int newHead, int newLength) {
    head = newHead;
    length = newLength;
}

void StaticList::setNext(int index, int nextIndex) {
    if (index >= 0 && index < MAX_SIZE) next[index] = nextIndex;
}

void StaticList::display() const {
    cout << "====== 车牌链表 (共" << length << "条) ======" << endl;
    int p = head, idx = 1;
    while (p != -1) {
        cout << idx++ << ". [" << p << "] "
             << data[p].plate << " | "
             << data[p].city  << " | "
             << data[p].owner << endl;
        p = next[p];
    }
    cout << "================================" << endl;
}