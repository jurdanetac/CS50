#include "helpers.h"
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Initialize needed variables
    double average      = 0;
    double decimal_part = 0;

    // For each row of pixels
    for (int i = 0; i < height; i++)
    {
        // For each pixel in row
        for (int j = 0; j < width; j++)
        {
            // Calculate average of the three channels
            average = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
            // Calculate the floating point of the average, if any
            decimal_part = average - (int) average;

            // Round if needed
            if (decimal_part >= 0.5)
            {
                average++;
            }

            // Set the new channels color for the pixel
            image[i][j].rgbtBlue  = (int) average;
            image[i][j].rgbtGreen = (int) average;
            image[i][j].rgbtRed   = (int) average;
        }
    }

    // BMP converted successfully
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int half = width / 2;

    // For each row of pixels
    for (int i = 0; i < height; i++)
    {
        // For each pixel in row (until middle of row)
        for (int j = 0; j < half; j++)
        {
            image[i][j] = image[i][width-j+1];
            // Set the new channels colors for the pixel
            // image[i][j].rgbtBlue  = ;
            // image[i][j].rgbtGreen = ;
            // image[i][j].rgbtRed   = ;
        }
    }

    // BMP converted successfully
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // BMP converted successfully
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    // BMP converted successfully
    return;
}
