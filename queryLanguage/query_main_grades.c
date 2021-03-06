#include "query_main_grades.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../jsonparser/structsmappings.h"
#include "query_main.h"

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))
#define ATTR_MAX_grades 7
// Parse Query String
// Returns: Query Struct
// Query Struct : key and value

/*
Inputs: cumgrade,<,70
*/
char **parseQueryGrades(char *query_raw) {
  char **query = (char **)malloc(sizeof(char *) * 8);
  int j;
  for (j = 0; j < 8; j++) {
    query[j] = (char *)malloc(50 * sizeof(char));
    memset(query[j], '\0', sizeof(query[j]));
  }

  char line[200];
  strcpy(line, query_raw);
  char delim[] = ",";
  char *ptr = strtok(line, delim);
  int i = 0;
  while (ptr != NULL) {
    strcpy(query[i++], ptr);
    ptr = strtok(NULL, delim);
  }
  memset(line, '\0', sizeof(line));
  return query;
}

char *trimWhitespaceGrades(char *str) {
  char *end;

  // Leading whitespace
  while (isspace(*str)) str++;

  if (*str == 0) return str;

  // Trailing space
  end = str + strlen(str) - 1;
  while (end > str && isspace(*end)) end--;

  end[1] = '\0';
  return str;
}
// Parse Attribute String
// Returns: Array of Attribute7
/*
Inputs: fname,lname | *
*/
char **parseAttrGrades(char *attr_raw) {
  char **attr = (char **)malloc(sizeof(char *) * 5);

  int j;
  for (j = 0; j < 5; j++) {
    attr[j] = (char *)malloc(50 * sizeof(char));
    memset(attr[j], '\0', sizeof(attr[j]));
  }

#ifdef DEBUG
  printf("'%s'\n", attr_raw);
  printf("'%d'\n", strcmp(attr_raw, "*"));
#endif

  if (strcmp(attr_raw, "*") == 0) {
    strcpy(attr[0], "enrollment_id");
    strcpy(attr[1], "student_id");
    strcpy(attr[2], "course");
    strcpy(attr[3], "school_term");
    strcpy(attr[4], "grade");
    return attr;
  }

  char line[200];
  strcpy(line, attr_raw);
  char delim[] = ",";
  char *ptr = strtok(line, delim);
  int i = 0;
  while (ptr != NULL) {
    strcpy(attr[i++], ptr);
    ptr = strtok(NULL, delim);
  }
  memset(line, '\0', sizeof(line));
  return attr;
}

// Input: grade Table, Query Struct
// Retuns: Array of Indexes
int scan_table_grade(char *query_raw, grade_table *grades, int *r) {
  int counter = 0;
  int i, j;

  if (strcmp(query_raw, "none") == 0) {
    for (i = 0; i < grades->cant; i++) {
      r[i] = i;
    }
    return grades->cant;
  }

  char **query = parseQueryGrades(query_raw);
  for (i = 0; i < grades->cant; i++) {
    if (strcmp(query[0], "student_id") == 0) {
      if (strcmp(query[1], "<") == 0) {
        if (grades->grade_records[i].student_id < atoi(query[2])) {
          r[counter] = i;
          counter++;
        }
      }
      if (strcmp(query[1], "=") == 0) {
        if (grades->grade_records[i].student_id == atoi(query[2])) {
          r[counter] = i;
          counter++;
        }
      }
      if (strcmp(query[1], ">") == 0) {
        if (grades->grade_records[i].student_id > atoi(query[2])) {
          r[counter] = i;
          counter++;
        }
      }
    }
    if (strcmp(query[0], "enrollment_id") == 0) {
      if (strcmp(query[1], "<") == 0) {
        if (grades->grade_records[i].enrollment_id < atoi(query[2])) {
          r[counter] = i;
          counter++;
        }
      }
      if (strcmp(query[1], "=") == 0) {
        if (grades->grade_records[i].enrollment_id == atoi(query[2])) {
          r[counter] = i;
          counter++;
        }
      }
      if (strcmp(query[1], ">") == 0) {
        if (grades->grade_records[i].enrollment_id > atoi(query[2])) {
          r[counter] = i;
          counter++;
        }
      }
    }
    if (strcmp(query[0], "course") == 0) {
      if (strcmp(grades->grade_records[i].course, query[2]) == 0) {
        r[counter] = i;
        counter++;
      }
    }
    if (strcmp(query[0], "school_term") == 0) {
      if (strcmp(grades->grade_records[i].school_term, query[2]) == 0) {
        r[counter] = i;
        counter++;
      }
    }
    if (strcmp(query[0], "grade") == 0) {
      double grade;
      sscanf(query[2], "%lf", &grade);

      if (strcmp(query[1], "<") == 0) {
        if (grades->grade_records[i].grade < grade) {
          r[counter] = i;
          counter++;
        }
      }
      if (strcmp(query[1], "=") == 0) {
        if (grades->grade_records[i].grade == grade) {
          r[counter] = i;
          counter++;
        }
      }
      if (strcmp(query[1], ">") == 0) {
        if (grades->grade_records[i].grade > grade) {
          r[counter] = i;
          counter++;
        }
      }
    }
  }

  for (int j = 0; j < 8; j++) {
    memset(query[j], '\0', sizeof(query[j]));
    free(query[j]);
  }
  free(query);

  return counter;
}

/*
* select;fname,lname;grades;cumgrade,<,70  //shows first and last name of the
grades with a cumulative grade lower than 70. Inputs:  grade_list, attributes:
fname,lname query: cumgrade,<,70 Outputs: string of grade with attributes;
*/

void query_table_grade(char *attributes, char *query, grade_table *grades_list,
                       char *result) {
  int p[1000];
  int len = scan_table_grade(query, grades_list, p);
  char **attr = parseAttrGrades(attributes);
  int i, j;

  for (i = 0; i < len; i++) {
    for (j = 0; j < 5; j++) {
      if (strcmp(attr[j], "student_id") == 0) {
        char student_id[50];
        sprintf(student_id, "%d", grades_list->grade_records[p[i]].student_id);
        strcat(result, student_id);
        strcat(result, "\t");
      }
      if (strcmp(attr[j], "enrollment_id") == 0) {
        char enrollment_id[50];
        sprintf(enrollment_id, "%d",
                grades_list->grade_records[p[i]].enrollment_id);
        strcat(result, enrollment_id);
        strcat(result, "\t");
      }
      if (strcmp(attr[j], "course") == 0) {
        strcat(result, grades_list->grade_records[p[i]].course);
        strcat(result, "\t");
      }
      if (strcmp(attr[j], "school_term") == 0) {
        strcat(result, grades_list->grade_records[p[i]].school_term);
        strcat(result, "\t");
      }
      if (strcmp(attr[j], "grade") == 0) {
        char s[50] = {0};
        sprintf(s, "%lf", grades_list->grade_records[p[i]].grade);
        strcat(result, s);
        strcat(result, "\t");
      }
    }
    const char *end = "\n";
    strcat(result, end);
  }

  for (int j = 0; j < 5; j++) {
    memset(attr[j], '\0', sizeof(attr[j]));
    free(attr[j]);
  }
  free(attr);
}

// Completed
/* insert;grades;*;245,543,Physics II,Spring 2020,98.3
//inserts new records in the table grades and provides values for all columns
Inputs:  grade_list,
        attributes: fname,lname | *
        val: 245,543,Physics II,Spring 2020,98.3
Outputs:
    string of grade with attributes;
*/
int insert_to_table_grade(char *val_raw, char *attr_raw,
                          grade_table *grades_list) {
  char **val = parseQueryGrades(val_raw);
  char **attr = parseAttrGrades(attr_raw);

#ifdef DEBUG
  int y;
  for (y = 0; y < sizeof(attr) / sizeof(char); y++) {
    printf("'%s'\n", attr[y]);
    printf("'%s'\n", val[y]);
  }
#endif

  int i;
  int size = grades_list->cant;
  // TODO: Handle Mismatch of size between val and attr
  // TODO: Handle grade Table Capacity
  for (i = 0; i < 5; i++) {
    if (strcmp(attr[i], "enrollment_id") == 0) {
      grades_list->grade_records[size].enrollment_id = atoi(val[i]);
    }
    if (strcmp(attr[i], "student_id") == 0) {
      grades_list->grade_records[size].student_id = atoi(val[i]);
    }
    if (strcmp(attr[i], "course") == 0) {
      strcpy(grades_list->grade_records[size].course, val[i]);
    }
    if (strcmp(attr[i], "school_term") == 0) {
      strcpy(grades_list->grade_records[size].school_term, val[i]);
    }
    if (strcmp(attr[i], "grade") == 0) {
      double cumgrade = atof(val[i]);
      grades_list->grade_records[size].grade = cumgrade;
    }
  }

  for (int j = 0; j < 8; j++) {
    memset(val[j], '\0', sizeof(val[j]));
    free(val[j]);
  }
  for (int j = 0; j < 5; j++) {
    memset(attr[j], '\0', sizeof(attr[j]));
    free(attr[j]);
  }
  free(val);
  free(attr);

  grades_list->cant++;
  return 0;
}

char **join_attrGrades(char *attr_raw) {
  char **attr = (char **)malloc(sizeof(char *) * 13);

  int j;
  for (j = 0; j < 13; j++) {
    attr[j] = (char *)malloc(50 * sizeof(char));
    memset(attr[j], '\0', sizeof(attr[j]));
  }

  if (strcmp(attr_raw, "*") == 0) {
    strcpy(attr[0], "enrollment_id");
    strcpy(attr[1], "student_id");
    strcpy(attr[2], "course");
    strcpy(attr[3], "school_term");
    strcpy(attr[4], "grade");
    strcpy(attr[5], "student_id");
    strcpy(attr[6], "fname");
    strcpy(attr[7], "lname");
    strcpy(attr[8], "sex");
    strcpy(attr[9], "status");
    strcpy(attr[10], "scholarship");
    strcpy(attr[11], "semester");
    strcpy(attr[12], "cumgrade");
    return attr;
  }

  char line[200];
  strcpy(line, attr_raw);
  char delim[] = ",";
  char *ptr = strtok(line, delim);
  int i = 0;
  while (ptr != NULL) {
    strcpy(attr[i++], ptr);
    ptr = strtok(NULL, delim);
  }
  memset(line, '\0', sizeof(line));
  return attr;
}

void join_grades_with_students(char *attr_raw, student_table *students_list,
                               grade_table *grades_list, char *result) {
  char **attr = join_attrGrades(attr_raw);

  for (int i = 0; i < grades_list->cant; i++) {
    int p[1000];
    char id[30];
    snprintf(id, sizeof(id), "student_id,=,%d",
             grades_list->grade_records[i].student_id);
    int len = scan_table_student(id, students_list, p);

    for (int k = 0; k < len; k++) {
      for (int j = 0; j < 13; j++) {
        if (strcmp(attr[j], "student_id") == 0) {
          char student_id[50];
          sprintf(student_id, "%d", grades_list->grade_records[i].student_id);
          strcat(result, student_id);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "enrollment_id") == 0) {
          char enrollment_id[50];
          sprintf(enrollment_id, "%d",
                  grades_list->grade_records[i].enrollment_id);
          strcat(result, enrollment_id);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "course") == 0) {
          strcat(result, grades_list->grade_records[i].course);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "school_term") == 0) {
          strcat(result, grades_list->grade_records[i].school_term);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "grade") == 0) {
          char s[50] = {0};
          sprintf(s, "%lf", grades_list->grade_records[i].grade);
          strcat(result, s);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "fname") == 0) {
          strcat(result, students_list->student_records[p[k]].fname);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "lname") == 0) {
          strcat(result, students_list->student_records[p[k]].lname);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "sex") == 0) {
          char sex[2];
          sex[0] = students_list->student_records[p[k]].sex;
          sex[1] = '\0';
          strcat(result, sex);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "status") == 0) {
          strcat(result, students_list->student_records[p[k]].status);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "scholarship") == 0) {
          strcat(result, students_list->student_records[p[k]].scholarship
                             ? "YES"
                             : "NO");
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "semester") == 0) {
          char semester[50];
          sprintf(semester, "%d",
                  students_list->student_records[p[k]].semester);
          strcat(result, semester);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "cumgrade") == 0) {
          char s[50];
          sprintf(s, "%lf", students_list->student_records[p[k]].cumgrade);
          strcat(result, s);
          strcat(result, "\t");
        }
      }
      const char *end = "\n";
      strcat(result, end);
    }
  }

  for (int j = 0; j < 13; j++) {
    memset(attr[j], '\0', sizeof(attr[j]));
    free(attr[j]);
  }
  free(attr);
}