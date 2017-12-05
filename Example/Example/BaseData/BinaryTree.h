#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <vector>
#include <queue>

////////////////////////////////////////////////////////////////////////////
//二叉树结构

template<typename valueType>
struct BinaryTreeNode
{
    valueType value;
    BinaryTreeNode<valueType>* pLeftTreeNode;
    BinaryTreeNode<valueType>* pRightTreeNode;
};

// 前序遍历：根节点->左子树->右子树
// 中序遍历：左子树->根节点->右子树
// 后序遍历：左子树->右子树->根节点

//递归：前序遍历
template<typename valueType>
void preOrder_Recursion(BinaryTreeNode<valueType>* pRootNode, std::vector<valueType>& oResult)
{
    if (!pRootNode)
        return;

    oResult.push_back(pRootNode->value);
    preOrder_Recursion(pRootNode->pLeftTreeNode, oResult);
    preOrder_Recursion(pRootNode->pRightTreeNode, oResult);
}

//递归：中序遍历
template<typename valueType>
void midOrder_Recursion(BinaryTreeNode<valueType>* pRootNode, std::vector<valueType>& oResult)
{
    if (!pRootNode)
        return;

    midOrder_Recursion(pRootNode->pLeftTreeNode, oResult);
    oResult.push_back(pRootNode->value);
    midOrder_Recursion(pRootNode->pRightTreeNode, oResult);
}

//递归：后序遍历
template<typename valueType>
void postOrder_Recursion(BinaryTreeNode<valueType>* pRootNode, std::vector<valueType>& oResult)
{
    if (!pRootNode)
        return;

    postOrder_Recursion(pRootNode->pLeftTreeNode, oResult);
    postOrder_Recursion(pRootNode->pRightTreeNode, oResult);
    oResult.push_back(pRootNode->value);
}

//层次遍历
template<typename valueType>
void levelOrder(BinaryTreeNode<valueType>* pRootNode, std::vector<valueType>& oResult)
{
    std::queue<BinaryTreeNode<valueType>*> oNodeQueue;
    oNodeQueue.push(pRootNode);
    while (!oNodeQueue.empty())
    {
        BinaryTreeNode<valueType>* pRootNode = oNodeQueue.front();
        oNodeQueue.pop();
        oResult.push_back(pRootNode->value);

        if (pRootNode && pRootNode->pLeftTreeNode)
            oNodeQueue.push(pRootNode->pLeftTreeNode);

        if (pRootNode && pRootNode->pRightTreeNode)
            oNodeQueue.push(pRootNode->pRightTreeNode);
    }
}

//循环：前序遍历
template<typename valueType>
void preOrder_Circle(BinaryTreeNode<valueType>* pRootNode, std::vector<valueType>& oResult);

//循环：中序遍历
template<typename valueType>
void midOrder_Circle(BinaryTreeNode<valueType>* pRootNode, std::vector<valueType>& oResult);

//循环：后序遍历
template<typename valueType>
void postOrder_Circle(BinaryTreeNode<valueType>* pRootNode, std::vector<valueType>& oResult);

#endif