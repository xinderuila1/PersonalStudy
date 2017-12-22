#ifndef JIRAUSERCUSTOMSETTING_H
#define JIRAUSERCUSTOMSETTING_H

/*!
*@file     �û��Զ���������
*@brief    
*@author   sunjj
*@date     2017��12��22��
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

//��Ʒ��Ϣ
struct ProductInfo
{
    QString sProductKey;   //��Ʒ��ʶ
    QString sProductName;  //��Ʒ����
};

//Jira��Ϣ
struct JiraInfo
{
    QString sUserName;  //����
    QString sPassword;  //����
    QString sJiraUrl;   //Jira url
};

//GEH��Ϣ
struct GEHInfo
{
    QString sUserName; //����
    QString sPassword; //����
};

//�Զ�������Ϣ
struct AutoBelong
{
    int nTimeInterval;  //����Զ�����һ�� ��λСʱh
};

//�ʼ�����
struct EmailContent
{
    QString sHeader;    //����ͷ
    QString sFSR;  //������
    QString sCSR;  //������
    QString sSJR;  //�ռ���
};

//�Զ�Ԥ����Ϣ
struct AutoWarning
{
    int nTimeInterval;  //����Զ�����һ�� ��λ����min
    int nHighCrashMinValue; //��Ƶ����Ԥ��ֵ
    QString sWarningUrl; //GEHԤ��·��
    QString sHeader;    //����ͷ
    QString sFSR;  //������
    QString sCSR;  //������
    QString sSJR;  //�ռ���
};

//Ĭ���û�
struct DefaultUser
{
    QString sUser;  //�û���
    QString sDomain; //����
};
#endif