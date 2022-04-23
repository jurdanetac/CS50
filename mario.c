#include <cs50.h>
#include <stdio.h>

void print_repeatedly(string message, int times);

int main(void)
{
    int height;

    /*
    expected output if height == 5

    -> 1234#12#
    -> 123##12##
    -> 12###12###
    -> 1####12####
    -> #####12#####
    */

    do
    {
        height = get_int("Height: ");
    }
    while (height <= 0);

    const int WIDTH = height;
    const int SEPARATION = 2;

    // for each row of the pyramid
    for (int i = 1; i <= height; i++)
    {
        // we count starting from 1 to be able to calculate occurrences of each in row
        int spaces = WIDTH - i;
        int hashtags = WIDTH - spaces;

        // print first pyramid
        print_repeatedly(" ", spaces);
        print_repeatedly("#", hashtags);

        // print separation between the two pyramids
        print_repeatedly(" ", SEPARATION);

        // print second pyramid
        print_repeatedly("#", hashtags);

        // jump to next row
        printf("\n");
    }
}

void print_repeatedly(string message, int times)
{
    for (int i = 0; i < times; i++)
    {
        printf("%s", message);
    }
}