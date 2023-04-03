#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check correct command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open the file, if failure inform the user
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Forensic image couldn't be opened for reading.\n");
        return 1;
    }

    // 0xff 0xd8 0xff [0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, or 0xef]

    const int BLOCK_SIZE = 512;
    BYTE buffer[BLOCK_SIZE] = {0};

    int jpg_count = 0;
    char filename[8] = {""};

    while (fread(buffer, BLOCK_SIZE, 1, raw_file))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            sprintf(filename, "%03d.jpg", jpg_count);
            jpg_count++;
            FILE *image = fopen(filename, "w");

            do
            {
                // Copy jpg header to image
                fwrite(buffer, BLOCK_SIZE, 1, image);
                fread(buffer, BLOCK_SIZE, 1, raw_file);
            }
            while (!(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0));
        }
    }

    fclose(raw_file);

    return 0;
}