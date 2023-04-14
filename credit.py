from cs50 import get_int


def main():
    while True:
        card = get_int("Number: ")
        if card > 0:
            break

    if luhns_check(card):
        determine_issuer(card)
    else:
        print("INVALID")


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

    print(sum)
    return False


def determine_issuer(credit_card):
    card_digits = len(str(credit_card))

    counted = 0

    while counted < card_digits - 1:
        second_digit = credit_card % 10
        credit_card = int(credit_card / 10)
        counted += 1

    match card_digits:
        case 13:
            if credit_card == 4:
                print("VISA")
        case 15:
            if credit_card == 3 and (second_digit == 4 or second_digit == 7):
                print("AMEX")
        case 16:
            if credit_card == 4:
                print("VISA")
            elif credit_card == 5 and (second_digit >= 1 and second_digit <= 5):
                print("MASTERCARD")
        case _:
            print("INVALID")


# If running from this file
if __name__ == "__main__":
    # Code execution begins here"
    main()