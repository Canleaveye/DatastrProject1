#include "quicksort.h"

QuickSort::QuickSort()
{

}

// 单例
QuickSort& QuickSort::getInstance() {
    static QuickSort singleton = QuickSort();
    return singleton;
}

//交换数据
bool QuickSort::swap(StaticList &list, int i, int j) {
    CarPlate temp = list.get(i);
    list.set(i, list.get(j));
    list.set(j,temp);
    return true;
}

// 快速排序的划分
int QuickSort::partition(StaticList& list,int low,int high) {
    // 选基准
    CarPlate pivot = list.get(low);
    int start = low;
    while(low < high) {
        // 先右边指针向左找比基准小的
        while(low < high && (list.get(high).plate > pivot.plate || list.get(high).plate == pivot.plate)) {
            high--;
        }
        // 左边指针往右找比基准大的
        while(low < high && (list.get(low).plate < pivot.plate || list.get(low).plate == pivot.plate)) {
            low++;
        }
        if(low < high) {
            swap(list,low,high);
        }
    }
    // 结束把开始地方填上
    swap(list,start,low);
    return low;

}
// 快速排序的实现
bool QuickSort::qsort(StaticList& list,int low,int high) {
    // 还没完成排序就一直递归
    if(low < high) {
        // 先进行左右划分
        int pivotpos = partition(list,low,high);
        // 先进行左半边划分的排序
        qsort(list,low,pivotpos - 1);
        // 右半边划分的排序
        qsort(list,pivotpos + 1,high);
    }
    return true;
}
