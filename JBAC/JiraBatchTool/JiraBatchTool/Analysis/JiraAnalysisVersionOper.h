#ifndef JIRAANALYSISVERSIONOPER_H
#define JIRAANALYSISVERSIONOPER_H

/*!
*@file     ���ݰ汾���ֹ����Ŷ�
*@brief    
*@author   sunjj
*@date     2017��8��1��
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/

#include "Common/JiraConsts.h"

class QXmlStreamReader;

class JiraAnalysisVersionOper
{
public:
    JiraAnalysisVersionOper();
    ~JiraAnalysisVersionOper();

    bool needDivideByVersion(const QString& sVersion, const QString& sScriptModuleID, const QString& sDllModuleID);
    CrashUpdateInfo* parseModule();

private:
    void beforeParse();
    void afterParse();

    void parseVersion(const QString& sVersion, const QString& sScriptModuleID, const QString& sDllModuleID);
    void loadModuleInfo();

    void loadModuleUser();
    void loadVersionInfo();

private:
    QXmlStreamReader *m_pXMLReader;
    JiraVersionContainer* m_pVersionContainer;
    CrashUpdateInfo *m_pCrashUpdateInfo;
    JiraUserModuleContainer* m_pUserModuleContainer;
};
#endif