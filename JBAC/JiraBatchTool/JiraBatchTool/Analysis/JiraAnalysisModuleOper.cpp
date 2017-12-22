#include "JiraAnalysisModuleOper.h"
#include "JiraAnalysisDumpListOper.h"
#include "Common/JiraUserCustomSetting.h"
#include <assert.h>

#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

/*!
*@brief        构造函数 
*@author       sunjj 2017年2月23日
*/
JiraAnalysisModuleOper::JiraAnalysisModuleOper(JiraAnalysisDumpListOper* pAnalysisDumpListOper)
    :m_pCrashInfoList(nullptr), m_pDllContainer(nullptr),
     m_pCrashUpdateInfo(nullptr), m_pAnalysisDumpListOper(pAnalysisDumpListOper)
{
    m_pCrashUpdateInfo = new CrashUpdateInfo;
    m_pCrashInfoList = new QStringList;
    m_pDllContainer = JiraUserCustomSetting::instance()->belongContainerByDll();
}

/*!
*@brief        析构函数 
*@author       sunjj 2017年2月23日
*/
JiraAnalysisModuleOper::~JiraAnalysisModuleOper()
{
    m_pAnalysisDumpListOper = nullptr;

    delete m_pCrashInfoList;
    m_pCrashInfoList = nullptr;

    delete m_pCrashUpdateInfo;
    m_pCrashUpdateInfo = nullptr;
}

/*!
*@brief        解析crash信息 
*@author       sunjj 2017年2月23日
*@return       CrashUpdateInfo*
*/
CrashUpdateInfo* JiraAnalysisModuleOper::parseModule()
{
    beforeParse();
    loadCrashInfo();
    parseCrashInfo();
    afterParse();

    return m_pCrashUpdateInfo;
}

/*!
*@brief        解析前处理 
*@author       sunjj 2017年2月23日
*/
void JiraAnalysisModuleOper::beforeParse()
{
    m_pCrashInfoList->clear();
    m_pCrashUpdateInfo->clear();
}

/*!
*@brief        解析后处理 
*@author       sunjj 2017年2月23日
*/
void JiraAnalysisModuleOper::afterParse()
{

}

/*!
*@brief        解析CrashInfo 
*@author       sunjj 2017年2月23日
*/
void JiraAnalysisModuleOper::parseCrashInfo()
{
    int nLastDllPriority(-1);
    QString sLastCrashDll;
    QRegExp oRegExp("[A-Za-z]+.dll");
    for (auto pCrashIter = m_pCrashInfoList->begin(); pCrashIter != m_pCrashInfoList->end(); ++pCrashIter)
    {
        QString sCrashInfo = *pCrashIter;
        if (oRegExp.indexIn(sCrashInfo, 0) == -1)
            continue;

        QString sDllName = oRegExp.cap(0).remove(".dll").toUpper();
        auto pFindIter = m_pDllContainer->find(sDllName);
        if (pFindIter == m_pDllContainer->end())
            continue;

        int nCurDllPriority = pFindIter->second.oFunInfo.nPriority;
        if (nLastDllPriority < nCurDllPriority)
        {
            nLastDllPriority = nCurDllPriority;
            sLastCrashDll = sDllName;
        }
    }

    auto pFindIter = m_pDllContainer->find(sLastCrashDll);
    if (pFindIter != m_pDllContainer->end())
    {
        JiraDllInfo oDllInfo = pFindIter->second;
        m_pCrashUpdateInfo->sDomain = oDllInfo.oModuleInfo.sDomain;
        m_pCrashUpdateInfo->sModuleID = oDllInfo.oModuleInfo.sModuleID;
        m_pCrashUpdateInfo->sDetailInfo = "";
        m_pCrashUpdateInfo->sAutoTest = "";
    }

    m_pCrashUpdateInfo->sRemarks = m_pAnalysisDumpListOper->remarks();
    m_pCrashUpdateInfo->sDumpUrl = m_pAnalysisDumpListOper->dumpUrl();
    m_pCrashUpdateInfo->sHighFrequency = m_pAnalysisDumpListOper->highFrequency();
    m_pCrashUpdateInfo->sStackInfo = m_pAnalysisDumpListOper->stackInfo();
    m_pCrashUpdateInfo->sCrashCount = m_pAnalysisDumpListOper->crashCount();
}

/*!
*@brief        加载Crash信息 
*@author       sunjj 2017年2月23日
*/
void JiraAnalysisModuleOper::loadCrashInfo()
{
    QString sCrashStackStr = m_pAnalysisDumpListOper->stackInfo();
    QTextStream oTextStream(&sCrashStackStr);
    oTextStream.setCodec("UTF-8");
    while (!oTextStream.atEnd())
    {
        QString sLineInfo = oTextStream.readLine().remove('\n').trimmed();
        m_pCrashInfoList->append(sLineInfo);
    }
}