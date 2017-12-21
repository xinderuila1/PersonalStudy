#ifndef JIRACPLUSTOPYTHONOPER_H
#define JIRACPLUSTOPYTHONOPER_H

/*!
*@file     C++��Python������
*@brief    
*@author   sunjj
*@date     2017��2��28��
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
    void analysisIssue();
    void searchIssue(const QString& sIssue);
    void updateIssue(CrashUpdateInfo *pDllInfo);
    QString productVersion();

private:
    PyObject *m_pModule;
    PyObject *m_pAnalysisClass; 
    PyObject *m_pAnalysisOper;  
    PyObject *m_pInitialiseMethod;
    JiraCrashKeyContainer *m_pSearchResult;
};
#endif