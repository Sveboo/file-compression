#ifndef HUFFMAN_H
#define HUFFMAN_H

enum error{OPEN_FILE};
typedef struct node {
    unsigned char symb;
    unsigned char isSymb;
    unsigned long freq;
    struct node * left, *right, *next;
}NODE;

typedef union bit2char {
    char symbol;
    struct bit {
        unsigned b1 : 1;
        unsigned b2 : 1;
        unsigned b3 : 1;
        unsigned b4 : 1;
        unsigned b5 : 1;
        unsigned b6 : 1;
        unsigned b7 : 1;
        unsigned b8 : 1;
    }mbit;
}BIT2CHAR;

void AddList(NODE **pphead, NODE *elem);

NODE* MakeTreeFromList(NODE * head);

NODE* MakeNodeFromNode(const NODE * left, const NODE * right);

NODE* MakeNode(char symb,unsigned long freq);

NODE* makeList(FILE *input, unsigned long *fileLen);

void compression(FILE *input);

void makeCode(NODE *head, char *s_string, unsigned long len, char** code, unsigned long *countSymb);

char* makeStr(char** codeTable,const unsigned long newLen, FILE *input,
              unsigned long* newFileLen, unsigned long* nullTail);

int writeResult(char ** codeTable, FILE* input, unsigned long fileLen, char *compFile,unsigned long *countSymb);

int uncompress(char* compresFile);
#endif

