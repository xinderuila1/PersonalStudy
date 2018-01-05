#ifndef FINDDEPENDONLIBOPER_H
#define FINDDEPENDONLIBOPER_H

/*!
*@file     ����������
*@brief    
*@author   sunjj
*@date     2018��1��5��
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/

#include <map>
#include <set>
#include <vector>
#include <QString>

using namespace std;

struct DependLibContainer;
typedef std::set<QString> NeedSearchPaths;  //������·��
typedef std::set<QString> DependSelfLib;    //���������⼯��
typedef std::set<QString> DependOtherLib;   //������������
typedef std::set<std::pair<QString, QString>> NeedSearchFiles;  //������ļ�
typedef std::map<QString, std::set<QString>> DependLibMap; //���������ļ�
typedef std::map<QString, DependLibContainer> DependLibRelation; //�����Աȹ�ϵͼ

struct DependLibContainer
{
    DependSelfLib oDependSelfLib;
    DependOtherLib oDependOtherLib;

    void clear()
    {
        oDependSelfLib.clear();
        oDependOtherLib.clear();
    }
};
class QXmlStreamReader;

class FindDependLibOper
{
public:
    FindDependLibOper();
    ~FindDependLibOper();

    void initNeedSearchPath(const NeedSearchPaths& oSearchPaths);
    DependLibRelation* calcDependLib();
    void outputJsonFile(const QString& sFilePath);

private:
    void calcDependSelfLib();
    void calcDependOtherLib();

    void loadNeedSearchFiles(const QString& sSearchDir, NeedSearchFiles& oNeedSearchFiles);
    void loadTotalDependLibs(const NeedSearchFiles& oNeedSearchFiles);

private:
    DependLibMap* m_pDependLibMap;
    DependLibRelation* m_pDependLibRelation;
    NeedSearchPaths m_pNeedSearchPaths;
    QXmlStreamReader *m_pXMLReader;
};
#endif