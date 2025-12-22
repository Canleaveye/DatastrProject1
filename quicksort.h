#ifndef QUICKSORT_H
#define QUICKSORT_H
#include "carplatesystem.h"

class QuickSort
{
public:
    // 快速排序划分操作
    int partition(StaticList& list,int low,int high);
    // 交换数据位置函数
    bool swap(StaticList& list,int i,int j);
    // 实现快速排序
    bool qsort(StaticList& list,int low,int high);
    static QuickSort& getInstance();
private:
    QuickSort();
};

#endif // QUICKSORT_H
