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
            average = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / (double) 3;
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

void fill_safe_image(int height, int width, RGBTRIPLE image[height][width],
                     int safe_height, int safe_width, RGBTRIPLE safe_image[safe_height][safe_width])
{
    RGBTRIPLE placeholder;
    placeholder.rgbtBlue  = 0;
    placeholder.rgbtGreen = 0;
    placeholder.rgbtRed   = 0;

    // Fill safe matrix with just 0'ed bytes
    for (int i = 0; i < safe_height; i++)
    {
        for (int j = 0; j < safe_width; j++)
        {
            safe_image[i][j] = placeholder;
        }
    }

    // Copy original image which is now bordered by 0's
    for (int i = 1; i < safe_height-1; i++)
    {
        for (int j = 1; j < safe_width-1; j++)
        {
            safe_image[i][j] = image[i-1][j-1];
        }
    }

    return;
}

void blur_pixel(int i, int j,
                int height, int width, RGBTRIPLE image[height][width],
                int safe_height, int safe_width, RGBTRIPLE safe_image[safe_height][safe_width])
{
    double average_blue  = 0.0;
    double average_green = 0.0;
    double average_red   = 0.0;

    // if pixel is upper left corner
    if (i == 1 && j == 1)
    {
        //
        average_red += safe_image[i][j].rgbtRed;
        average_red += safe_image[i][j+1].rgbtRed;
        average_red += safe_image[i+1][j].rgbtRed;
        average_red += safe_image[i+1][j+1].rgbtRed;
        average_red /= (double) 4;

        round_average(&average_red);
        image[i-1][j-1].rgbtRed = average_red;

        //
        average_green += safe_image[i][j].rgbtGreen;
        average_green += safe_image[i][j+1].rgbtGreen;
        average_green += safe_image[i+1][j].rgbtGreen;
        average_green += safe_image[i+1][j+1].rgbtGreen;
        average_green /= (double) 4;

        round_average(&average_green);
        image[i-1][j-1].rgbtGreen = average_green;

        //
        average_blue += safe_image[i][j].rgbtBlue;
        average_blue += safe_image[i][j+1].rgbtBlue;
        average_blue += safe_image[i+1][j].rgbtBlue;
        average_blue += safe_image[i+1][j+1].rgbtBlue;
        average_blue /= (double) 4;

        round_average(&average_blue);
        image[i-1][j-1].rgbtBlue = average_blue;
    }
    // pixel is upper right corner
    else if (i == 1 && j == width)
    {
        //
        average_red += safe_image[i][j].rgbtRed;
        average_red += safe_image[i][j-1].rgbtRed;
        average_red += safe_image[i+1][j-1].rgbtRed;
        average_red += safe_image[i+1][j].rgbtRed;
        average_red /= (double) 4;

        round_average(&average_red);
        image[i-1][j-1].rgbtRed = average_red;

        //
        average_green += safe_image[i][j].rgbtGreen;
        average_green += safe_image[i][j-1].rgbtGreen;
        average_green += safe_image[i+1][j].rgbtGreen;
        average_green += safe_image[i+1][j+1].rgbtGreen;
        average_green /= (double) 4;

        round_average(&average_green);
        image[i-1][j-1].rgbtGreen = average_green;

        //
        average_blue += safe_image[i][j].rgbtBlue;
        average_blue += safe_image[i][j+1].rgbtBlue;
        average_blue += safe_image[i+1][j].rgbtBlue;
        average_blue += safe_image[i+1][j+1].rgbtBlue;
        average_blue /= (double) 4;

        round_average(&average_blue);
        image[i-1][j-1].rgbtBlue = average_blue;
    }
    // pixel is lower left corner
    else if (i == height && j == 1)
    {
        //
        average_red += safe_image[i-1][j].rgbtRed;
        average_red += safe_image[i-1][j+1].rgbtRed;
        average_red += safe_image[i][j].rgbtRed;
        average_red += safe_image[i][j+1].rgbtRed;
        average_red /= (double) 4;

        round_average(&average_red);
        image[i-1][j-1].rgbtRed = average_red;

        //
        average_green += safe_image[i-1][j].rgbtGreen;
        average_green += safe_image[i-1][j+1].rgbtGreen;
        average_green += safe_image[i][j].rgbtGreen;
        average_green += safe_image[i][j+1].rgbtGreen;
        average_green /= (double) 4;

        round_average(&average_green);
        image[i-1][j-1].rgbtGreen = average_green;

        //
        average_blue += safe_image[i-1][j].rgbtBlue;
        average_blue += safe_image[i-1][j+1].rgbtBlue;
        average_blue += safe_image[i][j].rgbtBlue;
        average_blue += safe_image[i][j+1].rgbtBlue;
        average_blue /= (double) 4;

        round_average(&average_blue);
        image[i-1][j-1].rgbtBlue = average_blue;
    }
    // pixel is lower right corner
    else if (i == height && j == width)
    {
        //
        average_red += safe_image[i-1][j-1].rgbtRed;
        average_red += safe_image[i-1][j].rgbtRed;
        average_red += safe_image[i][j-1].rgbtRed;
        average_red += safe_image[i][j].rgbtRed;
        average_red /= (double) 4;

        round_average(&average_red);
        image[i-1][j-1].rgbtRed = average_red;

        //
        average_green += safe_image[i-1][j-1].rgbtGreen;
        average_green += safe_image[i-1][j].rgbtGreen;
        average_green += safe_image[i][j-1].rgbtGreen;
        average_green += safe_image[i][j].rgbtGreen;
        average_green /= (double) 4;

        round_average(&average_green);
        image[i-1][j-1].rgbtGreen = average_green;

        //
        average_blue += safe_image[i-1][j-1].rgbtBlue;
        average_blue += safe_image[i-1][j].rgbtBlue;
        average_blue += safe_image[i][j-1].rgbtBlue;
        average_blue += safe_image[i][j].rgbtBlue;
        average_blue /= (double) 4;

        round_average(&average_blue);
        image[i-1][j-1].rgbtBlue = average_blue;
    }
    else if (j == 1)
    {
        //
        average_red += safe_image[i-1][j].rgbtRed;
        average_red += safe_image[i-1][j+1].rgbtRed;
        average_red += safe_image[i][j].rgbtRed;
        average_red += safe_image[i][j+1].rgbtRed;
        average_red += safe_image[i+1][j].rgbtRed;
        average_red += safe_image[i+1][j+1].rgbtRed;
        average_red /= (double) 6;

        round_average(&average_red);
        image[i-1][j-1].rgbtRed = average_red;

        //
        average_green += safe_image[i-1][j].rgbtGreen;
        average_green += safe_image[i-1][j+1].rgbtGreen;
        average_green += safe_image[i][j].rgbtGreen;
        average_green += safe_image[i][j+1].rgbtGreen;
        average_green += safe_image[i+1][j].rgbtGreen;
        average_green += safe_image[i+1][j+1].rgbtGreen;
        average_green /= (double) 6;

        round_average(&average_green);
        image[i-1][j-1].rgbtGreen = average_green;

        //
        average_blue += safe_image[i-1][j].rgbtBlue;
        average_blue += safe_image[i-1][j+1].rgbtBlue;
        average_blue += safe_image[i][j].rgbtBlue;
        average_blue += safe_image[i][j+1].rgbtBlue;
        average_blue += safe_image[i+1][j].rgbtBlue;
        average_blue += safe_image[i+1][j+1].rgbtBlue;
        average_blue /= (double) 6;

        round_average(&average_blue);
        image[i-1][j-1].rgbtBlue = average_blue;
    }
    else if (j == width)
    {
        //
        average_red += safe_image[i-1][j-1].rgbtRed;
        average_red += safe_image[i-1][j].rgbtRed;
        average_red += safe_image[i][j-1].rgbtRed;
        average_red += safe_image[i][j].rgbtRed;
        average_red += safe_image[i+1][j-1].rgbtRed;
        average_red += safe_image[i+1][j].rgbtRed;
        average_red /= (double) 6;

        round_average(&average_red);
        image[i-1][j-1].rgbtRed = average_red;

        //
        average_green += safe_image[i-1][j-1].rgbtGreen;
        average_green += safe_image[i-1][j].rgbtGreen;
        average_green += safe_image[i][j-1].rgbtGreen;
        average_green += safe_image[i][j].rgbtGreen;
        average_green += safe_image[i+1][j-1].rgbtGreen;
        average_green += safe_image[i+1][j].rgbtGreen;
        average_green /= (double) 6;

        round_average(&average_green);
        image[i-1][j-1].rgbtGreen = average_green;

        //
        average_blue += safe_image[i-1][j-1].rgbtBlue;
        average_blue += safe_image[i-1][j].rgbtBlue;
        average_blue += safe_image[i][j-1].rgbtBlue;
        average_blue += safe_image[i][j].rgbtBlue;
        average_blue += safe_image[i+1][j-1].rgbtBlue;
        average_blue += safe_image[i+1][j].rgbtBlue;
        average_blue /= (double) 6;

        round_average(&average_blue);
        image[i-1][j-1].rgbtBlue = average_blue;
    }
    //else if (i == 1)
    //{
    //    //
    //    average_red += safe_image[i][j-1].rgbtRed;
    //    average_red += safe_image[i][j].rgbtRed;
    //    average_red += safe_image[i][j+1].rgbtRed;
    //    average_red += safe_image[i+1][j-1].rgbtRed;
    //    average_red += safe_image[i+1][j].rgbtRed;
    //    average_red += safe_image[i+1][j+1].rgbtRed;
    //    average_red /= (double) 6;
//
    //    round_average(&average_red);
    //    image[i-1][j-1].rgbtRed = average_red;
//
    //    //
    //    average_green += safe_image[i][j-1].rgbtGreen;
    //    average_green += safe_image[i][j].rgbtGreen;
    //    average_green += safe_image[i][j+1].rgbtGreen;
    //    average_green += safe_image[i+1][j-1].rgbtGreen;
    //    average_green += safe_image[i+1][j].rgbtGreen;
    //    average_green += safe_image[i+1][j+1].rgbtGreen;
    //    average_green /= (double) 6;
//
    //    round_average(&average_green);
    //    image[i-1][j-1].rgbtGreen = average_green;
//
    //    //
    //    average_blue += safe_image[i][j-1].rgbtBlue;
    //    average_blue += safe_image[i][j].rgbtBlue;
    //    average_blue += safe_image[i][j+1].rgbtBlue;
    //    average_blue += safe_image[i+1][j-1].rgbtBlue;
    //    average_blue += safe_image[i+1][j].rgbtBlue;
    //    average_blue += safe_image[i+1][j+1].rgbtBlue;
    //    average_blue /= (double) 6;
//
    //    round_average(&average_blue);
    //    image[i-1][j-1].rgbtBlue = average_blue;
    //}
    else if (i == height)
    {
        //
        average_red += safe_image[i-1][j-1].rgbtRed;
        average_red += safe_image[i-1][j].rgbtRed;
        average_red += safe_image[i-1][j+1].rgbtRed;
        average_red += safe_image[i][j-1].rgbtRed;
        average_red += safe_image[i][j].rgbtRed;
        average_red += safe_image[i][j+1].rgbtRed;
        average_red /= (double) 6;

        round_average(&average_red);
        image[i-1][j-1].rgbtRed = average_red;

        //
        average_green += safe_image[i-1][j-1].rgbtGreen;
        average_green += safe_image[i-1][j].rgbtGreen;
        average_green += safe_image[i-1][j+1].rgbtGreen;
        average_green += safe_image[i][j-1].rgbtGreen;
        average_green += safe_image[i][j].rgbtGreen;
        average_green += safe_image[i][j+1].rgbtGreen;
        average_green /= (double) 6;

        round_average(&average_green);
        image[i-1][j-1].rgbtGreen = average_green;

        //
        average_blue += safe_image[i-1][j-1].rgbtBlue;
        average_blue += safe_image[i-1][j].rgbtBlue;
        average_blue += safe_image[i][j+1].rgbtBlue;
        average_blue += safe_image[i][j-1].rgbtBlue;
        average_blue += safe_image[i][j].rgbtBlue;
        average_blue += safe_image[i][j+1].rgbtBlue;
        average_blue /= (double) 6;

        round_average(&average_blue);
        image[i-1][j-1].rgbtBlue = average_blue;
    }
    else
    {
        //
        average_red += safe_image[i-1][j-1].rgbtRed;
        average_red += safe_image[i-1][j].rgbtRed;
        average_red += safe_image[i-1][j+1].rgbtRed;
        average_red += safe_image[i][j-1].rgbtRed;
        average_red += safe_image[i][j].rgbtRed;
        average_red += safe_image[i][j+1].rgbtRed;
        average_red += safe_image[i+1][j-1].rgbtRed;
        average_red += safe_image[i+1][j].rgbtRed;
        average_red += safe_image[i+1][j+1].rgbtRed;
        average_red /= (double) 9;

        round_average(&average_red);
        image[i-1][j-1].rgbtRed = average_red;

        //
        average_green += safe_image[i-1][j-1].rgbtGreen;
        average_green += safe_image[i-1][j].rgbtGreen;
        average_green += safe_image[i-1][j+1].rgbtGreen;
        average_green += safe_image[i][j-1].rgbtGreen;
        average_green += safe_image[i][j].rgbtGreen;
        average_green += safe_image[i][j+1].rgbtGreen;
        average_green += safe_image[i+1][j-1].rgbtGreen;
        average_green += safe_image[i+1][j].rgbtGreen;
        average_green += safe_image[i+1][j+1].rgbtGreen;
        average_green /= (double) 9;

        round_average(&average_green);
        image[i-1][j-1].rgbtGreen = average_green;

        //
        average_blue += safe_image[i-1][j-1].rgbtBlue;
        average_blue += safe_image[i-1][j].rgbtBlue;
        average_blue += safe_image[i-1][j+1].rgbtBlue;
        average_blue += safe_image[i][j-1].rgbtBlue;
        average_blue += safe_image[i][j].rgbtBlue;
        average_blue += safe_image[i][j+1].rgbtBlue;
        average_blue += safe_image[i+1][j-1].rgbtBlue;
        average_blue += safe_image[i+1][j].rgbtBlue;
        average_blue += safe_image[i+1][j+1].rgbtBlue;
        average_blue /= (double) 9;

        round_average(&average_blue);
        image[i-1][j-1].rgbtBlue = average_blue;
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Technique for preventing accessing neighbors out of bounds safely
    // using a bigger matrix taken from https://stackoverflow.com/a/652411
    const int SAFE_HEIGHT = height + 2;
    const int SAFE_WIDTH  = width + 2;
    RGBTRIPLE safe_image[SAFE_HEIGHT][SAFE_WIDTH];

    fill_safe_image(height, width, image, SAFE_HEIGHT, SAFE_WIDTH, safe_image);

    // For each row of pixels
    for (int i = 1; i <= height; i++)
    {
        // For each pixel in row
        for (int j = 1; j <= width; j++)
        {
            blur_pixel(i, j, height, width, image, SAFE_HEIGHT, SAFE_WIDTH, safe_image);
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
