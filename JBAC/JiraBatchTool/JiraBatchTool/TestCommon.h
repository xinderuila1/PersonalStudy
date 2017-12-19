#ifndef TESTCOMMON_H
#define TESTCOMMON_H

/*!
*@file     Qt python 交互测试类
*@brief    
*@author   sunjj
*@date     2017年2月26日
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include <iostream>  
#include <Python.h>  
#include <pythonrun.h>
#include <QDebug>

using namespace std;  

//调用输出"Hello World"函数  
void HelloWorld()  
{  
    Py_Initialize();              //使用python之前，要调用Py_Initialize();这个函数进行初始化  
    PyObject * pModule = NULL;    //声明变量  
    PyObject * pFunc = NULL;      //声明变量  
    pModule =PyImport_ImportModule("123");              //这里是要调用的Python文件名  
    pFunc= PyObject_GetAttrString(pModule, "HelloWorld");   //这里是要调用的函数名  
    PyEval_CallObject(pFunc, NULL);                         //调用函数,NULL表示参数为空  
    Py_Finalize();                //调用Py_Finalize,这个和Py_Initialize相对应的.  
}  

//调用Add函数,传两个int型参数  
void Add()  
{  
    Py_Initialize();  

    PyObject * pModule = NULL;      
    PyObject * pFunc = NULL;        
    pModule =PyImport_ImportModule("123");      //Test001:Python文件名  
    pFunc= PyObject_GetAttrString(pModule, "add");  //Add:Python文件中的函数名  

    //创建参数:  
    PyObject *pArgs = PyTuple_New(2);               //函数调用的参数传递均是以元组的形式打包的,2表示参数个数  
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", 5));//0---序号  i表示创建int型变量  
    PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", 7));//1---序号  

    //返回值  
    PyObject *pReturn = NULL;  
    pReturn = PyEval_CallObject(pFunc, pArgs);      //调用函数  

    //将返回值转换为int类型  
    int result;  
    PyArg_Parse(pReturn, "i", &result);    //i表示转换成int型变量  
    qDebug() << "5+7 = " << result;  

    Py_Finalize();                  
}  

//参数传递的类型为字典  
void TestTransferDict()  
{  
    Py_Initialize();  

    PyObject * pModule = NULL;      
    PyObject * pFunc = NULL;        
    pModule =PyImport_ImportModule("Test001");      //Test001:Python文件名  
    pFunc= PyObject_GetAttrString(pModule, "TestDict"); //Add:Python文件中的函数名  

    //创建参数:  
    PyObject *pArgs = PyTuple_New(1);   
    PyObject *pDict = PyDict_New();   //创建字典类型变量  
    PyDict_SetItemString(pDict, "Name", Py_BuildValue("s", "WangYao")); //往字典类型变量中填充数据  
    PyDict_SetItemString(pDict, "Age", Py_BuildValue("i", 25));         //往字典类型变量中填充数据  
    PyTuple_SetItem(pArgs, 0, pDict);//0---序号  将字典类型变量添加到参数元组中  

    //返回值  
    PyObject *pReturn = NULL;  
    pReturn = PyEval_CallObject(pFunc, pArgs);      //调用函数  

    //处理返回值:  
    int size = PyDict_Size(pReturn);  
    qDebug() << "返回字典的大小为: " << size;  
    PyObject *pNewAge = PyDict_GetItemString(pReturn, "Age");  
    int newAge;  
    PyArg_Parse(pNewAge, "i", &newAge);  
    qDebug() << "True Age: " << newAge;  

    Py_Finalize();                  
}  

//测试类  
void TestClass()  
{  
    Py_Initialize();  

    PyObject * pModule = NULL;      
    PyObject * pFunc = NULL;        
    pModule =PyImport_ImportModule("123");      //Test001:Python文件名  
    pFunc= PyObject_GetAttrString(pModule, "TestDict"); //Add:Python文件中的函数名  

    //获取Person类  
    PyObject *pClassPerson = PyObject_GetAttrString(pModule, "Person");  
    //创建Person类的实例  
    PyObject *pInstancePerson = PyInstance_New(pClassPerson, NULL, NULL);  

    //调用方法  
    PyObject_CallMethod(pInstancePerson, "greet", "s", "Hello Kitty");   //s表示传递的是字符串,值为"Hello Kitty"  

    Py_Finalize();            
}

#endif