#ifndef JIRAINTELLIGENTWARNINGTHREAD_H
#define JIRAINTELLIGENTWARNINGTHREAD_H

/*!
*@file     ����Ԥ���߳�
*@brief    
*@author   sunjj
*@date     2017��12��11��
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/

#include <QThread>
#include <QTimer>
#include <QDate>

class JiraIntelligentWaringOper;

class JiraIntelligentWarningThread: public QThread
{
    Q_OBJECT
public:
    JiraIntelligentWarningThread(QObject *parent = 0);
    ~JiraIntelligentWarningThread();

protected:
    virtual void run();

private slots:
    void collectUserInfo();

private:
    QTimer* m_pTimer;
    QDate m_pCurrentDay;
    JiraIntelligentWaringOper* m_pIntelligentWaringOper;
};
#endif