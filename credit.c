#include <cs50.h>
#include <stdio.h>

int count_digits(long number);

bool luhns_check(long card);

void determine_issuer(long credit_card);


int main(void)
{
    long card;

    // prompt user for a valid card number until they cooperate
    do
    {
        card = get_long("Number: ");
    }
    while (!card);

    determine_issuer(card);
}

// count how many digits does an integer have
int count_digits(long number)
{
    int count = 0;

    do
    {
        // divide by 10 until number truncated to 0
        number /= 10;
        count++;
    }
    while (number);

    return count;
}

// implementation of Luhn's algorithm to syntactically verify card's number
bool luhns_check(long card_number)
{
    int current_digit, current_subdigit, digit = 0, product, sum = 0;

    do
    {
        // current digit from right to left
        current_digit = card_number % 10;
        // reduce the card's number one digit storing a floating point on long
        card_number /= 10;
        // how many digits have been counted
        digit++;

        // multiply every other digit by 2, starting with second-to-last digit
        if (!(digit % 2))
        {
            product = current_digit * 2;

            do
            {
                // current digit from right to left
                current_subdigit = product % 10;

                // reduce the card's number one digit
                product /= 10;

                // add those products’ digits together
                sum += current_subdigit;
            }
            while (product);
        }
        else
        {
            sum += current_digit;
        }

        // if the total modulo 10 is congruent to 0, the number is valid
        if (!(sum % 10))
        {
            return true;
        }
    }
    while (card_number);

    return false;
}

void determine_issuer(long credit_card)
{
    int card_digits = count_digits(credit_card);

    // looping modulus operator we can extract first two numbers of card
    int second_digit = 0, counted = 0;

    do
    {
        second_digit = credit_card % 10;
        // note credit_card will be reduced to a symbolic first_digit variable
        credit_card /= 10;
        counted++;
    }
    while (counted < card_digits - 1);

    switch (card_digits)
    {
        case 13:
            if (credit_card == 4)
            {
                printf("VISA");
                break;
            }

        case 15:
            if (credit_card == 3 && (second_digit == 4 || second_digit == 7))
            {
                printf("AMEX");
                // if the condition is not met, execution flow jumps to default
                break;
            }

        case 16:
        {
            if (credit_card == 4)
            {
                printf("VISA");
                break;
            }
            else if (credit_card == 5 && (second_digit >= 1 && second_digit <= 5))
            {
                printf("MASTERCARD");
                break;
            }
        }

        default:
            printf("INVALID");
            break;
    }

    // move cursor to next line
    printf("\n");
}