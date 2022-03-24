#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Huffman.h"
int main() {
    FILE *fr = fopen('tests.txt', "rb");
    int freq[256] = {0};
    if (!fr)
        return 1;
    fseek(fr, 0L, SEEK_END);
    long length = ftell(fr);
    fseek(fr, 0, SEEK_SET);
    for (int i = 0; i < length; ++i) {
        freq[(unsigned char) fgetc(fr)]++;
    }
    int count = strlen(str) / BIT8;
    *tail = strlen(str) % BIT8;
    *len = count + 1;
    BIT2CHAR symb;
    char * res = (char*)malloc((*len) * sizeof(char));
    for (int i = 0; i < *len; ++i)
    {
        symb.mbit.b1 = str[i*BIT8 + 0];
        symb.mbit.b2 = str[i*BIT8 + 1];
        symb.mbit.b3 = str[i*BIT8 + 2];
        symb.mbit.b4 = str[i*BIT8 + 3];
        symb.mbit.b5 = str[i*BIT8 + 4];
        symb.mbit.b6 = str[i*BIT8 + 5];
        symb.mbit.b7 = str[i*BIT8 + 6];
        symb.mbit.b8 = str[i*BIT8 + 7];
        res[i] = symb.symb;
    }

    fclose(fr);
}