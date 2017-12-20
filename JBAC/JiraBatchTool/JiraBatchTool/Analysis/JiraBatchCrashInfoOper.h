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

class QTime;
class JiraCPlusToPythonOper;
class JiraAnalysisModuleOper;
class JiraAnalysisBelongOper;
class JiraAnalysisVersionOper;
class JiraAnalysisDumpListOper;

class JiraBatchCrashInfoOper
{
public:
    JiraBatchCrashInfoOper();
    ~JiraBatchCrashInfoOper();

    void parseCrashInfo(const QString& sSql, bool bShowMsg = true);
private:
    void beforeBatch();
    void afterBatch();

    void searchCrashInfo(const QString& sSql);
    void analysisCrashInfo();
    void removeCrashFile(const QString& sCrashKey);
    void logAnalysisInfo(const QString& sCrashKey, bool bSccuess);
    void outputLogInfo();
    CrashUpdateInfo* parseCrashBelong(const QString& sCrashKey);

    void startTime();
    void endTime();

    void showMessage(bool bShowMsg);

private:
    JiraCrashKeyContainer *m_pCrashKeyMap;
    JiraCPlusToPythonOper *m_pCPlusToPythonOper;
    JiraAnalysisModuleOper *m_pAnalysisModuleOper;
    JiraAnalysisBelongOper *m_pAnalysisBelongOper;
    JiraAnalysisVersionOper* m_pAnalysisVersionOper;
    JiraAnalysisDumpListOper *m_pAnalysisDumpListOper;
    JiraAnalysisLogInfo *m_pAnalysisLogInfo;

    QTime *m_pTimer;
    QString m_sAnalysisTime;
};
#endif