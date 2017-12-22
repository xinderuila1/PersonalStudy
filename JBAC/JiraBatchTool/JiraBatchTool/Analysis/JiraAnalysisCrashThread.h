#ifndef JIRAANALYSISCRASHTHREAD_H
#define JIRAANALYSISCRASHTHREAD_H

/*!
*@file     自能分析Crash类
*@brief    
*@author   sunjj
*@date     2017年12月22日
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include <QThread>
#include <QTimer>
#include <QDate>
#include "Common/JiraConsts.h"

class JiraBatchCrashInfoOper;

class JiraAnalysisCrashThread: public QThread
{
    Q_OBJECT

public:
    JiraAnalysisCrashThread(QObject *parent = 0);
    ~JiraAnalysisCrashThread();

protected:
    virtual void run();

private slots:
    void autoAnalysisCrash();

private:
    QTimer* m_pTimer;
    int m_nInterval;
    JiraBatchCrashInfoOper* m_pBatchCrashInfoOper;
};
#endif