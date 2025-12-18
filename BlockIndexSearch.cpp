#include "BlockIndexSearch.h"

// 索引表单元构造函数
IndexTable::IndexTable() {
    start = -1;
    end = -1;
}

// 设置索引表单元的起始索引
int IndexTable::set_start(int s) {
    start = s;
    return start;
}

// 索引表单元的结束索引
int IndexTable::set_end(int e) {
    end = e;
    return end;
}

int IndexTable::get_start() const {
    return start;
}

int IndexTable::get_end() const {
    return end;
}

// 构造函数
BlockIndexSearch::BlockIndexSearch() {

}

// 单例模式实现
BlockIndexSearch& BlockIndexSearch::get_instance() {
    static BlockIndexSearch instance = BlockIndexSearch();
    return instance;
}


// 分块索引查找
void BlockIndexSearch::Init_table(StaticList& list) {
    // 先对链表进行链式基数排序
    ChainRadixSort::get_instance().sort(list);
    int l_curr = list.getHead();
    // 初始化索引表
    for(int i = 0;i < 26;i++) {
        table[i] = IndexTable();
    }
    // 再根据静态链表建立索引表
    while(l_curr != -1) {    
        // 确认索引
        int index = list.get(l_curr).plate[0] - 'A';
        // 如果是初始加入表中，同时把上一个结束加入。
        if(table[index].get_start() == -1) {
            table[index].set_start(l_curr);
        }
        // 更新结束位置
        table[index].set_end(l_curr);
        // 继续下一个
        l_curr = list.getNext(l_curr); 
    }
    return ;
}

// 根据分块索引进行查找
int BlockIndexSearch::search(StaticList& list,const string& plate) {
    // 根据第一个字母找到对应分块
    int index = plate[0] - 'A';
    int start = table[index].get_start();
    int end = table[index].get_end();
    // 开始进行折半查找
    while(start <= end) {
        int mid = (start + end) / 2;
        if(list.get(mid).plate == plate) {
            return mid; // 找到
        } else if(list.get(mid).plate < plate) {
            start = mid + 1;
        } else {
            end = mid - 1;
        }
    }
    // 未找到的情况
    return -1;
}