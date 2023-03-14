#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Repeat for each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if given candidate's name matches a valid candidate
        if (strcmp(candidates[i], name) == 0)
        {
            // ranks[rank] holds the number of candidate in the candidates list
            ranks[rank] = i;

            // Vote recorded successfully
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Repeat for each row
    for (int i = 0; i < candidate_count; i++)
    {
        // Repeat for each column
        for (int j = 0; j < candidate_count; j++)
        {
            // A candidate can't be preferred over itself
            if (i == j)
            {
                preferences[i][j] = 0;
                // Move onto next iteration
                continue;
            }

            // Repeat for each candidate
            for (int k = 0; k < candidate_count; k++)
            {
                // First candidate encountered determines if i preferred over j
                if (ranks[k] == i)
                {
                    // Increase preferences by one times, i is preferred over j
                    preferences[i][j]++;
                }
                else if (ranks[k] == j)
                {
                    // Quit iterating, j is preferred over i
                    break;
                }
            }
        }
    }

    /* // Print preferences[][]
    printf("\n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", preferences[i][j]);
        }
        printf("\n");
    }
    */


    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Pair to be added
    pair p;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // The i < j condition to loop over the top half of the matrix was taken from
            // https://stackoverflow.com/a/70023912
            if (preferences[i][j] > preferences[j][i] && i < j)
            {
                p.winner = i;
                p.loser = j;
            }
            else if (preferences[i][j] < preferences[j][i] && i < j)
            {
                p.winner = j;
                p.loser = i;
            }
            else
            {
                continue;
            }

            pairs[pair_count] = p;
            pair_count++;
        }
    }

    // Print pairs
    // printf("Pairs:\n");
    // for (int k = 0; k < pair_count; k++)
    // {
    //     printf("(%s, %s)\n", candidates[pairs[k].winner], candidates[pairs[k].loser]);
    // }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Bubble Sort pairs
    // References used to implement:
    // https://youtu.be/4oqjcKenCH8?t=5039
    // https://cs50.harvard.edu/x/2023/notes/3/#sorting
    // https://en.wikipedia.org/wiki/Bubble_sort#Implementation
    bool swaps = true;
    pair aux;

    do
    {
        swaps = false;

        for (int i = 0; i < pair_count; i++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                aux = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = aux;
                swaps = true;
                continue;
            }
        }
    }
    while (swaps);

    // Print preferences[][]
    // printf("\n");
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //         printf("%i ", preferences[i][j]);
    //     }
    //     printf("\n");
    // }

    // Print sorted pairs
    // printf("Sorted Pairs:\n");
    // for (int k = 0; k < pair_count; k++)
    // {
    //     printf("(%s, %s)\n", candidates[pairs[k].winner], candidates[pairs[k].loser]);
    // }

    return;
}


bool visited[MAX];
bool visiting[MAX];

bool creates_cycle(int v)
{
    int neighbors[MAX];
    int neighbor_count = 0;

    // Populate neighbors of current visited node
    for (int i = 0; i < pair_count; i++)
    {
        if (pairs[i].winner == v)
        {
            neighbors[neighbor_count] = pairs[i].loser;
            neighbor_count++;
        }
    }

    if (neighbor_count)
    {
        // Visit each neighbor
        for (int i = 0; i < neighbor_count; i++)
        {
            if (visiting[neighbors[i]])
            {
                return true;
            }

            visiting[neighbors[i]] = true;
            creates_cycle(neighbors[i]);
            visiting[neighbors[i]] = false;
            visited[v] = true;
            }
    }

    // Cycle not detected
    // return false;

    // for (int i = 0; i < neighbor_count; i++)
    // {
    // }

    // printf("%s has %i neighbors\n", candidates[neighbors[v]], neighbor_count);
    // for (int i = 0; i < neighbor_count; i++)
    // {
    //     printf("%s\n", candidates[neighbors[i]]);
    // }

    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (visited[pairs[i].winner])
        {
            continue;
        }

        creates_cycle(pairs[i].winner);
        locked[pairs[i].winner][pairs[i].loser] = true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", locked[i][j]);
        }
        printf("\n");
    }
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}