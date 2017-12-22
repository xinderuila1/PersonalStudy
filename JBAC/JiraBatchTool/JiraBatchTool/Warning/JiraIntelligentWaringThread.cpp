#include "JiraIntelligentWaringThread.h"
#include "JiraIntelligentWaringOper.h"
#include "Common/JiraUserCustomSetting.h"

/*!
*@brief        ���캯�� 
*@author       sunjj 2017��12��11��
*@param[in]    QObject *parent (= 0)
*/
JiraIntelligentWarningThread::JiraIntelligentWarningThread(JiraProductInfo* pProductInfo, QObject *parent /*= 0*/)
    :QThread(parent), m_nInterval(0)
{
    m_nInterval = JiraUserCustomSetting::instance()->autoWarning()->nTimeInterval;
    m_pIntelligentWaringOper = new JiraIntelligentWaringOper(pProductInfo);
    m_pCurrentDay = QDate::currentDate();
}


/*!
*@brief        �������� 
*@author       sunjj 2017��12��11��
*/
JiraIntelligentWarningThread::~JiraIntelligentWarningThread()
{
    delete m_pIntelligentWaringOper;
    m_pIntelligentWaringOper = nullptr;
}


/*!
*@brief        �߳�ִ�к���
*@author       sunjj 2017��12��11��
*/
void JiraIntelligentWarningThread::run()
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(collectUserInfo()), Qt::QueuedConnection);
    m_pTimer->start(1000 * 60 * m_nInterval);
    //m_pTimer->start(1000 * 10);
    exec();
}

/*!
*@brief        �����û���Ϣ 
*@author       sunjj 2017��12��11��
*/
void JiraIntelligentWarningThread::collectUserInfo()
{
    //��һ��Ļ������ԭʼ����
    bool bClear = m_pCurrentDay.daysTo(QDate::currentDate()) == 1;
    if (bClear)
        m_pCurrentDay = QDate::currentDate();

    m_pIntelligentWaringOper->analysisCurDayCrash(bClear);
}
