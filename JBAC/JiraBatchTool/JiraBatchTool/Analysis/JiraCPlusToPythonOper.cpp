#include "Analysis/JiraCPlusToPythonOper.h"
#include "Common/JiraConsts.h"
#include <QCoreApplication>

/*!
*@brief        ���캯�� 
*@author       sunjj 2017��2��28��
*/
JiraCPlusToPythonOper::JiraCPlusToPythonOper():m_pSearchResult(nullptr)
{
    Py_Initialize();

    m_pModule = PyImport_ImportModule("BatchJira");
    m_pSearchResult = new JiraCrashKeyContainer;
}

/*!
*@brief        �������� 
*@author       sunjj 2017��2��28��
*/
JiraCPlusToPythonOper::~JiraCPlusToPythonOper()
{
    delete m_pSearchResult;
    m_pSearchResult = nullptr;

    Py_Finalize();
}

/*!
*@brief        ���ز�ѯ��� 
*@author       sunjj 2017��2��28��
*@return       JiraCrashKeyContainer*
*/
JiraCrashKeyContainer* JiraCPlusToPythonOper::searchCrashInfo(const QString& sSqlStr)
{
    PyObject *pArgs = Py_BuildValue("(s)", sSqlStr.toStdString().c_str());   
    PyObject *pFunction = PyObject_GetAttrString(m_pModule, "searchCrashInfo");
    PyObject *pReturn = PyEval_CallObject(pFunction, pArgs); 
    if (!pReturn)
        return m_pSearchResult;

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
    Py_DECREF(pFunction);
    Py_DECREF(pArgs);

    return m_pSearchResult;
}

/*!
*@brief        ����Issue 
*@author       sunjj 2017��3��1��
*@param[in]    const QString& sIssue
*/
bool JiraCPlusToPythonOper::searchIssue(const QString& sIssue)
{
    //C�����ַ��� ����ģ����� �ӣ�����
    PyObject *pArgs = Py_BuildValue("(s)", sIssue.toStdString().c_str());  
    PyObject *pFunction = PyObject_GetAttrString(m_pModule, "searchIssue");
    PyObject *pReturn = PyEval_CallObject(pFunction, pArgs); 
    Py_DECREF(pReturn);
    Py_DECREF(pFunction);
    Py_DECREF(pArgs);
    return pReturn != nullptr;
}

/*!
*@brief        ����Issue
*@author       sunjj 2017��3��1��
*/
bool JiraCPlusToPythonOper::analysisIssue()
{
    PyObject *pFunction = PyObject_GetAttrString(m_pModule, "analysisIssue");
    PyObject *pReturn = PyEval_CallObject(pFunction, nullptr); 
    Py_DECREF(pReturn);
    Py_DECREF(pFunction);
    return pReturn != nullptr;
}

/*!
*@brief        ����Issue 
*@author       sunjj 2017��3��1��
*@param[in]    CrashUpdateInfo *pUpdateInfo
*/
bool JiraCPlusToPythonOper::updateIssue(CrashUpdateInfo *pUpdateInfo)
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
    PyObject *pFunction = PyObject_GetAttrString(m_pModule, "updateIssue");
    PyObject *pReturn = PyEval_CallObject(pFunction, pArgs);
    Py_DECREF(pReturn);
    Py_DECREF(pFunction);
    Py_DECREF(pDict);
    Py_DECREF(pArgs);
    return pReturn != nullptr;
}

/*!
*@brief        ����ǰ���� 
*@author       sunjj 2017��3��1��
*/
bool JiraCPlusToPythonOper::beforeAnalysis()
{
    PyObject *pArgs = Py_BuildValue("(s)", qApp->applicationDirPath().toStdString().c_str());  
    PyObject *pFunction = PyObject_GetAttrString(m_pModule, "beforeAnalysis");
    PyObject *pReturn = PyEval_CallObject(pFunction, pArgs); 
    Py_DECREF(pReturn);
    Py_DECREF(pFunction);
    Py_DECREF(pArgs);
    return pReturn != nullptr;
}

/*!
*@brief        ��������� 
*@author       sunjj 2017��3��1��
*/
bool JiraCPlusToPythonOper::afterAnalysis()
{
    PyObject *pFunction = PyObject_GetAttrString(m_pModule, "afterAnalysis");
    PyObject *pReturn = PyEval_CallObject(pFunction, nullptr); 
    Py_DECREF(pReturn);
    Py_DECREF(pFunction);
    return pReturn != nullptr;
}

/*!
*@brief        ���ڱ���ջ��Ϣ 
*@author       sunjj 2017��7��20��
*@param[in]    const QString& sIssue
*@return       bool
*/
bool JiraCPlusToPythonOper::downloadStack(const QString& sIssue)
{
    PyObject *pArgs = Py_BuildValue("(s)", sIssue.toStdString().c_str());  
    PyObject *pFunction = PyObject_GetAttrString(m_pModule, "downloadStack");
    PyObject *pReturn = PyEval_CallObject(pFunction, pArgs); 
    Py_DECREF(pReturn);
    Py_DECREF(pFunction);
    Py_DECREF(pArgs);
    return pReturn != nullptr;
}

/*!
*@brief        ��Ʒ�汾�� 
*@author       sunjj 2017��8��1��
*@return       QString
*/
QString JiraCPlusToPythonOper::productVersion()
{
    char* pFile;
    PyObject *pFunction = PyObject_GetAttrString(m_pModule, "productVersion");
    PyObject *pReturn = PyEval_CallObject(pFunction, nullptr); 
    PyArg_Parse(pReturn, "s", &pFile);
    QString sResult(pFile);
    Py_DECREF(pReturn);
    Py_DECREF(pFunction);
    return sResult;
}
