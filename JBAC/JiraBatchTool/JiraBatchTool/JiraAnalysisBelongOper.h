#ifndef JIRAANALYSISBELONGOPER_H
#define JIRAANALYSISBELONGOPER_H

/*!
*@file     ����������:�㷨˼·�Ǹ����Զ����ű���Ϣ���ֹ����Ŷ�
*@brief    
*@author   sunjj
*@date     2017��4��17��
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include "JiraConsts.h"

class QXmlStreamReader;
class JiraAnalysisDumpListOper;

class JiraAnalysisBelongOper
{
public:
    JiraAnalysisBelongOper(JiraAnalysisDumpListOper* pAnalysisDumpListOper);
    ~JiraAnalysisBelongOper();

    CrashUpdateInfo* parseModule(const QString& sCrashFile);

private:
    void beforeParse();
    void afterParse();

    void parseCrashInfo();
    void loadCrashBelongInfo();

private:
    QXmlStreamReader *m_pXMLReader;
    CrashUpdateInfo* m_pCrashUpdateInfo;
    CrashBelongContainer *m_pCrashBelongContainer;
    JiraAnalysisDumpListOper* m_pAnalysisDumpListOper;
};
#endif