#ifndef JIRAANALYSISDUMPOPER_H
#define JIRAANALYSISDUMPOPER_H

/*!
*@file     崩溃列表分析算法
*@brief    
*@author   sunjj
*@date     2017年7月20日
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

    void analysisStack(const QString& sIssue);
    
    QString stackUrlId();
    QString dumpUrl();
    QString remarks();
    QString highFrequency();

    void clearDumpCotainer();
    void refreshDumpCotainer(const QString& sIssue);
    AutoScriptContainer *dumpContainer();

private:
    AutoScriptContainer* m_pScriptContainer;
};
#endif