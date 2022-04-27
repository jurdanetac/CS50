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

    char digit;

    // do everything here prevents iterating the same string on three functions
    for (int i = 0; i <= LENGTH; i++)
    {
        digit = tolower(text[i]);

        if (isalpha(digit))
        {
            letters++;
        }
        else if (isspace(digit) || digit == '\0')
        {
            words++;
        }
        else if (digit == '.' || digit == '?' || digit == '!')
        {
            sentences++;
        }
    }

    int avg_letters = letters * 100 / words;
    int avg_sentences = sentences *100 / words;

    int index = round(0.0588 * avg_letters - 0.296 * avg_sentences - 15.8);

    // print user input to screen
    printf("%s\n", text);

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
        printf("Grade: %i", index);
    }

    printf("\n");
}