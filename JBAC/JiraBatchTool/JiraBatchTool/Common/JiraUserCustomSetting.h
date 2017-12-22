#ifndef JIRAUSERCUSTOMSETTING_H
#define JIRAUSERCUSTOMSETTING_H

/*!
*@file     用户自定义设置类
*@brief    
*@author   sunjj
*@date     2017年12月22日
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include "Common/JiraConsts.h"
#include <QString>

class QXmlStreamReader;

class JiraUserCustomSetting
{
public:
    static JiraUserCustomSetting* instance();
    ~JiraUserCustomSetting();

    std::shared_ptr<ProductInfo> productInfo();
    std::shared_ptr<JiraInfo> jiraInfo();
    std::shared_ptr<GEHInfo> gehInfo();

    std::shared_ptr<CrashBelongContainer> belongContainerByScript();
    std::shared_ptr<JiraDllContainer> belongContainerByDll();

    std::shared_ptr<JiraVersionContainer> yyVersionContainer();
    std::shared_ptr<JiraUserModuleContainer> yyUserModuleContainer();

    std::shared_ptr<AutoWarning> autoWarning();
    std::shared_ptr<AutoBelong> autoBelong();
    std::shared_ptr<JiraWarningVersionContainer> warningVersionContainer();

private:
    JiraUserCustomSetting();
    void initUserCustomSetting();

    void loadProductInfo();
    void loadJiraInfo();
    void loadGehInfo();
    void loadAutoBelongInfo();
    void loadAutoWarningInfo();
    void loadBelongRuleInfo();
    void loadDefaultUserInfo();
    void loadCrashBelongRule();

    void loadByAutoScriptInfo();
    void loadByStackDllInfo();
    void loadDllFuncInfo(JiraModuleInfo& oModuleInfo);
    void loadDllDetailInfo(JiraDllInfo& oDllInfo);

    void loadYYProductInfo();
    void loadYYModuleInfo();
    void loadYYVersionInfo();
    void loadWarningVersionInfo();

    QString moduleId(const QString& sKey);
private:
    QXmlStreamReader *m_pXMLReader;

    std::shared_ptr<JiraUserModuleContainer> m_pModuleContainer;

    std::shared_ptr<DefaultUser> m_pDefaultUser;
    std::shared_ptr<ProductInfo> m_pProductInfo;
    std::shared_ptr<JiraInfo> m_pJiraInfo;
    std::shared_ptr<GEHInfo> m_pGEHInfo;

    std::shared_ptr<AutoBelong> m_pAutoBelong;
    std::shared_ptr<EmailContent> m_pEmailContent;
    std::shared_ptr<AutoWarning> m_pAutoWarning;

    std::shared_ptr<CrashBelongContainer> m_pBelongContainerByScript;
    std::shared_ptr<JiraDllContainer> m_pBelongContainerByDll;

    std::shared_ptr<JiraVersionContainer> m_pYYVersionContainer;
    std::shared_ptr<JiraUserModuleContainer> m_pYYUserModuleContainer;
    std::shared_ptr<JiraWarningVersionContainer> m_pWarningVersionContainer;
};

#endif