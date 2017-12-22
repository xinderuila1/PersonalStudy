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
    Py_DECREF(m_pInitialiseMethod);
    Py_DECREF(m_pWarningOper);
    Py_DECREF(m_pWarningClass); 

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
    PyArg_Parse(pReturn, "s", &pResult);
    QString sResult(pResult);
    Py_DECREF(pReturn);
    return sResult;
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
    Py_DECREF(pReturn);
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

