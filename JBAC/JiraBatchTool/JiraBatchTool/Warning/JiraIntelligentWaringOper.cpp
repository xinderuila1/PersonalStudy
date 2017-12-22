#include "JiraIntelligentWaringOper.h"
#include "JiraIntelligentWaringPython.h"
#include "Common/JiraConsts.h"
#include "Warning/JiraIntelligentWaringOutputHtml.h"

#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QCoreApplication>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#define  MIN_CRASH_COUNT  2

//html文件 测试用
QString htmlFilePath()
{
    QString sProductInfo = "GTJ2017";
    QString sResult = qApp->applicationDirPath() + "//htmlFile//" + sProductInfo + ".html";
    if (QFile::exists(sResult))
        QFile::remove(sResult);
    return sResult;
}

/*!
*@brief        构造函数 
*@author       sunjj 2017年12月11日
*/
JiraIntelligentWaringOper::JiraIntelligentWaringOper()
{
    m_pIntelligentWaringPython = new JiraIntelligentWaringPython;
    m_pAnalysisedCrashUsers = new JiraAnalysisCrashUsers;
    m_pUnAnalysisCrashUsers = new JiraAnalysisCrashUsers;
    m_pHighCrashUsers = new JiraHighCrashUsers;
    m_pHighCrashCount = new JiraHighCrashCount;
}

/*!
*@brief        析构函数 
*@author       sunjj 2017年12月11日
*/
JiraIntelligentWaringOper::~JiraIntelligentWaringOper()
{
    delete m_pHighCrashCount;
    m_pHighCrashCount = nullptr;

    delete m_pHighCrashUsers;
    m_pHighCrashUsers = nullptr;

    delete m_pUnAnalysisCrashUsers;
    m_pUnAnalysisCrashUsers = nullptr;

    delete m_pAnalysisedCrashUsers;
    m_pAnalysisedCrashUsers = nullptr;

    delete m_pIntelligentWaringPython;
    m_pIntelligentWaringPython = nullptr;
}

/*!
*@brief        统计未分析的用户信息
*@author       sunjj 2017年12月11日
*/
void JiraIntelligentWaringOper::analysisCurDayCrash(bool bClear)
{
    beforeAnalysis(bClear);
    downloadUnAnalysisCrash();
    loadUnAnalysisCrash();
    analysisCurrentCrash();
    sendEmailToTesters();
    afterAnalysis();
}

/*!
*@brief        加载未分析列表
*@author       sunjj 2017年12月11日
*/
void JiraIntelligentWaringOper::loadUnAnalysisCrash()
{
    QJsonDocument oJsonDocument = QJsonDocument::fromJson(m_sUnAnalysisCrash.toUtf8());
    QJsonArray oCrashListArray = oJsonDocument.object().value(strList).toArray();
    for (int nIndex = 0; nIndex < oCrashListArray.size(); ++nIndex)
    {
        QJsonObject oJsonObject = oCrashListArray.at(nIndex).toObject();
        QString sDeviceID = oJsonObject.value(strDeviceId).toString();
        QString sDescription = oJsonObject.value(strDescription).toString();
        qint64 nCrashTime = oJsonObject.value(strCrashTime).toVariant().toLongLong();

        auto pFindDeviceID = m_pUnAnalysisCrashUsers->find(sDeviceID);
        if (pFindDeviceID == m_pUnAnalysisCrashUsers->end())
        {
            std::map<qint64, QString> oCrashTimeMap;
            oCrashTimeMap.insert(std::make_pair(nCrashTime, sDescription));
            m_pUnAnalysisCrashUsers->insert(std::make_pair(sDeviceID, oCrashTimeMap));
        } 
        else
        {
            std::map<qint64, QString>& oCrashTimeMap = pFindDeviceID->second;
            oCrashTimeMap.insert(std::make_pair(nCrashTime, sDescription));
        }
    }
}

/*!
*@brief        分析当前崩溃 
*@author       sunjj 2017年12月11日
*/
void JiraIntelligentWaringOper::analysisCurrentCrash()
{
    for (auto pNewDevicedIDIter = m_pUnAnalysisCrashUsers->begin(); pNewDevicedIDIter != m_pUnAnalysisCrashUsers->end(); ++pNewDevicedIDIter)
    {
        QString sNewDevicedID = pNewDevicedIDIter->first;
        std::map<qint64, QString> oNewCrashTimeMap = pNewDevicedIDIter->second;
        //某设备的总共崩溃次数
        int nAllCrashCount = oNewCrashTimeMap.size();

        auto pFindOldDevideIDIter = m_pAnalysisedCrashUsers->find(sNewDevicedID);
        if (pFindOldDevideIDIter == m_pAnalysisedCrashUsers->end())
        {
            m_pAnalysisedCrashUsers->insert(std::make_pair(sNewDevicedID, oNewCrashTimeMap));
            int nNewCrashCount = oNewCrashTimeMap.size();
            if (nNewCrashCount >= MIN_CRASH_COUNT)
            {
                m_pHighCrashUsers->insert(std::make_pair(sNewDevicedID, oNewCrashTimeMap));
                m_pHighCrashCount->insert(std::make_pair(sNewDevicedID, std::make_pair(nAllCrashCount, nAllCrashCount)));
            }
        }
        else
        {
            std::map<qint64, QString> oAskTesterMap;
            std::map<qint64, QString>& oOldCrashTimeMap = pFindOldDevideIDIter->second;

            //上次崩溃次数
            int nOldCrashCount = oOldCrashTimeMap.size();
            //当前崩溃次数
            int nNewCrashCount = oNewCrashTimeMap.size();
            //崩溃次数差距
            int nDifferCrashCount = nNewCrashCount - nOldCrashCount;
            //新增Crash是否提示测试人员
            bool bNeedAskTesters = nNewCrashCount >= MIN_CRASH_COUNT && nDifferCrashCount > 0;
            //新增记录项
            m_pHighCrashCount->insert(std::make_pair(sNewDevicedID, std::make_pair(nAllCrashCount, nDifferCrashCount)));

            for (auto pNewCrashTimeIter = oNewCrashTimeMap.begin(); pNewCrashTimeIter != oNewCrashTimeMap.end(); ++pNewCrashTimeIter)
            {
                qint64 nCrashTime = pNewCrashTimeIter->first;
                QString sDescription = pNewCrashTimeIter->second;

                auto pOldCrashTimeIter = oOldCrashTimeMap.find(nCrashTime);
                if (pOldCrashTimeIter == oOldCrashTimeMap.end())
                {
                    oOldCrashTimeMap.insert(std::make_pair(nCrashTime, sDescription));

                    if (bNeedAskTesters)
                        oAskTesterMap.insert(std::make_pair(nCrashTime, sDescription));
                }
            }

            if (bNeedAskTesters && !oAskTesterMap.empty())
            {
                m_pHighCrashUsers->insert(std::make_pair(sNewDevicedID, oAskTesterMap));
            }
        }
    }
}

/*!
*@brief        比较崩溃次数 
*@author       sunjj 2017年12月11日
*@param[in]    const JiraWarningCrashInfo& oFirstObj
*@param[in]    const JiraWarningCrashInfo& oSecondObj
*@return       bool
*/
bool compareHighLevel(const JiraWarningCrashInfo& oFirstObj, const JiraWarningCrashInfo& oSecondObj)
{
    return oFirstObj.nTotalCount > oSecondObj.nTotalCount;
}

/*!
*@brief        向测试人员发送邮件 
*@author       sunjj 2017年12月11日
*/
void JiraIntelligentWaringOper::sendEmailToTesters()
{
    if (!m_pHighCrashUsers->empty())
    {
        auto mapToString=[](const std::map<qint64, QString>& oCrashMap) -> QString
        {
            QString sResult = "";
            for (auto pIter = oCrashMap.begin(); pIter != oCrashMap.end(); ++pIter)
            {
                QString sDesciption = pIter->second;
                if (!sDesciption.isEmpty())
                {
                    sResult = (sResult + sDesciption + "\n");
                }
            }
            return sResult;
        };

        auto lastCrashTime=[](const std::map<qint64, QString>& oCrashMap) -> QString
        {
            if (oCrashMap.empty())
                return "";

            qint64 oTemp = oCrashMap.rbegin()->first;
            QDateTime oDataTime = QDateTime::fromMSecsSinceEpoch(oTemp);
            return oDataTime.toString("yyyy-MM-dd hh:mm:ss");
        };

        //新增用户描述信息
        JiraWarningCrashContainer oNewUserCrash;
        for (auto pHighCrashIter = m_pHighCrashUsers->begin(); pHighCrashIter != m_pHighCrashUsers->end(); ++pHighCrashIter)
        {
            auto pCrashCountIter = m_pHighCrashCount->find(pHighCrashIter->first);
            if (pCrashCountIter == m_pHighCrashCount->end())
                continue;

            JiraWarningCrashInfo oCrashInfo;
            oCrashInfo.sDeviceID = pHighCrashIter->first;
            oCrashInfo.nTotalCount = pCrashCountIter->second.first;
            oCrashInfo.nNewCount = pCrashCountIter->second.second;
            oCrashInfo.sLastCrashTime = lastCrashTime(pHighCrashIter->second);
            oCrashInfo.sDetailInfo = mapToString(pHighCrashIter->second);
        }

        //全部用户描述信息
        JiraWarningCrashContainer oAllUserCrash;
        for (auto pAnalysisedCrashIter = m_pAnalysisedCrashUsers->begin(); pAnalysisedCrashIter != m_pAnalysisedCrashUsers->end(); ++pAnalysisedCrashIter)
        {
            JiraWarningCrashInfo oCrashInfo;
            oCrashInfo.sDeviceID = pAnalysisedCrashIter->first;
            oCrashInfo.nTotalCount = pAnalysisedCrashIter->second.size();
            oCrashInfo.sLastCrashTime = lastCrashTime(pAnalysisedCrashIter->second);
            oCrashInfo.sDetailInfo = mapToString(pAnalysisedCrashIter->second);
        }

        //按照崩溃次数由大到小排序
        std::sort(oNewUserCrash.begin(), oNewUserCrash.end(), compareHighLevel);
        std::sort(oAllUserCrash.begin(), oAllUserCrash.end(), compareHighLevel);
        
        QString sHtmlPath = htmlFilePath();
        JiraProductInfo oProductInfo;
        oProductInfo.sProductKey = "GTJ2017";
        oProductInfo.sProductName = QStringLiteral("云计量土建");
        oProductInfo.sProductVersion = "1.0.9.0";

        JiraIntelligentWaringOutputHtml oOutputHtml;
        oOutputHtml.setoutputpath(sHtmlPath);
        oOutputHtml.setproductInfo(oProductInfo.productInfo());
        oOutputHtml.outputHtml(&oAllUserCrash, &oNewUserCrash);

        m_pIntelligentWaringPython->sendEmailToTesters(sHtmlPath);
    }
}

/*!
*@brief        分析前 
*@author       sunjj 2017年12月11日
*/
void JiraIntelligentWaringOper::beforeAnalysis(bool bClear)
{
    if (bClear)
        m_pAnalysisedCrashUsers->clear();
    m_pHighCrashCount->clear();
    m_pHighCrashUsers->clear();
    m_pUnAnalysisCrashUsers->clear();
    m_sUnAnalysisCrash.clear();
}

/*!
*@brief        分析后 
*@author       sunjj 2017年12月11日
*/
void JiraIntelligentWaringOper::afterAnalysis()
{
    m_pHighCrashCount->clear();
    m_pHighCrashUsers->clear();
    m_pUnAnalysisCrashUsers->clear();
    m_sUnAnalysisCrash.clear();
}

/*!
*@brief        下载所有的Crash文件 
*@author       sunjj 2017年12月11日
*/
void JiraIntelligentWaringOper::downloadUnAnalysisCrash()
{
    m_sUnAnalysisCrash = m_pIntelligentWaringPython->downloadUnAnalysisCrash();
}

