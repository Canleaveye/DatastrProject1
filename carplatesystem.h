#ifndef CAR_PLATE_SYSTEM_H
#define CAR_PLATE_SYSTEM_H
#pragma once
#include <string>
#include <vector>
using namespace std;

struct CarPlate {
    string plate;
    string city;

    CarPlate() {}
    CarPlate(const string& p, const string& c) : plate(p), city(c) {}

    bool operator<(const CarPlate& other) const {
        return plate < other.plate;
    }

    bool operator==(const CarPlate& other) const {
        return plate == other.plate;
    }
};

class StaticList {
    friend class Manager;
public:
    static const int MAX_SIZE = 1000;
    // 增加1：排序类辅助函数：用于修改链表头
    void setHead(int h);
    StaticList();
    void clear();
    int size() const;
    bool empty() const;
    bool add(const CarPlate& cp);
    CarPlate get(int idx) const;
    bool set(int idx, const CarPlate& cp);
    int find(const CarPlate& cp) const;
    bool remove(int idx);
    void display() const;

    // 辅助函数
    const CarPlate* getData() const;
    int getNext(int idx) const;
    int getHead() const;
    int getLength() const;
    void setNext(int idx, int nxt);
    // 把静态数组线性化
    void setToLink();

private:
    CarPlate data[MAX_SIZE];
    int next[MAX_SIZE];
    int length;
    int head;
    int freeList;

};

class Manager {
private:
    static const string DATA_FILE;
    static const string PROVINCES;

public:
    // 后端修改点1：公开获取城市名的工具函数
    static string getLiaoningCity(char word);


    // 验证和格式化
    static bool checkPlate(const string& plate);
    static string formatPlate(const string& plate);

    // 数据输入
    // 手动输入
    static bool addManual(StaticList& list,string p);

    // 从文件输入
    static int loadFromFile(StaticList& list,const string& file);

    // 随机生成数据
    static int inputRandom(StaticList& list,int count);

    // 文件操作
    static void saveToFile(const StaticList& list);

    // 统计
    static void showStats(const StaticList& list);

    static string getFileName() { return DATA_FILE; }
};

#endif
