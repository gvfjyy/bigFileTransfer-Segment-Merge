#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QDesktopServices>
#include <QThreadPool>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "filesegmentthread.h"
#include "filemergethread.h"
using namespace std;



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QThreadPool pool;


public Q_SLOTS:
    void selectFilesOfSegment();
    void selectFilesOfJson();
    QString splitFileNameFromPath(QString path);
    void toDoSegment();
    void toDoMerge();
    void openUrl(const QUrl& url);
private:
    void updateTableWidgetOfPage1();
    void updateTableWidgetOfPage2();
private:
    QStringList filesOfSegment;
    QStringList filesOfJson;
    int segmentedFile;
    int mergedFile;
    int sizeOfPerTmp;//切割后每个tmp文件的大小（MB）
    void initCustomUI();//初始化代码添加的UI
public://用于QRunnable通信
    Q_INVOKABLE void refreshSegmentStatue(QString msg);
    Q_INVOKABLE void refreshMergeStatue(QString msg);
};
#endif // MAINWINDOW_H
