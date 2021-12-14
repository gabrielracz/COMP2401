// FILE main.c

/******************************************/

// INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "person.h"

/********************************************/
// DEFINE
#define NUM_PEOPLE 7

/********************************************/

int main(int argc, char *argv[])
{
  struct person people[NUM_PEOPLE];
  int i;

  // populate the array
  for (i = 0; i < NUM_PEOPLE; i++) {
    populate(&people[i]);
  }

  // print the array
  for (i = 0; i < NUM_PEOPLE; i++) {
    print(&people[i]);
  }

  // sort based on age
  printf ("\nSort based on age using qsort(): \n");

  qsort(people, NUM_PEOPLE, sizeof(struct person), compareAge);

  // print the array
  for (i = 0; i < NUM_PEOPLE; i++) {
    print(&people[i]);
  }

  // sort based on average salary
  printf ("\nSort based on average salary using qsort(): \n");

  // call qsort()
  qsort(people, NUM_PEOPLE, sizeof(struct person), compareSalary);
  
   // print the array
  for (i = 0; i < NUM_PEOPLE; i++) {
    print(&people[i]);
  }

  // sort based on last name
  printf ("\nSort based on last name using qsort(): \n");

  // call qsort()
  qsort(people, NUM_PEOPLE, sizeof(struct person), compareLastName);
 
   // print the array
  for (i = 0; i < NUM_PEOPLE; i++) {
    print(&people[i]);
  }
  return 0;
}
