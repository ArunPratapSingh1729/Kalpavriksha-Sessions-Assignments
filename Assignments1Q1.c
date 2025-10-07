#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main()
{

    char input[100];
    char operator[50];
    int number[50];
    int numbercount = 0;
    int operatorcount = 0;
    int totalsum = 0;
    int tempnum = 0;

    printf("Enter the input \n");
    fgets(input, 100, stdin);

    int len = strlen(input);

    for (int i = 0; i < len; i++)
    {

        char ch = input[i];

        if (isdigit(ch))
        {
            tempnum = tempnum * 10 + (ch - '0');
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
        {
            int j = i + 1;
            while (j < len && (input[j] == ' ' || input[j] == '\n'))
            {
                j++; 
            }

            if (!isdigit(input[j]))
            {
                printf("Invalid expression \n");
                return 1;
            }
            number[numbercount++] = tempnum; 
            tempnum = 0;
            operator[operatorcount++] = ch;
        }
        else if (ch == ' ' || ch == '\n')
        {
            continue;
        }
        else
        {
            printf("Special Characters not allowed \n");
            return 1;
        }
    }

    number[numbercount++] = tempnum; 
    int nextnum = 0;

    for (int i = 0; i < operatorcount; i++)
    {

        char ch = operator[i];
        nextnum = number[i + 1];

        switch (ch)
        {
        case '/':
            if (nextnum == 0)
            {
                printf("Division by zero not allowed\n");
                return 1;
            }
            number[i] = number[i] / nextnum;
            break;

        case '*':
            number[i] = number[i] * nextnum;
            break;

        case '%':
            number[i] = number[i] % nextnum;
            break;

        case '+':
            number[i] = number[i] + nextnum;
            break;

        case '-':
            number[i] = number[i] - nextnum;
            break;

        default:

            break;
        }

        for (int j = i + 1; j < numbercount - 1; j++)
        {
            number[j] = number[j + 1];
        }
        for (int j = i; j < operatorcount - 1; j++)
        {
            operator[j] = operator[j + 1];
        }
        numbercount--;
        operatorcount--;
        i--;
    }

    printf("The result is %d  \n", number[0]);

    return 0;
}
