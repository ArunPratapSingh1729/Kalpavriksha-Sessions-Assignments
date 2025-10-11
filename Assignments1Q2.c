#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct User
{
    int id;
    char name[10];
    int age;
};

int valdiation(const char *name, int age)
{
    if (age < 0 || age > 100)
    {
        printf("Invalid age : %d \n", age);
        return 0;
    }
    if (strlen(name) == 0)
    {
        printf("ReEnter the name \n");
        return 0;
    }

    for (int i = 0; name[i] != '\0'; i++)
    {

        if (!isalpha(name[i]))
        {
            printf("Enter the Correct name \n");
            return 0;
        }
    }
    return 1;
}

void AddUser(int id, const char *name, int age)
{
    if (!valdiation(name, age))
        return;

    FILE *f = fopen("users.txt", "r");
    if (f == NULL)
    {
        f = fopen("users.txt", "a");
    }

    int found = 0;
    char line[100];
    struct User user;

    if (f != NULL)
    {
        while (fgets(line, sizeof(line), f) != NULL)
        {
            sscanf(line, "%d %s %d", &user.id, user.name, &user.age);
            if (id == user.id)
            {
                found = 1;
                break;
            }
        }
        fclose(f);
    }

    if (!found)
    {
        f = fopen("users.txt", "a");
        if (f == NULL)
        {
            perror("Error opening file");
            return;
        }
        fprintf(f, "%d %s %d\n", id, name, age);
        fclose(f);
    }
}

void ReadUser()
{
    FILE *f = fopen("users.txt", "r");
    if (f == NULL)
    {
        perror("Error ! \n");
        return;
    }
    struct User user;
    char line[100];
    int count = 0;

    printf("List Of All Users \n");
    while (fgets(line, sizeof(line), f) != NULL)
    {
        count = 1;
        sscanf(line, "%d %s %d", &user.id, user.name, &user.age);
        printf("%d , %s, %d \n", user.id, user.name, user.age);
    }

    if (count == 0)
    {
        printf("No User Found \n");
    }

    fclose(f);
}

void UpdateUser(int id, const char *name, int age)
{
    if (!valdiation(name, age))
        return;

    FILE *f = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (f == NULL)
    {
        perror("Error ! \n");
        return;
    }
    struct User user;
    char line[100];

    while (fgets(line, sizeof(line), f) != NULL)
    {
        sscanf(line, "%d %s %d", &user.id, user.name, &user.age);
        if (id == user.id)
        {
            fprintf(temp, "%d %s %d \n", id, name, age);
        }
        else
        {
            fprintf(temp, "%d %s %d \n", user.id, user.name, user.age);
        }
    }
    fclose(f);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");
}

void DeleteUser(int id)
{

    FILE *f = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (f == NULL)
    {
        perror("Error ! \n");
        return;
    }
    struct User user;
    char line[100];

    while (fgets(line, sizeof(line), f) != NULL)
    {
        sscanf(line, "%d %s %d", &user.id, user.name, &user.age);
        if (id == user.id)
        {
            continue;
        }
        else
        {
            fprintf(temp, "%d %s %d \n", user.id, user.name, user.age);
        }
    }
    fclose(f);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");
}

int main()
{
    int choice;
    struct User u;

    while (1)
    {
        printf("1. Add the User\n");
        printf("2. Read the Users\n");
        printf("3. Update the User\n");
        printf("4. Delete the User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            printf("Enter User ID: ");
            scanf("%d", &u.id);
            getchar();

            printf("Enter User Name: ");
            fgets(u.name, sizeof(u.name), stdin);
            u.name[strcspn(u.name, "\n")] = '\0';

            printf("Enter User Age: ");
            scanf("%d", &u.age);
            getchar();

            AddUser(u.id, u.name, u.age);
            break;

        case 2:
            ReadUser();
            break;

        case 3:
            printf("Enter User ID to update: ");
            scanf("%d", &u.id);
            getchar();

            printf("Enter new Name: ");
            fgets(u.name, sizeof(u.name), stdin);
            u.name[strcspn(u.name, "\n")] = 0;

            printf("Enter new Age: ");
            scanf("%d", &u.age);
            getchar();

            UpdateUser(u.id, u.name, u.age);
            break;

        case 4:
            printf("Enter User ID to delete: ");
            scanf("%d", &u.id);
            getchar();

            DeleteUser(u.id);
            break;

        case 5:
            printf("Exiting...\n");
            exit(0);

        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}
