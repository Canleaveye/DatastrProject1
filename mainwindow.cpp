#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    Manager::loadFromFile(carList, "history.txt");
    if (!carList.empty()) {
        FuzzySearch::getInstance().setStr(carList);
    }
    updateTableDisplay();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI() {

    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);
    this->setWindowTitle("🚗辽宁省汽车牌照快速查询系统");
    this->setWindowIcon(QIcon(":/car.png"));
    this->resize(1250, 900);

    // 设置布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 输入保存数据区
    QGroupBox *inputGroup = new QGroupBox("📥数据录入", this);
    QHBoxLayout *inputLayout = new QHBoxLayout(inputGroup);
    QPushButton *btnImport = new QPushButton("📂文件导入", this);
    QPushButton *btnRandom = new QPushButton("🎲随机生成", this);
    QPushButton *btnManual = new QPushButton("✍️手动添加", this);
    QPushButton *btnUndo = new QPushButton("🔙撤销手动添加", this);
    QPushButton *btnSave = new QPushButton("📂保存入本地",this);
    inputLayout->addWidget(btnImport);
    inputLayout->addWidget(btnRandom);
    inputLayout->addWidget(btnManual);
    inputLayout->addWidget(btnUndo);
    inputLayout->addWidget(btnSave);
    // 排序与查询区
    QGroupBox *opGroup = new QGroupBox("排序与查询", this);
    QHBoxLayout *opLayout = new QHBoxLayout(opGroup);

    QPushButton *btnRadixSort = new QPushButton("🔗链式基数排序", this);
    QPushButton *btnQuickSort = new QPushButton("⚡快速排序", this);

    QLabel *lblSearch = new QLabel("🔍查找车牌:", this);
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("输车牌号");

    QPushButton *btnBlockSearch = new QPushButton("🧱分块索引查找", this);
    QPushButton *btnBinarySearch = new QPushButton("🌓折半查找", this);
    QPushButton *btnFuzzySearch = new QPushButton("🌫️模糊搜索", this);



    opLayout->addWidget(btnRadixSort);
    opLayout->addWidget(btnQuickSort);
    opLayout->addSpacing(20);
    opLayout->addWidget(lblSearch);
    opLayout->addWidget(searchEdit);
    opLayout->addWidget(btnBlockSearch);
    opLayout->addWidget(btnBinarySearch);
    opLayout->addWidget(btnFuzzySearch);

    // 以表格形式展示数据
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "🆔链表索引" << "🚙车牌号" << "🏙️归属地");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑

    // 底部状态栏
    statusLabel = new QLabel("🟢系统就绪", this);


    // 添加到主布局
    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(opGroup);
    mainLayout->addWidget(tableWidget);
    mainLayout->addWidget(statusLabel);

    // 信号与槽连接
    connect(btnImport, &QPushButton::clicked, this, &MainWindow::onImportFile);
    connect(btnRandom, &QPushButton::clicked, this, &MainWindow::onRandomGenerate);
    connect(btnManual, &QPushButton::clicked, this, &MainWindow::onManualAdd);
    connect(btnRadixSort, &QPushButton::clicked, this, &MainWindow::onRadixSort);
    connect(btnQuickSort, &QPushButton::clicked, this, &MainWindow::onQuickSort);
    connect(btnBlockSearch, &QPushButton::clicked, this, &MainWindow::onBlockSearch);
    connect(btnBinarySearch, &QPushButton::clicked, this, &MainWindow::onBinarySearch);
    connect(btnFuzzySearch, &QPushButton::clicked, this, &MainWindow::onFuzzySearch);
    connect(btnUndo, &QPushButton::clicked, this, &MainWindow::onUndo);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSave);
}

// 刷新表格显示，即遍历静态链表
void MainWindow::updateTableDisplay() {
    tableWidget->setRowCount(0); // 清空

    if (carList.empty()) return;

    int current = carList.getHead();
    int row = 0;

    // 遍历静态链表
    while (current != -1) {
        CarPlate cp = carList.get(current);

        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(current))); // 这里的ID是数组下标
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(cp.plate)));
        tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(cp.city)));

        current = carList.getNext(current);
        row++;
    }
    statusLabel->setText(QString("当前数据量: %1 条").arg(carList.size()));
}



// 导入文件GUI函数
void MainWindow::onImportFile() {
    QString file = QFileDialog::getOpenFileName(this, "选择数据文件", "", "Text Files (*.txt);;All Files (*)");
    if (file.isEmpty()) {
        return;
    }

    carList.clear();

    // 调用 Manager 的函数
    int count = Manager::loadFromFile(carList, file.toStdString());
    FuzzySearch::getInstance().setStr(carList);
    updateTableDisplay();
    QMessageBox::information(this, "导入成功", QString("成功导入 %1 条数据").arg(count));
}


// 随机生成车牌号GUI函数
void MainWindow::onRandomGenerate() {
    bool ok;
    int count = QInputDialog::getInt(this, "随机生成", "请输入生成数量:", 100, 1, 1000, 1, &ok);
    // 是否取消
    if (!ok) {
        return;
    }

    // 调用 Manager 的接口
    int added = Manager::inputRandom(carList, count);
    FuzzySearch::getInstance().setStr(carList);
    // 展示在表格
    updateTableDisplay();
    QMessageBox::information(this, "生成完成", QString("成功生成 %1 条数据").arg(added));
}


// 手动添加车牌GUI交互
void MainWindow::onManualAdd() {
    QString text = QInputDialog::getText(this, "手动添加", "请输入车牌 (如 辽A12345):");
    if (text.isEmpty()) {
        return;
    }

    bool ok = Manager::addManual(carList, text.toStdString());

    if (ok) {
        FuzzySearch::getInstance().insert(text.toStdString());
        undoStack.push(text.toStdString());
        updateTableDisplay();
        QMessageBox::information(this, "成功", "添加成功");
    }
    else {
        QMessageBox::critical(this, "错误", "添加失败！\n输入格式错误 或 车牌已存在。");
    }
}


// 链式基数排序
void MainWindow::onRadixSort() {
    if (carList.empty()) {
        return;
    }
    // 调用单例
    bool ok = ChainRadixSort::getInstance().sort(carList);
    if(!ok) {
        QMessageBox::information(this, "排序失败", "很抱歉因为某种原因排序失败了");
    }
    else {
        updateTableDisplay();
        QMessageBox::information(this, "排序完成", "已完成链式基数排序");
    }
}


// 快速排序
void MainWindow::onQuickSort() {
    // 检查表格是否为空
    if (carList.size() <= 0) {
        QMessageBox::warning(this, "提示", "没有数据可排序");
        return;
    }
    carList.setToLink();
    // 调用快排类API
    QuickSort::getInstance().qsort(carList,carList.getHead(),carList.getLength()-1);

    //遍历链表显示数据
    updateTableDisplay();
    QMessageBox::information(this, "成功", QString("快速排序完成！"));
}


// 分块索引查找
void MainWindow::onBlockSearch() {
    QString qKey = searchEdit->text().trimmed().toUpper();
    string key = qKey.toStdString();
    if (key.empty()) {
        QMessageBox::warning(this, "提示", "请输入查询内容");
        return;
    }
    // 如果用户没输“辽”，自动补全
    if (key.find("辽") == string::npos) {
        key = "辽" + key;
    }

    BlockIndexSearch::getInstance().initTable(carList);
    updateTableDisplay(); // 排序后刷新一下

    int id = BlockIndexSearch::getInstance().search(carList, key);

    if (id > 0 || id == 0) {
        // 在表格中高亮选中
        int row = 0;
        int curr = carList.getHead();
        while(curr != -1) {
            if(curr == id) {
                tableWidget->selectRow(row);
                tableWidget->scrollToItem(tableWidget->item(row, 0));
                break;
            }
            curr = carList.getNext(curr);
            row++;
        }
        CarPlate cp = carList.get(id);
        QMessageBox::information(this, "查找成功",QString("分块索引查找成功!\n车牌: %1\n归属地: %2").arg(QString::fromStdString(cp.plate)).arg(QString::fromStdString(cp.city)));
    }
    else {
        QMessageBox::warning(this, "查找失败", "未找到该车牌 (分块查找)");
    }
}

// 折半查找
void MainWindow::onBinarySearch() {
    if (carList.empty()) {
        QMessageBox::warning(this, "查找失败", "表格为空，无法查询！");
        return;
    }
    QString qKey = searchEdit->text().trimmed().toUpper();
    string key = qKey.toStdString();
    if (key.empty()) {
        QMessageBox::warning(this, "提示", "请输入查询内容");
        return;
    }
    if (key.find("辽") == string::npos) {
        key = "辽" + key;
    }
    int id = BinarySearch::getInstance().search(carList,key);
    updateTableDisplay();
    // 和上面同理的高亮显示和弹窗提示
    if (id > 0 || id == 0) {
        int row = 0;
        int curr = carList.getHead();
        while(curr != -1) {
            if(curr == id) {
                tableWidget->selectRow(row);
                tableWidget->scrollToItem(tableWidget->item(row, 0));
                break;
            }
            curr = carList.getNext(curr);
            row++;
        }
        CarPlate cp = carList.get(id);
        QMessageBox::information(this, "查找成功",QString("折半查找成功!\n车牌: %1\n归属地: %2").arg(QString::fromStdString(cp.plate)).arg(QString::fromStdString(cp.city)));
    }
    else {
        QMessageBox::warning(this, "查找失败", "未找到该车牌 (折半查找)");
    }
}

// 模糊搜索槽函数实现
void MainWindow::onFuzzySearch() {
    // 获取输入
    string pre = searchEdit->text().trimmed().toStdString();
    if (pre.empty()) {
        QMessageBox::warning(this, "提示", "请输入要搜索的前缀（如 辽A）");
        return;
    }

    // 接收结果的数组
    const int MAX_RES = 100;
    string results[MAX_RES];

    // 调用模糊搜索算法
    int count = FuzzySearch::getInstance().search(pre, results);

    if (count == 0) {
        QMessageBox::information(this, "结果", "没有找到匹配的车牌");
        return;
    }

    // 将结果显示在表格中
    tableWidget->setRowCount(0); // 先清空表格
    statusLabel->setText(QString("模糊搜索找到: %1 条").arg(count));

    for (int i = 0; i < count && i < MAX_RES; i++) {
        // 在StaticList 里对应
        CarPlate temp;
        temp.plate = results[i];

        // 在静态链表中查找完整信息
        int idx = carList.find(temp);

        if (idx != -1) {
            CarPlate realData = carList.get(idx);

            tableWidget->insertRow(i);
            tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(idx)));
            tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(realData.plate)));
            tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(realData.city)));
        }
    }

    QMessageBox::information(this, "搜索完成", QString("找到 %1 条以 '%2' 开头的车牌").arg(count).arg(QString::fromStdString(pre)));
}


void MainWindow::onUndo() {
    // 判断栈状态
    if (undoStack.empty()) {
        QMessageBox::warning(this, "提示", "没有可撤销的操作！");
        return;
    }

    string plateToDelete = undoStack.getTop();

    // 去链表里删
    CarPlate temp;
    temp.plate = plateToDelete;

    int idx = carList.find(temp);
    if (idx != -1) {
        carList.remove(idx);

        // 出栈
        undoStack.pop();

        // 刷新
        updateTableDisplay();
        FuzzySearch::getInstance().setStr(carList); // 重建索引

        QMessageBox::information(this, "撤销成功", QString("已撤销添加: %1").arg(QString::fromStdString(plateToDelete)));
    }
    else {
        undoStack.pop();
        QMessageBox::warning(this, "提示", "数据已不存在");
    }
}
void MainWindow::onSave() {
    // 直接调用函数
    Manager::saveToFile(carList);
    QMessageBox::information(this,"提示","成功保存进本地history.txt文件！");
}
