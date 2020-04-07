#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QDir>

#include "json.h"
#include "fileOperator.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <exception>
using namespace std;
using json = nlohmann::json;


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

public Q_SLOTS:
    void selectFilesOfSegment();
    void selectFilesOfJson();
    QString splitFileNameFromPath(QString path);
    void toDoSegment();
    void toDoMerge();
private:
    void updateTableWidgetOfPage1();
    void updateTableWidgetOfPage2();
private:
    QStringList filesOfSegment;
    QStringList filesOfJson;
    int sizeOfPerTmp;
};
#endif // MAINWINDOW_H
