#include "JiraAnalysisVersionOper.h"
#include "Common/JiraUserCustomSetting.h"
#include <assert.h>

#include <QFile>
#include <QRegExp>
#include <QTextStream>
#include <QCoreApplication>

/*!
*@brief        ���캯�� 
*@author       sunjj 2017��8��1��
*/
JiraAnalysisVersionOper::JiraAnalysisVersionOper()
    :m_pCrashUpdateInfo(nullptr), m_pVersionContainer(nullptr), m_pUserModuleContainer(nullptr)
{
    m_pCrashUpdateInfo = new CrashUpdateInfo;
    m_pVersionContainer = JiraUserCustomSetting::instance()->yyVersionContainer();
    m_pUserModuleContainer = JiraUserCustomSetting::instance()->yyUserModuleContainer();
}

/*!
*@brief        �������� 
*@author       sunjj 2017��8��1��
*/
JiraAnalysisVersionOper::~JiraAnalysisVersionOper()
{
    delete m_pCrashUpdateInfo;
    m_pCrashUpdateInfo = nullptr;
}

/*!
*@brief        �Ƿ����ݰ汾���� 
*@author       sunjj 2017��8��1��
*@param[in]    const QString& sVersion
*@return       bool
*/
bool JiraAnalysisVersionOper::needDivideByVersion(const QString& sVersion, const QString& sScriptModuleID, const QString& sDllModuleID)
{
    beforeParse();
    parseVersion(sVersion, sScriptModuleID, sDllModuleID);
    afterParse();
    return !m_pCrashUpdateInfo->sDomain.isEmpty();
}


/*!
*@brief        ��������
*@author       sunjj 2017��8��1��
*@return       CrashUpdateInfo*
*/
CrashUpdateInfo* JiraAnalysisVersionOper::parseModule()
{
    return m_pCrashUpdateInfo;
}

/*!
*@brief        ����ǰ���� 
*@author       sunjj 2017��8��1��
*/
void JiraAnalysisVersionOper::beforeParse()
{
    m_pCrashUpdateInfo->clear();
}

/*!
*@brief        �������� 
*@author       sunjj 2017��8��1��
*/
void JiraAnalysisVersionOper::afterParse()
{
    //m_pCrashUpdateInfo->clear();
}

/*!
*@brief        ���������汾 
*@author       sunjj 2017��8��1��
*/
void JiraAnalysisVersionOper::parseVersion(const QString& sVersion, const QString& sScriptModuleID, const QString& sDllModuleID)
{
    if (sVersion.trimmed().isEmpty())
        return;

    bool bMatch = false;
    for (auto pIter = m_pVersionContainer->begin(); pIter != m_pVersionContainer->end(); ++pIter)
    {
        QRegExp oRegx(*pIter);
        bMatch = oRegx.exactMatch(sVersion);
        if (bMatch)
        {
            break;
        }
    }

    if (bMatch)
    {
        if (!sScriptModuleID.isEmpty())
        {
            auto pIter = m_pUserModuleContainer->find(sScriptModuleID);
            if (pIter != m_pUserModuleContainer->end())
            {
                JiraModuleInfo& oModuleInfo = pIter->second;
                m_pCrashUpdateInfo->sDomain = oModuleInfo.sDomain;
                m_pCrashUpdateInfo->sModuleID = sScriptModuleID;
            }
        }
        else if (!sDllModuleID.isEmpty())
        {
            auto pIter = m_pUserModuleContainer->find(sDllModuleID);
            if (pIter != m_pUserModuleContainer->end())
            {
                JiraModuleInfo& oModuleInfo = pIter->second;
                m_pCrashUpdateInfo->sDomain = oModuleInfo.sDomain;
                m_pCrashUpdateInfo->sModuleID = sDllModuleID;
            }
        }
        else
        {
            m_pCrashUpdateInfo->sDomain = "sunjj";
            m_pCrashUpdateInfo->sModuleID = "11125";
        }
    }
}