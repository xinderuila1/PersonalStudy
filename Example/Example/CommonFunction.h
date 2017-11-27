#ifndef COMMONFUNCTION_H 
#define COMMONFUNCTION_H

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

#endif