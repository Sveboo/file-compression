#include <stdio.h>
#include "Huffman.h"
#include <stdlib.h>

void AddList(NODE **pphead, int val) {
    NODE **pp = pphead, *pnew;
    while (*pp) {
        if (val < (*pp)->value)
            break;
        else
            pp = &((*pp)->pnext);
    }
    pnew = (NODE *) malloc(sizeof(NODE));
    pnew->value = val;
    pnew->pnext = *pp;
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

