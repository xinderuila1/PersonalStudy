#ifndef JIRAINTELLIGENTWARNINGPYTHON_H
#define JIRAINTELLIGENTWARNINGPYTHON_H

/*!
*@file     ����Ԥ����python������
*@brief    
*@author   sunjj
*@date     2017��12��11��
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