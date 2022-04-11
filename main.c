#include <stdio.h>
#include "Huffman.h"


int main() {
    FILE *input = fopen("Install League of Legends ru.exe", "rb");
    compression(input);
    uncompress("tests.hf");
    fclose(input);
}