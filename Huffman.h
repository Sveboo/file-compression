#ifndef HUFFMAN_H
#define HUFFMAN_H

enum error{END_SUCCESS=1, OPEN_FILE};
typedef struct node {
    unsigned char symb;
    unsigned char isSymb;
    unsigned long freq;
    struct node * left, *right, *next;
}NODE;


int compression(FILE *input);


int uncompress(char* compresFile);
#endif

