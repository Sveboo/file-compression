#include <stdio.h>
#include <stdlib.h>
#include "Huffman.h"
#define max_str 100

int main() {
    char * temp1 = (char*) malloc(sizeof(char) * max_str);
    char * temp2 = (char*) malloc(sizeof(char) * max_str);

    puts("Enter your filename:");
    gets(temp1);

    puts("Enter the name of compressed file:");
    gets(temp2);

    FILE *input = fopen(temp1, "rb");
    FILE *output = fopen(temp2, "wb");
    compression(input, output);
    fclose(input);
    fclose(output);

    temp1[0] = 0;
    puts("Enter the name of uncompressed file:");
    gets(temp1);

    FILE *source = fopen(temp2, "rb");
    FILE *reciev = fopen(temp1, "wb");
    uncompress(source, reciev);

}