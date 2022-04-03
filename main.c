#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Huffman.h"

int main() {
    FILE *input = fopen("tests.txt", "rb");
    compression(input);
   fclose(input);
}