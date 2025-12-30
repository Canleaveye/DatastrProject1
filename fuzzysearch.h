#ifndef FUZZYSEARCH_H
#define FUZZYSEARCH_H
#include <string>
#include "carplatesystem.h"
using namespace std;

// 用于实现模糊搜索

class FuzzySearch
{
public:
    ~FuzzySearch();
    static FuzzySearch& getInstance();

    int search(const string plate,string res[]);
    void insert(const string plate);
    void setStr(StaticList& l);
    void dfs(FuzzySearch* node,string res[],int& count);
private:
    // 是否走到车牌尾了
    bool end;

    FuzzySearch* find[256];
    // 此时对应的车牌号
    string p;
    FuzzySearch();

    void clearSon();
};

#endif // FUZZYSEARCH_H
