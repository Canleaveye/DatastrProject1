// 此文件主要用于实现链式排序
#include "CarPlateSystem.h"
// 实现链式排序的类
class  ChainRadixSort{
public:
    // 设置字母数字的索引映射表
    int set_index(char c);
    // 实现链式基数排序
    void sort(StaticList& list);
    // 采用单例模式
    static ChainRadixSort& get_instance();
private:
    // 构造函数
    ChainRadixSort();
};