from cs50 import get_int


def main():
    # Ask for pyramid's height until user cooperates
    while True:
        height = get_int("Height: ")
        if height > 0 and height < 9:
            break

    """
    Expected output if height == 5
    -> 1234#12#
    -> 123##12##
    -> 12###12###
    -> 1####12####
    -> #####12#####
    """

    WIDTH = height
    SEPARATION = 2

    for i in range(1, height + 1):
        spaces = WIDTH - i
        hashtags = WIDTH - spaces

        # We count starting from 1 to be able to calculate occurrences of each in row
        spaces = WIDTH - i
        hashtags = WIDTH - spaces

        # Print first pyramid
        print_repeatedly(" ", spaces)
        print_repeatedly("#", hashtags)

        # Print separation between the two pyramids
        print_repeatedly(" ", SEPARATION)

        # Print second pyramid
        print_repeatedly("#", hashtags)

        # Jump to next row
        print()


def print_repeatedly(message, times):
    for i in range(times):
        print(message, end="")


# If running from this file
if __name__ == "__main__":
    # Code execution begins here
    main()