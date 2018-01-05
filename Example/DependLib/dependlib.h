#ifndef DEPENDLIB_H
#define DEPENDLIB_H

#include <QtWidgets/QDialog>
#include "ui_dependlib.h"

class DependLib : public QDialog
{
    Q_OBJECT

public:
    DependLib(QWidget *parent = 0);
    ~DependLib();

private:
    Ui::DependLibClass ui;

private:
    void calcDependLib();
};

#endif // DEPENDLIB_H
