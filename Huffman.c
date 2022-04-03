#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Huffman.h"
#define BIT8 8

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


NODE* MakeNode(char symb, unsigned long freq){
    NODE * temp = (NODE*)malloc(sizeof(NODE));
    temp->freq = freq;
    temp->symb = symb;
    temp->isSymb = 1;
    temp->right = NULL;
    temp->left = NULL;
    return temp;
}

NODE* makeList(FILE *input){
    unsigned long freq[256] = {0};
    if (!input)
        return 1;
    fseek(input, 0L, SEEK_END);
    unsigned long length = ftell(input);
    fseek(input, 0, SEEK_SET);
    for (int i = 0; i < length; ++i) {
        freq[(unsigned char) fgetc(input)]++;
    }
    NODE *head = NULL;
    for (int i = 0; i < 256; ++i){
        if (freq[i] != 0){
            AddList(&head, MakeNode(i, freq[i]));
        }
    }
    return head;
}

void compression(FILE *input){
    NODE *head = makeList(input);

    head = MakeTreeFromList(head);

    char codeString[100] = {0};//можно увеличить длину - строка для хранения кода символа
    char ** codeTable = (char**)calloc(256 , sizeof (char*));
    unsigned long newLen = 0;
    makeCode(head, codeString, 0, codeTable, &newLen);

    unsigned long newFileLen = 0;
    unsigned long nullTail = 0;
    char* result = makeStr(codeTable, newLen, input, &newFileLen, &nullTail);

    writeFile(result, newFileLen, nullTail, codeTable);
}

void makeCode(NODE *head, char *s_string, unsigned long len, char** code,unsigned long *newLen)
{
    if(head-> isSymb)
    {
        s_string[len] = 0;
        code[head->symb] =  (char *) malloc(len * sizeof(char));
        strcpy(code[head->symb], s_string);
        *newLen += len;
        return;

    }
    s_string[len] = '0';
    makeCode(head->left, s_string, len + 1, code, newLen);
    s_string[len] = '1';
    makeCode(head->right, s_string, len + 1, code, newLen);
}

char* makeStr(char** codeTable,const unsigned long lenStr, FILE *input, unsigned long* newFileLen, unsigned long* nullTail ){
    fseek(input, 0L, SEEK_END);
    long fileLen = ftell(input);//length of file
    fseek(input, 0, SEEK_SET);// return to begin

    char* outString = (char*)malloc(lenStr * sizeof (char));

    for (int i = 0; i < fileLen; ++i){//replace temp -> codeTable
        unsigned char symbol = (unsigned char) fgetc(input);
        unsigned int codeLen = strlen(codeTable[symbol]);
        for (int j = 0; j < codeLen; ++j){
            strcpy(&outString[i + j], &codeTable[symbol][j]);
        }
    }

    unsigned int count = strlen(outString) / BIT8;
    *nullTail = strlen(outString) % BIT8;
    *newFileLen = count + 1;
    BIT2CHAR temp;
    char * result = (char*)malloc((*newFileLen) * sizeof(char));
    for (int i = 0; i < *newFileLen; ++i)
    {
        temp.mbit.b1 = (unsigned char)outString[i * BIT8 + 0];
        temp.mbit.b2 = (unsigned char)outString[i * BIT8 + 1];
        temp.mbit.b3 = (unsigned char)outString[i * BIT8 + 2];
        temp.mbit.b4 = (unsigned char)outString[i * BIT8 + 3];
        temp.mbit.b5 = (unsigned char)outString[i * BIT8 + 4];
        temp.mbit.b6 = (unsigned char)outString[i * BIT8 + 5];
        temp.mbit.b7 = (unsigned char)outString[i * BIT8 + 6];
        temp.mbit.b8 = (unsigned char)outString[i * BIT8 + 7];
        result[i] = temp.symbol;
    }
    return result;
}

void writeFile(char *result,unsigned long newFileLen,unsigned long nullTail, char ** code){}
