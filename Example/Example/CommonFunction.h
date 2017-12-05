#ifndef COMMONFUNCTION_H 
#define COMMONFUNCTION_H

#include <vector>
#include "BaseData/BinaryTree.h"


#define NEW_LINE_WRAP printf("\n");

//����ת��Ϊ�ַ���
std::string arrryToStr(int *pArray, int nLength)
{
    std::string sResult = "";
    if (!pArray)
    {
        return sResult;
    }
    
    for (int i = 0; i < nLength; ++i)
    {
        sResult += (std::to_string(long double(pArray[i])) + ":");
    }
    return sResult;
}

//������ת��Ϊ�ַ��� �������
std::string binaryTreeToString(const std::vector<int>& oResult)
{
    std::string sResult = "";
    for (auto pIter = oResult.begin(); pIter != oResult.end(); ++pIter)
    {
        sResult += ((std::to_string(long double(*pIter)) + ",")); 
    }
    return sResult;
}

#endif