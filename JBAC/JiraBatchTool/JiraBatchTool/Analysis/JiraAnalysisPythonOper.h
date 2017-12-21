#ifndef JIRACPLUSTOPYTHONOPER_H
#define JIRACPLUSTOPYTHONOPER_H

/*!
*@file     C++与Python交互类
*@brief    
*@author   sunjj
*@date     2017年2月28日
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include <Python.h>  
#include <QString>

#include "Common/JiraConsts.h"
struct JiraDllInfo;

class JiraAnalysisPythonOper
{
public:
    JiraAnalysisPythonOper();
    ~JiraAnalysisPythonOper();
    
    JiraCrashKeyContainer* searchCrashInfo(const QString& sSqlStr);
    void LoginPlatform(LoginInfo* pLogInfo);

    void beforeAnalysis();
    void afterAnalysis();
    void searchIssue(const QString& sIssue);
    void updateIssue(CrashUpdateInfo *pDllInfo);
    QString downloadStack(const QString& sStackId);
    QString analysisIssue();
    QString productVersion();

private:
    PyObject *m_pModule;
    PyObject *m_pAnalysisClass; 
    PyObject *m_pAnalysisOper;  
    PyObject *m_pInitialiseMethod;

    LoginInfo* m_pLoginInfo;
    JiraCrashKeyContainer *m_pSearchResult;
};
#endif