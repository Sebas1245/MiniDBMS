#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jsonparser/structsmappings.h"

void displayTable(student_table *students)
{
  int i;
  for (int i = 0; i < 125; i++)
    printf("=");
  printf("\n");
  printf("%-9s|%-24s|%-19s|%-9s|%-19s|%-9s|%-9s|%-9s\n", "Student ID",
         "First Name", "Last Name", "Sex", "Status", "Scholarship", "Semester",
         "Cumulative Grade");
  for (int i = 0; i < 125; i++)
    printf("=");
  printf("\n");

  for (i = 0; i < students->cant; i++)
  {
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

void displayRecord(int i, student_table *students)
{
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

// Parse Query String
// Returns: Query Struct
// Query Struct : key and value
int parseQuery(const char *query, query_t *q)
{
  const struct json_attr_t json_attrs_object[] = {
      {"val", t_string, STRUCTOBJECT(query_t, val),
       .len = sizeof(q.key)},
      {"key", t_string, STRUCTOBJECT(query_t, key),
       .len = sizeof(q.key)},
  };
  memset(q, '\0', sizeof(*q));
  return json_read_object(buf, json_attrs_object, NULL);
}

// Parse Attribute String
// Returns: Array of Attributes
char * parseAttr(const char *attr)
{
 
}

// Input: Student Table, Query Struct
// Retuns: Array of Indexes
int *scan_table_student(student_table *students, query_t query)
{
  static int r[1000];
  int counter = 0;
  int i for (i = index; i < students_list.cant - 1; i++)
  {
    if query
      ->key == "student_id"
      {
        if (students->student_records[i]->student_id == query->val)
        {
          r[counter] = i;
          counter++;
          return i;
        }
      }
    if query
      ->key == "fname"
      {
        if (students->student_records[i]->fname == query->val)
        {
          r[counter] = i;
          counter++;
          return i;
        }
      }
    if query
      ->key == "lname"
      {
        if (students->student_records[i]->lname == query->val)
        {
          r[counter] = i;
          counter++;
          return i;
        }
      }
    if query
      ->key == "sex"
      {
        if (students->student_records[i]->sex == query->val)
        {
          r[counter] = i;
          counter++;
          return i;
        }
      }
    if query
      ->key == "status"
      {
        if (students->student_records[i]->status == query->val)
        {
          r[counter] = i;
          counter++;
          return i;
        }
      }
    if query
      ->key == "scholarship"
      {
        if (students->student_records[i]->scholarship == query->val)
        {
          return i;
        }
      }
    if query
      ->key == "semester"
      {
        if (students->student_records[i]->semeter == query->val)
        {
          r[counter] = i;
          counter++;
          return i;
        }
      }
    if query
      ->key == "cumgrade"
      {
        if (students->student_records[i]->cumgrade == query->val)
        {
          r[counter] = i;
          counter++;
          return i;
        }
      }
  }
  return i;
}

char *query_table_student(student_table *students_list, const char *attributes, const char *query)
{
  char *result = malloc(2050);
  int *p =  scan_table_student(students_list, query);
  
  char* attr =  parseAttr()
  for (i = 0; i < sizeof(p); i++)
  {
    for (j = 0; j < sizeof(attr); j++)
    {
      if attr
        [i] == "student_id"
        {
          strcat(result, students_list->student_records[p[i]]->student_id);
        }
      if attr
        [i] == "fname"
        {
          strcat(result, students_list->student_records[p[i]]->fname);
        }
      if attr
        [i] == "lname"
        {
          strcat(result, students_list->student_records[p[i]]->lname);
        }
      if attr
        [i] == "sex"
        {
          strcat(result, students_list->student_records[p[i]]->sex);
        }
      if attr
        [i] == "status"
        {
          strcat(result, students_list->student_records[p[i]]->status);
        }
      if attr
        [i] == "scholarship"
        {
          strcat(result, students_list->student_records[p[i]]->scholarship);
        }
      if attr
        [i] == "semester"
        {
          strcat(result, students_list->student_records[p[i]]->semester);
        }
      if attr
        [i] == "cumgrade"
        {
          strcat(result, students_list->student_records[p[i]]->cumgrade);
        }
    }
  }

  return result;
}

int read_student_json(const char *buf, student_table *students_list)
{
  const struct json_attr_t json_attrs_object[] = {
      {"student_id", t_integer, STRUCTOBJECT(student_t, student_id)},
      {"fname", t_string, STRUCTOBJECT(student_t, fname),
       .len = sizeof(student_records.fname)},
      {"lname", t_string, STRUCTOBJECT(student_t, lname),
       .len = sizeof(student_records.lname)},
      {"sex", t_character, STRUCTOBJECT(student_t, sex)},
      {"status", t_string, STRUCTOBJECT(student_t, status),
       .len = sizeof(student_records.status)},
      {"scholarship", t_boolean, STRUCTOBJECT(student_t, scholarship)},
      {"semester", t_integer, STRUCTOBJECT(student_t, semester)},
      {"cumgrade", t_real, STRUCTOBJECT(student_t, cumgrade)},
      {NULL},
  };
  memset(students_list, '\0', sizeof(*students_list));
  return json_read_object(buf, json_attrs_object, NULL);
}

// Completed
int insert_to_table_student(const char *buf, student_table *students_list)
{
  int status = json_student_read(json_line, students);
  if (status == 0)
  {
    for (i = index; i < students.cant - 1; i++)
    {
      students_list->student_records[students_list->cant + i] = students->student_records[i];
    }
    return 0;
  }
  return 1;
}

// Status: Todo
// Implement Query for Multiple Rows
int update_table_student(const char *buf, const char *attributes, const char *query, student_table *students_list)
{
  int status = json_student_read(json_line, students);
  int index = scan_table(students, query);
  if (status == 0)
  {
    student_table *students = malloc(sizeof(student_table));
    students_list->student_records[i] = student->stident_records[0];
    return 0
  }
  return 1
}

// Implement Query for Multiple Rows
void remove_from_table_student(student_table *students, char *query)
{
  int i;
  int index = scan_table(students, query);
  for (i = index; i < students_list.cant - 1; i++)
    students->student_records[i] = students->student_records[i + 1];
  students->cant = i;
}

FILE *fStudents;
int main()
{
  printf(" M I N I  D B M S\n");
  fStudents = fopen("tables/students.json", "r");

  // Object to store all students records
  student_table *students = malloc(sizeof(student_table));
  // Object to store all grades records
  grade_table *grades = malloc(sizeof(grade_table));

  char json_line[1000] = "";
  char line[1000];

  if (fStudents == NULL)
  {
    printf("Could not open file");
    return 1;
  }

  // json read by json_student_read must be in one line
  while (fgets(line, sizeof(line), fStudents))
  {
    strcat(json_line, line);
  }

  // function that convers json objects into C objects
  int status = json_student_read(json_line, students);

  // TO ACCESS RECORDS OF STUDENTS -> students.students.records[i]
  // TO ACCESS RECORDS OF GRADES -> grades.grade_records[i]

  if (status != 0)
  {
    puts(json_error_string(status));
    return 1;
  }

  displayTable(students);

  fclose(fStudents);
  return 0;
}
