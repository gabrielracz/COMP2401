#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "studentList.h"

int main() {
  NodeType    *ourClassroom = NULL;
  NodeType    *currNode = NULL;
  StudentType *currStudent;
  char         str1[MAX_STR];
  char         str2[MAX_STR];

  printf("\nEnter student names and their majors (use -1 when done): ");
  while(1) {
    printf("\nEnter name: ");
    scanf("%s", str1);
    if (strcmp(str1, "-1") == 0)
      break;
    printf("Enter major: ");
    scanf("%s", str2);

    createStudent(str1, str2, &currStudent);
    insertStudent(&ourClassroom, currStudent, 0);
  }

  printf("\nHere is the list:\n");
  printf("%-15s %-15s\n", "NAME","MAJOR");
  printf("--------------- ---------------\n");

  currNode = ourClassroom;
  while(currNode != NULL) {
    printStudent(currNode->data);
    currNode = currNode->next;
  }

  printf("Who would you like to delete? ");
  scanf("%s", str1);
  printf("Deleting %s ...\n", str1);
  if (deleteStudent(&ourClassroom, str1) == -1) {
    printf("Error deleting student %s ... continuing with program ...\n", str1);
  }

  printf("\nHere is the list:\n");
  printf("%-15s %-15s\n", "NAME","MAJOR");
  printf("--------------- ---------------\n");

  currNode = ourClassroom;
  while(currNode != NULL) {
    printStudent(currNode->data);
    currNode = currNode->next;
  }

  freeList(ourClassroom);
}
