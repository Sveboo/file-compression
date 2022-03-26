#include <stdio.h>
#include <stdlib.h>
#include "Huffman.h"

void AddList(NODE **pphead, NODE *elem) {
    NODE **pp = pphead;
    while (*pp) {
        if (elem->freq < (*pp)->freq)
            break;
        else
            pp = &((*pp)->next);
    }
    NODE* pnew = elem;
    pnew->next = *pp;
    *pp = pnew;
}

NODE * MakeTreeFromList(NODE * head)
{
    while (head && head->next)
    {
        NODE * left = head;
        NODE * right = head->next;
        AddList(&(head->next->next), MakeNodeFromNode(left, right));
        head = head->next->next;
    }
    return head;
}

NODE *MakeNodeFromNode(const NODE * left, const NODE * right)
{
    NODE * res = (NODE*)malloc(sizeof(NODE));
    res->freq = left->freq + right->freq;
    res->isSymb = 0;
    res->symb = 0;
    res->left = left;
    res->right = right;
    res->next = 0;
    return res;
}

