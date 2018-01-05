#ifndef FINDDEPENDONLIBOPER_H
#define FINDDEPENDONLIBOPER_H

/*!
*@file     查找依赖库
*@brief    
*@author   sunjj
*@date     2018年1月5日
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
typedef std::set<QString> NeedSearchPaths;  //待搜索路径
typedef std::set<QString> DependSelfLib;    //自身依赖库集合
typedef std::set<QString> DependOtherLib;   //依赖的其他库
typedef std::set<std::pair<QString, QString>> NeedSearchFiles;  //待检查文件
typedef std::map<QString, std::set<QString>> DependLibMap; //库依赖的文件
typedef std::map<QString, DependLibContainer> DependLibRelation; //依赖对比关系图

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