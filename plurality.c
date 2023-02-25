#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // Iterate over candidate list
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if candidate exists
        if (strcmp(name, candidates[i].name) == 0)
        {
            // Add a vote to the candidate's vote count
            candidates[i].votes++;

            // Indicate successful ballot
            return true;
        }
    }

    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    candidate winner;
    winner.name  = "";
    winner.votes = 0;

    // Number of candidates with the same votes as the winner
    int ties     = 0;

    // Iterate over candidate list
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > winner.votes)
        {
            // Update winner
            winner = candidates[i];

            // Iterate over the rest of the candidate list
            for (int j = i; j < candidate_count; j++)
            {
                // Check if there's another candidate with the same vote count as the winner
                if (strcmp(candidates[j].name, winner.name) != 0 && candidates[j].votes == winner.votes)
                {
                    ties++;
                }
            }
        }
    }

    if (ties)
    {
        // Create array of tied candidates, first one is the first winner encountered
        candidate winners[ties + 1];
        winners[0] = winner;

        // Iterate over candidate list
        for (int i = 0; i < candidate_count; i++)
        {
            // Populate the winners array
            if (candidates[i].votes == winner.votes && strcmp(candidates[i].name, winner.name) != 0)
            {
                winners[i] = candidates[i];
            }
        }

        // Print the winners' names
        for (int i = 0; i <= ties; i++)
        {
            printf("%s\n", winners[i].name);
        }
    }
    else
    {
        // Print the sole winner name if no ties
        printf("%s\n", winner.name);
    }

    // Printing successfully completed
    return;
}