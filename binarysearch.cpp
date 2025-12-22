#include "binarysearch.h"

BinarySearch::BinarySearch()
{

}

BinarySearch& BinarySearch::getInstance() {
    static BinarySearch singleton = BinarySearch();
    return singleton;
}

// 下面函数用于折半查找
int BinarySearch::search(StaticList &list, string &p) {
    // 把链表设置成线性的
    list.setToLink();
    // 快速排序
    QuickSort::getInstance().qsort(list,list.getHead(),list.getLength()-1);
    // 进行折半查找
    int low = list.getHead();
    int high = list.getLength()-1;
    int found = -1;
    // 获取对应数组
    const CarPlate* arr = list.getData();
    while (low <= high) {
        int mid = (low + high) / 2;

        // 比较车牌字符串
        if (arr[mid].plate == p) {
            found = mid;
            break;
        }
        else if (arr[mid].plate > p) {
            high = mid - 1; // 中间值比目标大，去左边找
        }
        else {
            low = mid + 1;  // 中间值比目标小，去右边找
        }
    }
    return found;
}
