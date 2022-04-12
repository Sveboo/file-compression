#include <stdio.h>
#include "Huffman.h"


int main() {
    FILE *input = fopen("billy.bmp", "rb");
    compression(input);
    uncompress("tests.hf");
    fclose(input);
}