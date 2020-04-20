#ifndef FILESEGMENTTHREAD_H
#define FILESEGMENTTHREAD_H

#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <sys/stat.h>
#include <exception>
#include "json.h"
#include "fileOperator.h"
#include <string>
using json = nlohmann::json;


class fileSegmentThread : public QRunnable//QRunnable不是QObject的子类。
{
public:
    fileSegmentThread (QObject* obj,string InputFileName,int SegmentNum, string OutputJsonFileName);
protected:
    void run() Q_DECL_OVERRIDE;
private:
    string inputFileName;//待分割文件名
    int segmentNum;
    string outputJsonFileName;
private:
    QObject * m_pObj = nullptr;
};
#endif // FILESEGMENTTHREAD_H
