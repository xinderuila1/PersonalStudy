#ifndef JIRACONSTS_H
#define JIRACONSTS_H

/*!
*@file     ��������
*@brief    
*@author   sunjj
*@date     2017��2��26��
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include <map>
#include <vector>
#include <QString>

//ģ����Ϣ
struct JiraModuleInfo
{
    QString sModuleName;
    QString sAuthor;
    QString sDomain;
    QString sModuleID;

public:
    void clear()
    {
        sModuleName = "";
        sAuthor = "";
        sDomain = "";
        sModuleID = "";
    }
};

//��ģ����Ϣ
struct JiraFunInfo
{
    QString sFunName;
    QString sAuthor;
    QString sDomain;
    int nPriority;

public:
    JiraFunInfo():nPriority(-1){}

    void clear()
    {
        sFunName = "";
        sAuthor = "";
        sDomain = "";
    }
};

//dll�����Ϣ
struct JiraDllInfo
{
    QString sDllName;
    JiraModuleInfo oModuleInfo;
    JiraFunInfo oFunInfo;

public:
    void clear()
    {
        sDllName = "";
        oModuleInfo.clear();
        oFunInfo.clear();
    }
};

//Crash������Ϣ
struct CrashBelongInfo
{
    QString sIdentification;
    QString sModuleName;
    QString sAuthor;
    QString sDomain;
    QString sModuleID; 
    QString sAutoScriptInfo;
public:
    void clear()
    {
        sIdentification.clear();
        sModuleName.clear();
        sAuthor.clear();
        sDomain.clear();
        sModuleID.clear();
        sAutoScriptInfo.clear();
    }
};

//�Զ����ű���Ϣ
struct AutoScriptInfo
{
    bool bIsAutoScript;
    QString sSubmitter;
    QString sDetailInfo;
    QString sDumpUrl;
    QString sStackId;
    QString sCrashTime;
};

//Crash����Ҫ���µ���Ϣ
struct CrashUpdateInfo
{
    QString sDomain;
    QString sModuleID;
    QString sDetailInfo;    //Crash������Ϣ
    QString sAutoTest;      //Crash�Ƿ����Զ����ύ
    QString sRemarks;       //Crash��ע��Ϣ
    QString sDumpUrl;       //Crash��Ӧdump��ַ
    QString sHighFrequency; //Crash��Ƶ��(��������>=2)
    QString sStackInfo;     //��ջ��Ϣ
    QString sCrashCount;    //��������
public:
    void clear()
    {
        sDomain.clear();
        sModuleID.clear();
        sDetailInfo.clear();
        sAutoTest.clear();
        sRemarks.clear();
        sDumpUrl.clear();
        sHighFrequency.clear();
    }
};

//GEH/Jira�����Ϣ
struct LoginInfo
{
    QString sJiarUrl;     //Jira��¼URL
    QString sUserName;    //���˺�
    QString sPassword;    //������
    QString sProductCode; //��Ʒ��ʶ
public:
    void assign(LoginInfo* pLoginInfo)
    {
        sJiarUrl = pLoginInfo->sJiarUrl;
        sUserName = pLoginInfo->sUserName;
        sPassword = pLoginInfo->sPassword;
        sProductCode = pLoginInfo->sProductCode;
    }
};

//���ܷ���ר��
typedef std::map<QString, JiraDllInfo> JiraDllContainer;
typedef std::map<QString, QString> JiraCrashKeyContainer;
typedef std::map<QString, bool> JiraAnalysisLogInfo;
typedef std::map<QString, CrashBelongInfo> CrashBelongContainer;
typedef std::map<QString, JiraModuleInfo> JiraUserModuleContainer;
typedef std::vector<AutoScriptInfo> AutoScriptContainer;
typedef std::vector<QString> JiraVersionContainer;


//����Ԥ��ר��
typedef std::map<QString, std::map<qint64, QString>> JiraAnalysisCrashUsers; //δ�����û��豸���� �豸ID-����ʱ��-��������
typedef JiraAnalysisCrashUsers JiraHighCrashUsers;                            //���ص��ע�ĸ�Ƶ�α����û�
typedef std::map<QString, std::pair<int, int>> JiraHighCrashCount;            //��������ͳ��

struct JiraWarningCrashInfo
{
    QString sDeviceID;  //�豸ID
    int nTotalCount;    //�ܹ���������
    int nNewCount;      //������������
    QString sLastCrashTime; //������ʱ��
    QString sDetailInfo; //��������
};

typedef std::vector<JiraWarningCrashInfo> JiraWarningCrashContainer;  //������Ϣ����

struct JiraProductInfo
{
    QString sProductKey;   //��Ʒ��ʶ
    QString sProductName;  //��Ʒ����
    QString sProductVersion; //��Ʒ��ʶ
public:
    QString productInfo()
    {
        return sProductKey + sProductName + sProductVersion;
    }
};
static const QString strModule = "module";
static const QString strFunction = "function";
static const QString strVersion = "version";
static const QString strDll = "dll";
static const QString strName = "name";
static const QString strAuthor = "author";
static const QString strDomain = "domain";
static const QString strPriority = "priority";
static const QString strModuleID = "moduleID"; 
static const QString strIdentification = "identification";
static const QString strAutotest = "source";
static const QString strSubmitter = "username";
static const QString strDescription = "description";
static const QString strDumpId = "uid";
static const QString strStackId = "id";
static const QString strFrameIndex = "frame_index";
static const QString strSourceFile = "source_file";
static const QString strSourceLine = "source_line";
static const QString strCrashAddress = "crash_address";
static const QString strCrashReason = "crash_reason";
static const QString strCrashThreadIndex = "crash_thread_index";
static const QString strProduct = "product";
static const QString strUser = "user";
static const QString strAutoTestTag = "autotest";
static const QString strReport = "report";
static const QString strCrash = "crash";
static const QString strThreads = "threads";
static const QString strList = "list";
static const QString strCrashTime = "crashTime";
static const QString strDeviceId = "deviceId";

#define STR_IS_AUTO_TEST QStringLiteral("...�Զ�����")
#define SUBMITTERS QStringLiteral("...�ύ�ˣ�")
#define DETAILINFO QStringLiteral("...������")
#define CRASH_TIME QStringLiteral("...ʱ�䣺")

#define CRASH_ADDRESS QStringLiteral("������ַ:")
#define CRASH_REASON QStringLiteral("����ԭ��:")
#define CRASH_COUNT QStringLiteral("����%1��")

// #define DEVICE_CRASH_COUNT QStringLiteral("b_�豸��������")
// #define DEVICE_ID QStringLiteral("a_�豸ID")
// #define NEW_CRASH_COUNT QStringLiteral("c_����������Ŀ")
// #define LAST_CRASH_TIME QStringLiteral("d_ĩ�α���ʱ��")
// #define NEW_CRASH_DETAIL QStringLiteral("e_������������")

#define YES QStringLiteral("��")
#define NO QStringLiteral("��")

// Jira �����Ŷ�ID�ֶα�ʶ��
// �ֽ���㣺 18930 a
// �������㣺 18931 a
// CADʶ��  18933 a
// ����ƽ̨�� 18453 a
// ͼ��ƽ̨�� 18454 a
// ����ƽ̨bug�� 18455 a
// ����ƽ̨����18456
// ����ƽ̨API�� 18458
// �����Ŷӣ�    18935 a
// ��ҵ���Ŷӣ�  18936 a
// �����Ŷӣ�    18937 a
// ��汾������  18938 a
// ��Ӫ�Ŷӣ�21862 a

//11117[�ֽ����], 
//11118[��������], 
//11119[CADʶ��], 
//11120[����ƽ̨], 
//11121[ͼ��ƽ̨], 
//11122[������Ŀ], 
//11123[��ҵ��], 
//11124[������Ŀ], 
//11125[��Ӫ��Ŀ], 
//11126[��ģ��Ŀ], 
//11724[����ƽ̨], 
//-1


#endif