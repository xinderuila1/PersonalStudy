#ifndef JIRAINTELLIGENTWARNINGPYTHON_H
#define JIRAINTELLIGENTWARNINGPYTHON_H

/*!
*@file     智能预警与python交互类
*@brief    
*@author   sunjj
*@date     2017年12月11日
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include <Python.h>  
#include <QString>

#include "Common/JiraConsts.h"

class JiraIntelligentWaringPython
{
public:
    JiraIntelligentWaringPython();
    ~JiraIntelligentWaringPython();

    void LoginPlatform(LoginInfo* pLogInfo);
    QString downloadUnAnalysisCrash(JiraProductInfo* pProductInfo);
    void sendEmailToTesters(const QString& sHtmlPath, const QString& sHeader);

    QString versionBasicInfo(JiraProductInfo* pProductInfo, bool bCurday);
    QString versionCrashInfo(JiraProductInfo* pProductInfo, bool bCurday);

private:
    PyObject *m_pModule;
    PyObject *m_pWarningClass; 
    PyObject *m_pWarningOper;  
    PyObject *m_pInitialiseMethod;
};

#endif