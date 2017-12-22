#include "JiraAnalysisBelongOper.h"
#include "JiraAnalysisDumpListOper.h"
#include "Common/JiraUserCustomSetting.h"
#include <assert.h>

#include <QXmlStreamReader>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QRegExp>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

/*!
*@brief        构造函数 
*@author       sunjj 2017年4月17日
*/
JiraAnalysisBelongOper::JiraAnalysisBelongOper(JiraAnalysisDumpListOper* pAnalysisDumpListOper)
    : m_pCrashUpdateInfo(nullptr), m_pCrashBelongContainer(nullptr), m_pAnalysisDumpListOper(pAnalysisDumpListOper)
{
    m_pCrashBelongContainer = JiraUserCustomSetting::instance()->belongContainerByScript();
    m_pCrashUpdateInfo = new CrashUpdateInfo;
}

/*!
*@brief        析构函数 
*@author       sunjj 2017年4月17日
*/
JiraAnalysisBelongOper::~JiraAnalysisBelongOper()
{
    m_pAnalysisDumpListOper = nullptr;

    delete m_pCrashUpdateInfo;
    m_pCrashUpdateInfo = nullptr;
}

/*!
*@brief        解析归属团队 
*@author       sunjj 2017年4月17日
*@return       CrashUpdateInfo*
*/
CrashUpdateInfo* JiraAnalysisBelongOper::parseModule()
{
    beforeParse();
    parseCrashInfo();
    afterParse();

    return m_pCrashUpdateInfo;
}

/*!
*@brief        解析前处理 
*@author       sunjj 2017年4月17日
*/
void JiraAnalysisBelongOper::beforeParse()
{
    m_pCrashUpdateInfo->clear();
}

/*!
*@brief        解析后处理 
*@author       sunjj 2017年4月17日
*/
void JiraAnalysisBelongOper::afterParse()
{

}

/*!
*@brief        解析Crash信息 
*@author       sunjj 2017年4月17日
*/
void JiraAnalysisBelongOper::parseCrashInfo()
{
    QRegExp oRegExp("Test[A-Za-z]+_");
    AutoScriptContainer* pAutoScriptContainer = m_pAnalysisDumpListOper->dumpContainer();
    for (auto pAutoScriptIter = pAutoScriptContainer->begin(); pAutoScriptIter != pAutoScriptContainer->end(); ++pAutoScriptIter)
    {
        AutoScriptInfo oScriptInfo = *pAutoScriptIter;
        if (!oScriptInfo.bIsAutoScript)
            continue;
        
        if (oRegExp.indexIn(oScriptInfo.sDetailInfo, 0) == -1)
            continue;

        QString sAutoIdentification = oRegExp.cap(0).remove("Test").remove("_");
        auto pFindIter = m_pCrashBelongContainer->find(sAutoIdentification);
        if (pFindIter != m_pCrashBelongContainer->end())
        {
            CrashBelongInfo& oBelongInfo = pFindIter->second;
            m_pCrashUpdateInfo->sDomain = oBelongInfo.sDomain;
            m_pCrashUpdateInfo->sModuleID = oBelongInfo.sModuleID;
            m_pCrashUpdateInfo->sDetailInfo = oScriptInfo.sDetailInfo;
            m_pCrashUpdateInfo->sAutoTest = "11156";
            break;
        }
    }
    m_pCrashUpdateInfo->sDumpUrl = m_pAnalysisDumpListOper->dumpUrl();
    m_pCrashUpdateInfo->sRemarks = m_pAnalysisDumpListOper->remarks();
    m_pCrashUpdateInfo->sHighFrequency = m_pAnalysisDumpListOper->highFrequency();
    m_pCrashUpdateInfo->sStackInfo = m_pAnalysisDumpListOper->stackInfo();
    m_pCrashUpdateInfo->sCrashCount = m_pAnalysisDumpListOper->crashCount();
}