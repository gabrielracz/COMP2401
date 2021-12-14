#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"studentList.h"


// Allocates memory for a new student and initializes it with the given data
void createStudent(char *name, char *major, StudentType **student) {
  *student = (StudentType *) malloc(sizeof(StudentType));
  if (student == NULL) { 
    printf("Memory allocation error\n"); 
    exit(0); 
  }
  strcpy((*student)->name,  name);
  strcpy((*student)->major, major);
}


// Allocates memory for a new list Node
void createNode(NodeType **node, StudentType *data) {
  *node = (NodeType *) malloc(sizeof(NodeType));
  if (node == NULL) { 
    printf("Memory allocation error\n"); 
    exit(0); 
  }
  (*node)->data = data;
  (*node)->next = NULL;
}

// Prints a single student's information
void printStudent(StudentType *sPtr) {
  printf("%-15s %-15s\n", sPtr->name, sPtr->major);
}

// Free all items in a Singly-Linked List 
void freeList(NodeType *listItem) {
  NodeType *nextItem;
  
  while(listItem != NULL) {
    nextItem = listItem->next;
    free(listItem->data);
    free(listItem);
    listItem = nextItem;
  } 
}

// Insert the given student into the given list at the specified position.
// If position is 0, then insert as new head of the list.
void insertStudent(NodeType **head, StudentType *student, int pos){
  NodeType   *newNode;
  NodeType   *currNode, *prevNode;
  int         currPos;

  // Iterate through the list up to the position to insert 
  // at, keeping track of the previous node in the list so that
  // we can connect to it.
  prevNode = NULL;
  currNode = *head;
  currPos  = 0;
  while (currNode != NULL) {
    if (currPos == pos)
      break;
    currPos++;
    prevNode = currNode;
    currNode = currNode->next;
  }

  // If the position was invalid, then quit
  if (currPos != pos) {
    printf("invalid position\n");
    free(student); // needed for our code, but not in general
    return;
  }

  // Create the new node
  newNode = (NodeType *) malloc(sizeof(NodeType));
  if (newNode == NULL) { 
    printf("Memory allocation error\n"); 
    exit(0); 
  }
  newNode->data = student;
  newNode->next = currNode; // Connect rest of the list to the new node

  // If prevNode is NULL, then this is the first position in
  // the list, or the list was NULL to begin with.  Otherwise
  // we are inserting in the middle or at the end of the list.
  if (prevNode == NULL)
    *head = newNode;
  else
    prevNode->next = newNode;
}

// Delete the student with the given name from the given list.
// If position is 0, then change the head of the list.  Return
// -1 if the name was not found in the list, else return 0.
int deleteStudent(NodeType **head, char *nameToDelete) {
  NodeType *currNode, *prevNode;

  // Iterate through the list to find the student with the given
  // name, keeping track of the previous node in the list so that
  // we can disconnect it.
  prevNode = NULL;
  currNode = *head;
  while (currNode != NULL) {
    if (strcmp(currNode->data->name, nameToDelete) == 0)
      break;
    prevNode = currNode;
    currNode = currNode->next;
  }

  // If the name was not found, then quit with a -1
  if (currNode == NULL)
    return -1;

  // If the removed node was the head, then update the head,
  // otherwise move the next pointer around this removed node.
  if (prevNode == NULL)
    *head = currNode->next;
  else 
    prevNode->next = currNode->next;

  // Make sure to free up the node and the data!
  free(currNode->data); // does not necessarily need to be done here
  free(currNode);

  return 0;
}
