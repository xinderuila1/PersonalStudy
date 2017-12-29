#include "JiraIntelligentWaringPython.h"

/*!
*@brief        ���캯�� 
*@author       sunjj 2017��12��11��
*/
JiraIntelligentWaringPython::JiraIntelligentWaringPython()
{
    Py_Initialize();

    m_pModule = PyImport_ImportModule("WarningCrashOper");
    m_pWarningClass = PyObject_GetAttrString(m_pModule, "WarningCrashOper");
    m_pWarningOper = PyInstance_New(m_pWarningClass, NULL, NULL);
}

/*!
*@brief        �������� 
*@author       sunjj 2017��12��11��
*/
JiraIntelligentWaringPython::~JiraIntelligentWaringPython()
{
    FreeAndNilPyObject(m_pInitialiseMethod);
    FreeAndNilPyObject(m_pWarningOper);
    FreeAndNilPyObject(m_pWarningClass); 

    Py_Finalize();
}

/*!
*@brief        ����δ������Crash 
*@author       sunjj 2017��12��11��
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
*@brief        ��������Ա�����ʼ� 
*@author       sunjj 2017��12��11��
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
*@brief        ��¼�쳣����ƽ̨ 
*@author       sunjj 2017��12��22��
*@param[in]    LoginInfo* pLogInfo
*/
void JiraIntelligentWaringPython::LoginPlatform(LoginInfo* pLogInfo)
{
    m_pInitialiseMethod = PyObject_CallMethod(m_pWarningOper, "initialise", "ss", 
        pLogInfo->sUserName.toStdString().c_str(), 
        pLogInfo->sPassword.toStdString().c_str());
}

/*!
*@brief        �汾������������������������û����� 
*@author       sunjj 2017��12��23��
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
*@brief        �汾����״��������������Ӱ���û����� 
*@author       sunjj 2017��12��23��
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

