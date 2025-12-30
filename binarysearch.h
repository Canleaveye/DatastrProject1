#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H
#include "carplatesystem.h"
#include "quicksort.h"
// 折半查找功能

class BinarySearch
{
public:
    int search(StaticList& list,string& p);
    static BinarySearch& getInstance();
private:
    BinarySearch();
};

#endif // BINARYSEARCH_H
