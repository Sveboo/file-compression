#include <stdio.h>
#include "Huffman.h"


int main() {
    FILE *input = openFile("rb");
    FILE *output = openFile("wb");

    compression(input, output);

    fclose(input);
    fclose(output);

    FILE *source = openFile( "rb");
    FILE *reciev = openFile( "wb");

    uncompress(source, reciev);
}