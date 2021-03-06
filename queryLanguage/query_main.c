#include "query_main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../jsonparser/structsmappings.h"
#include "query_main_grades.h"

#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))
#define ATTR_MAX_STUDENTS 7
// Parse Query String
// Returns: Query Struct
// Query Struct : key and value

/*
Inputs: cumgrade,<,70
*/
char **parseQuery(char *query_raw) {
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

char *trimWhitespace(char *str) {
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
char **parseAttr(char *attr_raw) {
  char **attr = (char **)malloc(sizeof(char *) * 8);

  int j;
  for (j = 0; j < 8; j++) {
    attr[j] = (char *)malloc(50 * sizeof(char));
    memset(attr[j], '\0', sizeof(attr[j]));
  }

#ifdef DEBUG
  printf("'%s'\n", attr_raw);
  printf("'%d'\n", strcmp(attr_raw, "*"));
#endif

  if (strcmp(attr_raw, "*") == 0) {
    strcpy(attr[0], "student_id");
    strcpy(attr[1], "fname");
    strcpy(attr[2], "lname");
    strcpy(attr[3], "sex");
    strcpy(attr[4], "status");
    strcpy(attr[5], "scholarship");
    strcpy(attr[6], "semester");
    strcpy(attr[7], "cumgrade");
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

// Input: Student Table, Query Struct
// Retuns: Array of Indexes
int scan_table_student(char *query_raw, student_table *students, int *r) {
  int counter = 0;
  int i, j;

  if (strcmp(query_raw, "none") == 0) {
    for (i = 0; i < students->cant; i++) {
      r[i] = i;
    }
    return students->cant;
  }

  char **query = parseQuery(query_raw);
  for (i = 0; i < students->cant; i++) {
    if (strcmp(query[0], "student_id") == 0) {
      if (strcmp(query[1], "<") == 0) {
        if (students->student_records[i].student_id < atoi(query[2])) {
          r[counter] = i;
          counter++;
        }
      }
      if (strcmp(query[1], "=") == 0) {
        if (students->student_records[i].student_id == atoi(query[2])) {
          r[counter] = i;
          counter++;
        }
      }
      if (strcmp(query[1], ">") == 0) {
        if (students->student_records[i].student_id > atoi(query[2])) {
          r[counter] = i;
          counter++;
        }
      }
    }
    if (strcmp(query[0], "fname") == 0) {
      if (strcmp(students->student_records[i].fname, query[2]) == 0) {
        r[counter] = i;
        counter++;
      }
    }
    if (strcmp(query[0], "lname") == 0) {
      if (strcmp(students->student_records[i].lname, query[2]) == 0) {
        r[counter] = i;
        counter++;
      }
    }
    if (strcmp(query[0], "sex") == 0) {
      if (strcmp(&students->student_records[i].sex, query[2]) == 0) {
        r[counter] = i;
        counter++;
      }
    }
    if (strcmp(query[0], "status") == 0) {
      if (strcmp(students->student_records[i].status, query[2]) == 0) {
        r[counter] = i;
        counter++;
      }
    }
    if (strcmp(query[0], "scholarship") == 0) {
      if (students->student_records[i].scholarship == atoi(query[2])) {
        r[counter] = i;
        counter++;
      }
    }
    if (strcmp(query[0], "semester") == 0) {
      if (strcmp(query[1], "<") == 0) {
        if (students->student_records[i].semester < atoi(query[2])) {
          r[counter] = i;
          counter++;
        }
      }
      if (strcmp(query[1], "=") == 0) {
        if (students->student_records[i].semester == atoi(query[2])) {
          r[counter] = i;
          counter++;
        }
      }
      if (strcmp(query[1], ">") == 0) {
        if (students->student_records[i].semester > atoi(query[2])) {
          r[counter] = i;
          counter++;
        }
      }
    }
    if (strcmp(query[0], "cumgrade") == 0) {
      double cumgrade;
      sscanf(query[2], "%lf", &cumgrade);

      if (strcmp(query[1], "<") == 0) {
        if (students->student_records[i].cumgrade < atoi(query[2])) {
          r[counter] = i;
          counter++;
        }
      }
      if (strcmp(query[1], "=") == 0) {
        if (students->student_records[i].cumgrade == atoi(query[2])) {
          r[counter] = i;
          counter++;
        }
      }
      if (strcmp(query[1], ">") == 0) {
        if (students->student_records[i].cumgrade > atoi(query[2])) {
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
* select;fname,lname;students;cumgrade,<,70  //shows first and last name of the
students with a cumulative grade lower than 70. Inputs:  student_list,
        attributes: fname,lname
        query: cumgrade,<,70
Outputs:
    string of student with attributes;
*/

void query_table_student(char *attributes, char *query,
                         student_table *students_list, char *result) {
  int p[1000];
  int len = scan_table_student(query, students_list, p);
  char **attr = parseAttr(attributes);
  int i, j;

  for (i = 0; i < len; i++) {
    for (j = 0; j < sizeof(attr) / sizeof(char); j++) {
      if (strcmp(attr[j], "student_id") == 0) {
        char student_id[50];
        sprintf(student_id, "%d",
                students_list->student_records[p[i]].student_id);
        strcat(result, student_id);
        strcat(result, "\t");
      }
      if (strcmp(attr[j], "fname") == 0) {
        strcat(result, students_list->student_records[p[i]].fname);
        strcat(result, "\t");
      }
      if (strcmp(attr[j], "lname") == 0) {
        strcat(result, students_list->student_records[p[i]].lname);
        strcat(result, "\t");
      }
      if (strcmp(attr[j], "sex") == 0) {
        char sex[2];
        sex[0] = students_list->student_records[p[i]].sex;
        sex[1] = '\0';
        strcat(result, sex);
        strcat(result, "\t");
      }
      if (strcmp(attr[j], "status") == 0) {
        strcat(result, students_list->student_records[p[i]].status);
        strcat(result, "\t");
      }
      if (strcmp(attr[j], "scholarship") == 0) {
        strcat(result,
               students_list->student_records[p[i]].scholarship ? "YES" : "NO");
        strcat(result, "\t");
      }
      if (strcmp(attr[j], "semester") == 0) {
        char semester[50];
        sprintf(semester, "%d", students_list->student_records[p[i]].semester);
        strcat(result, semester);
        strcat(result, "\t");
      }
      if (strcmp(attr[j], "cumgrade") == 0) {
        char s[50];
        sprintf(s, "%lf", students_list->student_records[p[i]].cumgrade);
        strcat(result, s);
        strcat(result, "\t");
      }
    }
    const char *end = "\n";
    strcat(result, end);
  }
  for (int j = 0; j < 8; j++) {
    memset(attr[j], '\0', sizeof(attr[j]));
    free(attr[j]);
  }
  free(attr);
}

// Completed
/* insert;grades;*;245,543,Physics II,Spring 2020,98.3
//inserts new records in the table grades and provides values for all columns
Inputs:  student_list,
        attributes: fname,lname | *
        val: 245,543,Physics II,Spring 2020,98.3
Outputs:
    string of student with attributes;
*/
int insert_to_table_student(char *val_raw, char *attr_raw,
                            student_table *students_list) {
  char **val = parseQuery(val_raw);
  char **attr = parseAttr(attr_raw);

#ifdef DEBUG
  int y;
  for (y = 0; y < sizeof(attr) / sizeof(char); y++) {
    printf("'%s'\n", attr[y]);
    printf("'%s'\n", val[y]);
  }
#endif

  int i;
  int size = students_list->cant;
  // TODO: Handle Mismatch of size between val and attr
  if (!(sizeof(val) / sizeof(char) == sizeof(attr) / sizeof(char))) {
    return 1;
  }
  // TODO: Handle Student Table Capacity
  if (size == 1000) {
    return 1;
  }
  for (i = 0; i < sizeof(attr) / sizeof(char); i++) {
    if (strcmp(attr[i], "student_id") == 0) {
      students_list->student_records[size].student_id = atoi(val[i]);
    }
    if (strcmp(attr[i], "fname") == 0) {
      strcpy(students_list->student_records[size].fname, val[i]);
    }
    if (strcmp(attr[i], "lname") == 0) {
      strcpy(students_list->student_records[size].lname, val[i]);
    }
    if (strcmp(attr[i], "sex") == 0) {
      students_list->student_records[size].sex = (val[i] == "M") ? 'M' : 'F';
    }
    if (strcmp(attr[i], "status") == 0) {
      strcpy(students_list->student_records[size].status, val[i]);
    }
    if (strcmp(attr[i], "scholarship") == 0) {
      students_list->student_records[size].scholarship = atoi(val[i]);
    }
    if (strcmp(attr[i], "semester") == 0) {
      students_list->student_records[size].semester = atoi(val[i]);
    }
    if (strcmp(attr[i], "cumgrade") == 0) {
      double cumgrade = atof(val[i]);
      students_list->student_records[size].cumgrade = cumgrade;
    }
  }
  for (int j = 0; j < 8; j++) {
    memset(val[j], '\0', sizeof(val[j]));
    free(val[j]);
  }
  for (int j = 0; j < 8; j++) {
    memset(attr[j], '\0', sizeof(attr[j]));
    free(attr[j]);
  }
  free(val);
  free(attr);

  students_list->cant++;

  return 0;
}

char **join_attr(char *attr_raw) {
  char **attr = (char **)malloc(sizeof(char *) * 13);

  int j;
  for (j = 0; j < 13; j++) {
    attr[j] = (char *)malloc(50 * sizeof(char));
    memset(attr[j], '\0', sizeof(attr[j]));
  }

  if (strcmp(attr_raw, "*") == 0) {
    strcpy(attr[0], "student_id");
    strcpy(attr[1], "fname");
    strcpy(attr[2], "lname");
    strcpy(attr[3], "sex");
    strcpy(attr[4], "status");
    strcpy(attr[5], "scholarship");
    strcpy(attr[6], "semester");
    strcpy(attr[7], "cumgrade");
    strcpy(attr[8], "enrollment_id");
    strcpy(attr[9], "student_id");
    strcpy(attr[10], "course");
    strcpy(attr[11], "school_term");
    strcpy(attr[12], "grade");
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

void join_student_with_grades(char *attr_raw, student_table *students_list,
                              grade_table *grades_list, char *result) {
  char **attr = join_attr(attr_raw);

  for (int i = 0; i < students_list->cant; i++) {
    int p[1000];
    char id[30];
    snprintf(id, sizeof(id), "student_id,=,%d",
             students_list->student_records[i].student_id);
    int len = scan_table_grade(id, grades_list, p);

    for (int k = 0; k < len; k++) {
      for (int j = 0; j < 13; j++) {
        if (strcmp(attr[j], "student_id") == 0) {
          char student_id[50];
          sprintf(student_id, "%d",
                  students_list->student_records[i].student_id);
          strcat(result, student_id);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "fname") == 0) {
          strcat(result, students_list->student_records[i].fname);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "lname") == 0) {
          strcat(result, students_list->student_records[i].lname);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "sex") == 0) {
          char sex[2];
          sex[0] = students_list->student_records[i].sex;
          sex[1] = '\0';
          strcat(result, sex);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "status") == 0) {
          strcat(result, students_list->student_records[i].status);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "scholarship") == 0) {
          strcat(result,
                 students_list->student_records[i].scholarship ? "YES" : "NO");
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "semester") == 0) {
          char semester[50];
          sprintf(semester, "%d", students_list->student_records[i].semester);
          strcat(result, semester);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "cumgrade") == 0) {
          char s[50];
          sprintf(s, "%lf", students_list->student_records[i].cumgrade);
          strcat(result, s);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "enrollment_id") == 0) {
          char enrollment_id[50];
          sprintf(enrollment_id, "%d",
                  grades_list->grade_records[p[k]].enrollment_id);
          strcat(result, enrollment_id);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "course") == 0) {
          strcat(result, grades_list->grade_records[p[k]].course);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "school_term") == 0) {
          strcat(result, grades_list->grade_records[p[k]].school_term);
          strcat(result, "\t");
        }
        if (strcmp(attr[j], "grade") == 0) {
          char s[50] = {0};
          sprintf(s, "%lf", grades_list->grade_records[p[k]].grade);
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