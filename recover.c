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

    const int BLOCK_SIZE = 512;
    BYTE buffer[BLOCK_SIZE] = {0};

    FILE *image;
    int jpg_count = 0;
    char filename[8] = {""};

    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        printf("Block\n");
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            printf("Found the start of a jpg file\n");
            sprintf(filename, "%03d.jpg", jpg_count);
            image = fopen(filename, "w");
            jpg_count++;

            do
            {
                fwrite(buffer, 1, BLOCK_SIZE, image);
                fread(buffer, 1, BLOCK_SIZE, raw_file);
            }
            while (!(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0));

            fclose(image);
        }
    }

    fclose(raw_file);

    return 0;
}