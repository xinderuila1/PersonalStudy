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
*@brief        ���캯�� 
*@author       sunjj 2017��12��11��
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
*@brief        �������� 
*@author       sunjj 2017��12��11��
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
*@brief        ͳ��δ�������û���Ϣ
*@author       sunjj 2017��12��11��
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
*@brief        ����δ�����б�
*@author       sunjj 2017��12��11��
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
*@brief        ������ǰ���� 
*@author       sunjj 2017��12��11��
*/
void JiraIntelligentWaringOper::analysisCurrentCrash()
{
    for (auto pNewDevicedIDIter = m_pUnAnalysisCrashUsers->begin(); pNewDevicedIDIter != m_pUnAnalysisCrashUsers->end(); ++pNewDevicedIDIter)
    {
        QString sNewDevicedID = pNewDevicedIDIter->first;
        std::map<qint64, QString> oNewCrashTimeMap = pNewDevicedIDIter->second;
        //ĳ�豸���ܹ���������
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

            //�ϴα�������
            int nOldCrashCount = oOldCrashTimeMap.size();
            //��ǰ��������
            int nNewCrashCount = oNewCrashTimeMap.size();
            //�����������
            int nDifferCrashCount = nNewCrashCount - nOldCrashCount;
            //����Crash�Ƿ���ʾ������Ա
            bool bNeedAskTesters = nNewCrashCount >= MIN_CRASH_COUNT && nDifferCrashCount > 0;
            //������¼��
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
*@brief        �Ƚϱ������� 
*@author       sunjj 2017��12��11��
*@param[in]    const QJsonObject& oFirstObj
*@param[in]    const QJsonObject& oSecondObj
*@return       bool
*/
bool compareHighLevel(const QJsonObject& oFirstObj, const QJsonObject& oSecondObj)
{
    int nFirstValue = oFirstObj.value(QStringLiteral("b_�豸��������")).toVariant().toInt();
    int nSecondValue = oSecondObj.value(QStringLiteral("b_�豸��������")).toVariant().toInt();
    return nFirstValue > nSecondValue;
}

/*!
*@brief        �������Ա�����ʼ� 
*@author       sunjj 2017��12��11��
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
            oJsonObject.insert(QStringLiteral("a_�豸ID"), sDevicedId);
            oJsonObject.insert(QStringLiteral("b_�豸��������"), nAllCrashCount);
            oJsonObject.insert(QStringLiteral("c_����������Ŀ"), nNewCrashCount);
            oJsonObject.insert(QStringLiteral("d_ĩ�α���ʱ��"), sLastCrashTime);
            oJsonObject.insert(QStringLiteral("e_������������"), sDescription);
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
*@brief        ����ǰ 
*@author       sunjj 2017��12��11��
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
*@brief        ������ 
*@author       sunjj 2017��12��11��
*/
void JiraIntelligentWaringOper::afterAnalysis()
{
    m_pHighCrashCount->clear();
    m_pHighCrashUsers->clear();
    m_pUnAnalysisCrashUsers->clear();
    m_sUnAnalysisCrash.clear();
}

/*!
*@brief        �������е�Crash�ļ� 
*@author       sunjj 2017��12��11��
*/
void JiraIntelligentWaringOper::downloadUnAnalysisCrash()
{
    m_sUnAnalysisCrash = m_pIntelligentWaringPython->downloadUnAnalysisCrash();
}

