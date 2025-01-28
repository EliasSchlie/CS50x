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
} pair;

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
bool connection(int a, int b, int closed[]);
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
            preferences[i][j] = 0;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];
        for (int j = 0; j < candidate_count; j++){
            ranks[j] = -1;
        }
        
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
    for (int i = 0; i < candidate_count; i++){
        // printf("%i\n", ranks[i]);
        if (strcmp(candidates[i], name) == 0 && ranks[i] == -1){
            ranks[i] = rank;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++){
        for (int j = 0; j < candidate_count; j++){
            // printf("Rank i: %i, Rank j: %i\n", ranks[i], ranks[j]);
            if (ranks[i] < ranks[j]){
                preferences[i][j]++;
            }
            // printf("Preference: %i\n", preferences[i][j]);
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int count = 0;
    for (int i = 0; i < candidate_count; i++){
        for (int j = i; j < candidate_count; j++){
            // printf("Rank i: %i, Rank j: %i\n", ranks[i], ranks[j]);
            if (preferences[i][j] < preferences[j][i]){
                pairs[count].loser = i;
                pairs[count].winner = j;
                // printf("Candidate %i won over candidate %i\n", pairs[count].winner, pairs[count].loser);
                count++;
            } else if (preferences[i][j] > preferences[j][i]){
                pairs[count].loser = j;
                pairs[count].winner = i;
                // printf("Candidate %i won over candidate %i\n", pairs[count].winner, pairs[count].loser);
                count++;
            }
        }
    }
    pair_count = count;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    /*
    printf("Sorting starts\n");
    
    int p_strength[pair_count];
    for (int i = 0; i<pair_count; i++){
        p_strength[i] = ((preferences[pairs[i].winner][pairs[i].loser]) - (preferences[pairs[i].loser][pairs[i].winner]));
        // printf("Candidate %i won over candidate %i with strength: %i\n", pairs[i].winner, pairs[i].loser, p_strength[i]);
    }
    */


    // for each entry of list
    for (int j = 0; j < pair_count; j++){
        // Go through i-> end of list searching for highest pair
        int highest = 0;
        int high_pos = j;
        for (int i = j; i < pair_count; i++){
            int p1 = (preferences[pairs[i].winner][pairs[i].loser]) - (preferences[pairs[i].loser][pairs[i].winner]);
            if (p1 > highest){
                highest = p1;
                high_pos = i;
                // printf("Yes %i\n", high_pos);
            }
        }
        // Swap highest pair with pair at spot i
        int temp_w = pairs[high_pos].winner;
        int temp_l = pairs[high_pos].loser;
        pairs[high_pos].winner = pairs[j].winner;
        pairs[high_pos].loser = pairs[j].loser;
        pairs[j].winner = temp_w;
        pairs[j].loser = temp_l;        
    }
    /*
    for (int j = 0; j < pair_count; j++){
        printf("Candidate %i won over candidate %i\n", pairs[j].winner, pairs[j].loser);
    }
    */
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int j = 0; j < pair_count; j++){
        
        int w = pairs[j].winner;
        int l = pairs[j].loser;
        int closed[pair_count];
        for (int i = 0; i < pair_count; i++){
            closed[i] = -1;
        }
        if (!(connection(l, w, closed))){
            // Confirm lock
            locked[w][l] = true;
        }
    }

    /*
    for (int j = 0; j < MAX; j++){
        for (int i = 0; i < MAX; i++){
            printf("%i\n", locked[j][i]);
        }
    }
    */
    return;
}

bool connection(int a, int b, int closed[]){
    if (locked[a][b]){
        return true;
    }
    // Check close list
    for (int j = 0; j < pair_count; j++){
        if (closed[j] == a){
            return false;
        } else if (closed[j] == -1){
            closed[j] = a;
            break;
        }
    }
    // Interate through all people a won against
    for (int j = 0; j < candidate_count ;j++){
        if (locked[a][j]){
            if (connection(j,b,closed)){
                return true;
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    int j = 0;
    int check = 1;
    do{
        int i = 0;
        do{
            if (locked[i][j]){
                check = 0;
            }
            i++;
        }while (check && i < candidate_count);
        if (check == 1){
            printf("%s\n", candidates[j]);
            return;
        }
        check = 1;
        j++;
    }while (j < candidate_count);
    
    /*
    for (int j = 0; j < candidate_count ;j++){
        for (int i = 0; i < candidate_count ;i++){
            if (locked[i][j]){
                break;
            }
        }
    }
    */
    return;
}
