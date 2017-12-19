#ifndef TESTCOMMON_H
#define TESTCOMMON_H

/*!
*@file     Qt python ����������
*@brief    
*@author   sunjj
*@date     2017��2��26��
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include <iostream>  
#include <Python.h>  
#include <pythonrun.h>
#include <QDebug>

using namespace std;  

//�������"Hello World"����  
void HelloWorld()  
{  
    Py_Initialize();              //ʹ��python֮ǰ��Ҫ����Py_Initialize();����������г�ʼ��  
    PyObject * pModule = NULL;    //��������  
    PyObject * pFunc = NULL;      //��������  
    pModule =PyImport_ImportModule("123");              //������Ҫ���õ�Python�ļ���  
    pFunc= PyObject_GetAttrString(pModule, "HelloWorld");   //������Ҫ���õĺ�����  
    PyEval_CallObject(pFunc, NULL);                         //���ú���,NULL��ʾ����Ϊ��  
    Py_Finalize();                //����Py_Finalize,�����Py_Initialize���Ӧ��.  
}  

//����Add����,������int�Ͳ���  
void Add()  
{  
    Py_Initialize();  

    PyObject * pModule = NULL;      
    PyObject * pFunc = NULL;        
    pModule =PyImport_ImportModule("123");      //Test001:Python�ļ���  
    pFunc= PyObject_GetAttrString(pModule, "add");  //Add:Python�ļ��еĺ�����  

    //��������:  
    PyObject *pArgs = PyTuple_New(2);               //�������õĲ������ݾ�����Ԫ�����ʽ�����,2��ʾ��������  
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", 5));//0---���  i��ʾ����int�ͱ���  
    PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", 7));//1---���  

    //����ֵ  
    PyObject *pReturn = NULL;  
    pReturn = PyEval_CallObject(pFunc, pArgs);      //���ú���  

    //������ֵת��Ϊint����  
    int result;  
    PyArg_Parse(pReturn, "i", &result);    //i��ʾת����int�ͱ���  
    qDebug() << "5+7 = " << result;  

    Py_Finalize();                  
}  

//�������ݵ�����Ϊ�ֵ�  
void TestTransferDict()  
{  
    Py_Initialize();  

    PyObject * pModule = NULL;      
    PyObject * pFunc = NULL;        
    pModule =PyImport_ImportModule("Test001");      //Test001:Python�ļ���  
    pFunc= PyObject_GetAttrString(pModule, "TestDict"); //Add:Python�ļ��еĺ�����  

    //��������:  
    PyObject *pArgs = PyTuple_New(1);   
    PyObject *pDict = PyDict_New();   //�����ֵ����ͱ���  
    PyDict_SetItemString(pDict, "Name", Py_BuildValue("s", "WangYao")); //���ֵ����ͱ������������  
    PyDict_SetItemString(pDict, "Age", Py_BuildValue("i", 25));         //���ֵ����ͱ������������  
    PyTuple_SetItem(pArgs, 0, pDict);//0---���  ���ֵ����ͱ�����ӵ�����Ԫ����  

    //����ֵ  
    PyObject *pReturn = NULL;  
    pReturn = PyEval_CallObject(pFunc, pArgs);      //���ú���  

    //������ֵ:  
    int size = PyDict_Size(pReturn);  
    qDebug() << "�����ֵ�Ĵ�СΪ: " << size;  
    PyObject *pNewAge = PyDict_GetItemString(pReturn, "Age");  
    int newAge;  
    PyArg_Parse(pNewAge, "i", &newAge);  
    qDebug() << "True Age: " << newAge;  

    Py_Finalize();                  
}  

//������  
void TestClass()  
{  
    Py_Initialize();  

    PyObject * pModule = NULL;      
    PyObject * pFunc = NULL;        
    pModule =PyImport_ImportModule("123");      //Test001:Python�ļ���  
    pFunc= PyObject_GetAttrString(pModule, "TestDict"); //Add:Python�ļ��еĺ�����  

    //��ȡPerson��  
    PyObject *pClassPerson = PyObject_GetAttrString(pModule, "Person");  
    //����Person���ʵ��  
    PyObject *pInstancePerson = PyInstance_New(pClassPerson, NULL, NULL);  

    //���÷���  
    PyObject_CallMethod(pInstancePerson, "greet", "s", "Hello Kitty");   //s��ʾ���ݵ����ַ���,ֵΪ"Hello Kitty"  

    Py_Finalize();            
}

#endif