#pragma once
// 此部分实现按城市进行分块索引查找的功能
#include "carplatesystem.h"
#include "chainradixsort.h"

// 定义一个索引表单元
class IndexTable {
public:
    IndexTable();
    int setStart(int s);
    int setEnd(int e);
    int getStart() const;
    int getEnd() const;
private:
    int start; // 起始索引
    int end;   //结束索引
};

class BlockIndexSearch {
public:
    // 建立分块索引表
    void initTable(StaticList& list);
    // 分块查找
    int search(StaticList& list,const string& plate);
    // 单例模式
    static BlockIndexSearch& getInstance();
private:
    BlockIndexSearch();
    // 建立索引表
    IndexTable table[26];
};
