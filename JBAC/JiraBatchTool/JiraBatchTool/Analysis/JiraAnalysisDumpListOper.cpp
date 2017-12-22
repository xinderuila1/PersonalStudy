#include "JiraAnalysisDumpListOper.h"
#include "Common/JiraConsts.h"

#include <QDebug>
#include <QFile>
#include <QDateTime>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#define HIGH_FREQUENCY_COUNT  2

/*!
*@brief        分析栈信息 
*@author       sunjj 2017年7月20日
*@param[in]    const QString& sIssue
*/
void JiraAnalysisDumpListOper::analysisStack(const QString& sDumpJson)
{
    m_sCrashStackInfo.clear();
    QJsonDocument oJsonDocument = QJsonDocument::fromJson(sDumpJson.toUtf8());
    QString sReportStr = oJsonDocument.object().value(strReport).toString().replace("\\", "");
    oJsonDocument = QJsonDocument::fromJson(sReportStr.toUtf8());
        
    QJsonObject oCrashJsonObject = oJsonDocument.object().value(strCrash).toObject();
    QJsonObject oThreadJsonObject = oJsonDocument.object().value(strThreads).toObject();

    //Crash信息摘要
    QString sCrashAddress = oCrashJsonObject.value(strCrashAddress).toString();
    QString sCrashReson = oCrashJsonObject.value(strCrashReason).toString();
    QString sCrashThreadIndex = QString::number(oCrashJsonObject.value(strCrashThreadIndex).toVariant().toInt());
    m_sCrashStackInfo += CRASH_ADDRESS + sCrashAddress + "\n";
    m_sCrashStackInfo += CRASH_REASON + sCrashReson + "\n\n\n";

    //Crash堆栈信息
    QJsonArray oJsonArray = oThreadJsonObject.value(sCrashThreadIndex).toArray();
    for (int nIndex = 0; nIndex < oJsonArray.size(); ++nIndex)
    {
        QJsonObject oJsonObject = oJsonArray.at(nIndex).toObject();
        QString sFrame = QString::number(oJsonObject.value(strFrameIndex).toVariant().toUInt());
        QString sModule = oJsonObject.value(strModule).toString();
        QString sSourceFile = oJsonObject.value(strSourceFile).toString();
        QString sSourceLine = oJsonObject.value(strSourceLine).toString();
        QString sFunction = oJsonObject.value(strFunction).toString();
        m_sCrashStackInfo += sFrame + "  " + sModule + "    " + sFunction +  "    " + sSourceFile + "  " + sSourceLine + "\n";
    }
}

/*!
*@brief        构造函数 
*@author       sunjj 2017年7月20日
*/
JiraAnalysisDumpListOper::JiraAnalysisDumpListOper()
    :m_pScriptContainer(nullptr)
{
    m_pScriptContainer = new AutoScriptContainer;
}

/*!
*@brief        析构函数 
*@author       sunjj 2017年7月20日
*/
JiraAnalysisDumpListOper::~JiraAnalysisDumpListOper()
{
    delete m_pScriptContainer;
    m_pScriptContainer = nullptr;
}

/*!
*@brief        清空dump容器
*@author       sunjj 2017年7月20日
*/
void JiraAnalysisDumpListOper::clearDumpCotainer()
{
    m_sCrashStackInfo.clear();
    m_pScriptContainer->swap(AutoScriptContainer());
}

/*!
*@brief        刷新Dump列表容器 
*@author       sunjj 2017年7月20日
*@param[in]    const QString& sDumpJson
*/
void JiraAnalysisDumpListOper::refreshDumpCotainer(const QString& sDumpJson)
{
    QJsonDocument oJsonDocument = QJsonDocument::fromJson(sDumpJson.toUtf8());
    QJsonArray oJsonArray = oJsonDocument.object().value(strList).toArray();
    for (int nIndex = 0; nIndex < oJsonArray.size(); ++nIndex)
    {
        AutoScriptInfo oScriptInfo;
        QJsonObject oJsonObject = oJsonArray.at(nIndex).toObject();
        oScriptInfo.bIsAutoScript = oJsonObject.value(strAutotest).toString().trimmed() == strAutoTestTag;
        oScriptInfo.sSubmitter = oJsonObject.value(strSubmitter).toString();
        oScriptInfo.sDetailInfo = oJsonObject.value(strDescription).toString();
        oScriptInfo.sDumpUrl = oJsonObject.value(strDumpId).toString();
        qint64 nCrashTime = oJsonObject.value(strCrashTime).toVariant().toLongLong();
        oScriptInfo.sCrashTime = QDateTime::fromMSecsSinceEpoch(nCrashTime).toString("yyyy-MM-dd hh:mm:ss");
        oScriptInfo.sStackId = QString::number(oJsonObject.value(strStackId).toVariant().toULongLong());
        m_pScriptContainer->push_back(oScriptInfo);
    }
}

/*!
*@brief        崩溃列表信息容器 
*@author       sunjj 2017年7月20日
*@return       AutoScriptContainer *
*/
AutoScriptContainer * JiraAnalysisDumpListOper::dumpContainer()
{
    return m_pScriptContainer;
}

/*!
*@brief        分析栈信息Id  
*@author       sunjj 2017年7月20日
*@return       QString
*/
QString JiraAnalysisDumpListOper::stackUrlId()
{
    if (!m_pScriptContainer->empty())
    {
        AutoScriptInfo& oFirstScriptInfo = *m_pScriptContainer->begin();
        return oFirstScriptInfo.sStackId;
    }
    return QString();
}

/*!
*@brief        dump下载的url 
*@author       sunjj 2017年7月20日
*@return       QString
*/
QString JiraAnalysisDumpListOper::dumpUrl()
{
    if (!m_pScriptContainer->empty())
    {
        AutoScriptInfo& oFirstScriptInfo = *m_pScriptContainer->begin();
        return oFirstScriptInfo.sDumpUrl;
    }
    return QString();
}

/*!
*@brief        Crash备注信息
*@author       sunjj 2017年7月20日
*@return       QString
*/
QString JiraAnalysisDumpListOper::remarks()
{
    QString sRemarks = "";
    for (auto pIter = m_pScriptContainer->begin(); pIter != m_pScriptContainer->end(); ++pIter)
    {
        AutoScriptInfo oScriptInfo = *pIter;
        QString sDetailInfo = "";
        sDetailInfo += (DETAILINFO + oScriptInfo.sDetailInfo + "\n");
        sDetailInfo += (SUBMITTERS + oScriptInfo.sSubmitter + "\n");
        sDetailInfo += (CRASH_TIME + oScriptInfo.sCrashTime + "\n");
        sDetailInfo += (STR_IS_AUTO_TEST + (oScriptInfo.bIsAutoScript? YES : NO) + "\n");
        sDetailInfo += "\n\n\n";
        sRemarks += sDetailInfo;
    }
    return sRemarks;
}

/*!
*@brief        是否是高频次问题 
*@author       sunjj 2017年8月1日
*@return       bool
*/
QString JiraAnalysisDumpListOper::highFrequency()
{
    return (m_pScriptContainer->size() >= HIGH_FREQUENCY_COUNT)? "true" : "";
}

/*!
*@brief        堆栈信息 
*@author       sunjj 2017年12月21日
*@return       QString
*/
QString JiraAnalysisDumpListOper::stackInfo()
{
    return m_sCrashStackInfo;
}

/*!
*@brief        崩溃次数 
*@author       sunjj 2017年12月21日
*@return       QString
*/
QString JiraAnalysisDumpListOper::crashCount()
{
    return CRASH_COUNT.arg(m_pScriptContainer->size());
}
