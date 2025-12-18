// 此部分实现按城市进行分块索引查找的功能
#include "CarPlateSystem.h"
#include "ChainRadixSort.h"

// 定义一个索引表单元
class IndexTable {
public:
    IndexTable();
    int set_start(int s);
    int set_end(int e);
    int get_start() const;
    int get_end() const;
private:
    int start; // 起始索引
    int end;   //结束索引
};

class BlockIndexSearch {
public:
    // 建立分块索引表
    void Init_table(StaticList& list);
    // 分块查找
    int search(StaticList& list,const string& plate);
    // 单例模式
    static BlockIndexSearch& get_instance();
private:
    BlockIndexSearch();
    // 建立索引表
    IndexTable table[26];
};