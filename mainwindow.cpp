#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButton_page1_1,SIGNAL(clicked()),this,SLOT(selectFilesOfSegment()));
    QObject::connect(ui->pushButton_page2_1,SIGNAL(clicked()),this,SLOT(selectFilesOfJson()));
    QObject::connect(ui->pushButton_page1_4,SIGNAL(clicked()),this,SLOT(toDoSegment()));
    QObject::connect(ui->pushButton_page2_3,SIGNAL(clicked()),this,SLOT(toDoMerge()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

string qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}

void MainWindow::toDoSegment(){
    if(this->filesOfSegment.size()==0) return;
    this->sizeOfPerTmp = ui->spinBox_page1_1->value();
    for(int i=0;i<this->filesOfSegment.size();i++){
        ui->pushButton_page1_3->setText("正在处理:"+QString::number(i+1)+"/"+QString::number(this->filesOfSegment.size()));
        qApp->processEvents();
        int segmentNum = (QFile(this->filesOfSegment[i]).size()/(this->sizeOfPerTmp*1024*1024))+1;
        fileOperator  tmp;
        tmp.segment(qstr2str(splitFileNameFromPath(this->filesOfSegment[i])),segmentNum,qstr2str((splitFileNameFromPath(this->filesOfSegment[i])+".json")));
    }
    qDebug()<<"finish:toSegment"<<endl;
    ui->pushButton_page1_3->setText("分割完成");
    QMessageBox::information(NULL, "通知","分割完成",QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
    return;
}
void MainWindow::toDoMerge(){
    if(this->filesOfJson.size()==0) return;
    for(int i=0;i<this->filesOfJson.size();i++){
        ui->pushButton_page2_2->setText("正在处理:"+QString::number(i+1)+"/"+QString::number(this->filesOfJson.size()));
        qApp->processEvents();
        fileOperator tmp;
        qDebug()<<splitFileNameFromPath(this->filesOfJson[i])<<endl;
        tmp.merge(qstr2str(splitFileNameFromPath(this->filesOfJson[i])));
    }
    qDebug()<<"finish:toDoMerge"<<endl;
    ui->pushButton_page2_2->setText("合并完成");
    QMessageBox::information(NULL, "通知","合并完成",QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
    return;
}

void MainWindow::selectFilesOfSegment(){
    this->filesOfSegment = QFileDialog::getOpenFileNames(this,tr("文件选择"),tr("/home"),tr("任意文件(*)"));
    updateTableWidgetOfPage1();
    return;
}

void MainWindow::selectFilesOfJson(){
    this->filesOfJson = QFileDialog::getOpenFileNames(this,tr("文件选择"),tr("/home"),tr("json文件(*.json)"));
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

