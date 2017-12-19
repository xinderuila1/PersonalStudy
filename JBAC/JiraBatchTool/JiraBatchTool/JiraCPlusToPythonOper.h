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

#include "JiraConsts.h"
struct JiraDllInfo;

class JiraCPlusToPythonOper
{
public:
    JiraCPlusToPythonOper();
    ~JiraCPlusToPythonOper();
    
    JiraCrashKeyContainer* searchCrashInfo(const QString& sSqlStr);

    bool beforeAnalysis();
    bool afterAnalysis();
    bool searchIssue(const QString& sIssue);
    bool analysisIssue();
    bool downloadStack(const QString& sIssue);
    bool updateIssue(CrashUpdateInfo *pDllInfo);
    QString productVersion();

private:
    PyObject *m_pModule;

    JiraCrashKeyContainer *m_pSearchResult;
};
#endif