#include "Analysis/JiraBatchCrashInfoOper.h"
#include "Analysis/JiraAnalysisPythonOper.h"
#include "Analysis/JiraAnalysisModuleOper.h"
#include "Analysis/JiraAnalysisBelongOper.h"
#include "Analysis/JiraAnalysisDumpListOper.h"
#include "Analysis/JiraAnalysisVersionOper.h"

#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>

/*!
*@brief        构造函数 
*@author       sunjj 2017年2月28日
*/
JiraBatchCrashInfoOper::JiraBatchCrashInfoOper()
    :m_pCrashKeyMap(nullptr), m_pCPlusToPythonOper(nullptr), m_pAnalysisModuleOper(nullptr),
    m_pAnalysisLogInfo(nullptr), m_pAnalysisBelongOper(nullptr), m_pAnalysisVersionOper(nullptr)
{
    m_pCPlusToPythonOper = new JiraAnalysisPythonOper;
    m_pAnalysisDumpListOper = new JiraAnalysisDumpListOper;
    m_pAnalysisModuleOper = new JiraAnalysisModuleOper(m_pAnalysisDumpListOper);
    m_pAnalysisBelongOper = new JiraAnalysisBelongOper(m_pAnalysisDumpListOper);
    m_pAnalysisVersionOper = new JiraAnalysisVersionOper;
    m_pAnalysisLogInfo = new JiraAnalysisLogInfo;
}

/*!
*@brief        析构函数 
*@author       sunjj 2017年2月28日
*/
JiraBatchCrashInfoOper::~JiraBatchCrashInfoOper()
{
    delete m_pAnalysisLogInfo;
    m_pAnalysisLogInfo = nullptr;

    delete m_pAnalysisVersionOper;
    m_pAnalysisVersionOper = nullptr;

    delete m_pAnalysisBelongOper;
    m_pAnalysisBelongOper = nullptr;

    delete m_pAnalysisModuleOper;
    m_pAnalysisModuleOper = nullptr;

    delete m_pAnalysisDumpListOper;
    m_pAnalysisDumpListOper = nullptr;

    delete m_pCPlusToPythonOper;
    m_pCPlusToPythonOper = nullptr;
}

/*!
*@brief        批量解析CrashInfo 
*@author       sunjj 2017年2月28日
*@param[in]    const QString& sSql
*/
void JiraBatchCrashInfoOper::parseCrashInfo(const QString& sSql)
{
    beforeBatch();
    searchCrashInfo(sSql);
    analysisCrashInfo();
    afterBatch();
}

/*!
*@brief        批量处理前操作 
*@author       sunjj 2017年2月28日
*/
void JiraBatchCrashInfoOper::beforeBatch()
{
    m_pAnalysisLogInfo->clear();

    //清空日志信息
    QSettings oSettings(qApp->applicationDirPath() + "/logInfo/analysisCrashLog.ini", QSettings::IniFormat);
    oSettings.clear();
}

/*!
*@brief        批量处理后操作 
*@author       sunjj 2017年2月28日
*/
void JiraBatchCrashInfoOper::afterBatch()
{
    outputLogInfo();
    m_pCrashKeyMap->clear();
}

/*!
*@brief        查询JiraIssue信息 
*@author       sunjj 2017年2月28日
*@param[in]    const QString& sSql
*/
void JiraBatchCrashInfoOper::searchCrashInfo(const QString& sSql)
{
    m_pCrashKeyMap = m_pCPlusToPythonOper->searchCrashInfo(sSql);
} 

/*!
*@brief        分析单个问题信息 
*@author       sunjj 2017年2月28日
*/
void JiraBatchCrashInfoOper::analysisCrashInfo()
{
    CrashUpdateInfo* pUpdateInfo(nullptr);
    for (auto pIter = m_pCrashKeyMap->begin(); pIter != m_pCrashKeyMap->end(); ++pIter)
    {
        QString sCrashKey = pIter->first;
        m_pCPlusToPythonOper->searchIssue(sCrashKey);
        m_pCPlusToPythonOper->beforeAnalysis();
        m_pCPlusToPythonOper->analysisIssue();
        m_pAnalysisDumpListOper->refreshDumpCotainer(sCrashKey);
        m_pAnalysisDumpListOper->analysisStack(sCrashKey);
        pUpdateInfo = parseCrashBelong(sCrashKey);
        m_pAnalysisDumpListOper->clearDumpCotainer();
        m_pCPlusToPythonOper->updateIssue(pUpdateInfo);
        m_pCPlusToPythonOper->afterAnalysis();
        //logAnalysisInfo(sCrashKey, bSccuess);
    }
}

/*!
*@brief        Crash分析归类：优先自动化脚本，其次dll堆栈信息
*@author       sunjj 2017年4月17日
*@param[in]    const QString& sCrashKey
*/
CrashUpdateInfo* JiraBatchCrashInfoOper::parseCrashBelong(const QString& sCrashKey)
{
    CrashUpdateInfo* pFromScript = m_pAnalysisBelongOper->parseModule(sCrashKey);
    CrashUpdateInfo* pFromDllStack = m_pAnalysisModuleOper->parseModule(sCrashKey);

    //通过脚本分析出是钢筋土建计算的Crash
    //但是通过堆栈信息分析是图形平台，直接打给图形平台
    if ((pFromScript->sModuleID == "18930" || pFromScript->sModuleID == "18931") && (pFromDllStack->sModuleID == "18454"))
    {
        return pFromDllStack;
    }

    QString sProductVersion = m_pCPlusToPythonOper->productVersion();
    if (m_pAnalysisVersionOper->needDivideByVersion(sProductVersion, pFromScript->sModuleID, pFromDllStack->sModuleID))
    {
        CrashUpdateInfo* pVersionInfo = m_pAnalysisVersionOper->parseModule();
        pFromScript->sDomain = pVersionInfo->sDomain;
        pFromScript->sModuleID = pVersionInfo->sModuleID;
        pFromDllStack->sDomain = pVersionInfo->sDomain;
        pFromDllStack->sModuleID = pVersionInfo->sModuleID;
    }

    if (!pFromScript->sDetailInfo.isEmpty())
    {
        return pFromScript;
    }

    if (!pFromDllStack->sDomain.isEmpty())
    {
        return pFromDllStack;
    }
    return pFromScript;
}

/*!
*@brief        记录crash分析日志 
*@author       sunjj 2017年3月1日
*@param[in]    const QString& sCrashKey
*@param[in]    bool bSccuess
*/
void JiraBatchCrashInfoOper::logAnalysisInfo(const QString& sCrashKey, bool bSccuess)
{
    m_pAnalysisLogInfo->insert(std::make_pair(sCrashKey, bSccuess));
}

/*!
*@brief        输出日志信息 
*@author       sunjj 2017年3月1日
*/
void JiraBatchCrashInfoOper::outputLogInfo()
{
    JiraAnalysisLogInfo oSccuessed, oFailed;
    for (auto pIter = m_pAnalysisLogInfo->begin(); pIter != m_pAnalysisLogInfo->end(); ++pIter)
    {
        QString sKey = pIter->first;
        bool bSccuess = pIter->second;
        bSccuess? oSccuessed.insert(std::make_pair(sKey, bSccuess)) : oFailed.insert(std::make_pair(sKey, bSccuess));
    }

    QSettings oSettings(qApp->applicationDirPath() + "/logInfo/analysisCrashLog.ini", QSettings::IniFormat);
    oSettings.clear();

    oSettings.beginGroup("TotalInfo");
    oSettings.setValue("totalCount", m_pAnalysisLogInfo->size());
    oSettings.setValue("sccuessCount", oSccuessed.size());
    oSettings.setValue("failedCount", oFailed.size());
    oSettings.setValue("currentTime", QDateTime::currentDateTime().toString(Qt::ISODate));
    oSettings.endGroup();

    oSettings.beginGroup("SccuessIssue");
    for (auto pIter = oSccuessed.begin(); pIter != oSccuessed.end(); ++pIter)
    {
        QString sKey = pIter->first;
        oSettings.setValue(sKey, true);
    }
    oSettings.endGroup();

    oSettings.beginGroup("FailIssue");
    for (auto pIter = oFailed.begin(); pIter != oFailed.end(); ++pIter)
    {
        QString sKey = pIter->first;
        oSettings.setValue(sKey, false);
    }
    oSettings.endGroup();
}