#include "JiraIntelligentWaringOper.h"
#include "JiraIntelligentWaringPython.h"
#include "Common/JiraConsts.h"
#include "Common/JiraUserCustomSetting.h"
#include "Warning/JiraIntelligentWaringOutputHtml.h"

#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QCoreApplication>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#define  MIN_CRASH_COUNT  JiraUserCustomSetting::instance()->autoWarning()->nHighCrashMinValue

/*!
*@brief        构造函数 
*@author       sunjj 2017年12月11日
*/
JiraIntelligentWaringOper::JiraIntelligentWaringOper(JiraProductInfo* pProductInfo)
    :m_pProductInfo(nullptr)
{
    m_pIntelligentWaringPython = new JiraIntelligentWaringPython;
    m_pAnalysisedCrashUsers = new JiraAnalysisCrashUsers;
    m_pUnAnalysisCrashUsers = new JiraAnalysisCrashUsers;
    m_pHighCrashUsers = new JiraHighCrashUsers;
    m_pHighCrashCount = new JiraHighCrashCount;
    m_pProductInfo = new JiraProductInfo;
    m_pProductInfo->assign(pProductInfo);

    logingGehPlatForm();
}

/*!
*@brief        析构函数 
*@author       sunjj 2017年12月11日
*/
JiraIntelligentWaringOper::~JiraIntelligentWaringOper()
{
    delete m_pProductInfo;
    m_pProductInfo = nullptr;

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
            oNewUserCrash.push_back(oCrashInfo);
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
            oAllUserCrash.push_back(oCrashInfo);
        }

        //按照崩溃次数由大到小排序
        std::sort(oNewUserCrash.begin(), oNewUserCrash.end(), compareHighLevel);
        std::sort(oAllUserCrash.begin(), oAllUserCrash.end(), compareHighLevel);
        
        JiraCrashRate oTotalRate = todayCrashRateInfo();
        JiraCrashRate oTodayRate = totalCrashRateInfo();

        QString sHtmlPath = htmlFilePath();
        JiraIntelligentWaringOutputHtml oOutputHtml;
        oOutputHtml.setoutputpath(sHtmlPath);
        oOutputHtml.setproductInfo(m_pProductInfo->productInfo());
        oOutputHtml.setproductRate(&oTotalRate, &oTodayRate);
        oOutputHtml.outputHtml(&oAllUserCrash, &oNewUserCrash);

        m_pIntelligentWaringPython->sendEmailToTesters(sHtmlPath, m_pProductInfo->productInfo());
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
    m_sUnAnalysisCrash = m_pIntelligentWaringPython->downloadUnAnalysisCrash(m_pProductInfo);
}

/*!
*@brief        输出报告路径 
*@author       sunjj 2017年12月22日
*@return       QString
*/
QString JiraIntelligentWaringOper::htmlFilePath()
{
    QString sResult = qApp->applicationDirPath() + "/htmlFile/" + m_pProductInfo->productInfo() + ".html";
    if (QFile::exists(sResult))
        QFile::remove(sResult);
    return sResult;
}

/*!
*@brief        登录异常捕获平台 
*@author       sunjj 2017年12月22日
*/
void JiraIntelligentWaringOper::logingGehPlatForm()
{
    std::shared_ptr<GEHInfo> oGehInfo= JiraUserCustomSetting::instance()->gehInfo();
    LoginInfo oLoginInfo;
    oLoginInfo.sUserName = oGehInfo->sUserName;
    oLoginInfo.sPassword = oGehInfo->sPassword;
    m_pIntelligentWaringPython->LoginPlatform(&oLoginInfo);
}

/*!
*@brief        今天崩溃数据统计 
*@author       sunjj 2017年12月23日
*@return       JiraCrashRate
*/
JiraCrashRate JiraIntelligentWaringOper::todayCrashRateInfo()
{
    return calcCrashRateInfo(true);
}

/*!
*@brief        整体崩溃数据统计 
*@author       sunjj 2017年12月23日
*@return       JiraCrashRate
*/
JiraCrashRate JiraIntelligentWaringOper::totalCrashRateInfo()
{
    return calcCrashRateInfo(false);
}

/*!
*@brief        计算版本健康状况 
*@author       sunjj 2017年12月23日
*@param[in]    bool bToday
*@return       JiraCrashRate
*/
JiraCrashRate JiraIntelligentWaringOper::calcCrashRateInfo(bool bToday)
{
    JiraCrashRate oCrashRate;
    QString sBasicInfo = m_pIntelligentWaringPython->versionBasicInfo(m_pProductInfo, bToday);
    QJsonDocument oJsonDocument_total = QJsonDocument::fromJson(sBasicInfo.toUtf8());
    QJsonObject oAggregationsObj_total = oJsonDocument_total.object().value(strAggregations).toObject();

    QJsonObject oUserObj_total = oAggregationsObj_total.value(strUsers).toObject();
    oCrashRate.nUserCount = oUserObj_total.value(strValue).toVariant().toInt();
    QJsonObject oHitsObj_total = oJsonDocument_total.object().value(strHits).toObject();
    oCrashRate.nStartCount = oHitsObj_total.value(strTotal).toVariant().toInt();

    QString sCrashInfo = m_pIntelligentWaringPython->versionCrashInfo(m_pProductInfo, bToday);
    QJsonDocument oJsonDocument_crash = QJsonDocument::fromJson(sCrashInfo.toUtf8());
    QJsonObject oAggregationsObj_crash = oJsonDocument_crash.object().value(strAggregations).toObject();

    QJsonObject oUserObj_crash = oAggregationsObj_crash.value(strUsers).toObject();
    oCrashRate.nCrashUserCount = oUserObj_crash.value(strValue).toVariant().toInt();
    QJsonObject oHitsObj_crash = oJsonDocument_crash.object().value(strHits).toObject();
    oCrashRate.nCrashCount = oHitsObj_crash.value(strTotal).toVariant().toInt();

    oCrashRate.sCrashRate = (oCrashRate.nStartCount == 0)?  "0" : 
        (QString::number(((double)oCrashRate.nCrashCount / (double)oCrashRate.nStartCount) * 100, 'f', 2) + "%");
    oCrashRate.sUserRate = (oCrashRate.nUserCount == 0)? "0" : 
        (QString::number(((double)oCrashRate.nCrashUserCount / (double)oCrashRate.nUserCount) * 100, 'f', 2) + "%");
    return oCrashRate;
}

