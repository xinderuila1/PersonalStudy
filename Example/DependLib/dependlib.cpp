#include "dependlib.h"
#include "FindDependLibOper.h"

DependLib::DependLib(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    calcDependLib();
}

DependLib::~DependLib()
{

}

/*!
*@brief        ���������Ŀ� 
*@author       sunjj 2018��1��5��
*/
void DependLib::calcDependLib()
{
    NeedSearchPaths oNeedSearchPaths;

    FindDependLibOper oFindDependLibOper;
    oFindDependLibOper.initNeedSearchPath(oNeedSearchPaths);
    DependLibRelation* pDependLibContainer = oFindDependLibOper.calcDependLib();
    oFindDependLibOper.outputJsonFile("");
}
