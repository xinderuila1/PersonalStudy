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
*@brief        ���캯�� 
*@author       sunjj 2017��4��17��
*/
JiraAnalysisBelongOper::JiraAnalysisBelongOper(JiraAnalysisDumpListOper* pAnalysisDumpListOper)
    : m_pCrashUpdateInfo(nullptr), m_pCrashBelongContainer(nullptr), m_pAnalysisDumpListOper(pAnalysisDumpListOper)
{
    m_pCrashBelongContainer = JiraUserCustomSetting::instance()->belongContainerByScript();
    m_pCrashUpdateInfo = new CrashUpdateInfo;
}

/*!
*@brief        �������� 
*@author       sunjj 2017��4��17��
*/
JiraAnalysisBelongOper::~JiraAnalysisBelongOper()
{
    m_pAnalysisDumpListOper = nullptr;

    delete m_pCrashUpdateInfo;
    m_pCrashUpdateInfo = nullptr;
}

/*!
*@brief        ���������Ŷ� 
*@author       sunjj 2017��4��17��
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
*@brief        ����ǰ���� 
*@author       sunjj 2017��4��17��
*/
void JiraAnalysisBelongOper::beforeParse()
{
    m_pCrashUpdateInfo->clear();
}

/*!
*@brief        �������� 
*@author       sunjj 2017��4��17��
*/
void JiraAnalysisBelongOper::afterParse()
{

}

/*!
*@brief        ����Crash��Ϣ 
*@author       sunjj 2017��4��17��
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