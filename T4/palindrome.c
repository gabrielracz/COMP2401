#include<stdio.h>
#include<string.h>

// This is a forward reference
int palindrome(char *s);

int main() {

  char word[30];

  while (strcmp(word, "q") != 0) {
    printf ("Enter a word or \"q\" to stop: ");
    scanf ("%s", word);	
    int result = palindrome(word);
	if(result == 1)
		printf("%s is a palindrome\n\n", word);
	else
		printf("%s is not a palindrome\n\n", word);
  };

  return 0;

}
  
int palindrome(char *s)
{
	int len = strlen(s);
	int mid = len/2;
	for(int i = 0; i < len; i++){
		//printf("%c     %c\n", s[i], s[len-i-1]);
		if(s[i] != s[len-i-1])
			return 0;
	}
	return 1;
  // add code to test for palindrome and print a message  

}
