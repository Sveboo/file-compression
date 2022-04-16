#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct node {
    unsigned char symb;
    unsigned char isSymb;
    unsigned long freq;
    struct node * left, *right, *next;
}NODE;

FILE* openFile(char* proc);

void compression(FILE *input, FILE* output);

void uncompress(FILE *input, FILE *output);
#endif

