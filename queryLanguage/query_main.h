#ifndef query_manager
#define query_manager

#include "../jsonparser/structsmappings.h"

char **parseQuery(char *query_raw);
char **parseAttr(char *attr_raw);
char *trimWhitespace(char *str);
int scan_table_student(char *query, student_table *students, int *r);
void query_table_student(char *attributes, char *query,
                         student_table *students_list, char *result);
int insert_to_table_student(char *val_raw, char *attr_raw,
                            student_table *students_list);
char **join_attr(char *attr_raw);
void join_student_with_grades(char *attr_raw, student_table *students_list,
                              grade_table *grades_list, char *result);

#endif