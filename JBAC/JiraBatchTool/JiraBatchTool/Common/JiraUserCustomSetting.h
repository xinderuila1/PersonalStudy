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
struct ProductInfo;
struct JiraInfo;
struct GEHInfo;
struct AutoBelong;
struct EmailContent;
struct AutoWarning;
struct DefaultUser;

class JiraUserCustomSetting
{
public:
    static JiraUserCustomSetting* instance();
    ~JiraUserCustomSetting();

    std::shared_ptr<CrashBelongContainer> belongContainerByScript();
    std::shared_ptr<JiraDllContainer> belongContainerByDll();

    std::shared_ptr<JiraVersionContainer> yyVersionContainer();
    std::shared_ptr<JiraUserModuleContainer> yyUserModuleContainer();
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
};

//产品信息
struct ProductInfo
{
    QString sProductKey;   //产品标识
    QString sProductName;  //产品名称
};

//Jira信息
struct JiraInfo
{
    QString sUserName;  //域名
    QString sPassword;  //密码
    QString sJiraUrl;   //Jira url
};

//GEH信息
struct GEHInfo
{
    QString sUserName; //域名
    QString sPassword; //密码
};

//自动归类信息
struct AutoBelong
{
    int nTimeInterval;  //多久自动计算一次 单位小时h
};

//邮件内容
struct EmailContent
{
    QString sHeader;    //标题头
    QString sFSR;  //发送人
    QString sCSR;  //抄送人
    QString sSJR;  //收件人
};

//自动预警信息
struct AutoWarning
{
    int nTimeInterval;  //多久自动计算一次 单位分钟min
    int nHighCrashMinValue; //高频崩溃预警值
    QString sWarningUrl; //GEH预警路径
    QString sHeader;    //标题头
    QString sFSR;  //发送人
    QString sCSR;  //抄送人
    QString sSJR;  //收件人
};

//默认用户
struct DefaultUser
{
    QString sUser;  //用户名
    QString sDomain; //域名
};
#endif