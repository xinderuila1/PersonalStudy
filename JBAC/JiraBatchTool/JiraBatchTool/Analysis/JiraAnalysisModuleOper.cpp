#include "JiraAnalysisModuleOper.h"
#include "JiraAnalysisDumpListOper.h"
#include "Common/JiraUserCustomSetting.h"
#include <assert.h>

#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

/*!
*@brief        ���캯�� 
*@author       sunjj 2017��2��23��
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
*@brief        �������� 
*@author       sunjj 2017��2��23��
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
*@brief        ����crash��Ϣ 
*@author       sunjj 2017��2��23��
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
*@brief        ����ǰ���� 
*@author       sunjj 2017��2��23��
*/
void JiraAnalysisModuleOper::beforeParse()
{
    m_pCrashInfoList->clear();
    m_pCrashUpdateInfo->clear();
}

/*!
*@brief        �������� 
*@author       sunjj 2017��2��23��
*/
void JiraAnalysisModuleOper::afterParse()
{

}

/*!
*@brief        ����CrashInfo 
*@author       sunjj 2017��2��23��
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
*@brief        ����Crash��Ϣ 
*@author       sunjj 2017��2��23��
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