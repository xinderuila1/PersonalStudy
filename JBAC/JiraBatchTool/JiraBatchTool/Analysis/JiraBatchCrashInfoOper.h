#ifndef JIRABATCHCRASHINFOOPER_H
#define JIRABATCHCRASHINFOOPER_H

/*!
*@file     批量处理CrashInfo操作类
*@brief    
*@author   sunjj
*@date     2017年2月28日
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include <map>
#include <QString>
#include "Common/JiraConsts.h"

class JiraAnalysisPythonOper;
class JiraAnalysisModuleOper;
class JiraAnalysisBelongOper;
class JiraAnalysisVersionOper;
class JiraAnalysisDumpListOper;

class JiraBatchCrashInfoOper
{
public:
    JiraBatchCrashInfoOper();
    ~JiraBatchCrashInfoOper();

    void parseCrashInfo(const QString& sSql);
private:
    void beforeBatch();
    void afterBatch();

    void searchCrashInfo(const QString& sSql);
    void analysisCrashInfo();
    CrashUpdateInfo* parseCrashBelong();

private:
    JiraCrashKeyContainer *m_pCrashKeyMap;
    JiraAnalysisPythonOper *m_pAnalysisPythonOper;
    JiraAnalysisModuleOper *m_pAnalysisModuleOper;
    JiraAnalysisBelongOper *m_pAnalysisBelongOper;
    JiraAnalysisVersionOper* m_pAnalysisVersionOper;
    JiraAnalysisDumpListOper *m_pAnalysisDumpListOper;
};
#endif