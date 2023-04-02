#include "helpers.h"
#include <stdio.h>

void round_average(double* ptr);

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
            image[i][j] = image[i][width-(j+1)];
            image[i][width-(j+1)] = aux;
        }
    }

    // BMP converted successfully
    return;
}

// Checks whether the given color has decimal part, then it rounds accordingly
void round_average(double* ptr)
{
    // e.g. 1.5 - 1 = 0.5
    double decimal_part = *ptr - (int) *ptr;

    if (decimal_part >= 0.5)
    {
        *ptr = (int) *ptr + 1;
    }
    else
    {
        *ptr = (int) *ptr;
    }

    return;
}

void blur_pixel(int i, int j, int height, int width, RGBTRIPLE image[height][width])
{
    // int counter = 0;
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

            // check if not out of bounds
            if (neighbor_i >= 0 && neighbor_j >= 0 && neighbor_i < height && neighbor_j < width)
            {
                // counter++;
                neighbor_count++;
                average_blue  += image[neighbor_i][neighbor_j].rgbtBlue;
                average_green += image[neighbor_i][neighbor_j].rgbtGreen;
                average_red   += image[neighbor_i][neighbor_j].rgbtRed;
            }
        }
    }

    // printf("%i\n", counter);

    average_blue  /= (double) neighbor_count;
    average_green /= (double) neighbor_count;
    average_red   /= (double) neighbor_count;

    round_average(&average_blue);
    round_average(&average_green);
    round_average(&average_red);

    // printf("%f\n", average_blue);
    // printf("%f\n", average_green);
    // printf("%f\n", average_red);

    image[i][j].rgbtBlue  = average_blue;
    image[i][j].rgbtGreen = average_green;
    image[i][j].rgbtRed   = average_red;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // For each row of pixels
    for (int i = 0; i < height; i++)
    {
        // For each pixel in row
        for (int j = 0; j < width; j++)
        {
            blur_pixel(i, j, height, width, image);
        }
    }

    // BMP converted successfully
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    // BMP converted successfully
    return;
}
