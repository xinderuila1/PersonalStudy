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

//GEH/Jira相关信息
struct LoginInfo
{
    QString sJiarUrl;    //Jira登录URL
    QString sUserName;   //域账号
    QString sPassword;   //与密码
};

//dll信息容器
typedef std::map<QString, JiraDllInfo> JiraDllContainer;
typedef std::map<QString, QString> JiraCrashKeyContainer;
typedef std::map<QString, bool> JiraAnalysisLogInfo;
typedef std::map<QString, CrashBelongInfo> CrashBelongContainer;
typedef std::map<QString, JiraModuleInfo> JiraUserModuleContainer;
typedef std::vector<AutoScriptInfo> AutoScriptContainer;
typedef std::vector<QString> JiraVersionContainer;

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