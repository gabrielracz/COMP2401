#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STRING 16
#define NUM_STUDENTS 5

typedef struct {
  char street[MAX_STRING];
  char city[MAX_STRING];
} AddressType;

typedef struct {
  char firstName[MAX_STRING];
  char familyName[MAX_STRING];
  AddressType * address;
} StudentType;

// forward references
void populateStudent(StudentType *stu, char *firstName, char *familyName, char *street, char *city);
void populateStudentArray(StudentType** stuArray, int* size);
void printStudent(StudentType* student);
void cleanup(StudentType** stuArray, int size);



int main(int argc, char const *argv[]) {

  StudentType** stuArray = (StudentType**)malloc(NUM_STUDENTS * sizeof(StudentType*));
  int size = 0;
  populateStudentArray(stuArray, &size);

  //Print the students in the array
  printf("Student Array - Size: %d\n", size);
  printf("------------------------------\n");
  int i;
  for (i = 0; i < size; ++i) {
    printStudent(stuArray[i]);
  }
  cleanup(stuArray, size);
  return 0;
}

/**************************************************************/
/* populate a student passed in by reference

 input/output
 stu - pointer to an student to be initialized
 firstName, familyName, street, city - strings used for initialization

 assumption:
 stu has been previously allocated
 */
void populateStudent(StudentType *stu, char *firstName, char *familyName, char *street, char *city)
{
	strncpy(stu->firstName, firstName, MAX_STRING - 1);
	strncpy(stu->familyName, familyName, MAX_STRING - 1);

	stu->address = (AddressType*)malloc(sizeof(AddressType));
  strncpy(stu->address->street, street, MAX_STRING - 1);
  strncpy(stu->address->city, city, MAX_STRING - 1);
}


/**************************************************************/
/* populate a student pointer array passed in by reference

 input/output
 stuArray - array of student pointers passed in as a double pointer
 size - output parameter which returns the number of students initialized
 */
void populateStudentArray(StudentType** stuArray, int* size)
{
  int numStudents = 3;
  *size = 0;

  char *fn[NUM_STUDENTS] = {"John", "Jane", "David", "Dina", "Justin"};
  char *sn[NUM_STUDENTS] = {"Smith", "Johnson", "Mart", "Carp", "Farmer"};
  char *streets[NUM_STUDENTS] = {"21 Elm St.", "1002 Maple St.", "3 Ash Rd.", "63 Oak Ave.", "765 Pine St."};
  char *cities[NUM_STUDENTS] = {"Halifax, NS", "Ottawa, ON", "Montreal, QC", "Winnipeg, MB", "Calgary, AB"};

  int i;
  for (i = 0; i < numStudents; ++i) {
    stuArray[i] = (StudentType*)malloc(sizeof(StudentType));
    populateStudent(stuArray[i], fn[i], sn[i], streets[i], cities[i]);
    *size += 1;
  }
}


void printStudent(StudentType* student)
{
  printf("%s %s : %s, %s\n", student->firstName, student->familyName, student->address->street, student->address->city);
}


/**************************************************************/
/* deallocate memory used in student array

 input/output
 stuArray - array of student pointers passed in as a double pointer
 size - number of students in array
 */
void cleanup(StudentType** stuArray, int size)
{
	for(int i = 0; i < size; i++){
		StudentType* curStu = *(stuArray+i);
		free(curStu->address);
		free(curStu);
	}
	free(stuArray);
	return;
}
