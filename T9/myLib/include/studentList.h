#define MAX_STR 32

typedef struct Student{
  char   name[MAX_STR];
  char   major[MAX_STR];
}StudentType;

typedef struct Node{
  StudentType  *data;
  struct Node  *next;
}NodeType;

void createStudent(char*, char*, StudentType**);
void createNode(NodeType**, StudentType*);
void printStudent(StudentType*);
void freeList(NodeType*);
void insertStudent(NodeType**, StudentType*, int);
int  deleteStudent(NodeType**, char*);
