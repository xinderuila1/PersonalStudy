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
private:
    
    CrashUpdateInfo *m_pCrashUpdateInfo;
    std::shared_ptr<JiraVersionContainer> m_pVersionContainer;
    std::shared_ptr<JiraUserModuleContainer> m_pUserModuleContainer;
};
#endif