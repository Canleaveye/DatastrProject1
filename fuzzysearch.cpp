#include "fuzzysearch.h"

FuzzySearch::FuzzySearch() {
    end = false;
    p = "";
    for(int i = 0;i < 256;i++) {
        find[i] = NULL;
    }
}
FuzzySearch::~FuzzySearch() {
    for(int i = 0;i < 256;i++) {
        if(find[i] != NULL) {
            delete find[i];
        }
        find[i] = NULL;
    }
}

FuzzySearch& FuzzySearch::getInstance() {
    static FuzzySearch singleton = FuzzySearch();
    return singleton;
}

void FuzzySearch::insert(const string plate) {
    FuzzySearch* cur = this;
    for(char c:plate) {
        int id = (unsigned char) c;
        if(cur->find[id] == NULL) {
            cur->find[id] = new FuzzySearch();
        }
        cur = cur->find[id];
    }
    cur->end = true;
    cur->p = plate;

}

void FuzzySearch::clearSon() {
    for(int i = 0; i < 256; i++) {
        if (this->find[i] != nullptr) {
            delete this->find[i];

            // 【保险】删掉后，一定要把指针指回 nullptr，防止变成野指针
            this->find[i] = nullptr;
        }
    }

    // 2. 把我自己洗白 (重置状态)
    this->end = false;
    this->p = "";
}

void FuzzySearch::setStr(StaticList &l) {
    this->clearSon();
    int cur = l.getHead();
    while(cur != -1) {
        CarPlate cp = l.get(cur);
        this->insert(cp.plate);
        cur = l.getNext(cur);
    }
}

int FuzzySearch::search(const string plate, string res[]) {
    FuzzySearch* cur = this;
    int count = 0;
    for(char c:plate) {
        int id = (unsigned char) c;
        if(cur->find[id] == NULL) {
            return 0;
        }
        cur = cur->find[id];
    }
    cur->dfs(cur, res, count);
    return count;
}

void FuzzySearch::dfs(FuzzySearch* node,string res[],int& count) {
    if(node->end) {
        res[count] = node->p;
        count++;
    }

    for(int i = 0;i < 256;i++) {
        if(node->find[i] != NULL) {
            node->find[i]->dfs(node->find[i],res,count);
        }
    }

}
