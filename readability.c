#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // prompt user for input
    string text = get_string("Text: ");
    int LENGTH = strlen(text);

    int words = 0;
    int letters = 0;
    int sentences = 0;

    char digit;

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

    // print user input to screen
    printf("letters: %i\n", letters);
    printf("words: %i\n", words);
    printf("sentences: %i\n", sentences);
}