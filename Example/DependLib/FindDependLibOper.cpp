#include "FindDependLibOper.h"
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QXmlStreamReader>
#include <assert.h>

/*!
*@brief        ���캯�� 
*@author       sunjj 2018��1��5��
*/
FindDependLibOper::FindDependLibOper()
    :m_pDependLibRelation(nullptr), m_pXMLReader(nullptr)
{
    m_pXMLReader = new QXmlStreamReader;
    m_pDependLibRelation = new DependLibRelation;
    m_pDependLibMap = new DependLibMap;
}

/*!
*@brief        �������� 
*@author       sunjj 2018��1��5��
*/
FindDependLibOper::~FindDependLibOper()
{
    delete m_pDependLibMap;
    m_pDependLibMap = nullptr;

    delete m_pDependLibRelation;
    m_pDependLibRelation = nullptr;

    delete m_pXMLReader;
    m_pXMLReader = nullptr;
}

/*!
*@brief        ��ʼ��������·������ 
*@author       sunjj 2018��1��5��
*@param[in]    const NeedSearchPaths& oSearchPaths
*/
void FindDependLibOper::initNeedSearchPath(const NeedSearchPaths& oSearchPaths)
{
    m_pNeedSearchPaths = oSearchPaths;
}

/*!
*@brief        �����������ϵ 
*@author       sunjj 2018��1��5��
*@return       DependLibRelation*
*/
DependLibRelation* FindDependLibOper::calcDependLib()
{
    m_pDependLibMap->clear();
    m_pDependLibRelation->clear();

    for (auto pIter = m_pNeedSearchPaths.begin(); pIter != m_pNeedSearchPaths.end(); ++pIter)
    {
        NeedSearchFiles oNeedSearchFiles;
        loadNeedSearchFiles(*pIter, oNeedSearchFiles);
        loadTotalDependLibs(oNeedSearchFiles);
    }

    calcDependSelfLib();
    calcDependOtherLib();

    return m_pDependLibRelation;
}

/*!
*@brief        �������ļ�·��
*@author       sunjj 2018��1��5��
*@param[in]    const QString& sSearchDir
*@param[out]   NeedSearchFiles& oNeedSearchFiles
*/
void FindDependLibOper::loadNeedSearchFiles(const QString& sSearchDir, NeedSearchFiles& oNeedSearchFiles)
{
    //�ļ�·�������
    oNeedSearchFiles.clear();
    QDir oSearchDir(sSearchDir);
    if(!oSearchDir.exists())
        return;

    //��ȡ��ѡ�ļ����͹�����
    QStringList oFileFilter;
    oFileFilter << QString("*.vcxproj");


    //��������������ù�����
    QDirIterator oDirIterator(sSearchDir, oFileFilter, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while(oDirIterator.hasNext())
    {
        oDirIterator.next();
        QFileInfo oFileInfo = oDirIterator.fileInfo();
        if (oFileInfo.isFile())
        {
            std::pair<QString, QString> oPair;
            oPair.first = oFileInfo.baseName();
            oPair.second = oFileInfo.absoluteFilePath();
            oNeedSearchFiles.insert(oPair);
        }
    }
}

/*!
*@brief        �������������Ŀ��ļ�
*@author       sunjj 2018��1��5��
*@param[in]    const NeedSearchFiles& oNeedSearchFiles
*/
void FindDependLibOper::loadTotalDependLibs(const NeedSearchFiles& oNeedSearchFiles)
{
    for (auto pIter = oNeedSearchFiles.begin(); pIter != oNeedSearchFiles.end(); ++pIter)
    {
        QString sFileName = pIter->first;
        QString sFilePath = pIter->second;

        QFile oSearchFile(sFilePath);
        if (!oSearchFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            assert(false);
            continue;
        }

        m_pXMLReader->setDevice(&oSearchFile);
        m_pXMLReader->readNext();

        while (!m_pXMLReader->atEnd())
        {
            if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == "ItemDefinitionGroup" &&
                m_pXMLReader->attributes().value("Condition").toString().contains("Release"))
            {
                while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == "ItemDefinitionGroup"))
                {
                    if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == "AdditionalDependencies") 
                    {
                        std::set<QString> oDependLibSet;
                        QStringList sDependLibList = m_pXMLReader->readElementText().split(";");
                        for (auto pIter = sDependLibList.begin(); pIter != sDependLibList.end(); ++pIter)
                        {
                            if (*pIter != "%(AdditionalDependencies)")
                            {
                                QString sLibName = (*pIter).split(".").at(0);
                                oDependLibSet.insert(sLibName);
                            }
                        }
                        m_pDependLibMap->insert(std::make_pair(sFileName, oDependLibSet));
                        continue;
                    }
                    m_pXMLReader->readNext();
                }
            }
            m_pXMLReader->readNext();
        }
    }
}

/*!
*@brief        ������������Ķ�̬��
*@author       sunjj 2018��1��5��
*/
void FindDependLibOper::calcDependSelfLib()
{
    for (auto pIter = m_pDependLibMap->begin(); pIter != m_pDependLibMap->end(); ++pIter)
    {
        DependLibContainer oDependLibContainer;
        oDependLibContainer.oDependSelfLib = pIter->second;
        m_pDependLibRelation->insert(std::make_pair(pIter->first, oDependLibContainer));
    }
}

/*!
*@brief        ���������Ŀ��������� 
*@author       sunjj 2018��1��5��
*/
void FindDependLibOper::calcDependOtherLib()
{
    auto dependCurLib=[&](const QString& sLibName, DependOtherLib& oDependOtherLib)->void
    {
        for (auto pIter = m_pDependLibMap->begin(); pIter != m_pDependLibMap->end(); ++pIter)
        {
            QString sCurLibName = pIter->first;
            std::set<QString>& oDependLibSet = pIter->second;
            if (oDependLibSet.find(sLibName) != oDependLibSet.end())
                oDependOtherLib.insert(sCurLibName);
        }
    };

    for (auto pIter = m_pDependLibRelation->begin(); pIter != m_pDependLibRelation->end(); ++pIter)
    {
        QString sCurLibName = pIter->first;
        dependCurLib(sCurLibName, pIter->second.oDependOtherLib);
    }
}

/*!
*@brief        ���Json���� 
*@author       sunjj 2018��1��5��
*@param[in]    const QString& sFilePath
*/
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

void FindDependLibOper::outputJsonFile(const QString& sFilePath)
{
    QJsonArray oResult;
    for (auto pFirstIter = m_pDependLibRelation->begin(); pFirstIter != m_pDependLibRelation->end(); ++pFirstIter)
    {
        QString sCurLibName = pFirstIter->first;
        DependLibContainer& oDependLibContainer = pFirstIter->second;

        QJsonArray oDependSelfArray;
        DependSelfLib oDependSelfLib = oDependLibContainer.oDependSelfLib;
        for (auto pIter = oDependSelfLib.begin(); pIter != oDependSelfLib.end(); ++pIter)
        {
            oDependSelfArray.append(QJsonValue(*pIter));
        }

        QJsonArray oDependOtherArray;
        DependSelfLib oDependOtherLib = oDependLibContainer.oDependOtherLib;
        for (auto pIter = oDependOtherLib.begin(); pIter != oDependOtherLib.end(); ++pIter)
        {
            oDependOtherArray.append(QJsonValue(*pIter));
        }

        QJsonObject oLibInfoObj;
        oLibInfoObj.insert(QStringLiteral("0.��Ŀ����"), QJsonValue(sCurLibName));
        oLibInfoObj.insert(QStringLiteral("1.������Ŀ��"), QJsonValue((int)oDependLibContainer.oDependSelfLib.size()));
        oLibInfoObj.insert(QStringLiteral("2.��������Ŀ��"), QJsonValue((int)oDependLibContainer.oDependOtherLib.size()));
         oLibInfoObj.insert(QStringLiteral("3.������Ŀ�б�"), oDependSelfArray);
         oLibInfoObj.insert(QStringLiteral("4.��������Ŀ�б�"), oDependOtherArray);
        oResult.append(oLibInfoObj);
    }

    QJsonDocument oJsonDocument;
    oJsonDocument.setArray(oResult);
    
    QString sJsonResult = oJsonDocument.toJson();
    qDebug() << sJsonResult;
}



