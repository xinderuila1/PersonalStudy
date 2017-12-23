#ifndef JIRAINTELLEGENTWARNINGOUTPUTHTML_H
#define JIRAINTELLEGENTWARNINGOUTPUTHTML_H

/*!
*@file     输出结果数据到html
*@brief    
*@author   sunjj
*@date     2017年12月21日
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include <QFile>
#include "Common/JiraConsts.h"

class JiraIntelligentWaringOutputHtml
{
public:
    void setoutputpath(const QString& sFilePath);
    void setproductInfo(const QString& sProductInfo);
    void setproductRate(JiraCrashRate* pTotalHealthyInfo, JiraCrashRate* pTodayHealthyInfo);
    void outputHtml(JiraWarningCrashContainer* pAllUser, JiraWarningCrashContainer*  pNewUser);

private:
    void addHtmlHeadv_Common(QFile* pFile);
    void addHtmlEndv_Common(QFile* pFile);

    void addTableHead_AllUser(QFile* pFile);
    void addTableContent_AllUser(QFile* pFile);
    void addTableEnd_AllUser(QFile* pFile);

    void addTableHead_NewUser(QFile* pFile);
    void addTableContent_NewUser(QFile* pFile);
    void addTableEnd_NewUser(QFile* pFile);

    void addTableHead_ProductInfo(QFile* pFile);
    void addTableContent_ProductInfo(QFile* pFile);
    void addTableEnd_ProductInfo(QFile* pFile);

private:
    QString m_sFilePath;
    QString m_sProductInfo;

    JiraWarningCrashContainer* m_pAllUser;
    JiraWarningCrashContainer* m_pNewUser;

    JiraCrashRate* m_pTotalHealthyInfo;
    JiraCrashRate* m_pTodayHealthyInfo;
};

#endif