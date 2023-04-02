#include "helpers.h"
#include <stdio.h>
#include <math.h>

void round_average(double *ptr);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Initialize needed variables
    double average = 0;

    // For each row of pixels
    for (int i = 0; i < height; i++)
    {
        // For each pixel in row
        for (int j = 0; j < width; j++)
        {
            // Calculate average of the three channels
            average = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / (double) 3;

            round_average(&average);

            // Set the new channels color for the pixel
            image[i][j].rgbtBlue  = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed   = average;
        }
    }

    // BMP converted successfully
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    const int HALF = width / 2;

    // For each row of pixels
    for (int i = 0; i < height; i++)
    {
        // For each pixel in row (until middle of row)
        for (int j = 0; j < HALF; j++)
        {
            // Since when performing a swap the old value is lost
            RGBTRIPLE aux = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = aux;
        }
    }

    // BMP converted successfully
    return;
}

// Checks whether the given color has decimal part, then it rounds accordingly
void round_average(double *ptr)
{
    // e.g. 1.5 - 1 = 0.5
    double decimal_part = *ptr - (int) * ptr;

    if (decimal_part >= 0.5)
    {
        *ptr = (int) * ptr + 1;
    }
    else
    {
        *ptr = (int) * ptr;
    }

    return;
}

void blur_pixel(int i, int j, int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE untouched_image[height][width])
{
    int neighbor_i       = 0;
    int neighbor_j       = 0;
    int neighbor_count   = 0;
    double average_blue  = 0.0;
    double average_green = 0.0;
    double average_red   = 0.0;

    // For each possible neighbor
    for (int r = -1; r < 2; r++)
    {
        for (int c = -1; c < 2; c++)
        {
            neighbor_i = i + r;
            neighbor_j = j + c;

            // Check if neighbor not out of bounds
            if (neighbor_i >= 0 && neighbor_j >= 0 && neighbor_i < height && neighbor_j < width)
            {
                neighbor_count++;
                average_blue  += untouched_image[neighbor_i][neighbor_j].rgbtBlue;
                average_green += untouched_image[neighbor_i][neighbor_j].rgbtGreen;
                average_red   += untouched_image[neighbor_i][neighbor_j].rgbtRed;
            }
        }
    }

    average_blue  /= (double) neighbor_count;
    average_green /= (double) neighbor_count;
    average_red   /= (double) neighbor_count;

    round_average(&average_blue);
    round_average(&average_green);
    round_average(&average_red);

    image[i][j].rgbtBlue  = average_blue;
    image[i][j].rgbtGreen = average_green;
    image[i][j].rgbtRed   = average_red;
}

// Copy image to another array since applying filters affects values
// Bug found thanks to this explanation https://stackoverflow.com/a/62210080
void duplicate_image(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE untouched_image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            untouched_image[i][j] = image[i][j];
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE untouched_image[height][width];
    duplicate_image(height, width, image, untouched_image);

    // For each row of pixels
    for (int i = 0; i < height; i++)
    {
        // For each pixel in row
        for (int j = 0; j < width; j++)
        {
            blur_pixel(i, j, height, width, image, untouched_image);
        }
    }

    // BMP converted successfully
    return;
}

void compute_gx(int i, int j, int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE untouched_image[height][width], int *gx_blue, int *gx_green, int *gx_red)
{
    const int GX[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};

    // Reset channels values to zero
    *gx_blue  = 0;
    *gx_green = 0;
    *gx_red   = 0;

    int neighbor_i     = 0;
    int neighbor_j     = 0;
    int neighbor_count = 0;

    unsigned long calculation_placeholder = 0;

    // For each possible neighbor
    for (int r = -1; r < 2; r++)
    {
        for (int c = -1; c < 2; c++)
        {
            neighbor_i = i + r;
            neighbor_j = j + c;

            // Check if neighbor not out of bounds
            if (neighbor_i >= 0 && neighbor_j >= 0 && neighbor_i < height && neighbor_j < width)
            {
                calculation_placeholder = GX[r+1][c+1] * untouched_image[neighbor_i][neighbor_j].rgbtBlue;

                if (calculation_placeholder > 255)
                {
                    *gx_blue += 255;
                }
                else
                {
                    *gx_blue += calculation_placeholder;
                }

                calculation_placeholder = GX[r+1][c+1] * untouched_image[neighbor_i][neighbor_j].rgbtGreen;

                if (calculation_placeholder > 255)
                {
                    *gx_green += 255;
                }
                else
                {
                    *gx_green += calculation_placeholder;
                }

                calculation_placeholder = GX[r+1][c+1] * untouched_image[neighbor_i][neighbor_j].rgbtRed;

                if (calculation_placeholder > 255)
                {
                    *gx_red += 255;
                }
                else
                {
                    *gx_red += calculation_placeholder;
                }
            }
        }
    }

    return;
}

void compute_gy(int i, int j, int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE untouched_image[height][width], int *gy_blue, int *gy_green, int *gy_red)
{
    const int GY[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

    // Reset channels values to zero
    *gy_blue  = 0;
    *gy_green = 0;
    *gy_red   = 0;

    int neighbor_i     = 0;
    int neighbor_j     = 0;
    int neighbor_count = 0;

    unsigned long calculation_placeholder = 0;

    // For each possible neighbor
    for (int r = -1; r < 2; r++)
    {
        for (int c = -1; c < 2; c++)
        {
            neighbor_i = i + r;
            neighbor_j = j + c;

            // Check if neighbor not out of bounds
            if (neighbor_i >= 0 && neighbor_j >= 0 && neighbor_i < height && neighbor_j < width)
            {
                calculation_placeholder = GY[r+1][c+1] * untouched_image[neighbor_i][neighbor_j].rgbtBlue;

                if (calculation_placeholder > 255)
                {
                    *gy_blue += 255;
                }
                else
                {
                    *gy_blue += calculation_placeholder;
                }

                calculation_placeholder = GY[r+1][c+1] * untouched_image[neighbor_i][neighbor_j].rgbtGreen;

                if (calculation_placeholder > 255)
                {
                    *gy_green += 255;
                }
                else
                {
                    *gy_green += calculation_placeholder;
                }

                calculation_placeholder = GY[r+1][c+1] * untouched_image[neighbor_i][neighbor_j].rgbtRed;

                if (calculation_placeholder > 255)
                {
                    *gy_red += 255;
                }
                else
                {
                    *gy_red += calculation_placeholder;
                }
            }
        }
    }

    // printf("%i %i %i\n", *gy_red, *gy_green, *gy_blue);

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE untouched_image[height][width];
    duplicate_image(height, width, image, untouched_image);

    int gx_blue  = 0;
    int gy_blue  = 0;
    int blue     = 0;

    int gx_green = 0;
    int gy_green = 0;
    int green    = 0;

    int gx_red   = 0;
    int gy_red   = 0;
    int red      = 0;

    // For each row of pixels
    for (int i = 0; i < height; i++)
    {
        // For each pixel in row
        for (int j = 0; j < width; j++)
        {
            compute_gx(i, j, height, width, image, untouched_image, &gx_blue, &gx_green, &gx_red);
            compute_gy(i, j, height, width, image, untouched_image, &gy_blue, &gy_green, &gy_red);
            // printf("%i %i %i\n", gx_blue, gx_green, gx_red);
            // printf("%i %i %i\n", gy_blue, gy_green, gy_red);
            // return;
            blue  = sqrt(pow(gx_blue, 2)  + pow(gy_blue, 2));
            green = sqrt(pow(gx_green, 2) + pow(gy_green, 2));
            red   = sqrt(pow(gx_red, 2)   + pow(gy_red, 2));

            image[i][j].rgbtBlue  = blue;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtRed   = red;
        }
    }

    // BMP converted successfully
    return;
}
