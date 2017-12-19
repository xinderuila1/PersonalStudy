#include "JiraAnalysisVersionOper.h"
#include <assert.h>

#include <QXmlStreamReader>
#include <QFile>
#include <QRegExp>
#include <QTextStream>
#include <QCoreApplication>

/*!
*@brief        ���캯�� 
*@author       sunjj 2017��8��1��
*/
JiraAnalysisVersionOper::JiraAnalysisVersionOper()
    :m_pCrashUpdateInfo(nullptr), m_pVersionContainer(nullptr), m_pXMLReader(nullptr), m_pUserModuleContainer(nullptr)
{
    m_pXMLReader = new QXmlStreamReader;
    m_pCrashUpdateInfo = new CrashUpdateInfo;
    m_pVersionContainer = new JiraVersionContainer;
    m_pUserModuleContainer = new JiraUserModuleContainer;
    loadModuleUser();
}

/*!
*@brief        �������� 
*@author       sunjj 2017��8��1��
*/
JiraAnalysisVersionOper::~JiraAnalysisVersionOper()
{
    delete m_pUserModuleContainer;
    m_pUserModuleContainer = nullptr;

    delete m_pVersionContainer;
    m_pVersionContainer = nullptr;

    delete m_pCrashUpdateInfo;
    m_pCrashUpdateInfo = nullptr;

    delete m_pXMLReader;
    m_pXMLReader = nullptr;
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

/*!
*@brief        ����ģ���û� 
*@author       sunjj 2017��8��16��
*/
void JiraAnalysisVersionOper::loadModuleUser()
{
    QString sDllInfo = qApp->applicationDirPath()  + "/YYCustomVersion.xml";

    QFile oFile(sDllInfo);
    if (!oFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        assert(false);
        return;
    }

    m_pXMLReader->setDevice(&oFile);
    m_pXMLReader->readNext();

    while (!m_pXMLReader->atEnd())
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strModule)
        {
            loadModuleInfo();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strProduct)
        {
            loadVersionInfo();
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        ����ģ����Ϣ 
*@author       sunjj 2017��8��1��
*/
void JiraAnalysisVersionOper::loadModuleInfo()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strModule))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strUser)
        {
            JiraModuleInfo oModuleInfo;
            oModuleInfo.sModuleName = m_pXMLReader->attributes().value(strName).toString();
            oModuleInfo.sAuthor = m_pXMLReader->attributes().value(strAuthor).toString();
            oModuleInfo.sDomain = m_pXMLReader->attributes().value(strDomain).toString();
            oModuleInfo.sModuleID = m_pXMLReader->attributes().value(strModuleID).toString();
            m_pUserModuleContainer->insert(std::make_pair(oModuleInfo.sModuleID, oModuleInfo));
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        ���ذ汾��Ϣ 
*@author       sunjj 2017��8��16��
*/
void JiraAnalysisVersionOper::loadVersionInfo()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strProduct))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strVersion)
        {
            QString sProductVersion = m_pXMLReader->readElementText();
            m_pVersionContainer->push_back(sProductVersion);
        }
        m_pXMLReader->readNext();
    }
}
