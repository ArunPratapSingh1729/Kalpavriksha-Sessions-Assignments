#include <stdio.h>
#include <string.h>
#include <ctype.h>

int n = 0; 

struct Student
{
    char name[50];
    int rollNo;
    float subject1;
    float subject2;
    float subject3;
    char grade;
    char performance[6]; 
};

void calcSumAndAvg(float a, float b, float c, float *avg, float *sum)
{
    *sum = a + b + c;
    *avg = *sum / 3;
}

void assignGrade(float *avg, char *grade)
{
    switch ((int)(*avg - 5) / 10)
    {
    case 8:
    case 9:
        *grade = 'A';
        break;
    case 7:
        *grade = 'B';
        break;
    case 5:
    case 6:
        *grade = 'C';
        break;
    case 3:
    case 4:
        *grade = 'D';
        break;
    default:
        *grade = 'F';
    }
}

void assignPerformance(char *grade, char performance[6])
{
    switch (*grade)
    {
    case 'A':
        strcpy(performance, "*****");
        break;
    case 'B':
        strcpy(performance, "****");
        break;
    case 'C':
        strcpy(performance, "***");
        break;
    case 'D':
        strcpy(performance, "**");
        break;
    default:
        strcpy(performance, "");
        break;
    }
}

void printRollNumbers(struct Student students[], int i)
{
    if (i == n)
        return;
    printf("%d ", students[i].rollNo);
    printRollNumbers(students, i + 1);
}

void sortStudents(struct Student students[])
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (students[j].rollNo > students[j + 1].rollNo)
            {
                struct Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
}

void inputStudents(struct Student students[])
{
    for (int i = 0; i < n; i++)
    {
        while (1)
        {
            printf("Enter the name of student %d: ", i + 1);
            fgets(students[i].name, sizeof(students[i].name), stdin);

            students[i].name[strcspn(students[i].name, "\n")] = '\0';

            if (strlen(students[i].name) == 0)
            {
                printf("Name cannot be empty. Try again.\n");
                continue;
            }

            int valid = 0;
            for (int j = 0; students[i].name[j] != '\0'; j++)
            {
                if (!isalpha(students[i].name[j]))
                {
                    printf("Name must contain only letters. Try again.\n");
                    valid = 1;
                    break;
                }
            }
            if (!valid)
                break;
        }

        while (1)
        {
            printf("Enter roll number of student %d: ", i + 1);
            if (scanf("%d", &students[i].rollNo) != 1)
            {
                printf("Invalid input. Enter a number.\n");
                while (getchar() != '\n') 
                continue;
            }
            getchar(); 

            if (students[i].rollNo < 0)
            {
                printf("Roll number cannot be negative. Try again.\n");
                continue;
            }

            int duplicate = 0;
            for (int k = 0; k < i; k++)
            {
                if (students[i].rollNo == students[k].rollNo)
                {
                    printf("Roll number already exists. Enter again.\n");
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate)
                break;
        }

        float *marks[3] = {&students[i].subject1, &students[i].subject2, &students[i].subject3};
        for (int i = 0; i < 3; i++)
        {
            while (1)
            {
                printf("Enter marks for Subject %d (0-100) of student %d: ", i + 1, i + 1);
                if (scanf("%f", marks[i]) != 1)
                {
                    printf("Invalid input. Enter a number.\n");
                    while (getchar() != '\n'){}
                }
                getchar(); 

                if (*marks[i] < 0 || *marks[i] > 100)
                {
                    printf("Marks must be between 0 and 100. Try again.\n");
                    continue;
                }
                break;
            }
        }

        float sum = 0, avg = 0;
        calcSumAndAvg(students[i].subject1, students[i].subject2, students[i].subject3, &avg, &sum);
        assignGrade(&avg, &students[i].grade);
        if (avg <= 35){
             continue;
        }
        else{
            assignPerformance(&students[i].grade, students[i].performance);
        }

        printf("\n");
    }
}

void outputStudents(struct Student students[])
{
    sortStudents(students);

    printf("\nStudent Details:\n");
    for (int i = 0; i < n; i++)
    {
        printf("Name: %s\nRollNo: %d\nGrade: %c\nPerformance: %s\n\n",
               students[i].name, students[i].rollNo, students[i].grade, students[i].performance);
    }

    printf("All Roll Numbers:\n");
    printRollNumbers(students, 0);
    printf("\n");
}

int main()
{
    printf("Enter the number of students: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > 1000)
    {
        printf("Invalid number of students. Must be 1-1000.\n");
        return 1;
    }
    getchar(); 

    struct Student students[n];

    inputStudents(students);
    outputStudents(students);

    return 0;
}
