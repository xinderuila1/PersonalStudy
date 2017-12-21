#ifndef JIRAANALYSISDUMPOPER_H
#define JIRAANALYSISDUMPOPER_H

/*!
*@file     �����б�����㷨
*@brief    
*@author   sunjj
*@date     2017��7��20��
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include <QString>
#include "Common/JiraConsts.h"

class JiraAnalysisDumpListOper
{
public:
    JiraAnalysisDumpListOper();
    ~JiraAnalysisDumpListOper();

    void analysisStack(const QString& sDumpJson);
    
    QString stackUrlId();
    QString dumpUrl();
    QString remarks();
    QString highFrequency();
    QString stackInfo();

    void clearDumpCotainer();
    void refreshDumpCotainer(const QString& sDumpJson);
    AutoScriptContainer *dumpContainer();

private:
    QString m_sCrashStackInfo;
    AutoScriptContainer* m_pScriptContainer;
};
#endif