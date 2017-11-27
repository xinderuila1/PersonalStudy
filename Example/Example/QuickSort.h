#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "gtest/gtest.h"

//////////////////////////////////////////////////////////////////////////
//快速排序：不稳定排序。通过选取关键字，一趟排序过后分割成独立的两部分子序列，
//左侧子序列值小于关键字，右侧子序列值大于关键字,重复执行左右子序列，直到整个序列有序。
//时间复杂度：平均O(nlogN) 最优O(nlogN) 最差O(n2)
//空间复杂度：平均O(logN)
//中枢轴取值：取数据末尾值

namespace QuickSort
{
    void swapArray(int *pArray, int nStart, int nEnd)
    {
        int nTemp = pArray[nStart];
        pArray[nStart] = pArray[nEnd];
        pArray[nEnd] = nTemp;
    }

    int partition(int *pArray, int nStart, int nEnd)
    {
        int nKeyValue = pArray[nEnd];
        while (nStart < nEnd)
        {
            while (nStart < nEnd && pArray[nStart] <= nKeyValue)
                ++nStart;
            swapArray(pArray, nStart, nEnd);

            while (nStart < nEnd && pArray[nEnd] >= nKeyValue)
                --nEnd;
            swapArray(pArray, nStart, nEnd);
        }
        return nEnd;
    }

    void qSort(int *pArray, int nStart, int nEnd)
    {
        if (nStart < nEnd)
        {
            int nPartition = partition(pArray, nStart, nEnd);
            qSort(pArray, nStart, nPartition - 1);
            qSort(pArray, nPartition + 1, nEnd);
        }
    }

    //快速排序核心算法
    void quickSort(int *pArray, int nlength)
    {
        if (!pArray)
        {
            throw std::runtime_error("pArray is nullptr");
            return;
        }
        qSort(pArray, 0, nlength - 1);
    }
}

#endif