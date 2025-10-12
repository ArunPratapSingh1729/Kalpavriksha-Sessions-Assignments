#include <stdio.h>
#include <string.h>
#include <ctype.h>

int n = 0; // global variable

struct Student
{
    char name[50];
    int rollno;
    float Subject1Marks;
    float Subject2Marks;
    float Subject3Marks;
    char grade;
    char performance[5];
};

void functiontoCalculateSum_Average(float a, float b, float c, float *avg, float *sum)
{
    *sum = a + b + c;
    *avg = *sum / 3;
}

void functiontoCalculateGrade(float *avg, char *Grade)
{

    if (*avg >= 85)
        *Grade = 'A';
    else if (*avg >= 70)
        *Grade = 'B';
    else if (*avg >= 50)
        *Grade = 'C';
    else if (*avg >= 35)
        *Grade = 'D';
    else
        *Grade = 'F';
}

void functiontoCalculatePerformace(char *Grade, char Performance[5])
{

    switch (*Grade)
    {
    case 'A':
        strcpy(Performance, "*****");
        break;
    case 'B':
        strcpy(Performance, "****");
        break;
    case 'C':
        strcpy(Performance, "***");
        break;
    case 'D':
        strcpy(Performance, "**");
        break;
    default:
        strcpy(Performance, "");
        break;
    }
}

void RecursionFunctionToPrintAllRollNumbers(struct Student S[], int i)
{
    // block variable
    if (i == n)
    {
        return;
    }

    printf("%d ", S[i].rollno);
    RecursionFunctionToPrintAllRollNumbers(S, i + 1);
}

void sort(struct Student S[])
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (S[j].rollno > S[j + 1].rollno)
            {
                // Swap the two students
                struct Student temp = S[j];
                S[j] = S[j + 1];
                S[j + 1] = temp;
            }
        }
    }
}
void input(struct Student S[])
{
    float avg = 0; // local variable
    float sum = 0; // local variable
    char Grade;

    for (int i = 0; i < n; i++)
    {

    Point0:
        printf("Enter the name for the Student no : %i \n", i + 1);
        fgets(S[i].name, 50, stdin);
        S[i].name[strcspn(S[i].name, "\n")] = '\0';

        for (int j = 0; S[i].name[j] != '\0'; j++)
        {
            if (!isalpha(S[i].name[j]))
            {
                printf("Please Enter the Correct Name \n");
                goto Point0;
            }
        }

    Point1:
        printf("Enter the rollno for the Student no : %i \n", i + 1);
        scanf("%d", &S[i].rollno);
        getchar();

        if (S[i].rollno < 0)
        {
            printf("Reagain Enter the correct Roll No \n");
            goto Point1;
        }
        int k = 0;
        while (k < i)
        {
            if (S[i].rollno == S[k].rollno)
            {
                printf("Roll Number Already Exists \n");
                goto Point1;
            }
            k++;
        }

    Point2:
        printf("Enter the marks for the Student no : %i in Subject 1\n", i + 1);
        scanf("%f", &S[i].Subject1Marks);
        getchar();

        if (S[i].Subject1Marks < 0 || S[i].Subject1Marks > 100)
        {
            printf("Marks must be between 0 and 100. Enter again.\n");
            goto Point2;
        }

    Point3:
        printf("Enter the marks for the Student no : %i in Subject 2\n", i + 1);
        scanf("%f", &S[i].Subject2Marks);
        getchar();

        if (S[i].Subject2Marks < 0 || S[i].Subject2Marks > 100)
        {
            printf("Marks must be between 0 and 100. Enter again.\n");
            goto Point3;
        }

    Point4:
        printf("Enter the marks for the Student no : %i in Subject 3\n", i + 1);
        scanf("%f", &S[i].Subject3Marks);
        getchar();

        if (S[i].Subject3Marks < 0 || S[i].Subject3Marks > 100)
        {
            printf("Marks must be between 0 and 100. Enter again.\n");
            goto Point4;
        }

        functiontoCalculateSum_Average(S[i].Subject1Marks, S[i].Subject2Marks, S[i].Subject3Marks, &avg, &sum);
        functiontoCalculateGrade(&avg, &S[i].grade);

        if (avg >= 35)
        {
            functiontoCalculatePerformace(&S[i].grade, S[i].performance);
        }
        else
        {
            continue; // use of continue
        }

        printf("\n");
    }
}

void output(struct Student S[])
{
    sort(S);

    for (int i = 0; i < n; i++)
    {
        printf("\n Student Name : %s \n Student RollNo : %d \n Grade : %c \n Performance : %s \n",
               S[i].name, S[i].rollno, S[i].grade, S[i].performance);
    }

    printf("Roll Numbers of All the Students are : \n");

    RecursionFunctionToPrintAllRollNumbers(S, 0);
}

int main()
{
    printf("Enter the number of Student Details you want to enter \n");
    scanf("%d", &n);
    getchar();

    struct Student S[n];

    input(S);

    output(S);

    return 0;
}
