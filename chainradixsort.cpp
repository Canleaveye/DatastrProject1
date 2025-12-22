#include "chainradixsort.h"
#include "carplatesystem.h"
// 构造函数
ChainRadixSort::ChainRadixSort() {

}
// 单例模式实现
ChainRadixSort& ChainRadixSort::getInstance() {
    // 确保唯一实例,保证ChainRadixSort对象只被创建一次
    static ChainRadixSort singleton = ChainRadixSort();
    return singleton;
}

// 先对字母和数字进行映射为链式基数排序做准备
/* 映射规则：
   '0'-'9' 映射为 0-9
    'A'-'Z' 映射为 10-35
*/
int ChainRadixSort::setIndex(char c) {
    if(c >= '0' && c <= '9') {
        return c - '0';
    }
    else if(c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    }
    // 非法情况检验
    return -1;
}

char ChainRadixSort::arrangeLength(string& p,int k) {
    char res = p[k];
    if(k == 0) {
        res = 'L';
        return res;
    }
    else if(k > 0 && k < 10) {
        res = p[k + 2];
        return res;
    }
    else {
        res = '0';
        return res;
    }
}

// 实现链式基数排序算法
bool ChainRadixSort::sort(StaticList& list) {
    // 对静态链表进行链式基数排序
//    list.display(); // 排序前的链表
    const int LENGTH = 7;
    const int R = 36;
    // 两个数组h和t，用于记录链表头和尾
    int h[R], t[R];
    int head = list.getHead();
    for(int i = LENGTH-1;i >= 0;i--) {
        for(int j = 0;j < R;j++) {
            h[j] = t[j] = -1;
        }
        // 遍历链表进行分配
        int p = head;
        // 当p还没指向空时
        while(p != -1) {
            // 获取该节点的数据
            CarPlate num = list.get(p);
            char c = arrangeLength(num.plate,i);
            // 映射相应的索引
            int index = setIndex(c);
            // 保留下一个节点的位置
            int nextp = list.getNext(p);
            // 分配到对应的桶中
            // 如果桶为空，则设置成头节点
            if(h[index] == -1) {
                h[index] = p;
            }
            else {
                list.setNext(t[index], p);
            }
            // 设置成尾节点
            t[index] = p;
            p = nextp;
        }

        // 收集阶段
        // 合成一个新链表，直接更改静态链表
        // 连接各个桶
        head = -1;
        int tail = -1; // 新链表的尾节点
        for(int j = 0;j < R;j++) {
            if(h[j] != -1) {
                // 头节点为空
                if(head == -1) {
                    head = h[j];
                }
                else {
                    list.setNext(tail, h[j]);
                }
                tail = t[j]; // 更新尾节点
            }
        }
        // 去除非法情况
        if(head == -1) {
            return false;
        }
        if(tail != -1) {
            list.setNext(tail, -1); // 尾节点指向空
        }
    }
    // 设置新链表节点
    list.setHead(head);
//    list.display(); // 排序后的链表(控制台调试用的)
    return true;
}

