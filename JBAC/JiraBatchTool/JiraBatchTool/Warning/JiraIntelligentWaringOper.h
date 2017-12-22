#ifndef JIRAINTELLIGENTWARNINGOPER_H
#define JIRAINTELLIGENTWARNINGOPER_H

/*!
*@file     智能预警操作
*@brief    
*@author   sunjj
*@date     2017年12月11日
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
    JiraAnalysisCrashUsers* m_pUnAnalysisCrashUsers; //未分析用户
    JiraAnalysisCrashUsers* m_pAnalysisedCrashUsers; //已分析用户
    JiraHighCrashUsers* m_pHighCrashUsers;
    JiraHighCrashCount* m_pHighCrashCount;
    JiraProductInfo* m_pProductInfo;
    QString m_sUnAnalysisCrash;
};

#endif