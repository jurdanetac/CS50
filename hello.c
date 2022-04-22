// bring CS50's declaration of string to our program
#include <cs50.h>
// bring printf to our program
#include <stdio.h>

int main(void)
{
    // prompt user for his name
    string name = get_string("What is your name? ");
    // show user's name in screen
    printf("hello, %s\n", name);
}