#ifndef MYSTACK_H
#define MYSTACK_H
#include <string>
using namespace std;
// 实现撤回功能的函数

struct HistoryRecord {
    string plate; // 被添加的车牌号
};

class Mystack
{
public:
    Mystack();
    bool empty();
    bool full();
    int size();
    bool push(string p);
    bool pop();
    string getTop();
private:
    string data[100];
    int top;
};

#endif // MYSTACK_H
