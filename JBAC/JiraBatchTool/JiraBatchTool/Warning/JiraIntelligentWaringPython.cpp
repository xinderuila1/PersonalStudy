#include "JiraIntelligentWaringPython.h"

/*!
*@brief        构造函数 
*@author       sunjj 2017年12月11日
*/
JiraIntelligentWaringPython::JiraIntelligentWaringPython()
{
    Py_Initialize();

    m_pModule = PyImport_ImportModule("WarningCrashOper");
    m_pWarningClass = PyObject_GetAttrString(m_pModule, "WarningCrashOper");
    m_pWarningOper = PyInstance_New(m_pWarningClass, NULL, NULL);
}

/*!
*@brief        析构函数 
*@author       sunjj 2017年12月11日
*/
JiraIntelligentWaringPython::~JiraIntelligentWaringPython()
{
    FreeAndNilPyObject(m_pInitialiseMethod);
    FreeAndNilPyObject(m_pWarningOper);
    FreeAndNilPyObject(m_pWarningClass); 

    Py_Finalize();
}

/*!
*@brief        下载未分析的Crash 
*@author       sunjj 2017年12月11日
*/
QString JiraIntelligentWaringPython::downloadUnAnalysisCrash(JiraProductInfo* pProductInfo)
{
    char* pResult;
    PyObject *pReturn = PyObject_CallMethod(m_pWarningOper, "downloadUnAnalysisCrash", "ss", 
        pProductInfo->sProductKey.toStdString().c_str(), 
        pProductInfo->sVersionId.toStdString().c_str());
    if (pReturn)
    {
        PyArg_Parse(pReturn, "s", &pResult);
        QString sResult(pResult);
        FreeAndNilPyObject(pReturn);
        return sResult;
    }
    return "";
}

/*!
*@brief        给测试人员发送邮件 
*@author       sunjj 2017年12月11日
*@return       bool
*/
void JiraIntelligentWaringPython::sendEmailToTesters(const QString& sHtmlPath, const QString& sHeader)
{
    PyObject *pReturn = PyObject_CallMethod(m_pWarningOper, "sendEmailToTesters", "ss",
        sHtmlPath.toStdString().c_str(),
        sHeader.toStdString().c_str());
    FreeAndNilPyObject(pReturn);
}

/*!
*@brief        登录异常捕获平台 
*@author       sunjj 2017年12月22日
*@param[in]    LoginInfo* pLogInfo
*/
void JiraIntelligentWaringPython::LoginPlatform(LoginInfo* pLogInfo)
{
    m_pInitialiseMethod = PyObject_CallMethod(m_pWarningOper, "initialise", "ss", 
        pLogInfo->sUserName.toStdString().c_str(), 
        pLogInfo->sPassword.toStdString().c_str());
}

/*!
*@brief        版本健康基本情况（启动次数，用户数） 
*@author       sunjj 2017年12月23日
*@param[in]    bool bCurday
*@return       QString
*/
QString JiraIntelligentWaringPython::versionBasicInfo(JiraProductInfo* pProductInfo, bool bCurday)
{
    char* pResult;
    PyObject *pReturn = PyObject_CallMethod(m_pWarningOper, "versionBasicInfo", "sss", 
        pProductInfo->sProductKey.toStdString().c_str(), 
        pProductInfo->sVersionName.toStdString().c_str(),
        (bCurday? "" : "beginDay"));
    PyArg_Parse(pReturn, "s", &pResult);
    QString sResult(pResult);
    FreeAndNilPyObject(pReturn);
    return sResult;
}

/*!
*@brief        版本崩溃状况（崩溃次数，影响用户数） 
*@author       sunjj 2017年12月23日
*@param[in]    bool bCurday
*@return       String
*/
QString JiraIntelligentWaringPython::versionCrashInfo(JiraProductInfo* pProductInfo, bool bCurday)
{
    char* pResult;
    PyObject *pReturn = PyObject_CallMethod(m_pWarningOper, "versionCrashInfo", "sss", 
        pProductInfo->sProductKey.toStdString().c_str(), 
        pProductInfo->sVersionName.toStdString().c_str(),
        (bCurday? "" : "beginDay"));
    PyArg_Parse(pReturn, "s", &pResult);
    QString sResult(pResult);
    FreeAndNilPyObject(pReturn);
    return sResult;
}

