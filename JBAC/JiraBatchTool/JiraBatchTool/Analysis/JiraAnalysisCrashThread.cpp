#include "Analysis/JiraAnalysisCrashThread.h"
#include "Analysis/JiraBatchCrashInfoOper.h"
#include "Common/JiraUserCustomSetting.h"

/*!
*@brief        构造函数 
*@author       sunjj 2017年12月22日
*@param[in]    QObject *parent (= 0)
*/
JiraAnalysisCrashThread::JiraAnalysisCrashThread(QObject *parent /*= 0*/)
    :m_pTimer(nullptr), m_pBatchCrashInfoOper(nullptr)
{
    m_pTimer = new QTimer;
    m_nInterval = JiraUserCustomSetting::instance()->autoBelong()->nTimeInterval;
    m_pBatchCrashInfoOper = new JiraBatchCrashInfoOper;
}

/*!
*@brief        析构函数 
*@author       sunjj 2017年12月22日
*/
JiraAnalysisCrashThread::~JiraAnalysisCrashThread()
{
    delete m_pBatchCrashInfoOper;
    m_pBatchCrashInfoOper = nullptr;

    delete m_pTimer;
    m_pTimer = nullptr;
}

/*!
*@brief        线程执行 
*@author       sunjj 2017年12月22日
*/
void JiraAnalysisCrashThread::run()
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(autoAnalysisCrash()), Qt::QueuedConnection);
    m_pTimer->start(1000 * 60 * 60 * m_nInterval);
    //m_pTimer->start(1000 * 10);
    exec();
}

/*!
*@brief        自动分析 
*@author       sunjj 2017年12月22日
*/
void JiraAnalysisCrashThread::autoAnalysisCrash()
{
    QString sSearchSql = JiraUserCustomSetting::instance()->jiraInfo()->sFilterSql;
    sSearchSql += QStringLiteral(" AND 问题归属= NULL");
    m_pBatchCrashInfoOper->parseCrashInfo(sSearchSql);
}

