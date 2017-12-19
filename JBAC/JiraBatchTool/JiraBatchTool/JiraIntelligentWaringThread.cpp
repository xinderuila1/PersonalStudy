#include "JiraIntelligentWaringThread.h"
#include "JiraIntelligentWaringOper.h"

/*!
*@brief        构造函数 
*@author       sunjj 2017年12月11日
*@param[in]    QObject *parent (= 0)
*/
JiraIntelligentWarningThread::JiraIntelligentWarningThread(QObject *parent /*= 0*/)
    :QThread(parent)
{
    m_pIntelligentWaringOper = new JiraIntelligentWaringOper;
    m_pCurrentDay = QDate::currentDate();
}


/*!
*@brief        析构函数 
*@author       sunjj 2017年12月11日
*/
JiraIntelligentWarningThread::~JiraIntelligentWarningThread()
{
    delete m_pIntelligentWaringOper;
    m_pIntelligentWaringOper = nullptr;
}


/*!
*@brief        线程执行函数
*@author       sunjj 2017年12月11日
*/
void JiraIntelligentWarningThread::run()
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(collectUserInfo()), Qt::QueuedConnection);
    m_pTimer->start(1000 * 10);
    exec();
}

/*!
*@brief        汇总用户信息 
*@author       sunjj 2017年12月11日
*/
void JiraIntelligentWarningThread::collectUserInfo()
{
    //隔一天的话，清空原始数据
    bool bClear = m_pCurrentDay.daysTo(QDate::currentDate()) == 1;
    if (bClear)
        m_pCurrentDay = QDate::currentDate();

    //for test
    bClear = true;
    m_pIntelligentWaringOper->analysisCurDayCrash(bClear);
}
