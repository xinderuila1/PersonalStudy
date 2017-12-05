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

//////////////////////////////////////////////////////////////////////////
//二叉树镜像
#include "OfferCase/MirrorBinaryTree.h"
#include "BaseData/BinaryTree.h"

TEST(BINARYTREE, MIRRORBINARYTREE)
{
    BinaryTreeNode<int>* pLeft1 = new BinaryTreeNode<int>;
    pLeft1->pLeftTreeNode = pLeft1->pRightTreeNode = nullptr;
    pLeft1->value = 30;

    BinaryTreeNode<int>* pRight1 = new BinaryTreeNode<int>;
    pRight1->pLeftTreeNode = pRight1->pRightTreeNode = nullptr;
    pRight1->value = 40;

    BinaryTreeNode<int>* pRoot = new BinaryTreeNode<int>;
    BinaryTreeNode<int>* pLeft = new BinaryTreeNode<int>;
    pLeft->pLeftTreeNode = pLeft1;
    pLeft->pRightTreeNode = nullptr;
    pLeft->value = 60;

    BinaryTreeNode<int>* pRight = new BinaryTreeNode<int>;
    pRight->pLeftTreeNode = pRight1;
    pRight->pRightTreeNode = nullptr;
    pRight->value = 80;

    pRoot->pLeftTreeNode = pLeft;
    pRoot->pRightTreeNode = pRight;
    pRoot->value = 100;

    std::vector<int> oResult;
    preOrder_Recursion(pRoot, oResult);
    printf("preOrder: %s", binaryTreeToString(oResult).c_str());
    NEW_LINE_WRAP

    oResult.clear();
    midOrder_Recursion(pRoot, oResult);
    printf("midOrder: %s", binaryTreeToString(oResult).c_str());
    NEW_LINE_WRAP

    oResult.clear();
    postOrder_Recursion(pRoot, oResult);
    printf("postOrder: %s", binaryTreeToString(oResult).c_str());
    NEW_LINE_WRAP

    oResult.clear();
    levelOrder(pRoot, oResult);
    printf("levelOrder: %s", binaryTreeToString(oResult).c_str());
    NEW_LINE_WRAP

    mirrorBinaryTree_Recursion(pRoot);
    mirrorBinaryTree_Circle(pRoot);
    EXPECT_EQ(1, 2);
}
#endif