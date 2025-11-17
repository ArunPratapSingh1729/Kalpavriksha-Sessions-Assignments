#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h";

typedef struct Player
{
    int id;
    char name[50];
    char teamName[50];
    char role[20];
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct Player *next;
} Player;

typedef struct Team
{
    int id;
    char name[50];
    int totalPlayers;
    float avgBattingStrikeRate;
    struct Team *next;
} Team;

Player *playersHead = NULL;
Team *teamsHead = NULL;

Team *findTeamByName(const char *teamName)
{
    Team *t = teamsHead;
    while (t)
    {
        if (strcmp(t->name, teamName) == 0)
            return t;
        t = t->next;
    }
    return NULL;
}

float getPerformanceIndex(Player *p)
{
    if (strcmp(p->role, "Batsman") == 0)
        return (p->battingAverage * p->strikeRate) / 100.0f;
    else if (strcmp(p->role, "Bowler") == 0)
        return (p->wickets * 2.0f) + (100.0f - p->economyRate);
    else if (strcmp(p->role, "All-rounder") == 0)
        return ((p->battingAverage * p->strikeRate) / 100.0f) + (p->wickets * 2.0f);
    else
        return 0.0f;
}

int isDuplicatePlayer(int id, const char *name, const char *teamName)
{
    Player *p = playersHead;
    while (p)
    {
        if (p->id == id)
            return 1;
        if (strcmp(p->name, name) == 0 && strcmp(p->teamName, teamName) == 0)
            return 1;
        p = p->next;
    }
    return 0;
}

void initTeams()
{
    for (int i = 0; i < 10; i++)
    {
        Team *t = (Team *)malloc(sizeof(Team));
        if (!t)
        {
            printf("Memory allocation failed for Team\n");
            exit(1);
        }
        t->id = i + 1;
        strcpy(t->name, teams[i]);
        t->totalPlayers = 0;
        t->avgBattingStrikeRate = 0.0f;
        t->next = teamsHead;
        teamsHead = t;
    }
}

void initPlayers()
{
    for (int i = 0; i < 200; i++)
    {
        Player *p = (Player *)malloc(sizeof(Player));
        if (!p)
        {
            printf("Memory allocation failed for Player\n");
            exit(1);
        }

        p->id = players[i].id;
        strcpy(p->name, players[i].name);
        strcpy(p->teamName, players[i].team);
        strcpy(p->role, players[i].role);
        p->totalRuns = players[i].totalRuns;
        p->battingAverage = players[i].battingAverage;
        p->strikeRate = players[i].strikeRate;
        p->wickets = players[i].wickets;
        p->economyRate = players[i].economyRate;
        p->performanceIndex = getPerformanceIndex(p);

        Team *t = findTeamByName(p->teamName);
        if (!t)
        {
            printf("Warning: Player '%s' has invalid team name '%s' (skipping)\n",
                   p->name, p->teamName);
            free(p);
            continue;
        }

        p->next = playersHead;
        playersHead = p;

        t->totalPlayers++;
    }
}

void updateTeamStats()
{
    Team *t = teamsHead;
    while (t)
    {
        int count = 0;
        float sumSR = 0.0f;
        Player *p = playersHead;

        while (p)
        {
            if (strcmp(p->teamName, t->name) == 0)
            {
                if (strcmp(p->role, "Batsman") == 0 || strcmp(p->role, "All-rounder") == 0)
                {
                    sumSR += p->strikeRate;
                    count++;
                }
            }
            p = p->next;
        }

        t->avgBattingStrikeRate = (count > 0) ? (sumSR / count) : 0.0f;
        t = t->next;
    }
}

void mergeTeams(Team **arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Team **L = (Team **)malloc(n1 * sizeof(Team *));
    Team **R = (Team **)malloc(n2 * sizeof(Team *));
    if (!L || !R)
    {
        printf("Memory allocation failed in mergeTeams\n");
        exit(1);
    }

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (L[i]->avgBattingStrikeRate >= R[j]->avgBattingStrikeRate)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSortTeams(Team **arr, int left, int right)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    mergeSortTeams(arr, left, mid);
    mergeSortTeams(arr, mid + 1, right);

    mergeTeams(arr, left, mid, right);
}

void mergePlayers(Player **arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Player **L = (Player **)malloc(n1 * sizeof(Player *));
    Player **R = (Player **)malloc(n2 * sizeof(Player *));
    if (!L || !R)
    {
        printf("Memory allocation failed in mergePlayers\n");
        exit(1);
    }

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (L[i]->performanceIndex >= R[j]->performanceIndex)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSortPlayers(Player **arr, int left, int right)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    mergeSortPlayers(arr, left, mid);
    mergeSortPlayers(arr, mid + 1, right);
    mergePlayers(arr, left, mid, right);
}

void addPlayer()
{
    Player *p = (Player *)malloc(sizeof(Player));
    if (!p)
    {
        printf("Memory allocation failed for Player\n");
        return;
    }

    printf("Enter Player ID: ");
    scanf("%d", &p->id);

    printf("Enter Player Name: ");
    scanf("%49s", p->name);

    char teamName[50];
    printf("Enter Team Name: ");
    scanf("%49s", teamName);

    Team *t = findTeamByName(teamName);
    if (!t)
    {
        printf("Invalid Team Name '%s'!\n", teamName);
        free(p);
        return;
    }

    if (isDuplicatePlayer(p->id, p->name, t->name))
    {
        printf("Duplicate player detected! ID or Name+Team already exists.\n");
        free(p);
        return;
    }

    strcpy(p->teamName, t->name);

    printf("Enter Role (Batsman/Bowler/All-rounder): ");
    scanf("%19s", p->role);

    printf("Enter Total Runs: ");
    scanf("%d", &p->totalRuns);

    printf("Enter Batting Average: ");
    scanf("%f", &p->battingAverage);

    printf("Enter Strike Rate: ");
    scanf("%f", &p->strikeRate);

    printf("Enter Wickets: ");
    scanf("%d", &p->wickets);

    printf("Enter Economy Rate: ");
    scanf("%f", &p->economyRate);

    p->performanceIndex = getPerformanceIndex(p);

    p->next = playersHead;
    playersHead = p;

    t->totalPlayers++;
    printf("Player added successfully to Team %s!\n", t->name);
}

void displayPlayersOfTeam()
{
    char teamName[50];
    printf("Enter Team Name: ");
    scanf("%49s", teamName);

    Team *t = findTeamByName(teamName);
    if (!t)
    {
        printf("Invalid Team Name '%s'!\n", teamName);
        return;
    }

    printf("Players of Team %s:\n", t->name);
    printf("ID Name Role Runs Avg SR Wkts ER Perf.Index\n");

    int count = 0;
    Player *p = playersHead;
    while (p)
    {
        if (strcmp(p->teamName, t->name) == 0)
        {
            printf("%d %s %s %d %.2f %.2f %d %.2f %.2f\n",
                   p->id, p->name, p->role, p->totalRuns,
                   p->battingAverage, p->strikeRate,
                   p->wickets, p->economyRate, p->performanceIndex);
            count++;
        }
        p = p->next;
    }

    updateTeamStats();
    printf("Total Players: %d\n", count);
    printf("Average Batting Strike Rate: %.2f\n", t->avgBattingStrikeRate);
}

void displayTeamsByAvgSR()
{
    updateTeamStats();

    int tcount = 0;
    Team *t = teamsHead;
    while (t)
    {
        tcount++;
        t = t->next;
    }

    if (tcount == 0)
    {
        printf("No teams available.\n");
        return;
    }

    Team **arr = (Team **)malloc(sizeof(Team *) * tcount);
    if (!arr)
    {
        printf("Memory allocation failed in displayTeamsByAvgSR\n");
        return;
    }

    t = teamsHead;
    for (int i = 0; i < tcount; i++)
    {
        arr[i] = t;
        t = t->next;
    }

    if (tcount > 1)
        mergeSortTeams(arr, 0, tcount - 1);

    printf("ID Team Name AvgBatSR TotalPlayers\n");
    for (int i = 0; i < tcount; i++)
        printf("%d %s %.2f %d\n",
               arr[i]->id, arr[i]->name,
               arr[i]->avgBattingStrikeRate, arr[i]->totalPlayers);

    free(arr);
}

void displayTopKByRole()
{
    char teamName[50];
    int k;
    char role[20];

    printf("Enter Team Name: ");
    scanf("%49s", teamName);

    printf("Enter Role (Batsman/Bowler/All-rounder): ");
    scanf("%19s", role);

    printf("Enter number of players (K): ");
    scanf("%d", &k);

    Player *list[200];
    int count = 0;
    Player *p = playersHead;
    while (p)
    {
        if (strcmp(p->teamName, teamName) == 0 && strcmp(p->role, role) == 0)
            list[count++] = p;
        p = p->next;
    }

    if (count == 0)
    {
        printf("No players found for Team %s with role %s\n", teamName, role);
        return;
    }

    if (count > 1)
        mergeSortPlayers(list, 0, count - 1);

    printf("Top %d %s of Team %s:\n", k, role, teamName);
    printf("ID Name Role Runs Avg SR Wkts ER Perf.Index\n");

    for (int i = 0; i < k && i < count; i++)
    {
        printf("%d %s %s %d %.2f %.2f %d %.2f %.2f\n",
               list[i]->id, list[i]->name, list[i]->role,
               list[i]->totalRuns, list[i]->battingAverage,
               list[i]->strikeRate, list[i]->wickets,
               list[i]->economyRate, list[i]->performanceIndex);
    }
}

void displayAllByRoleGlobal()
{
    char role[20];
    printf("Enter Role (Batsman/Bowler/All-rounder): ");
    scanf("%19s", role);

    Player *list[200];
    int count = 0;
    Player *p = playersHead;
    while (p)
    {
        if (strcmp(p->role, role) == 0)
            list[count++] = p;
        p = p->next;
    }

    if (count == 0)
    {
        printf("No players found with role %s\n", role);
        return;
    }

    if (count > 1)
        mergeSortPlayers(list, 0, count - 1);

    printf("%s across all teams:\n", role);
    printf("ID Name Team Role Runs Avg SR Wkts ER Perf.Index\n");
    for (int i = 0; i < count; i++)
        printf("%d %s %s %s %d %.2f %.2f %d %.2f %.2f\n",
               list[i]->id, list[i]->name, list[i]->teamName,
               list[i]->role, list[i]->totalRuns,
               list[i]->battingAverage, list[i]->strikeRate,
               list[i]->wickets, list[i]->economyRate,
               list[i]->performanceIndex);
}

int main()
{
    initTeams();
    initPlayers();

    while (1)
    {
        printf("\nICC ODI Player Performance Analyzer\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display all Players of specific role Across All Teams by performance index\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        int ch;
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            addPlayer();
            break;
        case 2:
            displayPlayersOfTeam();
            break;
        case 3:
            displayTeamsByAvgSR();
            break;
        case 4:
            displayTopKByRole();
            break;
        case 5:
            displayAllByRoleGlobal();
            break;
        case 6:
            return 0;
        default:
            printf("Invalid choice!\n");
        }
    }
    return 0;
}
