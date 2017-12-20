#ifndef JIRABATCHTOOL_H
#define JIRABATCHTOOL_H

#include <QtWidgets/QDialog>

class QPushButton;
class QLineEdit;

class JiraBatchTool : public QDialog
{
    Q_OBJECT

public:
    JiraBatchTool(QWidget *parent = 0);
    ~JiraBatchTool();

private slots:
    void beginAnalysis();
    void showLogInfo();

private:
    void initUI();
    void initConnect();

private:

    QPushButton *m_pAnalysisBtn, *m_pShowLogInfo;
    QLineEdit *m_pJQLInfo;
};

#endif // JIRABATCHTOOL_H
