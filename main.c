#include <stdio.h>
#include "Huffman.h"


int main() {
    FILE *input = fopen("billy.jpg", "rb");
    compression(input);
    uncompress("tests.hf");
    fclose(input);
}