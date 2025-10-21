#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void assign(int N, int ***arr)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            int *value = malloc(sizeof(int));
            *value = rand() % 256;
            *(*(arr + i) + j) = value;
        }
}

void printfunction(int N, int ***arr)
{
    printf("\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%3d ", *(*(*(arr + i) + j)));
        printf("\n");
    }
}

void rotatefunction(int N, int ***arr)
{
    for (int layer = 0; layer < N / 2; layer++)
    {
        int first = layer;
        int last = N - 1 - layer;

        for (int i = first; i < last; i++)
        {
            int offset = i - first;

            int **top = (*(arr + first) + i);
            int **left = (*(arr + last - offset) + first);
            int **bottom = (*(arr + last) + last - offset);
            int **right = (*(arr + i) + last);

            int *temp = *top;
            *top = *left;
            *left = *bottom;
            *bottom = *right;
            *right = temp;
        }
    }
}

void smoothfunction(int N, int ***arr)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int sum = 0, count = 0;

            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    int neighbouri = i + dx, neighbourj = j + dy;
                    if (neighbouri >= 0 && neighbouri < N && neighbourj >= 0 && neighbourj < N)
                    {
                        sum += *(*(*(arr + neighbouri) + neighbourj)) % 1000;
                        count++;
                    }
                }
            }

            int avg = sum / count;
            *(*(*(arr + i) + j)) += avg * 1000;
        }
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            *(*(*(arr + i) + j)) = *(*(*(arr + i) + j)) / 1000;
}

int main()
{
    srand(time(NULL));

    int N;
    while (1)
    {        
        printf("Enter matix size: ");
        scanf("%d", &N);
        
        if(N < 2 && N > 10){
            printf("Invalid size \n");
            continue;
        }
        break;
    }

    int ***arr = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++)
    {
        *(arr + i) = (int **)malloc(N * sizeof(int *));
    }

    assign(N, arr);

    printf("Original Matrix:\n");
    printfunction(N, arr);

    rotatefunction(N, arr);
    printf("\nMatrix after 90 degree clockwise rotation:");
    printfunction(N, arr);

    smoothfunction(N, arr);
    printf("\nAfter Smoothing the Matrix:");
    printfunction(N, arr);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            free(*(*(arr + i) + j));
        free(*(arr + i));
    }
    free(arr);

    return 0;
}
