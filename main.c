#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jsonparser/structsmappings.h"
#include "queryLanguage/query_main.h"

void displayTable(student_table* students) {
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

void displayRecord(int i, student_table* students) {
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

FILE* fStudents;
int main() {
  printf(" M I N I  D B M S\n");
  fStudents = fopen("tables/students.json", "r");

  // Object to store all students records
  student_table* students = malloc(sizeof(student_table));
  // Object to store all grades records
  grade_table* grades = malloc(sizeof(grade_table));

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

  // #ifdef DEBUG

  // ---- Attr Test ----//
  // Test * Should return all parameters array
  // char ** attr = parseAttr("*");
  // printf("%s\n", attr[0]);
  // printf("%s\n", attr[1]);

  // Test * Should return  student_id,fname,lname
  //  char **attr2 = parseAttr("student_id,fname,lname");
  // printf("%s\n", attr2[0]);
  // printf("%s\n", attr2[1]);
  // printf("%s\n", attr2[2]);

  // ---- Query Test ----//
  // Test * Should return all query parameters
  // char ** query = parseQuery("student_id,==,892");
  // printf("%s\n", query[0]);
  // printf("%s\n", query[2]);

  // ---- Scan Table Test --- //
  // int* i = scan_table_student("student_id,=,892", students);
  // printf("%d\n", i[0]);

  // ---- Query Table Test --- //
  printf("Query Table Test\n");
  // Test * Should return fname,lname parameters of student with id 892
  char* result = query_table_student("sex,status,fname,lname",
                                     "student_id,=,892", students);
  printf("%s", result);

  // --- Especial case int double values in table
  // Test * Should return sex attr parameters of student with id 892
  char* result2 =
      query_table_student("scholarship", "student_id,=,892", students);
  printf("%s", result2);

  // Test * Should return student_id attr parameters of student with id 892
  char* result3 =
      query_table_student("student_id", "student_id,=,892", students);
  printf("%s", result3);

  //  Test * Should return cumgrade attr parameters of student with id 892
  char* result4 = query_table_student("cumgrade", "student_id,=,892", students);
  printf("%s", result4);

  //  Test * Should return cumgrade attr parameters of student with id 892
  char* result5 = query_table_student("*", "student_id,<,892", students);
  printf("%s", result5);
  // select;*;students;student_id,=,656
  // select;sex,status,fname,lname;students;student_id,=,435
  // select;*;students;student_id,<,656
  // gcc -O -o server ./jsonparser/mjson/mjson.c ./jsonparser/structsmappings.c
  // ./queryLanguage/query_main.c main.c
  // ---- Query Grade Table Test --- //

  /*
  // Test * Should return fname,lname parameters of student with id 892
    char* result =
  query_table_student("sex,status,fname,lname","student_id,==,892",students);
    printf("%s", result);

  // --- Especial case int double values in table
    // Test * Should return sex attr parameters of student with id 892
    char* result2 =
  query_table_student("scholarship","student_id,==,892",students); printf("%s",
  result2);

    // Test * Should return student_id attr parameters of student with id 892
    char* result3 =
  query_table_student("student_id","student_id,==,892",students); printf("%s",
  result3);

    //  Test * Should return cumgrade attr parameters of student with id 892
    char* result4 =
  query_table_student("cumgrade","student_id,==,892",students); printf("%s",
  result4);

    //  Test * Should return cumgrade attr parameters of student with id 892
    char* result5 = query_table_student("*","student_id,==,892",students);
    printf("%s", result5);
  */
  insert_to_table_student("920,Chris,Alvar,M,Local,0,6,98.3","*",students);
  int commitStudents = commitToDBStudents(students);

  // #endif

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
