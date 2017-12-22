#include "JiraBatchTool.h"
#include "Common/TestCommon.h"
#include "Analysis/JiraBatchCrashInfoOper.h"
#include "Common/JiraUserCustomSetting.h"
#include "Warning/JiraIntelligentWaringThread.h"
#include "Analysis/JiraAnalysisCrashThread.h"

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QtWidgets/QWidget>

#include <QCoreApplication>
#include <QDesktopServices>
#include <QFile>
#include <QUrl>

static const QString sHintInfo = QStringLiteral("���Ĺ��ܣ��Զ�����Crash�����Ŷӡ��Զ�����Crash���Զ����dump�ļ����Զ����pdb dump���ص�ַ��֧��������!\n\n0.���õ�ǰ��Jira������Ϊproject in (GTJTJ) AND issuetype = Crash AND status in (�򿪵�, ������) AND �������= NULL!\n1.���߰�װ·�����ܺ������ַ�\n2.��֤Jira�Ĺ������������ȷ\n3.����Jira��������䣬�����ʼ��������\n4.�鿴��־���Կ���crash�����ĸ�����Ϣ");

 //�������ͣ�GTJ��Ʒ��Crash���͡�δ����ġ��޹����Ŷӵ�
static const QString sDefaultJql = QStringLiteral("project in (GTJTJ) AND issuetype = Crash AND status in (�򿪵�, ������) AND �������= NULL");

#include "Warning/JiraIntelligentWaringOutputHtml.h"


JiraBatchTool::JiraBatchTool(QWidget *parent)
    : QDialog(parent)
{
//     initUI();
//     initConnect();
    initUIForm();
    JiraUserCustomSetting::instance();
    initAutoWarningOper();
    initAutoBelongOper();
}

JiraBatchTool::~JiraBatchTool()
{

}

void JiraBatchTool::initUI()
{
    QVBoxLayout* pMainVBox = new QVBoxLayout;
    QWidget *pMainWidget = new QWidget;
    QVBoxLayout *pMainBox = new QVBoxLayout;
    pMainWidget->setLayout(pMainBox);
    pMainVBox->addWidget(pMainWidget);
    setLayout(pMainVBox);

    QLabel *pLable = new QLabel(QStringLiteral("Jira��������"), this);
    m_pJQLInfo = new QLineEdit;
    //m_pJQLInfo->setText(sDefaultJql);

    QHBoxLayout *pSearchHBox = new QHBoxLayout;
    pSearchHBox->addWidget(pLable);
    pSearchHBox->addWidget(m_pJQLInfo);

    m_pAnalysisBtn = new QPushButton(QStringLiteral("��ʼ����"), this);
    m_pShowLogInfo = new QPushButton(QStringLiteral("�鿴��־"), this);
    QHBoxLayout *pOerHBox = new QHBoxLayout;
    pOerHBox->addWidget(m_pAnalysisBtn);
    pOerHBox->addWidget(m_pShowLogInfo);

    QTextEdit *pHintInfo = new QTextEdit;
    pHintInfo->setReadOnly(true);
    pHintInfo->setText(sHintInfo);

    pMainBox->addLayout(pSearchHBox);
    pMainBox->addLayout(pOerHBox);
    pMainBox->addWidget(pHintInfo);

    setWindowTitle(QStringLiteral("JBAC  by sunjj"));
    resize(800, 300);
}

void JiraBatchTool::initConnect()
{
    connect(m_pAnalysisBtn, SIGNAL(clicked()), this, SLOT(beginAnalysis()));
    connect(m_pShowLogInfo, SIGNAL(clicked()), this, SLOT(showLogInfo()));
}

void JiraBatchTool::beginAnalysis()
{
    QString sUserJQL = m_pJQLInfo->text().trimmed();
    QString sSearchJQL = sUserJQL.isEmpty()? sDefaultJql : sDefaultJql + " AND " + sUserJQL;
    JiraBatchCrashInfoOper().parseCrashInfo(sSearchJQL);
}

void JiraBatchTool::showLogInfo()
{
    QString sLogInfo = qApp->applicationDirPath() + "/logInfo/analysisCrashLog.ini";
     if (QFile::exists(sLogInfo))
         QDesktopServices::openUrl(QUrl(sLogInfo, QUrl::TolerantMode));
}

/*!
*@brief        ��ʼ���Զ�Ԥ������ 
*@author       sunjj 2017��12��22��
*/
void JiraBatchTool::initAutoWarningOper()
{
    std::shared_ptr<ProductInfo> oProductInfo = JiraUserCustomSetting::instance()->productInfo();
    std::shared_ptr<JiraWarningVersionContainer> oWarningVersionContainer = JiraUserCustomSetting::instance()->warningVersionContainer();
    for (auto pIter = oWarningVersionContainer->begin(); pIter != oWarningVersionContainer->end(); ++pIter)
    {
        YYNeedWarningVersion oNeedWarningVersion = *pIter;
        JiraProductInfo oNeedWarningProduct;
        oNeedWarningProduct.sVersionName = oNeedWarningVersion.sVersionName;
        oNeedWarningProduct.sVersionId = oNeedWarningVersion.sVersionId;
        oNeedWarningProduct.sProductKey = oProductInfo->sProductKey;
        oNeedWarningProduct.sProductName = oProductInfo->sProductName;
        JiraIntelligentWarningThread* pThread = new JiraIntelligentWarningThread(&oNeedWarningProduct, this);
        pThread->start();
    }
}

/*!
*@brief        �Զ���������� 
*@author       sunjj 2017��12��22��
*/
void JiraBatchTool::initAutoBelongOper()
{
    JiraAnalysisCrashThread* pThread = new JiraAnalysisCrashThread;
    pThread->start();
}

/*!
*@brief        ��ʼ������ 
*@author       sunjj 2017��12��22��
*/
void JiraBatchTool::initUIForm()
{
    QHBoxLayout* pMainHBox = new QHBoxLayout;
    QLabel* pLable = new QLabel;
    pLable->setText(QStringLiteral("��̨������... ^_^ ��������� ^_^"));
    pMainHBox->addWidget(pLable);
    setLayout(pMainHBox);

    setWindowTitle(QStringLiteral("JBAC(Plus)  by sunjj"));
    resize(300, 100);
}

//������˼·��
//     1.��ȡ���ⵥissue        
//     2.��ȡcrash���ӵ�ַ      
//     3.�����ȡcrash��ϸ���桢dmp��ַ��pdb��ַ����·����   
//     4.����crash���棬���ع����Ŷ�
//     5.���¹����Զ�  
//     6.�������������Ϣ��dmp��ַ��pdb��ַ�ȣ�  
