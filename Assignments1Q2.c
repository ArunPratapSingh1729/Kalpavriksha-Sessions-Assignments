#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User
{
    int id;
    char name[10];
    int age;
};

void AddUser(int id, const char *name, int age)
{
    FILE *f = fopen("users.txt", "r"); 
    if (f == NULL)
    {
        FILE *f = fopen("users.txt", "a");
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

    printf("List Of All Users \n");
    while (fgets(line, sizeof(line), f) != NULL)
    {
        sscanf(line, "%d %s %d", &user.id, user.name, &user.age);
        printf("%d , %s, %d \n", user.id, user.name, user.age);
    }

    fclose(f);
}

int main()
{

    struct User u1 = {1, "Arun", 12};
    struct User u2 = {2, "Pratap", 21};
    struct User u3 = {3, "Singh", 22};

    // we can also use the array here like stuct User user[100];

    AddUser(u1.id, u1.name, u1.age);
    AddUser(u2.id, u2.name, u2.age);
    AddUser(u3.id, u3.name, u3.age);
    // printf("Users added succesfully");

    ReadUser(); // To Read The users Data


    return 0;
}
