#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jsonparser/structsmappings.h"

void displayTable(student_table *students) {
  int i;
  for (int i = 0; i < 125; i++) printf("=");
  printf("\n");
  printf("%-9s|%-24s|%-19s|%-9s|%-19s|%-9s|%-9s|%-9s\n", "Student ID",
         "First Name", "Last Name", "Sex", "Status", "Scholarship", "Semester",
         "Cumulative Grade");
  for (int i = 0; i < 125; i++) printf("=");
  printf("\n");

  for (i = 0; i < students->cant; i++) {
    printf("%-10d", students->student_records[i].student_id);
    printf("%-25s", students->student_records[i].fname);
    printf("%-20s", students->student_records[i].lname);
    printf("%-10c", students->student_records[i].sex);
    printf("%-20s", students->student_records[i].status);
    printf("%-20s", students->student_records[i].scholarship ? "Yes" : "No");
    printf("%-10d", students->student_records[i].semester);
    printf("%-10f", students->student_records[i].cumgrade);
    printf("\n");
  }
}

FILE *fStudents;
FILE *fGrade;
int main() {
  printf(" M I N I  D B M S\n");


  fStudents = fopen("tables/students.json", "r");
  fGrade = fopen("tables/grades.json", "r");
  // Object to store all students records
  student_table *students = malloc(sizeof(student_table));
  // Object to store all grades records
  grade_table *grades = malloc(sizeof(grade_table));

  char json_line[1000] = "";
  char line[1000];

  if (fStudents == NULL) {
    printf("Could not open students file");
    return 1;
  }

  if (fGrade == NULL){
    printf("Could not open grades file");
    return 1;
  }

  // json read by json_student_read must be in one line
  while (fgets(line, sizeof(line), fStudents)) {
    strcat(json_line, line);
  }

  // function that convers json objects into C objects
  int statusS = json_student_read(json_line, students);

  memset(line,0,1000);
  memset(json_line,0,1000);
  
  // json read by json_student_read must be in one line
  while (fgets(line, sizeof(line), fGrade)) {
    strcat(json_line, line);
  }

  // function that convers json objects into C objects
  int statusG = json_grade_read(json_line, grades);

  // TO ACCESS RECORDS OF STUDENTS -> students.students.records[i]
  // TO ACCESS RECORDS OF GRADES -> grades.grade_records[i]

  if (statusS != 0) {
    puts(json_error_string(statusS));
    return 1;
  }else if(statusG != 0 ){
    puts(json_error_string(statusG));
  }

  displayTable(students);
   fclose(fStudents);

  int iWrite = commitToDBGrades(grades);


 
  return 0;
}
