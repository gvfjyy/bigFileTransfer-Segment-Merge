#include "filesegmentthread.h"

fileSegmentThread::fileSegmentThread(QObject* obj,string InputFileName,int SegmentNum, string OutputJsonFileName)
{
    inputFileName = InputFileName;
    segmentNum = SegmentNum;
    outputJsonFileName = OutputJsonFileName;
    m_pObj = obj;
}

void fileSegmentThread::run(){
    fileOperator  tmp;
    tmp.segment(inputFileName,segmentNum,outputJsonFileName);
    QMetaObject::invokeMethod(m_pObj,"refreshSegmentStatue",Q_ARG(QString,QString::fromStdString(inputFileName)));
}
