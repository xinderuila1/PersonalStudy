#include "JiraIntelligentWaringOper.h"
#include "JiraIntelligentWaringPython.h"

#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QCoreApplication>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#define  MIN_CRASH_COUNT  2

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
    QJsonArray oCrashListArray = oJsonDocument.object().value("list").toArray();
    for (int nIndex = 0; nIndex < oCrashListArray.size(); ++nIndex)
    {
        QJsonObject oJsonObject = oCrashListArray.at(nIndex).toObject();
        QString sDeviceID = oJsonObject.value("deviceId").toString();
        QString sDescription = oJsonObject.value("description").toString();
        qint64 nCrashTime = oJsonObject.value("crashTime").toVariant().toLongLong();

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
*@param[in]    const QJsonObject& oFirstObj
*@param[in]    const QJsonObject& oSecondObj
*@return       bool
*/
bool compareHighLevel(const QJsonObject& oFirstObj, const QJsonObject& oSecondObj)
{
    int nFirstValue = oFirstObj.value(QStringLiteral("b_设备崩溃总数")).toVariant().toInt();
    int nSecondValue = oSecondObj.value(QStringLiteral("b_设备崩溃总数")).toVariant().toInt();
    return nFirstValue > nSecondValue;
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


        std::vector<QJsonObject> oJsonObjectVec;

        QJsonArray oJsonArray;
        for (auto pHighCrashIter = m_pHighCrashUsers->begin(); pHighCrashIter != m_pHighCrashUsers->end(); ++pHighCrashIter)
        {
            QString sDevicedId = pHighCrashIter->first;
            QString sDescription = mapToString(pHighCrashIter->second);
            QString sLastCrashTime = lastCrashTime(pHighCrashIter->second);
            auto pCrashCountIter = m_pHighCrashCount->find(sDevicedId);
            if (pCrashCountIter == m_pHighCrashCount->end())
                continue;

            int nAllCrashCount = pCrashCountIter->second.first;
            int nNewCrashCount = pCrashCountIter->second.second;
            
            QJsonObject oJsonObject;
            oJsonObject.insert(QStringLiteral("a_设备ID"), sDevicedId);
            oJsonObject.insert(QStringLiteral("b_设备崩溃总数"), nAllCrashCount);
            oJsonObject.insert(QStringLiteral("c_新增崩溃数目"), nNewCrashCount);
            oJsonObject.insert(QStringLiteral("d_末次崩溃时间"), sLastCrashTime);
            oJsonObject.insert(QStringLiteral("e_新增崩溃描述"), sDescription);
            oJsonObjectVec.push_back(oJsonObject);
        }

        std::sort(oJsonObjectVec.begin(), oJsonObjectVec.end(), compareHighLevel);

        for (auto pHighCrashIter = oJsonObjectVec.begin(); pHighCrashIter != oJsonObjectVec.end(); ++pHighCrashIter)
        {
            QJsonObject oJsonObject = *pHighCrashIter;
            oJsonArray.append(QJsonValue(oJsonObject));
        }
        
        QJsonDocument oJsonDocument;
        oJsonDocument.setArray(oJsonArray);
        QString sSendEmailContent = oJsonDocument.toJson();

        m_pIntelligentWaringPython->sendEmailToTesters(sSendEmailContent);
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

