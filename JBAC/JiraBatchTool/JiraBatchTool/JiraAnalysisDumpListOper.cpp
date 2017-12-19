#include "JiraAnalysisDumpListOper.h"
#include "JiraConsts.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

/*!
*@brief        分析栈信息 
*@author       sunjj 2017年7月20日
*@param[in]    const QString& sIssue
*/
void JiraAnalysisDumpListOper::analysisStack(const QString& sIssue)
{
    QString sCrashStackInfo;
    QFile oCrashFile(qApp->applicationDirPath() +"/crashInfo/" + sIssue + "-CrashInfo.txt");
    if (oCrashFile.open(QFile::ReadWrite | QIODevice::Text))
    {
        QString sCrashInfo = oCrashFile.readAll();
        QJsonDocument oJsonDocument = QJsonDocument::fromJson(sCrashInfo.toUtf8());
        QString sReportStr = oJsonDocument.object().value("report").toString().replace("\\", "");
        oJsonDocument = QJsonDocument::fromJson(sReportStr.toUtf8());
        
        QJsonObject oCrashJsonObject = oJsonDocument.object().value("crash").toObject();
        QJsonObject oThreadJsonObject = oJsonDocument.object().value("threads").toObject();

        //Crash信息摘要
        QString sCrashAddress = oCrashJsonObject.value(strCrashAddress).toString();
        QString sCrashReson = oCrashJsonObject.value(strCrashReason).toString();
        QString sCrashThreadIndex = QString::number(oCrashJsonObject.value(strCrashThreadIndex).toVariant().toInt());
        sCrashStackInfo += "crash_address:" + sCrashAddress + "\n";
        sCrashStackInfo += "crash_reason:" + sCrashReson + "\n\n\n";

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
            sCrashStackInfo += sFrame + "  " + sModule + "    " + sSourceFile + "  " + sSourceLine + "  " + sFunction + "\n";
        }
    }
    
    if (!sCrashStackInfo.isEmpty())
    {
        oCrashFile.close();
        oCrashFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream oTextStream(&oCrashFile);
        oTextStream << sCrashStackInfo;
        oTextStream.flush();
        oCrashFile.close();
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
    m_pScriptContainer->swap(AutoScriptContainer());
}

/*!
*@brief        刷新Dump列表容器 
*@author       sunjj 2017年7月20日
*@param[in]    const QString& sIssue
*/
void JiraAnalysisDumpListOper::refreshDumpCotainer(const QString& sIssue)
{
    QFile oCrashFile(qApp->applicationDirPath() +"/crashInfo/" + sIssue + ".json");
    if (oCrashFile.open(QFile::ReadOnly | QIODevice::Text))
    {
        QString sCrashInfo = oCrashFile.readAll();
        QJsonDocument oJsonDocument = QJsonDocument::fromJson(sCrashInfo.toUtf8());
        QJsonArray oJsonArray = oJsonDocument.object().value("list").toArray();
        for (int nIndex = 0; nIndex < oJsonArray.size(); ++nIndex)
        {
            AutoScriptInfo oScriptInfo;
            QJsonObject oJsonObject = oJsonArray.at(nIndex).toObject();
            oScriptInfo.bIsAutoScript = oJsonObject.value(strAutotest).toString().trimmed() == "autotest";
            oScriptInfo.sSubmitter = oJsonObject.value(strSubmitter).toString();
            oScriptInfo.sDetailInfo = oJsonObject.value(strDescription).toString();
            oScriptInfo.sDumpUrl = oJsonObject.value(strDumpId).toString();
            oScriptInfo.sStackId = QString::number(oJsonObject.value(strStackId).toVariant().toULongLong());
            m_pScriptContainer->push_back(oScriptInfo);
        }
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
    QJsonArray oJsonArray;
    for (auto pIter = m_pScriptContainer->begin(); pIter != m_pScriptContainer->end(); ++pIter)
    {
        AutoScriptInfo oScriptInfo = *pIter;
        QJsonObject oJsonObject;
        oJsonObject.insert("IsAutoTest", oScriptInfo.bIsAutoScript);
        oJsonObject.insert("Submitter", oScriptInfo.sSubmitter);
        oJsonObject.insert("DetailInfo", oScriptInfo.sDetailInfo);
        oJsonArray.append(oJsonObject);
    }
    QJsonDocument oJsonDocument;
    oJsonDocument.setArray(oJsonArray);
    return oJsonDocument.toJson();
}

/*!
*@brief        是否是高频次问题 
*@author       sunjj 2017年8月1日
*@return       bool
*/
QString JiraAnalysisDumpListOper::highFrequency()
{
    return (m_pScriptContainer->size() >= 2)? "true" : "";
}
