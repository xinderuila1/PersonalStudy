#ifndef JIRAANALYSISCRASHTHREAD_H
#define JIRAANALYSISCRASHTHREAD_H

/*!
*@file     ���ܷ���Crash��
*@brief    
*@author   sunjj
*@date     2017��12��22��
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