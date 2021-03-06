#ifndef JIRACONSTS_H
#define JIRACONSTS_H

/*!
*@file     常量定义
*@brief    
*@author   sunjj
*@date     2017年2月26日
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include <map>
#include <vector>
#include <QString>

#define FreeAndNilPyObject(pPyObject) {if (pPyObject){Py_DECREF(pPyObject); pPyObject = nullptr;}}

//模块信息
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

//子模块信息
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

//dll相关信息
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

//Crash归属信息
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

//自动化脚本信息
struct AutoScriptInfo
{
    bool bIsAutoScript;
    QString sSubmitter;
    QString sDetailInfo;
    QString sDumpUrl;
    QString sStackId;
    QString sCrashTime;
};

//Crash中需要更新的信息
struct CrashUpdateInfo
{
    QString sDomain;
    QString sModuleID;
    QString sDetailInfo;    //Crash描述信息
    QString sAutoTest;      //Crash是否是自动化提交
    QString sRemarks;       //Crash备注信息
    QString sDumpUrl;       //Crash对应dump地址
    QString sHighFrequency; //Crash高频次(崩溃次数>=2)
    QString sStackInfo;     //堆栈信息
    QString sCrashCount;    //崩溃次数
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

//运营需要监控的版本
struct YYNeedWarningVersion
{
    QString sVersionName;  //版本名称
    QString sVersionId;    //版本Id
};

//GEH/Jira相关信息
struct LoginInfo
{
    QString sJiarUrl;     //Jira登录URL
    QString sUserName;    //域账号
    QString sPassword;    //与密码
    QString sProductCode; //产品标识
public:
    void assign(LoginInfo* pLoginInfo)
    {
        sJiarUrl = pLoginInfo->sJiarUrl;
        sUserName = pLoginInfo->sUserName;
        sPassword = pLoginInfo->sPassword;
        sProductCode = pLoginInfo->sProductCode;
    }
};

//智能分类专用
typedef std::map<QString, JiraDllInfo> JiraDllContainer;
typedef std::map<QString, QString> JiraCrashKeyContainer;
typedef std::map<QString, bool> JiraAnalysisLogInfo;
typedef std::map<QString, CrashBelongInfo> CrashBelongContainer;
typedef std::map<QString, JiraModuleInfo> JiraUserModuleContainer;
typedef std::vector<AutoScriptInfo> AutoScriptContainer;
typedef std::vector<QString> JiraVersionContainer;
typedef std::vector<YYNeedWarningVersion> JiraWarningVersionContainer;

//智能预警专用
typedef std::map<QString, std::map<qint64, QString>> JiraAnalysisCrashUsers; //未分析用户设备集合 设备ID-崩溃时间-崩溃描述
typedef JiraAnalysisCrashUsers JiraHighCrashUsers;                            //需重点关注的高频次崩溃用户
typedef std::map<QString, std::pair<int, int>> JiraHighCrashCount;            //崩溃次数统计

struct JiraWarningCrashInfo
{
    QString sDeviceID;  //设备ID
    int nTotalCount;    //总共崩溃数量
    int nNewCount;      //新增崩溃数量
    QString sLastCrashTime; //最后崩溃时间
    QString sDetailInfo; //崩溃描述
};

typedef std::vector<JiraWarningCrashInfo> JiraWarningCrashContainer;  //崩溃信息容器

struct JiraProductInfo
{
    QString sProductKey;   //产品标识
    QString sProductName;  //产品名称
    QString sVersionName;  //产品标识
    QString sVersionId;    //版本Id
public:
    void assign(JiraProductInfo* pProductInfo)
    {
        sProductKey = pProductInfo->sProductKey;
        sProductName = pProductInfo->sProductName;
        sVersionName = pProductInfo->sVersionName;
        sVersionId = pProductInfo->sVersionId;
    }

    QString productInfo()
    {
        return sProductKey + "__" + sProductName +  "__"  + sVersionName;
    }
};


//////////////////////////////////////////////////////////////////////////
//智能预警专用

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
    QString sFilterSql; //过滤语句
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

struct JiraCrashRate
{
    int nStartCount;    //启动次数
    int nCrashCount;    //崩溃次数
    int nUserCount;     //用户总数
    int nCrashUserCount; //崩溃用户数
    QString sCrashRate;  //崩溃比率
    QString sUserRate;   //影响用户率
};
//////////////////////////////////////////////////////////////////////////
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
static const QString strVersionId = "versionId";
static const QString strAggregations = "aggregations";
static const QString strUsers = "users";
static const QString strValue = "value";
static const QString strHits = "hits";
static const QString strTotal = "total";

//配置文件专用
static const QString strProductInfo = "ProductInfo";
static const QString strProductKey = "ProductKey";
static const QString strProductName = "ProductName";
static const QString strJiraInfo = "JiraInfo";
static const QString strUserName = "UserName";
static const QString strUserPassword = "UserPassword";
static const QString strJiraUrl = "JiraUrl";
static const QString strGEHInfo = "GEHInfo";
static const QString strAutoBelong = "AutoBelong";
static const QString strTimeInterval = "TimeInterval";
static const QString strAutoWarning = "AutoWarning";
static const QString strWarningUrl = "WarningUrl";
static const QString strEmailContent = "EmailContent";
static const QString strHeader = "Header";
static const QString strFSR = "FSR";
static const QString strCSR = "CSR";
static const QString strSJR = "SJR";
static const QString strDefaultBelong = "DefaultBelong";
static const QString strCrashBelongRule = "CrashBelongRule";
static const QString strByAutoScript = "ByAutoScript";
static const QString strByStackDll = "ByStackDll";
static const QString strHighCrashMinValue = "HighCrashMinValue";
static const QString strBelongInfo = "BelongInfo";
static const QString strYYProduct = "YYProduct";
static const QString strNeedWarningVersion = "NeedWarningVersion";
static const QString strFilterSql = "FilterSql";
static const QString strSql = "sql";

#define STR_IS_AUTO_TEST QStringLiteral("...自动化：")
#define SUBMITTERS QStringLiteral("...提交人：")
#define DETAILINFO QStringLiteral("...描述：")
#define CRASH_TIME QStringLiteral("...时间：")

#define CRASH_ADDRESS QStringLiteral("崩溃地址:")
#define CRASH_REASON QStringLiteral("崩溃原因:")
#define CRASH_COUNT QStringLiteral("崩溃%1次")

// #define DEVICE_CRASH_COUNT QStringLiteral("b_设备崩溃总数")
// #define DEVICE_ID QStringLiteral("a_设备ID")
// #define NEW_CRASH_COUNT QStringLiteral("c_新增崩溃数目")
// #define LAST_CRASH_TIME QStringLiteral("d_末次崩溃时间")
// #define NEW_CRASH_DETAIL QStringLiteral("e_新增崩溃描述")

#define YES QStringLiteral("是")
#define NO QStringLiteral("否")

// Jira 归属团队ID字段标识类
// 钢筋计算： 18930 a
// 土建计算： 18931 a
// CAD识别：  18933 a
// 公共平台： 18453 a
// 图形平台： 18454 a
// 计量平台bug： 18455 a
// 计量平台需求：18456
// 计量平台API： 18458
// 做法团队：    18935 a
// 云业务团队：  18936 a
// 报表团队：    18937 a
// 大版本升级：  18938 a
// 运营团队：21862 a

//11117[钢筋计算], 
//11118[土建计算], 
//11119[CAD识别], 
//11120[公共平台], 
//11121[图形平台], 
//11122[做法项目], 
//11123[云业务], 
//11124[报表项目], 
//11125[运营项目], 
//11126[建模项目], 
//11724[计量平台], 
//-1


#endif