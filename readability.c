#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void)
{
    // prompt user for input
    string text = get_string("Text: ");
    int LENGTH = strlen(text);

    int words = 0;
    int letters = 0;
    int sentences = 0;

    // used as auxiliary on for loop to count
    char digit;

    // doing everything here prevents iterating the same string on 3 functions
    for (int i = 0; i <= LENGTH; i++)
    {
        // prevent uppercase and reevaluating the same char
        digit = tolower(text[i]);

        // is alphabetical
        if (isalpha(digit))
        {
            letters++;
        }
        // is space or end of string
        else if (isspace(digit) || digit == '\0')
        {
            words++;
        }
        // is end of sentence
        else if (digit == '.' || digit == '?' || digit == '!')
        {
            sentences++;
        }
    }

    // note that multiplying '100' will alter our result since float * int trunc
    float avg_letters = letters * 100.0 / words;
    float avg_sentences = sentences * 100.0 / words;


    // print Coleman-Liau index rounded to nearest integer
    int index = round(0.0588 * avg_letters - 0.296 * avg_sentences - 15.8);

    if (index >= 16)
    {
        printf("Grade 16+");
    }
    else if (index < 1)
    {
        printf("Before Grade 1");
    }
    else
    {
        printf("Grade %i", index);
    }

    printf("\n");
}