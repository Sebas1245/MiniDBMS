#ifndef query_manager_grades
#define query_manager_grades

#include "../jsonparser/structsmappings.h"

char **parseQueryGrades(char *query_raw);
char **parseAttrGrades(char *attr_raw);
char *trimWhitespaceGrades(char *str);
int scan_table_grade(char *query, grade_table *grades, int *r);
void query_table_grade(char *attributes, char *query, grade_table *grades_list,
                       char *result);
int insert_to_table_grade(char *val_raw, char *attr_raw,
                          grade_table *grades_list);
char **join_attrGrades(char *attr_raw);
void join_grades_with_students(char *attr_raw, student_table *students_list,
                               grade_table *grades_list, char *result);

#endif