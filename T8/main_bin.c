#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "person.h"

int fexists(char *fileName);
int readBin(FILE* fid); 

int main(int argc, char *argv[]) {

  struct personalInfo person;
  FILE *fid;
  int i;
  int rc;

// add coded

  if (argc < 2) {
      printf("usage %s filename \n",argv[0]);
      return(1);
  }

  // check if file exists
  rc = fexists(argv[1]);

  // print message depending whether the file exists
  if(rc == 1){
	printf("File %s exists\n", argv[1]);
	
	FILE* fid = fopen(argv[1], "rb+");
	struct personalInfo p;
	int r;
	r = fread(&p, sizeof(struct personalInfo), 1, fid);
	if(r == 1){
		printPerson(&p);
	}
	
	p.salary[0] = 47500;
	p.salary[1] = 40000;
	p.salary[2] = 23000;
	p.salary[3] = 51000;
	p.salary[4] = 37000;
	p.age = 36;
	fseek(fid, 3*sizeof(struct personalInfo), SEEK_SET);
	fwrite(&p, sizeof(struct personalInfo), 1, fid);

	int currentPosition = ftell(fid);
	int recordNumber = currentPosition / sizeof(struct personalInfo);
	printf("\nRecord Number: %d\n", recordNumber);
	fseek(fid, -sizeof(struct personalInfo), SEEK_CUR);
	fread(&p, sizeof(struct personalInfo), 1, fid);
	printPerson(&p);
	printf("\n\n");

	//Read all records into array
	fseek(fid, 0, SEEK_END);
	int fileSize = ftell(fid);
	int numRecords = fileSize / sizeof(struct personalInfo);
	rewind(fid);

	struct personalInfo* people = malloc(sizeof(struct personalInfo) * numRecords);
	for(int i = 0; i < numRecords; i++){
		struct personalInfo cur;
		fread(&cur, sizeof(struct personalInfo), 1, fid);
		people[i] = cur;
		printPerson(&cur);
	}


  }else{
	printf("File %s does not exist\n", argv[1]);
  }

  return 0;
}

/************************************************************************/
// Return whether the given file exists in the current directory.
// TODO: Complete this function.
int fexists(char *fileName)
{
    FILE *fp = NULL;
    int rc = 0;

	// open the file

    fp = fopen(fileName, "r");

	// determine the recturn code
	if(fp == NULL){
		return 0;
	}else{
		fclose(fp);
		return 1;
	}
}

int readBin(FILE* fid){
	struct personalInfo p;
	int rc;
	rc = fread(&p, sizeof(struct personalInfo), 1, fid);
	if(rc == 1){
		printPerson(&p);
	}
	fclose(fid);
	return rc;
}
