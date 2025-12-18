// CarPlateSystem.h
#ifndef CAR_PLATE_SYSTEM_H
#define CAR_PLATE_SYSTEM_H

#include <string>
using namespace std;

// 车牌实体结构定义
struct CarPlate {
    string plate;  // 车牌号
    string city;   // 城市
    string owner;  // 车主姓名

    CarPlate() : plate(""), city(""), owner("") {}
    CarPlate(const string& p, const string& c, const string& o = "")
        : plate(p), city(c), owner(o) {}
};

class StaticList {
    // 添加点1
    friend class ChainRadixSort;
private:
    static const int MAX_SIZE = 1000;  // 静态链表最大容量

    CarPlate data[MAX_SIZE];  // 数据区
    int next[MAX_SIZE];       // “指针”区（索引）
    int length;               // 当前链表长度
    int head;                 // 头指针（索引）
    int freeList;             // 空闲链表头指针

public:
    // 构造 / 析构
    StaticList();
    ~StaticList();

    // 基础操作
    void clear();
    int  getLength() const;
    bool isEmpty() const;
    bool append(const CarPlate& data);
    CarPlate get(int index) const;
    bool set(int index, const CarPlate& data);
    int  find(const CarPlate& data) const;   // 按 plate 匹配
    bool remove(int index);                  // 按索引删除（需存在于链）

    // 访问内部
    const CarPlate* getDataArray() const;
    // 修改点2
    int  getNext(int index) const;           // 仅暴露单个节点的 next
    int  getHead() const;

    // 修改内部结构
    void setList(int newHead, int newLength);
    void setNext(int index, int nextIndex);

    // 调试输出
    void display() const;
};

// 设计一个类管理文件的输入
class DataLoader {

};

#endif // CAR_PLATE_SYSTEM_H