#include "JiraIntelligentWaringPython.h"

/*!
*@brief        ���캯�� 
*@author       sunjj 2017��12��11��
*/
JiraIntelligentWaringPython::JiraIntelligentWaringPython()
{
    Py_Initialize();

    m_pModule = PyImport_ImportModule("IntelligentWaring");
}

/*!
*@brief        �������� 
*@author       sunjj 2017��12��11��
*/
JiraIntelligentWaringPython::~JiraIntelligentWaringPython()
{
    Py_Finalize();
}

/*!
*@brief        ����δ������Crash 
*@author       sunjj 2017��12��11��
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
*@brief        ��������Ա�����ʼ� 
*@author       sunjj 2017��12��11��
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

