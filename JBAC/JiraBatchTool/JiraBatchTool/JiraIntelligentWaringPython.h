#ifndef JIRAINTELLIGENTWARNINGPYTHON_H
#define JIRAINTELLIGENTWARNINGPYTHON_H

/*!
*@file     ����Ԥ����python������
*@brief    
*@author   sunjj
*@date     2017��12��11��
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include <Python.h>  
#include <QString>

class JiraIntelligentWaringPython
{
public:
    JiraIntelligentWaringPython();
    ~JiraIntelligentWaringPython();

    QString downloadUnAnalysisCrash();
    bool sendEmailToTesters(const QString& sEmailContent);

private:
    PyObject *m_pModule;
};

#endif