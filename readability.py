from cs50 import get_string


# Main program section
def main():
    # Prompt user for input
    text = get_string("Text: ")

    words = 0
    letters = 0
    sentences = 0

    for char in text:
        # Is alphabetical
        if char.isalpha():
            letters += 1
        # Is space
        elif char.isspace():
            words += 1
        # Is end of sentence
        elif char in [".", "?", "!"]:
            sentences += 1

    avg_letters = letters * 100 / words
    avg_sentences = sentences * 100 / words

    # Print Coleman-Liau index rounded to nearest integer
    index = round(0.0588 * avg_letters - 0.296 * avg_sentences - 15.8)

    if index >= 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


# If running from this file
if __name__ == "__main__":
    # Code execution begins here"
    main()