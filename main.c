#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Huffman.h"

int main() {
    char * filename = "billy.txt";
    FILE *input = fopen(filename, "rb");
    compression(input, "out.txt");
    fclose(input);
}