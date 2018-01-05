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
*@brief        计算依赖的库 
*@author       sunjj 2018年1月5日
*/
void DependLib::calcDependLib()
{
    NeedSearchPaths oNeedSearchPaths;

    FindDependLibOper oFindDependLibOper;
    oFindDependLibOper.initNeedSearchPath(oNeedSearchPaths);
    DependLibRelation* pDependLibContainer = oFindDependLibOper.calcDependLib();
    oFindDependLibOper.outputJsonFile("");
}
