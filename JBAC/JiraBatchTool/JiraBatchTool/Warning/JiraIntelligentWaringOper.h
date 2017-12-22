#ifndef JIRAINTELLIGENTWARNINGOPER_H
#define JIRAINTELLIGENTWARNINGOPER_H

/*!
*@file     ����Ԥ������
*@brief    
*@author   sunjj
*@date     2017��12��11��
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include <QString>
#include <map>
#include <vector>
#include "Common/JiraConsts.h"

class JiraIntelligentWaringPython;

class JiraIntelligentWaringOper
{
public:
    JiraIntelligentWaringOper(JiraProductInfo* pProductInfo);
    ~JiraIntelligentWaringOper();

    void analysisCurDayCrash(bool bClear);

private:
    void beforeAnalysis(bool bClear);
    void afterAnalysis();

    void downloadUnAnalysisCrash();
    void loadUnAnalysisCrash();
    void analysisCurrentCrash();
    void sendEmailToTesters();

    void logingGehPlatForm();
    QString htmlFilePath();
private:
    JiraIntelligentWaringPython* m_pIntelligentWaringPython;
    JiraAnalysisCrashUsers* m_pUnAnalysisCrashUsers; //δ�����û�
    JiraAnalysisCrashUsers* m_pAnalysisedCrashUsers; //�ѷ����û�
    JiraHighCrashUsers* m_pHighCrashUsers;
    JiraHighCrashCount* m_pHighCrashCount;
    JiraProductInfo* m_pProductInfo;
    QString m_sUnAnalysisCrash;
};

#endif