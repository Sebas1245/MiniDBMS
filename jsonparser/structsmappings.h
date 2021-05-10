#ifndef structsmappings
#define structsmappings

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "mjson/mjson.h"

#define MAXOBJECTS 1000  //maximum number of records 

// Struct to hold one student record
struct student_t{
    int student_id;
    char fname[30];
    char lname[50];
    char sex;
    char status[8];
    bool scholarship;
    int semester;
    double cumgrade;
};
typedef struct student_t student_t;

//Struct to hold array of student records
struct student_table{
    int cant; //cantidad de regristros
    student_t student_records[MAXOBJECTS];
};
typedef struct student_table student_table;


//Struct to hold one record of grades
struct grade_t{
    int enrollment_id;
    int student_id;
    char course[30];
    char school_term[30];
    double grade;
};
typedef struct grade_t grade_t;

//Struct to hold array of grades records
struct grade_table{
    int cant; //cantidad de regristros
    grade_t grade_records[MAXOBJECTS];
};
typedef struct grade_table grade_table;


//Struct to hold one record of grades
struct query_t{
    char key[30];
    char val[30];
};
typedef struct query_t query_t;


//Parser functions
int json_student_read(const char *buf, student_table *obj_list);
int json_grade_read(const char *buf, grade_table *obj_list);

int commitToDBStudents(student_table *obj_list);
int commitToDBGrades(grade_table *obj_list);

int countChar(char* fileName);
#endif