#include "JiraBatchTool.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include "Analysis/JiraBatchCrashInfoOper.h"

int main(int argc, char *argv[])
{
    QApplication oApp(argc, argv);

    QStringList oArguments = QCoreApplication::arguments();
    if (oArguments.size() >= 2)
    {
        QString sJql = oArguments.at(1);
        JiraBatchCrashInfoOper().parseCrashInfo(sJql, false);
        std::cout << "Sccuess ^_^" << std::endl;
        return 1;
    }
    else
    {
        JiraBatchTool w;
        w.show();
        return oApp.exec();
    }
}
