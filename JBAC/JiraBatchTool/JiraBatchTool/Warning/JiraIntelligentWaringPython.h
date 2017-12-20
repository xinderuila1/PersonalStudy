#ifndef JIRAINTELLIGENTWARNINGPYTHON_H
#define JIRAINTELLIGENTWARNINGPYTHON_H

/*!
*@file     智能预警与python交互类
*@brief    
*@author   sunjj
*@date     2017年12月11日
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