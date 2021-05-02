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
int main() {
  printf(" M I N I  D B M S\n");
  fStudents = fopen("tables/students.json", "r");

  // Object to store all students records
  student_table *students = malloc(sizeof(student_table));
  // Object to store all grades records
  grade_table *grades = malloc(sizeof(grade_table));

  char json_line[1000] = "";
  char line[1000];

  if (fStudents == NULL) {
    printf("Could not open file");
    return 1;
  }

  // json read by json_student_read must be in one line
  while (fgets(line, sizeof(line), fStudents)) {
    strcat(json_line, line);
  }

  // function that convers json objects into C objects
  int status = json_student_read(json_line, students);

  // TO ACCESS RECORDS OF STUDENTS -> students.students.records[i]
  // TO ACCESS RECORDS OF GRADES -> grades.grade_records[i]

  if (status != 0) {
    puts(json_error_string(status));
    return 1;
  }

  displayTable(students);

  fclose(fStudents);
  return 0;
}
