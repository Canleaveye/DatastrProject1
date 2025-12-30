#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <map>
#include "carplatesystem.h"
using namespace std;

// 静态变量的声明
const string Manager::DATA_FILE = "history.txt";
const string Manager::PROVINCES = "辽";


StaticList::StaticList() : length(0), head(-1), freeList(0) {
    for (int i = 0; i < MAX_SIZE - 1; i++)
        next[i] = i + 1;
    next[MAX_SIZE - 1] = -1;
}

void StaticList::clear() {
    for (int i = 0; i < MAX_SIZE - 1; i++)
        next[i] = i + 1;
    next[MAX_SIZE - 1] = -1;
    freeList = 0;
    head = -1;
    length = 0;
}

int StaticList::size() const {
    return length;
}
bool StaticList::empty() const {
    return length == 0;
}

bool StaticList::add(const CarPlate& cp) {
    if (freeList == -1) {
        return false;
    }

    int node = freeList;
    freeList = next[freeList];

    data[node] = cp;
    next[node] = -1;
    // 设置头节点
    if (head == -1) {
        head = node;
    }

    else {
        int p = head;
        while (next[p] != -1) {
            p = next[p];
        }
        next[p] = node;
    }
    length++;
    return true;
}

CarPlate StaticList::get(int idx) const {
    if (idx < 0 || idx >= MAX_SIZE) {
        return CarPlate();
    }
    return data[idx];
}

bool StaticList::set(int idx, const CarPlate& cp) {
    if (idx < 0 || idx >= MAX_SIZE) {
        return false;
    }
    data[idx] = cp;
    return true;
}

int StaticList::find(const CarPlate& cp) const {
    int p = head;
    while (p != -1) {
        if (data[p].plate == cp.plate) return p;
        p = next[p];
    }
    return -1;
}

bool StaticList::remove(int idx) {
    if (head == -1) return false;

    if (head == idx) {
        head = next[head];
    }
    else {
        int p = head;
        while (p != -1 && next[p] != idx) p = next[p];
        if (p == -1) return false;
        next[p] = next[idx];
    }

    next[idx] = freeList;
    freeList = idx;
    length--;
    return true;
}

void StaticList::display() const {
    cout << "\n=== 车牌列表 (" << length << "条) ===" << endl;
    if (length == 0) {
        cout << "空" << endl;
        return;
    }

    cout << left << setw(5) << "序号"
        << setw(15) << "车牌号"
        << setw(10) << "城市" << endl;
    cout << string(30, '-') << endl;

    int p = head, idx = 1;
    while (p != -1) {
        cout << setw(5) << idx++
            << setw(15) << data[p].plate
            << setw(10) << data[p].city << endl;
        p = next[p];
    }
    cout << "========================" << endl;
}

const CarPlate* StaticList::getData() const {
    return data;
}
int StaticList::getNext(int idx) const {
    return (idx >= 0 && idx < MAX_SIZE) ? next[idx] : -1;
}
int StaticList::getHead() const {
    return head;
}
void StaticList::setNext(int idx, int nxt) {
    if (idx >= 0 && idx < MAX_SIZE) next[idx] = nxt;
}


bool Manager::checkPlate(const string& plate) {
    // 基础车牌长度检查
    if (plate.length() != 9 && plate.length() != 10) {
        // cout << "车牌长度错误" << endl;(测试用)
        return false;
    }

    // 省份检查
    string prov = plate.substr(0, 3);
    bool found = false;
    if (prov == PROVINCES) {
        found = true;
    }
    if (!found) return false;

    // 城市代码检查
    char citycode = plate[3];
    if (!((citycode >= 'A' && citycode <= 'Z') || (citycode >= 'a' && citycode <= 'z'))) {
        return false;
    }

    const string rightword = "ABCDEFGHJKLMNPV";
    // 排除非法字符
    if (rightword.find(citycode) == string::npos) {
        return false; // 拒绝接收
    }

    // 号码部分检查,下标4开始截取
    string number = plate.substr(4);

    int numberLength = number.length();
    if (numberLength != 5 && numberLength != 6) {
        return false;
    }

    // 检查号码内容（数字或字母）
    for (char c : number) {
        if (!((c >= '0' && c <= '9')||(c >= 'A' && c <= 'Z')||(c >= 'a' && c <= 'z'))) {
            return false;
        }
    }
    return true;
}

// 格式化，把小写字母转化成大写
string Manager::formatPlate(const string& plate) {
    string res = plate;
    for(char& c:res) {
        if(c >= 'a' && c <= 'z') {
            c = c - 32;
        }
    }
    return res;
}


// 根据车牌号的字母匹配对应的城市
string Manager::getLiaoningCity(char word) {
    word = toupper(word);
    switch (word) {
        case 'A': return "沈阳";
        case 'B': return "大连";
        case 'C': return "鞍山";
        case 'D': return "抚顺";
        case 'E': return "本溪";
        case 'F': return "丹东";
        case 'G': return "锦州";
        case 'H': return "营口";
        case 'J': return "阜新";
        case 'K': return "辽阳";
        case 'L': return "盘锦";
        case 'M': return "铁岭";
        case 'N': return "朝阳";
        case 'P': return "葫芦岛";
        case 'V': return "省直机关";
        default: return "辽宁其他";
    }
}


// 手动添加车牌号，返回是否添加成功
bool Manager::addManual(StaticList& list, string p) {
    // 去除首尾空格
    p.erase(0, p.find_first_not_of(" \t"));
    p.erase(p.find_last_not_of(" \t") + 1);
    string plate = formatPlate(p);
    if (!checkPlate(plate)) {
        return false;
    }

    // 查重
    CarPlate temp(plate, "");
    if (list.find(temp) != -1) {
        return false; // 已存在
    }

    // 获取城市。
    char citycode = plate[3];
    string cityname = getLiaoningCity(citycode);

    return list.add(CarPlate(plate, cityname));
}



// 文件输入数据
int Manager::loadFromFile(StaticList& list, const string& file) {
    string file_1 = file;
    ifstream data(file_1);
    // 打开文件
    if (!data.is_open()) {
        return 0; // 打开失败
    }

    string line;
    int success = 0;
    // 一行行读取
    while(getline(data, line)) {
        // 跳过空行和注释
        if (line.empty() || line[0] == '#') continue;

        string plate = line;

        // 去除首尾空格，换行符，Tab键
        plate.erase(0, plate.find_first_not_of(" \t\r\n"));
        plate.erase(plate.find_last_not_of(" \t\r\n") + 1);

        // 检验是否含非法字符
        if (!checkPlate(plate)) {
            continue;
        }

        // 格式化字符串
        plate = formatPlate(plate);

        // 提取城市代码并自动获取城市名(我这里QT的环境是UTF-8)
        char cityCodeChar = plate[3];

        string city = getLiaoningCity(cityCodeChar);

        // 查重
        CarPlate temp(plate, "");
        if (list.find(temp) != -1) {
            continue;
        }

        // 添加到链表
        if (list.add(CarPlate(plate, city))) {
            success++;
        }
    }
    data.close();
    return success;
}

// 随机生成车牌号数据
int Manager::inputRandom(StaticList& list, int count) {
    int space = StaticList::MAX_SIZE - list.size();
    // 检查是否超出表长
    if (count > space) {
        count = space;
    }
    if (count <= 0) {
        return 0;
    }
    // 设置随机数种子，确保只生成一次
    static bool seed = false;
    if (!seed) {
        srand(time(nullptr));
        seed = true;
    }
    const string rightcode = "ABCDEFGHJKLMNP";
    int success = 0;

    for (int i = 0; i < count; i++) {
        char citycode = rightcode[rand() % rightcode.length()];
        int numlen = (rand() % 10 < 8) ? 5 : 6; // 设置车牌号位数，默认油车占比多一些

        string number = "";
        // 设置随机号码
        for(int j=0; j<numlen; j++) {
            if (rand() % 10 < 3) {
                 char c = 'A' + rand() % 26;
                 if (c != 'I' && c != 'O') {
                     number += c;
                 }
                 else {
                     number += (rand() % 10 + '0');
                 }
            }
            else {
                number += (rand() % 10 + '0');
            }
        }
        string plate = "辽";
        plate += citycode;
        plate += number;
        string cityname = getLiaoningCity(citycode);

        if (list.add(CarPlate(plate, cityname))) {
            success++;
        }
    }
    return success;
}


void Manager::saveToFile(const StaticList& list) {
    ofstream local("history.txt");
    if(!local.is_open()) {
        cout << "无法写入文件！" << endl;
        return;
    }
    int p = list.getHead();
    while (p != -1) {
        CarPlate cp = list.get(p);
        // 格式：车牌号 (换行)
        // 只存车牌号,和导入的文件一个格式
        local << cp.plate << endl;
        p = list.getNext(p);
    }

    local.close();
}


void Manager::showStats(const StaticList& list) {
    cout << "\n=== 系统统计 ===" << endl;
    cout << "总记录: " << list.size() << " 条" << endl;
    cout << "总容量: " << StaticList::MAX_SIZE << " 条" << endl;
    cout << "剩余空间: " << StaticList::MAX_SIZE - list.size() << " 条" << endl;

    double rate = static_cast<double>(list.size()) / StaticList::MAX_SIZE * 100;
    cout << "使用率: " << fixed << setprecision(1) << rate << "%" << endl;

    if (list.empty()) return;

    map<string, int> cityStats;
    int p = list.getHead();

    while (p != -1) {
        CarPlate cp = list.get(p);
        cityStats[cp.city]++;
        p = list.getNext(p);
    }

    cout << "\n城市分布:" << endl;
    for (const auto& entry : cityStats) {
        double percent = static_cast<double>(entry.second) / list.size() * 100;
        cout << "  " << entry.first << ": " << entry.second
            << " (" << fixed << setprecision(1) << percent << "%)" << endl;
    }
}


// 修改链表表头函数
void StaticList::setHead(int h) {
    head = h;
}

// 把静态链表线性化函数（为快速排序和折半查找写的函数）
void StaticList::setToLink() {

    // 将静态链表彻底线性化
    if (length == 0) {
        return;
    }

    // 把链表转化为物理有序
    vector<CarPlate> temp;
    temp.reserve(length);

    int curr = head;
    while (curr != -1) {
        temp.push_back(data[curr]);
        curr = next[curr];
    }

    // 将暂存的数据按顺序写回 data 数组
    for (int i = 0; i < length; i++) {
        data[i] = temp[i];
        if (i < length - 1) {
            next[i] = i + 1;
        }
        else {
            next[i] = -1;
        }
    }

    // 重置头指针和空闲链表
    head = 0;

    // 重建空闲链表
    if (length < MAX_SIZE) {
        freeList = length;
        for (int i = length; i < MAX_SIZE - 1; i++) {
            next[i] = i + 1;
        }
        next[MAX_SIZE - 1] = -1;
    }
    else {
        freeList = -1;
    }
}

// 获取链表长
int StaticList::getLength() const {
    return length;
}
