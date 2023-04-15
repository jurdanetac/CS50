from cs50 import get_int


# Main program section
def main():
    while True:
        card = get_int("Number: ")
        if card > 0 and len(str(card)) < 17:
            break

    if luhns_check(card):
        determine_issuer(card)
    else:
        print("INVALID")


# Checks wether provided card number is, indeed, valid
def luhns_check(card_number):
    digit = 0
    sum = 0

    while card_number:
        current_digit = card_number % 10
        card_number = int(card_number / 10)
        digit += 1

        if not (digit % 2):
            product = current_digit * 2

            while product:
                current_subdigit = product % 10
                product = int(product / 10)
                sum += current_subdigit
        else:
            sum += current_digit

    if not (sum % 10):
        return True

    return False


# Determine the card's issuer
def determine_issuer(credit_card):
    card_digits = len(str(credit_card))
    counted = 0

    while counted < card_digits - 1:
        # Read one number of the credit card, starting with the last one
        second_digit = credit_card % 10
        # Shorten credit card number one place
        credit_card = int(credit_card / 10)
        # Increase counted numbers by one
        counted += 1

    # Print who issued card
    match card_digits:
        case 13:
            if credit_card == 4:
                print("VISA")
            else:
                print("INVALID")
        case 15:
            if credit_card == 3 and (second_digit == 4 or second_digit == 7):
                print("AMEX")
            else:
                print("INVALID")
        case 16:
            if credit_card == 4:
                print("VISA")
            elif credit_card == 5 and (second_digit >= 1 and second_digit <= 5):
                print("MASTERCARD")
            else:
                print("INVALID")
        case _:
            print("INVALID")


# If running from this file
if __name__ == "__main__":
    # Code execution begins here"
    main()