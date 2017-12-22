#include "Analysis/JiraBatchCrashInfoOper.h"
#include "Analysis/JiraAnalysisPythonOper.h"
#include "Analysis/JiraAnalysisModuleOper.h"
#include "Analysis/JiraAnalysisBelongOper.h"
#include "Analysis/JiraAnalysisDumpListOper.h"
#include "Analysis/JiraAnalysisVersionOper.h"
#include "Common/JiraUserCustomSetting.h"

/*!
*@brief        ���캯�� 
*@author       sunjj 2017��2��28��
*/
JiraBatchCrashInfoOper::JiraBatchCrashInfoOper()
    :m_pCrashKeyMap(nullptr), m_pAnalysisPythonOper(nullptr), m_pAnalysisModuleOper(nullptr),
    m_pAnalysisBelongOper(nullptr), m_pAnalysisVersionOper(nullptr)
{
    m_pAnalysisPythonOper = new JiraAnalysisPythonOper;
    m_pAnalysisDumpListOper = new JiraAnalysisDumpListOper;
    m_pAnalysisModuleOper = new JiraAnalysisModuleOper(m_pAnalysisDumpListOper);
    m_pAnalysisBelongOper = new JiraAnalysisBelongOper(m_pAnalysisDumpListOper);
    m_pAnalysisVersionOper = new JiraAnalysisVersionOper;
}

/*!
*@brief        �������� 
*@author       sunjj 2017��2��28��
*/
JiraBatchCrashInfoOper::~JiraBatchCrashInfoOper()
{
    delete m_pAnalysisVersionOper;
    m_pAnalysisVersionOper = nullptr;

    delete m_pAnalysisBelongOper;
    m_pAnalysisBelongOper = nullptr;

    delete m_pAnalysisModuleOper;
    m_pAnalysisModuleOper = nullptr;

    delete m_pAnalysisDumpListOper;
    m_pAnalysisDumpListOper = nullptr;

    delete m_pAnalysisPythonOper;
    m_pAnalysisPythonOper = nullptr;
}

/*!
*@brief        ��������CrashInfo 
*@author       sunjj 2017��2��28��
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
*@brief        ��������ǰ���� 
*@author       sunjj 2017��2��28��
*/
void JiraBatchCrashInfoOper::beforeBatch()
{
    std::shared_ptr<ProductInfo> oProductInfo = JiraUserCustomSetting::instance()->productInfo();
    std::shared_ptr<JiraInfo> oJiraInfo = JiraUserCustomSetting::instance()->jiraInfo();

    LoginInfo oLoginInfo;
    oLoginInfo.sJiarUrl = oJiraInfo->sJiraUrl;
    oLoginInfo.sUserName = oJiraInfo->sUserName;
    oLoginInfo.sPassword = oJiraInfo->sPassword;
    oLoginInfo.sProductCode = oProductInfo->sProductKey;
    m_pAnalysisPythonOper->LoginPlatform(&oLoginInfo);
}

/*!
*@brief        ������������ 
*@author       sunjj 2017��2��28��
*/
void JiraBatchCrashInfoOper::afterBatch()
{
    m_pCrashKeyMap->clear();
}

/*!
*@brief        ��ѯJiraIssue��Ϣ 
*@author       sunjj 2017��2��28��
*@param[in]    const QString& sSql
*/
void JiraBatchCrashInfoOper::searchCrashInfo(const QString& sSql)
{
    m_pCrashKeyMap = m_pAnalysisPythonOper->searchCrashInfo(sSql);
} 

/*!
*@brief        ��������������Ϣ 
*@author       sunjj 2017��2��28��
*/
void JiraBatchCrashInfoOper::analysisCrashInfo()
{
    QString sDumpJson = "";    //�����б�
    QString sCrashStack = "";  //��ջ��Ϣ
    for (auto pIter = m_pCrashKeyMap->begin(); pIter != m_pCrashKeyMap->end(); ++pIter)
    {
        QString sCrashKey = pIter->first;
        m_pAnalysisPythonOper->searchIssue(sCrashKey);
        m_pAnalysisPythonOper->beforeAnalysis();
        sDumpJson = m_pAnalysisPythonOper->analysisIssue();
        m_pAnalysisDumpListOper->refreshDumpCotainer(sDumpJson);
        sCrashStack = m_pAnalysisPythonOper->downloadStack(m_pAnalysisDumpListOper->stackUrlId());
        m_pAnalysisDumpListOper->analysisStack(sCrashStack);
        m_pAnalysisPythonOper->updateIssue(parseCrashBelong());
        m_pAnalysisDumpListOper->clearDumpCotainer();
        m_pAnalysisPythonOper->afterAnalysis();
    }
}

/*!
*@brief        Crash�������ࣺ�����Զ����ű������dll��ջ��Ϣ
*@author       sunjj 2017��4��17��
*/
CrashUpdateInfo* JiraBatchCrashInfoOper::parseCrashBelong()
{
    CrashUpdateInfo* pFromScript = m_pAnalysisBelongOper->parseModule();
    CrashUpdateInfo* pFromDllStack = m_pAnalysisModuleOper->parseModule();

    //ͨ���ű��������Ǹֽ����������Crash
    //����ͨ����ջ��Ϣ������ͼ��ƽ̨��ֱ�Ӵ��ͼ��ƽ̨
    if ((pFromScript->sModuleID == "18930" || pFromScript->sModuleID == "18931") && (pFromDllStack->sModuleID == "18454"))
    {
        return pFromDllStack;
    }

    QString sProductVersion = m_pAnalysisPythonOper->productVersion();
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