#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Huffman.h"
#include "dict.h"

void AddList(NODE **head, NODE *element) {// добавление нового символа в массив
    NODE **tempNode = head;

    while (*tempNode) {//вставка исходя из его частоты
        if (element->freq < (*tempNode)->freq)
            break;
        else
            tempNode = &((*tempNode)->next);
    }

    NODE* newNode = element;
    newNode->next = *tempNode;
    *tempNode = newNode;
}

NODE *MakeNodeFromNode(const NODE * left, const NODE * right)// объединение двух узлов
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


NODE * MakeTreeFromList(NODE * head)// формирование бинарного дерева
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


NODE* MakeNode(char symb, unsigned long freq){// создание узла для нового символа
    NODE * temp = (NODE*)malloc(sizeof(NODE));
    temp->freq = freq;
    temp->symb = symb;
    temp->isSymb = 1;
    temp->right = NULL;
    temp->left = NULL;
    return temp;
}


NODE* makeList(FILE *input, unsigned long *fileLen){//получение данных файла
    unsigned long freq[256] = {0};
    fseek(input, 0L, SEEK_END);
    *fileLen =  ftell(input);
    fseek(input, 0, SEEK_SET);

    for (int i = 0; i < *fileLen; ++i) {
        freq[(unsigned char) fgetc(input)]++;
    }

    NODE *head = NULL;

    for (int i = 0; i < 256; ++i){// создание списка символов
        if (freq[i] != 0){
            AddList(&head, MakeNode(i, freq[i]));
        }
    }

    return head;
}


void makeCode(NODE *head, char *s_string, unsigned long len, char** code,unsigned long *countSymb)
{//составление кода, индекс - символ по таблице ASCII

    if(head-> isSymb)
    {
        s_string[len] = 0;
        code[head->symb] =  (char *) malloc(256 * sizeof(char));
        strcpy(code[head->symb], s_string);
        *countSymb += 1;
        return;
    }

    s_string[len] = '0';
    makeCode(head->left, s_string, len + 1, code, countSymb);// рекурсивный перебор левой части

    s_string[len] = '1';
    makeCode(head->right, s_string, len + 1, code, countSymb);// рекурсивный перебор правой части
}

int writeResult(char ** codeTable, FILE* input, unsigned long fileLen, char *compFile, unsigned long *countSymb){
    FILE* output = fopen(compFile,"wb");

    if(!(output)){
        return OPEN_FILE;
    }

    printf("The original file has %d characters\n", fileLen);
    char buffer[512] = {0};
    fseek(input, 0, SEEK_SET);//помещение указателя на начало файла
    fseek(output, 8, SEEK_SET);//Считывание в единицах 8-разрядных двоичных чисел
    unsigned long pt1 = 8;
    unsigned char compInf = 0;
    unsigned long curLen = 0;

    for (int i = 0; i < fileLen; ++i) {
        unsigned char symb = fgetc(input);
        if (codeTable[symb]) {
            strcat(buffer, codeTable[symb]);
            curLen = strlen(buffer);

            while (curLen >= 8)// если кол-во символов в buffer не менее 8
            {
                for (int j = 0; j < 8; ++j)//запись в файл данных в виде восьмизначного двоичного числа
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
    if (curLen > 0)//если в buffer остался код менее 8 символов, необходимых для записи
        {
            strcat(buffer, "00000000");// заполняем нулями недостающее кол-во чисел для записи
            for (int j = 0; j < 8; ++j)
            {
                if (buffer[j] == '1') {compInf = (compInf << 1) | 1;}
                else {compInf = compInf << 1;}
            }
            fwrite(&compInf, 1, 1, output);
            pt1 ++;
        }

    fseek(output, 0, SEEK_SET);//запись информации о кодировке в сжатый файл
    fwrite(&fileLen,1,sizeof(fileLen),output);// длина исходного файла
    fwrite(&pt1, sizeof(long), 1, output);// указатель на позицию для чтения файла
    // ptr1 = длина закодированной информации
    fseek(output, pt1, SEEK_SET);// переход к позиции
    fwrite(&countSymb, sizeof(unsigned long), 1, output);// количество символов в исходном файле

    for (int i = 0; i < 256; i ++) {// запись символа и соответствующего ему кода

        if (codeTable[i]) {
            char* temp = codeTable[i];
            fwrite(&i, 1, 1, output);
            pt1++;
            unsigned  long realLenC = strlen(codeTable[i]);
            fwrite(&realLenC, 1, 1, output);
            pt1++;
            if (realLenC % 8 != 0)// если код содержит меньше 8 символов, заполняем пр-ство незначащими нулями
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
                fwrite(&outInf, 1, 1, output);
                pt1++;
            }

            codeTable[i] = temp;
        }
    }

    fclose(output);
    printf("The compressed file has: %d characters\n", pt1 + 4);
}

int compression(FILE *input){
    unsigned long fileLen = 0;
    NODE *head = makeList(input, &fileLen);
    head = MakeTreeFromList(head);
    char codeString[256] = {0};
    char ** codeTable = (char**)calloc(256 , sizeof (char*));
    unsigned long countSymb = 0;
    makeCode(head, codeString, 0, codeTable, &countSymb);
    writeResult(codeTable, input, fileLen, "tests.hf", countSymb);
    return END_SUCCESS;
}

STRMAP* makeDict(FILE* input, unsigned long countSymb){
    char buf[256], bx[256];
    long helpValue;
    STRMAP* dict = smNew(countSymb);
    for (int i = 0; i < countSymb; ++i){
        char symb;
        unsigned char temp;
        fread(&symb, 1, 1, input);
        fread(&temp, 1, 1, input);
        unsigned long SymbLen = (unsigned long) temp;// длина символа

        for (int j = 0; j < SymbLen / 8 + ((SymbLen % 8) ? 1 : 0); ++j)// длина некратна 8=>конец кода в следующем байте информации
        {
            fread(&temp, 1, 1, input);
            helpValue = temp;
            _itoa(helpValue, buf, 2);
            helpValue = strlen(buf);

            for (int l = 8; l > helpValue; l --)
            {
                strcat(bx, "0");//если символов недостаточно, заполняем позиции незначащими 0
            }
            strcat(bx, buf);
        }

        bx[SymbLen] = 0;
        smPut(dict,  bx, symb);
        bx[0] = 0;

    }
    return dict;
}

int uncompress(char* compresFile){
    char buf[256], bx[256];
    FILE *input, *output;
    char c_name[512] = {0};
    input = fopen(compresFile, "rb");
    if (input == NULL)
    {
        return OPEN_FILE;
    }
    output = fopen("unncompress.bmp", "wb");

    if (output == NULL)
    {
        return OPEN_FILE;
    }

    fseek(input,0,SEEK_END);
    long len = ftell(input);
    fseek(input,0,SEEK_SET);
    unsigned long fileLen;
    fread(&fileLen, sizeof(long), 1, input);
    long helpValue;
    fread(&helpValue, sizeof(long), 1, input);
    fseek(input, helpValue, SEEK_SET);
    unsigned long countSymb;
    fread(&countSymb, sizeof(long), 1, input);

    STRMAP* dict = makeDict(input, countSymb);

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

        for (int l = 8; l > helpValue; l --)// если длина закодированной информации<8,заполняем пространство 0
        {
            strcat(bx, "0");
        }
        strcat(bx, buf);
        int flag = 1;

        while (flag) {// поиск соответствие кода символу
            flag = 0;

            for (int j = 1; j < strlen(bx)+1; ++j) {
                strncpy(ka, bx, j);
                if (smExists(dict, ka)) {// нашли такой код
                    temp = smGet(dict, ka);// получаем символ по коду
                    fwrite(&temp, 1, 1, output);
                    ++i;
                    strcpy(bx, bx + j);
                    memset(ka, 0, sizeof ka);
                    flag = 1;
                    break;
                }
            }
            if (i == fileLen) break;
        }

        if (i == fileLen) break;
    }

    fclose(input);
    fclose(output);
    return END_SUCCESS;
}