#include "filemergethread.h"

fileMergeThread::fileMergeThread(QObject* obj, string JsonName)
{
    jsonName = JsonName;
    m_pObj = obj;
}

void fileMergeThread::run(){
    fileOperator  tmp;
    tmp.merge(jsonName);
    QMetaObject::invokeMethod(m_pObj,"refreshMergeStatue",Q_ARG(QString,QString::fromStdString(jsonName)));
}
