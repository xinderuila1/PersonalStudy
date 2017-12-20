#ifndef JIRAINTELLIGENTWARNINGOPER_H
#define JIRAINTELLIGENTWARNINGOPER_H

/*!
*@file     ����Ԥ������
*@brief    
*@author   sunjj
*@date     2017��12��11��
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#include <QString>
#include <map>
#include <vector>

class JiraIntelligentWaringPython;
         
typedef std::map<QString, std::map<qint64, QString>> JiraAnalysisCrashUsers; //δ�����û��豸���� �豸ID-����ʱ��-��������
typedef JiraAnalysisCrashUsers JiraHighCrashUsers;                            //���ص��ע�ĸ�Ƶ�α����û�
typedef std::map<QString, std::pair<int, int>> JiraHighCrashCount;            //��������ͳ��

class JiraIntelligentWaringOper
{
public:
    JiraIntelligentWaringOper();
    ~JiraIntelligentWaringOper();

    void analysisCurDayCrash(bool bClear);

private:
    void beforeAnalysis(bool bClear);
    void afterAnalysis();

    void downloadUnAnalysisCrash();
    void loadUnAnalysisCrash();
    void analysisCurrentCrash();
    void sendEmailToTesters();

private:
    JiraIntelligentWaringPython* m_pIntelligentWaringPython;
    JiraAnalysisCrashUsers* m_pUnAnalysisCrashUsers; //δ�����û�
    JiraAnalysisCrashUsers* m_pAnalysisedCrashUsers; //�ѷ����û�
    JiraHighCrashUsers* m_pHighCrashUsers;
    JiraHighCrashCount* m_pHighCrashCount;
    QString m_sUnAnalysisCrash;
};

#endif