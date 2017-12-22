#include "Common/JiraUserCustomSetting.h"

#include <QXmlStreamReader>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

#include <assert.h>

/*!
*@brief        单例模式
*@author       sunjj 2017年12月22日
*@return       JiraUserCustomSetting*
*/
JiraUserCustomSetting* JiraUserCustomSetting::instance()
{
    static JiraUserCustomSetting oUserCustomSetting;
    return &oUserCustomSetting;
}


/*!
*@brief        构造函数 
*@author       sunjj 2017年12月22日
*/
JiraUserCustomSetting::JiraUserCustomSetting()
    :m_pXMLReader(nullptr)
{
    m_pModuleContainer = std::make_shared<JiraUserModuleContainer>();

    m_pDefaultUser = std::make_shared<DefaultUser>();
    m_pProductInfo = std::make_shared<ProductInfo>();
    m_pJiraInfo = std::make_shared<JiraInfo>();
    m_pGEHInfo = std::make_shared<GEHInfo>();
    m_pEmailContent = std::make_shared<EmailContent>();
    m_pAutoBelong = std::make_shared<AutoBelong>();
    m_pAutoWarning = std::make_shared<AutoWarning>();
    m_pBelongContainerByScript = std::make_shared<CrashBelongContainer>();
    m_pBelongContainerByDll = std::make_shared<JiraDllContainer>();
    m_pYYVersionContainer = std::make_shared<JiraVersionContainer>();
    m_pYYUserModuleContainer = std::make_shared<JiraUserModuleContainer>();
    m_pWarningVersionContainer = std::make_shared<JiraWarningVersionContainer>();

    m_pXMLReader = new QXmlStreamReader;
    initUserCustomSetting();
}

/*!
*@brief        析构函数 
*@author       sunjj 2017年12月22日
*/
JiraUserCustomSetting::~JiraUserCustomSetting()
{
    delete m_pXMLReader;
    m_pXMLReader = nullptr;
}

/*!
*@brief        初始用户自定义设置 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::initUserCustomSetting()
{
    QString sUserSetting = qApp->applicationDirPath()  + "/JBAC_User_Setting.xml";

    QFile oFile(sUserSetting);
    if (!oFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        assert(false);
        return;
    }

    m_pXMLReader->setDevice(&oFile);
    m_pXMLReader->readNext();

    while (!m_pXMLReader->atEnd())
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strProductInfo)
        {
            loadProductInfo();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strJiraInfo)
        {
            loadJiraInfo();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strGEHInfo)
        {
            loadGehInfo();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strAutoBelong)
        {
            loadAutoBelongInfo();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strAutoWarning)
        {
            loadAutoWarningInfo();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strBelongInfo)
        {
            loadBelongRuleInfo();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strDefaultBelong)
        {
            loadDefaultUserInfo();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strCrashBelongRule)
        {
            loadCrashBelongRule();
        }

        m_pXMLReader->readNext();
    }
}

/*!
*@brief        加载产品信息 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::loadProductInfo()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strProductInfo))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strProductKey)
        {
            m_pProductInfo->sProductKey = m_pXMLReader->readElementText();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strProductName)
        {
            m_pProductInfo->sProductName = m_pXMLReader->readElementText();
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        加载Jira信息 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::loadJiraInfo()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strJiraInfo))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strUserName)
        {
            m_pJiraInfo->sUserName = m_pXMLReader->readElementText();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strUserPassword)
        {
            m_pJiraInfo->sPassword = m_pXMLReader->readElementText();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strJiraUrl)
        {
            m_pJiraInfo->sJiraUrl = m_pXMLReader->readElementText();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strFilterSql)
        {
            m_pJiraInfo->sFilterSql = m_pXMLReader->attributes().value(strSql).toString();
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        加载GEH信息 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::loadGehInfo()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strGEHInfo))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strUserName)
        {
            m_pGEHInfo->sUserName = m_pXMLReader->readElementText();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strUserPassword)
        {
            m_pGEHInfo->sPassword = m_pXMLReader->readElementText();
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        加载自动归属信息 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::loadAutoBelongInfo()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strAutoBelong))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strTimeInterval)
        {
            m_pAutoBelong->nTimeInterval = m_pXMLReader->readElementText().toInt();
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        加载自动预警信息 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::loadAutoWarningInfo()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strAutoWarning))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strTimeInterval)
        {
            m_pAutoWarning->nTimeInterval = m_pXMLReader->readElementText().toInt();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strHighCrashMinValue)
        {
            m_pAutoWarning->nHighCrashMinValue = m_pXMLReader->readElementText().toInt();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strHeader)
        {
            m_pAutoWarning->sHeader = m_pXMLReader->readElementText();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strFSR)
        {
            m_pAutoWarning->sFSR = m_pXMLReader->readElementText();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strCSR)
        {
            m_pAutoWarning->sCSR = m_pXMLReader->readElementText();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strSJR)
        {
            m_pAutoWarning->sSJR = m_pXMLReader->readElementText();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strNeedWarningVersion)
        {
            loadWarningVersionInfo();
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        划分原则信息 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::loadBelongRuleInfo()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strBelongInfo))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strModule)
        {
            JiraModuleInfo oModuleInfo;
            oModuleInfo.sModuleName = m_pXMLReader->attributes().value(strIdentification).toString();
            oModuleInfo.sModuleID = m_pXMLReader->attributes().value(strModuleID).toString();
            m_pModuleContainer->insert(std::make_pair(oModuleInfo.sModuleName, oModuleInfo));
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        默认分配到用户 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::loadDefaultUserInfo()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strDefaultBelong))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strModule)
        {
            m_pDefaultUser->sUser = m_pXMLReader->attributes().value(strAuthor).toString();
            m_pDefaultUser->sDomain = m_pXMLReader->attributes().value(strDomain).toString();
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        Crash划分原则 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::loadCrashBelongRule()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strCrashBelongRule))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strByAutoScript)
        {
           loadByAutoScriptInfo();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strByStackDll)
        {
           loadByStackDllInfo();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strYYProduct)
        {
            loadYYProductInfo();
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        按照脚本划分 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::loadByAutoScriptInfo()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strByAutoScript))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strModule)
        {
            CrashBelongInfo oBelongInfo;
            oBelongInfo.sIdentification = m_pXMLReader->attributes().value(strIdentification).toString();
            oBelongInfo.sModuleName = m_pXMLReader->attributes().value(strName).toString();
            oBelongInfo.sAuthor = m_pXMLReader->attributes().value(strAuthor).toString();
            oBelongInfo.sDomain = m_pXMLReader->attributes().value(strDomain).toString();
            oBelongInfo.sModuleID = moduleId(oBelongInfo.sIdentification); 
            m_pBelongContainerByScript->insert(std::make_pair(oBelongInfo.sIdentification, oBelongInfo));
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        查找模块ID 
*@author       sunjj 2017年12月22日
*@param[in]    const QString& sKey
*@return       QString
*/
QString JiraUserCustomSetting::moduleId(const QString& sKey)
{
    auto pIter = m_pModuleContainer->find(sKey);
    if (pIter != m_pModuleContainer->end())
    {
        return pIter->second.sModuleID;
    }

    return "";
}

/*!
*@brief        dll归属团队 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::loadByStackDllInfo()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strByStackDll))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strModule)
        {
            JiraModuleInfo oModuleInfo;
            oModuleInfo.sModuleName = m_pXMLReader->attributes().value(strIdentification).toString();
            oModuleInfo.sAuthor = m_pXMLReader->attributes().value(strAuthor).toString();
            oModuleInfo.sDomain = m_pXMLReader->attributes().value(strDomain).toString();
            oModuleInfo.sModuleID = moduleId(oModuleInfo.sModuleName);

            loadDllFuncInfo(oModuleInfo);
       }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        dll划分归属团队小模块 
*@author       sunjj 2017年12月22日
*@param[out]   JiraModuleInfo& oModuleInfo
*/
void JiraUserCustomSetting::loadDllFuncInfo(JiraModuleInfo& oModuleInfo)
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strModule))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strFunction)
        {
            JiraFunInfo oFunInfo;
            oFunInfo.sFunName = m_pXMLReader->attributes().value(strName).toString();
            oFunInfo.sAuthor = m_pXMLReader->attributes().value(strAuthor).toString();
            oFunInfo.sDomain = m_pXMLReader->attributes().value(strDomain).toString();
            oFunInfo.nPriority = m_pXMLReader->attributes().value(strPriority).toInt();

            JiraDllInfo oDllInfo;
            oDllInfo.oModuleInfo = oModuleInfo;
            oDllInfo.oFunInfo = oFunInfo;

            loadDllDetailInfo(oDllInfo);
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        dll划分具体模块
*@author       sunjj 2017年12月22日
*@param[out]   JiraDllInfo& oDllInfo
*/
void JiraUserCustomSetting::loadDllDetailInfo(JiraDllInfo& oDllInfo)
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strFunction))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strDll)
        {
            QString sDllName = m_pXMLReader->readElementText();
            oDllInfo.sDllName = sDllName.toUpper();
            m_pBelongContainerByDll->insert(std::make_pair(oDllInfo.sDllName, oDllInfo));
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        加载运营配置信息 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::loadYYProductInfo()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strYYProduct))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strModule)
        {
            loadYYModuleInfo();
        }
        else if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strProduct)
        {
            loadYYVersionInfo();
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        运营版本归属团队 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::loadYYModuleInfo()
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
            m_pYYUserModuleContainer->insert(std::make_pair(oModuleInfo.sModuleID, oModuleInfo));
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        运营版本信息 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::loadYYVersionInfo()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strProduct))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strVersion)
        {
            QString sProductVersion = m_pXMLReader->readElementText();
            m_pYYVersionContainer->push_back(sProductVersion);
        }
        m_pXMLReader->readNext();
    }
}

/*!
*@brief        需要监控版本信息 
*@author       sunjj 2017年12月22日
*/
void JiraUserCustomSetting::loadWarningVersionInfo()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == strNeedWarningVersion))
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == strVersion)
        {
            YYNeedWarningVersion oWarningVersion;
            oWarningVersion.sVersionName = m_pXMLReader->attributes().value(strName).toString();
            oWarningVersion.sVersionId = m_pXMLReader->attributes().value(strVersionId).toString();
            m_pWarningVersionContainer->push_back(oWarningVersion);
        }
        m_pXMLReader->readNext();
    }
}


/*!
*@brief        根据脚本划分归属团队容器 
*@author       sunjj 2017年12月22日
*@return       std::shared_ptr<CrashBelongContainer>
*/
std::shared_ptr<CrashBelongContainer> JiraUserCustomSetting::belongContainerByScript()
{
    return m_pBelongContainerByScript;
}

/*!
*@brief        根据堆栈划分归属团队容器 
*@author       sunjj 2017年12月22日
*@return       std::shared_ptr<JiraDllContainer>
*/
std::shared_ptr<JiraDllContainer> JiraUserCustomSetting::belongContainerByDll()
{
    return m_pBelongContainerByDll;
}

/*!
*@brief        运营版本标识 
*@author       sunjj 2017年12月22日
*@return       std::shared_ptr<JiraVersionContainer>
*/
std::shared_ptr<JiraVersionContainer> JiraUserCustomSetting::yyVersionContainer()
{
    return m_pYYVersionContainer;
}

/*!
*@brief        运营版本Crash划分规则容器 
*@author       sunjj 2017年12月22日
*@return       std::shared_ptr<JiraUserModuleContainer>
*/
std::shared_ptr<JiraUserModuleContainer> JiraUserCustomSetting::yyUserModuleContainer()
{
    return m_pYYUserModuleContainer;
}

/*!
*@brief        Jira信息 
*@author       sunjj 2017年12月22日
*@return       std::shared_ptr<JiraInfo>
*/
std::shared_ptr<JiraInfo> JiraUserCustomSetting::jiraInfo()
{
    return m_pJiraInfo;
}

/*!
*@brief        geh信息 
*@author       sunjj 2017年12月22日
*@return       std::shared_ptr<GEHInfo>
*/
std::shared_ptr<GEHInfo> JiraUserCustomSetting::gehInfo()
{
    return m_pGEHInfo;
}

/*!
*@brief        产品信息 
*@author       sunjj 2017年12月22日
*@return       std::shared_ptr<ProductInfo>
*/
std::shared_ptr<ProductInfo> JiraUserCustomSetting::productInfo()
{
    return m_pProductInfo;
}

/*!
*@brief        自动预警设置 
*@author       sunjj 2017年12月22日
*@return       std::shared_ptr<AutoWarning>
*/
std::shared_ptr<AutoWarning> JiraUserCustomSetting::autoWarning()
{
    return m_pAutoWarning;
}

/*!
*@brief        需要监控的运营版本 
*@author       sunjj 2017年12月22日
*@return       std::shared_ptr<JiraWarningVersionContainer>
*/
std::shared_ptr<JiraWarningVersionContainer> JiraUserCustomSetting::warningVersionContainer()
{
    return m_pWarningVersionContainer;
}

/*!
*@brief        自动归类 
*@author       sunjj 2017年12月22日
*@return       std::shared_ptr<AutoBelong>
*/
std::shared_ptr<AutoBelong> JiraUserCustomSetting::autoBelong()
{
    return m_pAutoBelong;
}
