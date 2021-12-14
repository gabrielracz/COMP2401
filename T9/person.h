// FILE is person.h

#ifndef HEADER_PERSON
#define HEADER_PERSON

/************************************************/
// DEFINE

#define NAME_LENGTH 32
#define NUM_YEARS 5

/************************************************/
// structures

struct person {
  unsigned int id;
  unsigned short age;
  float salary[NUM_YEARS];
  char first_name[NAME_LENGTH];
  char last_name[NAME_LENGTH];
};

typedef struct person PersonRec;

/************************************************/
// prototypes


void populate(struct person *p);
void print(struct person *p);

// prototypes for comparator functions

int compareAge(const void *p1, const void *p2);
int compareSalary(const void *p1, const void *p2);
int compareLastName(const void *p1, const void *p2);

#endif
