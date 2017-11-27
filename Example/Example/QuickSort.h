#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "gtest/gtest.h"

//////////////////////////////////////////////////////////////////////////
//�������򣺲��ȶ�����ͨ��ѡȡ�ؼ��֣�һ���������ָ�ɶ����������������У�
//���������ֵС�ڹؼ��֣��Ҳ�������ֵ���ڹؼ���,�ظ�ִ�����������У�ֱ��������������
//ʱ�临�Ӷȣ�ƽ��O(nlogN) ����O(nlogN) ���O(n2)
//�ռ临�Ӷȣ�ƽ��O(logN)
//������ȡֵ��ȡ����ĩβֵ

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

    //������������㷨
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