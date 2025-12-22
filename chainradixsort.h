#ifndef CHAINRADIXSORT_H
#define CHAINRADIXSORT_H
#pragma once
// 此文件主要用于实现链式排序
#include "carplatesystem.h"
// 实现链式排序的类
class  ChainRadixSort{
public:
    // 设置字母数字的索引映射表
    int setIndex(char c);
    // 实现链式基数排序
    bool sort(StaticList& list);
    // 采用单例模式
    static ChainRadixSort& getInstance();
    // 把车牌号统一长度
    char arrangeLength(string& p,int k);
private:
    // 构造函数
    ChainRadixSort();
};

#endif // CHAINRADIXSORT_H
