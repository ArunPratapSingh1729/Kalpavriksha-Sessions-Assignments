#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(){

  char entry[100];   //character array (string)  for taking input
  char operators[50]; //operator array to store the operators
  int numbers[50]; //numbers array to store the digits
  int Onum = 0;    //counter for the operators
  int Nnum = 0 ;   //counter for the numbers
  int num = 0;     
  printf("Enter the Input : \n");
  fgets(entry ,100, stdin);

  int len = strlen(entry);
  for(int i =0;i<len; i++){

    
      if (isdigit(entry[i]))
      {
          num = num * 10 + (entry[i] - '0');
      }
      else if (entry[i] == '+' || entry[i] == '-' || entry[i] == '*' || entry[i] == '/' || entry[i] == '%')  
      {
          int j = i + 1;
          while (j < len && (entry[j] == ' ' || entry[j] == '\n'))
          {
              j++; // this will skip mutliple spaces
          }

        if(j >= len || !isdigit(entry[j])){
            printf("Invalid expression \n");
            return 1;
        }
        numbers[Nnum++] = num;  //for handling the multiple digit number 
        num = 0;
          operators[Onum++] = entry[i];
      }
      else if (entry[i] == ' ' || entry[i] == '\n')
      {
          continue;
      }
      else
      {
          printf("Special Characters not allowed \n");
          return 1;
      }
  }
  numbers[Nnum++] = num;


for(int i=0; i<Onum; i++){
    int nextnum = numbers[i+1];
    if (operators[i] == '/' || operators[i] == '*' || operators[i] == '%')
    {
        if (operators[i] == '/' && numbers[i + 1] == 0)
        {
            printf("Division by zero not allowed\n");
            return 1;
        }
        else if (operators[i] == '/')
        {
            numbers[i] = numbers[i] / numbers[i + 1];
        }
        else if (operators[i] == '*')
        {
            numbers[i] = numbers[i] * numbers[i + 1];
        }
        else if (operators[i] == '%')
        {
            numbers[i] = numbers[i] % numbers[i + 1];
        }
        
        for(int j = i+1; j < Nnum-1; j++){
            numbers[j] = numbers[j+1];
        }
        for(int j=i; j < Onum-1; j++){
            operators[j] = operators[j+1];
        }
        Nnum --;
        Onum --;
        i --;
    }
}

for (int i = 0; i < Onum; i++)
{
    int nextnum = numbers[i + 1];
    if (operators[i] == '+' || operators[i] == '-' )
    {
        if (operators[i] == '+')
        {
            numbers[i] = numbers[i] + numbers[i + 1];
        }
        else if (operators[i] == '-')
        {
            numbers[i] = numbers[i] -numbers[i + 1];
        }
     

        for (int j = i + 1; j < Nnum - 1; j++)
        {
            numbers[j] = numbers[j + 1];
        }
        for (int j = i; j < Onum - 1; j++)
        {
            operators[j] = operators[j + 1];
        }
        Nnum--;
        Onum--;
        i--;
    }
}

printf("The result is %d  \n" , numbers[0]);

 return 0;
} 