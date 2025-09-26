#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(){

  char entry[100];
  char operators[50];
  int numbers[50];
  int Onum = 0;    //counter for the operators
  int Nnum = 0 ;   //counter for the numbers

  printf("Enter the Input : \n");
  fgets(entry ,100, stdin);

  for(int i =0;i<strlen(entry); i++){
      if (isdigit(entry[i]))
      {
          numbers[Nnum++] = entry[i] - '0';
      }
      else if (entry[i] == '+' || entry[i] == '-' || entry[i] == '*' || entry[i] == '/' || entry[i] == '%')
      {
          operators[Onum++] = entry[i];
      }
      else if (entry[i] == ' ' || entry[i] == '\n')
      {
          continue;
      }
      else
      {
          printf("Special Characters not allowed");
          return 1;
      }
  }

 printf("Total numbers of digits are %d \n" , Nnum);

 printf("Total numbers of characters are %d \n", Onum);

 return 0;
} 