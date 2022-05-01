#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

int verify_key(const string KEY);

int main(int argc, string argv[])
{
    // only run if cli arguments are the program itself and the key
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    const string KEY = argv[1];

    // verify_key returns a non-zero value if key isn't valid
    if (verify_key(KEY))
    {
        return 1;
    }

    // prompt user for text to be encrypted
    const string PLAINTEXT = get_string("plaintext: ");
    const int STR_LENGTH = strlen(PLAINTEXT);
    // create array that will hold plaintext after encryption
    char CIPHERTEXT[STR_LENGTH];

    for (int i = 0, current_digit; i < STR_LENGTH; i++)
    {
        current_digit = PLAINTEXT[i];

        // subtract to set ascii equivalent to 0, then pick key char
        if (islower(current_digit))
        {
            CIPHERTEXT[i] = tolower(KEY[current_digit - 97]);
        }
        else if (isupper(current_digit))
        {
            CIPHERTEXT[i] = toupper(KEY[current_digit - 65]);
        }
        else
        {
            CIPHERTEXT[i] = current_digit;
        }
    }

    // print ciphertext char by char
    printf("ciphertext: ");

    for (int i = 0; i < STR_LENGTH; i++)
    {
        printf("%c", CIPHERTEXT[i]);
    }

    printf("\n");

    // program finished gracefully
    return 0;
}

int verify_key(const string KEY)
{
    // in this case must be 26
    const int KEY_LENGTH = strlen(KEY);

    if (KEY_LENGTH != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // iterate to verify if there are repeated chars in key
    // for char in key
    for (int i = 0, current_digit; i < KEY_LENGTH; i++)
    {
        current_digit = tolower(KEY[i]);

        // if current digit is not alphabetical
        if (!isalpha(current_digit))
        {
            printf("Key must contain only alphabetical characters.\n");
            return 1;
        }

        // for char in rest of key (without current digit)
        for (int j = i + 1, current_subdigit; j < KEY_LENGTH; j++)
        {
            current_subdigit = tolower(KEY[j]);

            // if ascii codes are equal characters are equal
            // more codes: asciitable.com
            if (current_digit == current_subdigit)
            {
                printf("Key cannot contain repeated letter.s\n");
                return 1;
            }
        }
    }

    return 0;
}