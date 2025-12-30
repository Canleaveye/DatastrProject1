#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QCompleter>
#include <QStringListModel>
// 引入后端文件
#include "carplatesystem.h"
#include "chainradixsort.h"
#include "blockindexsearch.h"
#include "quicksort.h"
#include "binarysearch.h"
#include "fuzzysearch.h"
#include "mystack.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 按钮对应的功能函数
    void onImportFile();        // 文件导入
    void onRandomGenerate();    // 随机生成
    void onManualAdd();         // 手动添加
    void onRadixSort();         // 链式基数排序
    void onQuickSort();         // 快速排序 (用于折半查找前置)
    void onBlockSearch();       // 分块索引查找
    void onBinarySearch();      // 折半查找
    void onFuzzySearch();
    void onUndo();
    void onSave();
private:
    // UI 组件
    QTableWidget *tableWidget;  // 显示数据的表格
    QLineEdit *searchEdit;      // 搜索框
    QLabel *statusLabel;        // 底部状态栏

    // 核心数据对象
    StaticList carList;
    Mystack undoStack;
    // 辅助函数
    void setupUI();
    void updateTableDisplay();  // 将 StaticList 的数据显示到表格

};

#endif // MAINWINDOW_H
