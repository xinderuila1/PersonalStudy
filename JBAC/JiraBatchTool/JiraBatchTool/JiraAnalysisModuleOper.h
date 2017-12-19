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
#include "JiraConsts.h"

class QXmlStreamReader;
class JiraAnalysisDumpListOper;

class JiraAnalysisModuleOper
{
public:
    JiraAnalysisModuleOper(JiraAnalysisDumpListOper* pAnalysisDumpListOper);
    ~JiraAnalysisModuleOper();

    CrashUpdateInfo* parseModule(const QString& sCrashFile);

private:
    void beforeParse();
    void afterParse();

    void parseCrashInfo();
    void loadCrashInfo(const QString& sCrashFile);

    void loadDllInfo();
    void loadModuleInfo();
    void loadFuncInfo(JiraModuleInfo& oModuleInfo);
    void loadDetailInfo(JiraDllInfo& oDllInfo);

private:
    QStringList *m_pCrashInfoList;
    QXmlStreamReader *m_pXMLReader;
    JiraDllContainer *m_pDllContainer;
    CrashUpdateInfo *m_pCrashUpdateInfo;
    JiraAnalysisDumpListOper* m_pAnalysisDumpListOper;
};
#endif