#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User
{
    int id;
    char name[10];
    int age;
};

int main()
{

    struct User u1 = {1, "Arun", 12};
    struct User u2 = {2, "Pratap", 21};
    struct User u3 = {3, "Arun", 22};

    // we can also use the array here like stuct User user[100];

    return 0;
}
