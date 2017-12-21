#include "Analysis/JiraAnalysisPythonOper.h"
#include "Common/JiraConsts.h"
#include <QCoreApplication>

/*!
*@brief        ���캯�� 
*@author       sunjj 2017��2��28��
*/
JiraAnalysisPythonOper::JiraAnalysisPythonOper():m_pSearchResult(nullptr)
{
    Py_Initialize();

    m_pModule = PyImport_ImportModule("AnalysisCrashOper");
    m_pAnalysisClass = PyObject_GetAttrString(m_pModule, "AnalysisCrashOper");
    m_pAnalysisOper = PyInstance_New(m_pAnalysisClass, NULL, NULL);
    
    m_pSearchResult = new JiraCrashKeyContainer;
}

/*!
*@brief        �������� 
*@author       sunjj 2017��2��28��
*/
JiraAnalysisPythonOper::~JiraAnalysisPythonOper()
{
    Py_DECREF(m_pInitialiseMethod);
    Py_DECREF(m_pAnalysisOper);
    Py_DECREF(m_pAnalysisClass); 

    delete m_pSearchResult;
    m_pSearchResult = nullptr;

    Py_Finalize();
}

/*!
*@brief        ��¼ƽ̨
*@author       sunjj 2017��12��21��
*@param[in]    LoginInfo* pLogInfo
*/
void JiraAnalysisPythonOper::LoginPlatform(LoginInfo* pLogInfo)
{
    char* pJiraUrl = const_cast<char*>(pLogInfo->sJiarUrl.toStdString().c_str());
    char* pUserName = const_cast<char*>(pLogInfo->sUserName.toStdString().c_str());
    char* pPassWord = const_cast<char*>(pLogInfo->sPassword.toStdString().c_str());

    m_pInitialiseMethod = PyObject_CallMethod(m_pAnalysisOper, "initialise", "sss", pJiraUrl, pUserName, pPassWord);
}

/*!
*@brief        ���ز�ѯ��� 
*@author       sunjj 2017��2��28��
*@return       JiraCrashKeyContainer*
*/
JiraCrashKeyContainer* JiraAnalysisPythonOper::searchCrashInfo(const QString& sSqlStr)
{
    char* pSearchSql = const_cast<char*>(sSqlStr.toStdString().c_str());
    PyObject *pReturn = PyObject_CallMethod(m_pAnalysisOper, "searchCrashInfo", "s", pSearchSql);
    PyObject *pKey, *pValue;
    Py_ssize_t pos = 0;
    m_pSearchResult->clear();
    while (PyDict_Next(pReturn, &pos, &pKey, &pValue)) 
    {
        QString sCrashKey = PyString_AsString(pKey);
        QString sCrashInfo = PyString_AsString(pValue);
        m_pSearchResult->insert(std::make_pair(sCrashKey, sCrashInfo));
    }  
    Py_DECREF(pReturn);
    return m_pSearchResult;
}

/*!
*@brief        ����Issue 
*@author       sunjj 2017��3��1��
*@param[in]    const QString& sIssue
*/
void JiraAnalysisPythonOper::searchIssue(const QString& sIssue)
{
    char* pIssue = const_cast<char*>(sIssue.toStdString().c_str());
    PyObject *pReturn = PyObject_CallMethod(m_pAnalysisOper, "searchIssue", "s", pIssue);
    Py_DECREF(pReturn);
}

/*!
*@brief        ����Issue
*@author       sunjj 2017��3��1��
*/
void JiraAnalysisPythonOper::analysisIssue()
{
    PyObject *pReturn = PyObject_CallMethod(m_pAnalysisOper, "analysisIssue", nullptr);
    Py_DECREF(pReturn);
}

/*!
*@brief        ����Issue 
*@author       sunjj 2017��3��1��
*@param[in]    CrashUpdateInfo *pUpdateInfo
*/
void JiraAnalysisPythonOper::updateIssue(CrashUpdateInfo *pUpdateInfo)
{
    PyObject *pArgs = PyTuple_New(1);   
    PyObject *pDict = PyDict_New(); 
    PyDict_SetItemString(pDict, "customfield_10801", Py_BuildValue("s", pUpdateInfo->sModuleID.toStdString().c_str()));
    PyDict_SetItemString(pDict, "assignee", Py_BuildValue("s", pUpdateInfo->sDomain.toStdString().c_str()));
    PyDict_SetItemString(pDict, "description", Py_BuildValue("s", pUpdateInfo->sDetailInfo.toStdString().c_str()));
    PyDict_SetItemString(pDict, "autoTest", Py_BuildValue("s", pUpdateInfo->sAutoTest.toStdString().c_str()));
    PyDict_SetItemString(pDict, "remark", Py_BuildValue("s", pUpdateInfo->sRemarks.toStdString().c_str()));
    PyDict_SetItemString(pDict, "dumpUrl", Py_BuildValue("s", pUpdateInfo->sDumpUrl.toStdString().c_str()));
    PyDict_SetItemString(pDict, "highFrequency", Py_BuildValue("s", pUpdateInfo->sHighFrequency.toStdString().c_str()));
    PyTuple_SetItem(pArgs, 0, pDict); 
    PyObject *pReturn = PyObject_CallMethod(m_pAnalysisOper, "updateIssue", "O", pArgs);
    Py_DECREF(pReturn);
    Py_DECREF(pDict);
    Py_DECREF(pArgs);
}

/*!
*@brief        ����ǰ���� 
*@author       sunjj 2017��3��1��
*/
void JiraAnalysisPythonOper::beforeAnalysis()
{
    char* pExepath = const_cast<char*>(qApp->applicationDirPath().toStdString().c_str());
    PyObject *pReturn = PyObject_CallMethod(m_pAnalysisOper, "beforeAnalysis", "s", pExepath);
    Py_DECREF(pReturn);
}

/*!
*@brief        ��������� 
*@author       sunjj 2017��3��1��
*/
void JiraAnalysisPythonOper::afterAnalysis()
{
    PyObject *pReturn = PyObject_CallMethod(m_pAnalysisOper, "afterAnalysis", nullptr);
    Py_DECREF(pReturn);
}

/*!
*@brief        ��Ʒ�汾�� 
*@author       sunjj 2017��8��1��
*@return       QString
*/
QString JiraAnalysisPythonOper::productVersion()
{
    char* pFile;
    PyObject *pReturn = PyObject_CallMethod(m_pAnalysisOper, "productVersion", nullptr);
    PyArg_Parse(pReturn, "s", &pFile);
    QString sResult(pFile);
    Py_DECREF(pReturn);
    return sResult;
}
