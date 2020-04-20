#ifndef FILEMERGETHREAD_H
#define FILEMERGETHREAD_H

#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <sys/stat.h>
#include <exception>
#include "json.h"
#include "fileOperator.h"
#include <string>
using json = nlohmann::json;


class fileMergeThread : public QRunnable
{
public:
    fileMergeThread(QObject* obj, string JsonName);
private:
    void run() Q_DECL_OVERRIDE;
    string jsonName;
private:
    QObject * m_pObj = nullptr;
};

#endif // FILEMERGETHREAD_H
