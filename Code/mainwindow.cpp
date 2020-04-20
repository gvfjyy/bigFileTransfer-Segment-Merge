#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initCustomUI();
    QObject::connect(ui->pushButton_page1_1,SIGNAL(clicked()),this,SLOT(selectFilesOfSegment()));
    QObject::connect(ui->pushButton_page2_1,SIGNAL(clicked()),this,SLOT(selectFilesOfJson()));
    QObject::connect(ui->pushButton_page1_4,SIGNAL(clicked()),this,SLOT(toDoSegment()));
    QObject::connect(ui->pushButton_page2_3,SIGNAL(clicked()),this,SLOT(toDoMerge()));
    ui->textBrowser->setOpenLinks(false);
    connect(ui->textBrowser, SIGNAL(anchorClicked(const QUrl&)),this, SLOT(openUrl(const QUrl&)));
    pool.setMaxThreadCount(4);
}

void MainWindow::initCustomUI(){
    ui->statusBar->setMinimumWidth(this->width());
    ui->pushButton_page1_3->setVisible(false);
    ui->pushButton_page2_2->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openUrl(const QUrl& url){
    QDesktopServices::openUrl(url);
}

string qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}
void MainWindow::refreshSegmentStatue(QString msg){
    ui->pushButton_page1_3->setText(msg);
    this->segmentedFile+=1;
    QString message = "Segment: "+QString::number(this->segmentedFile)+"/"+QString::number(this->filesOfSegment.size());
    ui->statusBar->showMessage(message);
    if(this->segmentedFile==this->filesOfSegment.size()){
        QMessageBox::information(nullptr, "Notice", "Segment Finish!", QMessageBox::Yes | QMessageBox::Yes);
    }
}
void MainWindow::refreshMergeStatue(QString msg){
    ui->pushButton_page2_2->setText(msg);
    this->mergedFile+=1;
    QString message = "Segment: "+QString::number(this->mergedFile)+"/"+QString::number(this->filesOfJson.size());
    ui->statusBar->showMessage(message);
    if(this->mergedFile==this->filesOfJson.size()){
        QMessageBox::information(nullptr, "Notice", "Merge Finish!", QMessageBox::Yes | QMessageBox::Yes);
    }
}
void MainWindow::toDoSegment(){
    ui->statusBar->showMessage("Segment Start!");
    this->segmentedFile = 0;
    if(this->filesOfSegment.size()==0) return;
    this->sizeOfPerTmp = ui->spinBox_page1_1->value();
    for(int i=0;i<this->filesOfSegment.size();i++){
        int segmentNum = int(QFile(this->filesOfSegment[i]).size()/(this->sizeOfPerTmp*1024*1024))+1;
        string inputFileName = qstr2str(splitFileNameFromPath(this->filesOfSegment[i]));
        string outputJsonFileName = qstr2str((splitFileNameFromPath(this->filesOfSegment[i])+".conf"));
        pool.start(new fileSegmentThread(this,inputFileName,segmentNum,outputJsonFileName));
    }
    return;
}
void MainWindow::toDoMerge(){
    ui->statusBar->showMessage("Merge Start");
    this->mergedFile = 0;
    if(this->filesOfJson.size()==0) return;
    for(int i=0;i<this->filesOfJson.size();i++){
        pool.start(new fileMergeThread(this,qstr2str(splitFileNameFromPath(this->filesOfJson[i]))));
    }
    return;
}

void MainWindow::selectFilesOfSegment(){
    this->filesOfSegment = QFileDialog::getOpenFileNames(this,tr("文件选择"),tr("/home"),tr("任意文件(*)"));
    updateTableWidgetOfPage1();
    return;
}

void MainWindow::selectFilesOfJson(){
    this->filesOfJson = QFileDialog::getOpenFileNames(this,tr("文件选择"),tr("/home"),tr("json文件(*.conf)"));
    updateTableWidgetOfPage2();
}

QString MainWindow::splitFileNameFromPath(QString path){
    int indexOdDot=path.lastIndexOf("/");
    QString filename=path.mid(indexOdDot+1);
    return filename;
}

void MainWindow::updateTableWidgetOfPage1(){
    if(this->filesOfSegment.size()==0) return;
    ui->tableWidget_page1_1->clear();
    int lengthOfName = max(500,int(ui->tableWidget_page1_1->width()*0.5));
    int lengthOfSize = ui->tableWidget_page1_1->width() - lengthOfName - 25;
    int num_col=2;
    int num_row=this->filesOfSegment.length()+1;
    ui->tableWidget_page1_1->setColumnCount(num_col);
    ui->tableWidget_page1_1->setRowCount(num_row);
    ui->tableWidget_page1_1->verticalHeader()->setVisible(false);
    ui->tableWidget_page1_1->horizontalHeader()->setVisible(false);
    ui->tableWidget_page1_1->setShowGrid(false);

    for(int i=0;i<num_row;i++){
        ui->tableWidget_page1_1->setRowHeight(i,25);
    }
    ui->tableWidget_page1_1->setColumnWidth(0,lengthOfName);
    ui->tableWidget_page1_1->setColumnWidth(1,lengthOfSize);

    ui->tableWidget_page1_1->setItem(0,0,new QTableWidgetItem("名称"));
    ui->tableWidget_page1_1->setItem(0,1,new QTableWidgetItem("大小"));

    for(int i=0;i<this->filesOfSegment.size();i++){
        ui->tableWidget_page1_1->setItem(i+1,0,new QTableWidgetItem(splitFileNameFromPath(this->filesOfSegment[i])));
        QString tmp = QString::number(QFile(this->filesOfSegment[i]).size()/1024/1024)+"MB";
        ui->tableWidget_page1_1->setItem(i+1,1,new QTableWidgetItem(tmp));
    }
    return;
}

void MainWindow::updateTableWidgetOfPage2(){
    if(this->filesOfJson.size()==0) return;
    ui->tableWidget_page2_1->clear();
    int num_col=1;
    int num_row=this->filesOfJson.length()+1;
    ui->tableWidget_page2_1->setColumnCount(num_col);
    ui->tableWidget_page2_1->setRowCount(num_row);
    ui->tableWidget_page2_1->verticalHeader()->setVisible(false);
    ui->tableWidget_page2_1->horizontalHeader()->setVisible(false);
    ui->tableWidget_page2_1->setShowGrid(false);

    for(int i=0;i<num_row;i++){
        ui->tableWidget_page2_1->setRowHeight(i,25);
    }
    ui->tableWidget_page2_1->setColumnWidth(0,ui->tableWidget_page2_1->width()-25);

    ui->tableWidget_page2_1->setItem(0,0,new QTableWidgetItem("名称"));

    for(int i=0;i<this->filesOfJson.size();i++){
        ui->tableWidget_page2_1->setItem(i+1,0,new QTableWidgetItem(splitFileNameFromPath(this->filesOfJson[i])));
    }
    return;
}

