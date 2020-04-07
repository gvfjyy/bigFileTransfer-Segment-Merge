#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QFile file(QDir::currentPath()+"/style.qss");//发布时使用
    //QFile file("D:/imageRotate/style.qss");

    if (file.open(QFile::ReadOnly))
    {
        QString stylesheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(stylesheet);
        file.close();
    }
    else
    {
        QMessageBox::warning(NULL, "warning","openStyleSheetFailure",QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
    }
    w.show();
    return a.exec();
}
