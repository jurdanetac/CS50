// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    uint8_t header[HEADER_SIZE] = {0};
    int16_t sample = 0;
    uint8_t byte   = 0;

    // printf("%li\n", sizeof(byte));
    // printf("%i\n", HEADER_SIZE);
    // printf("%p\n", &header);
    // printf("%p\n", &header[0]);

    // TODO: Copy header from input file to output file

    // Copy header one byte at a time
    for (int i = 0; i < HEADER_SIZE; i++)
    {
        fread(&header, sizeof(byte), 1, input);
        fwrite(&header, sizeof(byte), 1, output);
    }


    // TODO: Read samples from input file and write updated data to output file
    while (fread(&buffer, sizeof(buffer), 1, input))
    {
        buffer *= factor;
        fwrite(&buffer, sizeof(buffer), 1, output);
    }


    // Close files
    fclose(input);
    fclose(output);
}
