#ifndef MIRRORBINARYTREE_H
#define MIRRORBINARYTREE_H

#include <stack>
#include "BaseData/BinaryTree.h"

//////////////////////////////////////////////////////////////////////////
//二叉树镜像

//递归方式
void mirrorBinaryTree_Recursion(BinaryTreeNode<int>* pBinaryTree)
{
    if (!pBinaryTree || (!pBinaryTree->pLeftTreeNode && !pBinaryTree->pRightTreeNode))
    {
        return;
    }

    BinaryTreeNode<int>* pTempNode = pBinaryTree->pLeftTreeNode;
    pBinaryTree->pLeftTreeNode = pBinaryTree->pRightTreeNode;
    pBinaryTree->pRightTreeNode = pTempNode;

    if (pBinaryTree->pLeftTreeNode)
        mirrorBinaryTree_Recursion(pBinaryTree->pLeftTreeNode);

    if (pBinaryTree->pRightTreeNode)
        mirrorBinaryTree_Recursion(pBinaryTree->pRightTreeNode);
};

//循环方式
void mirrorBinaryTree_Circle(BinaryTreeNode<int>* pBinaryTree)
{
    if (!pBinaryTree)
        return;

    std::stack<BinaryTreeNode<int>*> oCircleStack;
    oCircleStack.push(pBinaryTree);
    
    while (!oCircleStack.empty())
    {
        BinaryTreeNode<int>* pRootNode = oCircleStack.top();
        oCircleStack.pop();

        if (pRootNode && (pRootNode->pLeftTreeNode || pRootNode->pRightTreeNode))
        {
            BinaryTreeNode<int>* pTempNode = pRootNode->pLeftTreeNode;
            pRootNode->pLeftTreeNode = pRootNode->pRightTreeNode;
            pRootNode->pRightTreeNode = pTempNode;
        }

        if (pRootNode && pRootNode->pLeftTreeNode)
            oCircleStack.push(pRootNode->pLeftTreeNode);

        if (pRootNode && pRootNode->pRightTreeNode)
            oCircleStack.push(pRootNode->pRightTreeNode);
    }
};

#endif