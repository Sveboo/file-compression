#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Huffman.h"
#include "dict.h"
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


NODE* makeList(FILE *input, unsigned long *fileLen){
    unsigned long freq[256] = {0};
    if (!input)
        return OPEN_FILE;
    fseek(input, 0L, SEEK_END);
    *fileLen =  ftell(input);
    fseek(input, 0, SEEK_SET);
    for (int i = 0; i < *fileLen; ++i) {
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
    unsigned long fileLen = 0;
    NODE *head = makeList(input, &fileLen);

    head = MakeTreeFromList(head);

    char codeString[256] = {0};//можно увеличить длину - строка для хранения кода символа
    char ** codeTable = (char**)calloc(256 , sizeof (char*));
    unsigned long countSymb = 0;
    makeCode(head, codeString, 0, codeTable, &countSymb);

    for (int i = 0; i < 256; ++i){
        if (codeTable[i]){
            printf("%s - %c\n", codeTable[i], i);
        }
    }

    writeResult(codeTable, input, fileLen, "tests.hf", countSymb);
    //unsigned long newFileLen = 0;
    //unsigned long nullTail = 0;
    //char* result = makeStr(codeTable, countSymb, input, &newFileLen, &nullTail);
}

void makeCode(NODE *head, char *s_string, unsigned long len, char** code,unsigned long *countSymb)
{
    if(head-> isSymb)
    {
        s_string[len] = 0;
        code[head->symb] =  (char *) malloc(256 * sizeof(char));
        strcpy(code[head->symb], s_string);
        *countSymb += 1;
        return;

    }
    s_string[len] = '0';
    makeCode(head->left, s_string, len + 1, code, countSymb);
    s_string[len] = '1';
    makeCode(head->right, s_string, len + 1, code, countSymb);
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
    *newFileLen = count + 1;BIT2CHAR temp;
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

int writeResult(char ** codeTable, FILE* input, unsigned long fileLen, char *compFile, unsigned long *countSymb){
    FILE* output = fopen(compFile,"wb");
    printf("The original file has %d characters\n", fileLen);
    char buffer[512] = {0};
    fseek(input, 0, SEEK_SET);//Set the pointer at the beginning of the file
    fseek(output, 8, SEEK_SET);//Read in units of 8-bit binary numbers
    unsigned long pt1 = 8;
    unsigned char compInf = 0;
    unsigned long curLen = 0;
    for (int i = 0; i < fileLen; ++i) {
        unsigned char symb = fgetc(input);
        if (codeTable[symb]) {
            strcat(buffer, codeTable[symb]);
            curLen = strlen(buffer);
            while (curLen >= 8)//When the number of remaining characters is not less than 8
            {
                for (int j = 0; j <
                                8; ++j)//According to the eight-digit binary number converted into decimal ASCII code, write the file once for compression
                {
                    if (buffer[j] == '1') { compInf = (compInf << 1) | 1; }
                    else { compInf = compInf << 1; }
                }
                fwrite(&compInf, 1, 1, output);
                pt1++;
                strcpy(buffer, buffer + 8);
                curLen = strlen(buffer);
            }
        }
    }
    if (curLen > 0)//When the number of remaining characters is less than 8
        {
            strcat(buffer, "00000000");
            for (int j = 0; j < 8; ++j)
            {
                if (buffer[j] == '1') {compInf = (compInf << 1) | 1;}
                else {compInf = compInf << 1;}//Add zero to the insufficient number of digits
            }
            fwrite(&compInf, 1, 1, output);
            pt1 ++;
        }

    fseek(output, 0, SEEK_SET);//Write the encoding information into the storage file
    fwrite(&fileLen,1,sizeof(fileLen),output);
    fwrite(&pt1, sizeof(long), 1, output);
    fseek(output, pt1, SEEK_SET);
    fwrite(&countSymb, sizeof(unsigned long), 1, output);
    for (int i = 0; i < 256; i ++) {
        if (codeTable[i]) {
            char* temp = codeTable[i];
            fwrite(&i, 1, 1, output);
            pt1++;
            unsigned  long realLenC = strlen(codeTable[i]);
            fwrite(&realLenC, 1, 1, output);
            pt1++;
            if (realLenC % 8 != 0)//When the number of digits is less than 8, add zero to the number
            {
                for (int j = realLenC % 8; j < 8; ++j)
                    strcat(codeTable[i], "0");
            }

            while (codeTable[i][0] != 0) {
                unsigned  long outInf = 0;
                for (int j = 0; j < 8; ++j) {
                    if (codeTable[i][j] == '1') outInf = (outInf << 1) | 1;
                    else outInf = outInf << 1;
                }
                strcpy(codeTable[i], codeTable[i] + 8);
                fwrite(&outInf, 1, 1, output);//Write the resulting encoding information to the file
                pt1++;
            }

            codeTable[i] = temp;
        }
    }
    fclose(output);
    printf("The compressed file has: %d characters\n", pt1 + 4);
}

int uncompress(char* compresFile){
    char buf[256], bx[256];
    FILE *input, *output;
    char c_name[512] = {0};
    input = fopen(compresFile, "rb");
    if (input == NULL)
    {
        return OPEN_FILE;//If the opening fails, an error message will be output
    }
    output = fopen("uncompress.jpg", "wb");
    if (output == NULL)
    {
        return OPEN_FILE;
    }

    fseek(input,0,SEEK_END);
    long len = ftell(input);
    fseek(input,0,SEEK_SET);
    unsigned long fileLen;
    fread(&fileLen, sizeof(long), 1, input);//Read the original file length
    long helpValue;
    fread(&helpValue, sizeof(long), 1, input);
    fseek(input, helpValue, SEEK_SET);
    unsigned long countSymb;
    fread(&countSymb, sizeof(long), 1, input);//Read the parameters of the original file
    STRMAP* dict = smNew(countSymb);
    for (int i = 0; i < countSymb; ++i){
        char symb;
        unsigned char temp;
        fread(&symb, 1, 1, input);
        fread(&temp, 1, 1, input);
        unsigned long SymbLen = (unsigned long) temp;//len of symbol
        for (int j = 0; j < SymbLen / 8 + ((SymbLen % 8) ? 1 : 0); ++j)//read bit-code
        {
            fread(&temp, 1, 1, input);
            helpValue = temp;
            _itoa(helpValue, buf, 2);
            helpValue = strlen(buf);
            for (int l = 8; l > helpValue; l --)
            {
                strcat(bx, "0");//If the number of bits is insufficient, perform zero padding
            }
            strcat(bx, buf);
        }
        bx[SymbLen] = 0;
        smPut(dict,  bx, symb);

        printf("%s - %c\n",bx, smGet(dict,bx));
        bx[0] = 0;

    }
    printf("end dict\n");
    fseek(input, 8, SEEK_SET);
    unsigned long i = 0;
    bx[0] = 0;
    while (1)
    {
        unsigned char temp;
        char ka[256] = {0};
        fread(&temp, 1, 1, input);
        helpValue = temp;
        _itoa(helpValue, buf, 2);
        helpValue = strlen(buf);
        for (int l = 8; l > helpValue; l --)
        {
            strcat(bx, "0");
        }
        strcat(bx, buf);
        printf("buf %s\n", buf);
        printf("bx %s\n", bx);
        int flag = 1;
        while (flag) {
            flag = 0;
            for (int j = 1; j < strlen(bx)+1; ++j) {
                strncpy(ka, bx, j);
                temp = smGet(dict, ka);
                printf("ka %s\n", ka);
                if (temp) {
                    fwrite(&temp, 1, 1, output);
                    ++i;
                    strcpy(bx, bx + j);
                    memset(ka, 0, sizeof ka);
                    printf("found %c\n", temp);
                 //   printf("new ka\n");
                    flag = 1;
                    break; // нахер
                }
            }
            if (i == fileLen) break;
        }

        if (i == fileLen) break;
    }

    fclose(input);
    fclose(output);
    return 1;
}