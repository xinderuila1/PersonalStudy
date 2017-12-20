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

class JiraIntelligentWaringPython;
         
typedef std::map<QString, std::map<qint64, QString>> JiraAnalysisCrashUsers; //未分析用户设备集合 设备ID-崩溃时间-崩溃描述
typedef JiraAnalysisCrashUsers JiraHighCrashUsers;                            //需重点关注的高频次崩溃用户
typedef std::map<QString, std::pair<int, int>> JiraHighCrashCount;            //崩溃次数统计

class JiraIntelligentWaringOper
{
public:
    JiraIntelligentWaringOper();
    ~JiraIntelligentWaringOper();

    void analysisCurDayCrash(bool bClear);

private:
    void beforeAnalysis(bool bClear);
    void afterAnalysis();

    void downloadUnAnalysisCrash();
    void loadUnAnalysisCrash();
    void analysisCurrentCrash();
    void sendEmailToTesters();

private:
    JiraIntelligentWaringPython* m_pIntelligentWaringPython;
    JiraAnalysisCrashUsers* m_pUnAnalysisCrashUsers; //未分析用户
    JiraAnalysisCrashUsers* m_pAnalysisedCrashUsers; //已分析用户
    JiraHighCrashUsers* m_pHighCrashUsers;
    JiraHighCrashCount* m_pHighCrashCount;
    QString m_sUnAnalysisCrash;
};

#endif