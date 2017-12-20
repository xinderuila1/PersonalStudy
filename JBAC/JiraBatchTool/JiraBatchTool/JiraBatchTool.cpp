#include "JiraBatchTool.h"
#include "Common/TestCommon.h"
#include "Analysis/JiraBatchCrashInfoOper.h"
#include "Warning/JiraIntelligentWaringThread.h"

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

static const QString sHintInfo = QStringLiteral("核心功能：自动分析Crash所属团队、自动分配Crash、自动添加dump文件、自动添加pdb dump下载地址、支持命令行!\n\n0.内置的前置Jira过滤器为project in (GTJTJ) AND issuetype = Crash AND status in (打开的, 进行中) AND 问题归属= NULL!\n1.工具安装路径不能含中文字符\n2.保证Jira的过滤器语句是正确\n3.输入Jira过滤器语句，点击开始分析即可\n4.查看日志可以看到crash分析的各种信息");

 //问题类型：GTJ产品、Crash类型、未解决的、无归属团队的
static const QString sDefaultJql = QStringLiteral("project in (GTJTJ) AND issuetype = Crash AND status in (打开的, 进行中) AND 问题归属= NULL");

JiraBatchTool::JiraBatchTool(QWidget *parent)
    : QDialog(parent)
{
    //ui.setupUi(this);
    initUI();
    initConnect();
    JiraIntelligentWarningThread* pThread = new JiraIntelligentWarningThread(this);
    pThread->start();
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

    QLabel *pLable = new QLabel(QStringLiteral("Jira过滤器："), this);
    m_pJQLInfo = new QLineEdit;
    //m_pJQLInfo->setText(sDefaultJql);

    QHBoxLayout *pSearchHBox = new QHBoxLayout;
    pSearchHBox->addWidget(pLable);
    pSearchHBox->addWidget(m_pJQLInfo);

    m_pAnalysisBtn = new QPushButton(QStringLiteral("开始分析"), this);
    m_pShowLogInfo = new QPushButton(QStringLiteral("查看日志"), this);
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

//问题解决思路：
//     1.获取问题单issue        
//     2.获取crash链接地址      
//     3.爬虫获取crash详细报告、dmp地址、pdb地址链接路径等   
//     4.分析crash报告，返回归属团队
//     5.更新归属自断  
//     6.添加其他额外信息（dmp地址、pdb地址等）  
