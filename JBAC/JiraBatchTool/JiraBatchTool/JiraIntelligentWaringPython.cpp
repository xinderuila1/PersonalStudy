#include "JiraIntelligentWaringPython.h"

/*!
*@brief        构造函数 
*@author       sunjj 2017年12月11日
*/
JiraIntelligentWaringPython::JiraIntelligentWaringPython()
{
    Py_Initialize();

    m_pModule = PyImport_ImportModule("IntelligentWaring");
}

/*!
*@brief        析构函数 
*@author       sunjj 2017年12月11日
*/
JiraIntelligentWaringPython::~JiraIntelligentWaringPython()
{
    Py_Finalize();
}

/*!
*@brief        下载未分析的Crash 
*@author       sunjj 2017年12月11日
*/
QString JiraIntelligentWaringPython::downloadUnAnalysisCrash()
{
    char* pFile;
    PyObject *pFunction = PyObject_GetAttrString(m_pModule, "downloadUnAnalysisCrash");
    PyObject *pReturn = PyEval_CallObject(pFunction, nullptr); 
    PyArg_Parse(pReturn, "s", &pFile);
    QString sResult(pFile);
    Py_DECREF(pReturn);
    Py_DECREF(pFunction);
    return sResult;
}

/*!
*@brief        给测试人员发送邮件 
*@author       sunjj 2017年12月11日
*@return       bool
*/
bool JiraIntelligentWaringPython::sendEmailToTesters(const QString& sEmailContent)
{
    PyObject *pArgs = Py_BuildValue("(s)", sEmailContent.toStdString().c_str()); 
    PyObject *pFunction = PyObject_GetAttrString(m_pModule, "sendEmailToTesters");
    PyObject *pReturn = PyEval_CallObject(pFunction, pArgs); 
    Py_DECREF(pReturn);
    Py_DECREF(pFunction);
    Py_DECREF(pArgs);
    return pReturn != nullptr;
}

