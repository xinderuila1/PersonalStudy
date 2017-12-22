#include "Warning/JiraIntelligentWaringOutputHtml.h"
#include <QTextStream>

static const QString g_sRowHead    = "<tr class=\"NormalRow\">";
static const QString g_sRowEnd     = "</tr>";
static const QString g_sHuangH     = "\n";
static const QString g_sTD_Begin   = " <td>";
static const QString g_sTD_Begin_Center = " <td align=center>";
static const QString g_sTD_End     = "</td>";

/*!
*@brief        输出文件路径
*@author       sunjj 2017年12月21日
*@param[in]    const QString& sFilePath
*/
void JiraIntelligentWaringOutputHtml::setoutputpath(const QString& sFilePath)
{
    m_sFilePath = sFilePath;
}

/*!
*@brief        产品标识 
*@author       sunjj 2017年12月21日
*@param[in]    const QString& sProductInfo
*/
void JiraIntelligentWaringOutputHtml::setproductInfo(const QString& sProductInfo)
{
    m_sProductInfo = sProductInfo;
}

/*!
*@brief        输出html文件 
*@author       sunjj 2017年12月21日
*@param[in]    JiraWarningCrashContainer* pAllUser
*@param[in]    JiraWarningCrashContainer* pNewUser
*/
void JiraIntelligentWaringOutputHtml::outputHtml(JiraWarningCrashContainer* pAllUser, JiraWarningCrashContainer* pNewUser)
{
    m_pAllUser = pAllUser;
    m_pNewUser = pNewUser;

    QFile *pFile = new QFile(m_sFilePath);
    if (pFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        addHtmlHeadv_Common(pFile);
        addTableHead_NewUser(pFile);
        addTableContent_NewUser(pFile);
        addTableEnd_NewUser(pFile);

        addTableHead_AllUser(pFile);
        addTableContent_AllUser(pFile);
        addTableEnd_AllUser(pFile);
        addHtmlEndv_Common(pFile);
    }
    delete pFile;
    pFile = nullptr;
}

/*!
*@brief        新增用户HTML头
*@author       sunjj 2017年12月21日
*@param[in]    QFile* pFile
*/
void JiraIntelligentWaringOutputHtml::addHtmlHeadv_Common(QFile* pFile)
{
    QTextStream logFile(pFile);
    logFile<<"<html>\n";
    logFile<<"<head>\n";
    logFile<<" <META http-equiv=\"Content-Type\" content=\"text/html; charset=UFT-8\">\n";
    logFile<<" <style type=\"text/css\">\n";
    logFile<<"    body { background-color: white; }\n";
    logFile<<"    .Blank { background-color: #FFFFFF;}\n";
    logFile<<"    .StartRow { background-color: #FBDBCB;}\n";
    logFile<<"    .NormalRow { background-color: #FBFBE3;}\n";
    logFile<<"    .LabelRow { background-color: #DFEBFB;} \n";
    logFile<<"    .StartColQD { background-color: #DFF3DF;}\n";
    logFile<<"    .StartColDE { background-color: #E7E3F7;}\n";
    logFile<<" </style>\n";
    logFile<<QStringLiteral("  <title>%1版本</br>当天高频次崩溃用户统计</title>\n").arg(m_sProductInfo);
    logFile<<"</head>\n";
    logFile<<"\n";
    logFile<<"<body>\n";
    logFile<<QStringLiteral("<p align=center><b>%1版本</br>当天高频次崩溃用户统计</b></p>\n").arg(m_sProductInfo);
    logFile<<"\n";
    logFile<<"<hr>\n";
}

/*!
*@brief        新增用户HTML尾 
*@author       sunjj 2017年12月21日
*@param[in]    QFile* pFile
*/
void JiraIntelligentWaringOutputHtml::addHtmlEndv_Common(QFile* pFile)
{
    QTextStream logFile(pFile);
    logFile<<"</tbody>\n";
    logFile<<"</table>\n";
    logFile<<"<br>\n";
    logFile<<"<p align=center style=\"font-size: 11pt\">Copyright (c) 1998-2016 Glodon Corp</p>\n";
    logFile<<"<br>\n";
    logFile<<"</body>\n";
    logFile<<"</html>\n";
}

/*!
*@brief        输出表头 
*@author       sunjj 2017年12月21日
*@param[in]    QFile* pFile
*/
void JiraIntelligentWaringOutputHtml::addTableHead_AllUser(QFile* pFile)
{
    QTextStream logFile(pFile);
    logFile<<"<table border=\"1\" cellpadding=\"6\" cellspacing=\"0\" ";
    logFile<<"style=\"border-collapse: collapse; font-size: 10pt\" bordercolor=\"black\">\n";

    logFile<<"<tr class=\"StartRow\">\n";
    logFile<<QStringLiteral("<td colspan=4><font color=\"red\"><b>今天全部崩溃</b></font></td>\n");
    logFile<<"</tr>\n";
    logFile<<"<tr class=\"LabelRow\">\n";
    logFile<<QStringLiteral("<th width=\"20%\">设备ID</th>\n");
    logFile<<QStringLiteral("<th width=\"12%\">崩溃总数</th>\n");
    logFile<<QStringLiteral("<th width=\"15%\">末次崩溃时间</th>\n");
    logFile<<QStringLiteral("<th>描述</th>\n");
    logFile<<"</tr>\n";
}

/*!
*@brief        输出当前崩溃用户预览 
*@author       sunjj 2017年12月21日
*@param[in]    QFile* pFile
*/
void JiraIntelligentWaringOutputHtml::addTableContent_AllUser(QFile* pFile)
{
    QTextStream logFile(pFile);
    for (auto pIter = m_pAllUser->begin(); pIter != m_pAllUser->end(); ++pIter)
    {
        logFile << g_sRowHead << g_sHuangH;
        JiraWarningCrashInfo oCrashInfo = *pIter;
        logFile << g_sTD_Begin << oCrashInfo.sDeviceID << g_sTD_End << g_sHuangH;
        logFile << g_sTD_Begin_Center << oCrashInfo.nTotalCount << g_sTD_End <<g_sHuangH;
        logFile << g_sTD_Begin << oCrashInfo.sLastCrashTime << g_sTD_End << g_sHuangH;
        logFile << g_sTD_Begin << oCrashInfo.sDetailInfo << g_sTD_End << g_sHuangH;
        logFile << g_sRowEnd << g_sHuangH;
    }
}

/*!
*@brief        添加表头 
*@author       sunjj 2017年12月21日
*@param[in]    QFile* pFile
*/
void JiraIntelligentWaringOutputHtml::addTableEnd_AllUser(QFile* pFile)
{
    QTextStream logFile(pFile);
    logFile<<"<tr>\n";
    logFile<<"<td colspan=4 style=\"font-size: 0pt\"><br></td>\n";
    logFile<<"</tr>\n";
    logFile<<"\n";
}

/*!
*@brief        新增用户表头 
*@author       sunjj 2017年12月21日
*@param[in]    QFile* pFile
*/
void JiraIntelligentWaringOutputHtml::addTableHead_NewUser(QFile* pFile)
{
    QTextStream logFile(pFile);
    logFile<<"<table border=\"1\" cellpadding=\"6\" cellspacing=\"0\" ";
    logFile<<"style=\"border-collapse: collapse; font-size: 10pt\" bordercolor=\"black\">\n";
    logFile<<"<tbody>\n";

   // QTextStream logFile(pFile);
    logFile<<"<tr class=\"StartRow\">\n";
    logFile<<QStringLiteral("<td colspan=5><font color=\"red\"><b>今天新增高频崩溃</b></font></td>\n");
    logFile<<"</tr>\n";
    logFile<<"<tr class=\"LabelRow\">\n";
    logFile<<QStringLiteral("<th width=\"20%\">设备ID</th>\n");
    logFile<<QStringLiteral("<th width=\"12%\">崩溃总数</th>\n");
    logFile<<QStringLiteral("<th width=\"12%\">崩溃新增数</th>\n");
    logFile<<QStringLiteral("<th width=\"15%\">末次崩溃时间</th>\n");
    logFile<<QStringLiteral("<th>描述</th>\n");
    logFile<<"</tr>\n";
}

/*!
*@brief        新增用户表内容 
*@author       sunjj 2017年12月21日
*@param[in]    QFile* pFile
*/
void JiraIntelligentWaringOutputHtml::addTableContent_NewUser(QFile* pFile)
{
    QTextStream logFile(pFile);
    for (auto pIter = m_pNewUser->begin(); pIter != m_pNewUser->end(); ++pIter)
    {
        logFile << g_sRowHead << g_sHuangH;
        JiraWarningCrashInfo oCrashInfo = *pIter;
        logFile << g_sTD_Begin << oCrashInfo.sDeviceID << g_sTD_End << g_sHuangH;
        logFile << g_sTD_Begin_Center << oCrashInfo.nTotalCount << g_sTD_End <<g_sHuangH;
        logFile << g_sTD_Begin_Center << oCrashInfo.nNewCount << g_sTD_End << g_sHuangH;
        logFile << g_sTD_Begin << oCrashInfo.sLastCrashTime << g_sTD_End << g_sHuangH;
        logFile << g_sTD_Begin << oCrashInfo.sDetailInfo << g_sTD_End << g_sHuangH;
        logFile << g_sRowEnd << g_sHuangH;
    }
}

/*!
*@brief        新增用户结尾 
*@author       sunjj 2017年12月21日
*@param[in]    QFile* pFile
*/
void JiraIntelligentWaringOutputHtml::addTableEnd_NewUser(QFile* pFile)
{
    QTextStream logFile(pFile);
    logFile<<"<tr>\n";
    logFile<<"<td colspan=5 style=\"font-size: 0pt\"><br></td>\n";
    logFile<<"</tr>\n";
    logFile<<"</table>\n";
}


