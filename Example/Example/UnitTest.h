#ifndef UNITTEST_H
#define UNITTEST_H

//////////////////////////////////////////////////////////////////////////
//单元测试

//////////////////////////////////////////////////////////////////////////
//排序算法类
#include "QuickSort.h"
#include "CommonFunction.h"

//快速排序
TEST(SORT, QuickSortTest)
{
    int aArray[10] = {10, 30, 15, 8, 40, 1, 3, 60, 2, 18};
    QuickSort::quickSort(aArray, 10);
    EXPECT_STREQ("1:2:3:8:10:15:18:30:40:60:", arrryToStr(aArray, 10).c_str());
}

#endif