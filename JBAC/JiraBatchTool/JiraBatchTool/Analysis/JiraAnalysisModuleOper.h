#ifndef JIRAANALYSISMODULEOPER_H
#define JIRAANALYSISMODULEOPER_H

/*!
*@file     ����������:�㷨˼·�Ǹ���dll��ջ��Ϣ���ֹ����Ŷ�
*@brief    
*@author   sunjj
*@date     2017��2��23��
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/

#include <QString>
#include <QStringList>
#include "Common/JiraConsts.h"

class JiraAnalysisDumpListOper;

class JiraAnalysisModuleOper
{
public:
    JiraAnalysisModuleOper(JiraAnalysisDumpListOper* pAnalysisDumpListOper);
    ~JiraAnalysisModuleOper();

    CrashUpdateInfo* parseModule();

private:
    void beforeParse();
    void afterParse();

    void parseCrashInfo();
    void loadCrashInfo();
private:
    QStringList *m_pCrashInfoList;
    CrashUpdateInfo *m_pCrashUpdateInfo;
    JiraAnalysisDumpListOper* m_pAnalysisDumpListOper;
    std::shared_ptr<JiraDllContainer> m_pDllContainer;
};
#endif