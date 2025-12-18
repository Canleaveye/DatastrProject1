#include "CarPlateSystem.h"
#include "ChainRadixSort.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// 生成随机车牌号 (格式:  01A1234, 辽宁省内)
string generateRandomPlate() {
    // 辽宁省城市代码: 01-14 (沈阳01, 大连02, 鞍山03...)
    string plate = "";
    
    // 前两位：城市代码 00-14
    int cityCode = rand() % 15;
    plate += ('0' + cityCode / 10);
    plate += ('0' + cityCode % 10);
    
    // 第三位：字母 A-Z
    plate += ('A' + rand() % 26);
    
    // 后四位：数字 0-9
    for(int i = 0; i < 4; i++) {
        plate += ('0' + rand() % 10);
    }
    
    return plate;
}

// 获取城市名称
string getCityName(const string& plate) {
    string code = plate.substr(0, 2);
    if(code == "01") return "沈阳";
    if(code == "02") return "大连";
    if(code == "03") return "鞍山";
    if(code == "04") return "抚顺";
    if(code == "05") return "本溪";
    if(code == "06") return "丹东";
    if(code == "07") return "锦州";
    if(code == "08") return "营口";
    if(code == "09") return "阜新";
    if(code == "10") return "辽阳";
    if(code == "11") return "盘锦";
    if(code == "12") return "铁岭";
    if(code == "13") return "朝阳";
    if(code == "14") return "葫芦岛";
    return "未知";
}

int main() {
    srand(time(NULL));
    
    cout << "========================================" << endl;
    cout << "    辽宁省汽车牌照快速查询系统" << endl;
    cout << "========================================" << endl;
    
    // 创建静态链表
    StaticList list;
    
    // 添加测试数据
    cout << "\n>>> 生成随机测试数据..." << endl;
    
    // 手动添加一些固定数据便于验证
    list.append(CarPlate("02B7238", "大连", "张三"));
    list.append(CarPlate("01A1234", "沈阳", "李四"));
    list.append(CarPlate("03C5678", "鞍山", "王五"));
    list.append(CarPlate("01B9999", "沈阳", "赵六"));
    list.append(CarPlate("02A0001", "大连", "钱七"));
    list.append(CarPlate("01A0001", "沈阳", "孙八"));
    list.append(CarPlate("14Z9999", "葫芦岛", "周九"));
    list.append(CarPlate("07D3456", "锦州", "吴十"));
    
    // 随机生成几个
    for(int i = 0; i < 5; i++) {
        string plate = generateRandomPlate();
        string city = getCityName(plate);
        list.append(CarPlate(plate, city, "车主" + to_string(i+1)));
    }
    
    // 获取单例并进行排序
    ChainRadixSort& sorter = ChainRadixSort:: get_instance();
    
    cout << "\n>>> 开始链式基数排序..." << endl;
    sorter.sort(list);
    
    cout << "\n>>> 排序完成！" << endl;
    
    return 0;
}