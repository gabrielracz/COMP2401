#include<stdio.h>
#include<string.h>

// This is a forward reference
int palindrome(char *s);

int main() {

  char word[30];

    
  printf ("Enter a word or \"q\" to stop: ");
  scanf ("%s", word);

  while (strcmp(word, "q") != 0) {
    palindrome(word);
    printf ("Enter a word or \"q\" to stop: ");
    scanf ("%s", word);
  };

  return 0;

}
  
int palindrome(char *s)
{

  // add code to test for palindrome and print a message  

}
