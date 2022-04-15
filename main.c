#include <stdio.h>
#include "Huffman.h"


int main() {
    FILE *input = fopen("billy.jpg", "rb");
    FILE *output = fopen("Delete League of Legends ru.hf", "wb");
    compression(input, output);

    FILE *uncput = fopen("billy_2.jpg", "wb");
    uncompress(output, uncput);

    fclose(input);
    fclose(output);
    fclose(uncput);
}