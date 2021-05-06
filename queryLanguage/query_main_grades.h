#ifndef query_manager_grades
#define query_manager_grades

#include "../jsonparser/structsmappings.h"

char **parseQuery(char *query_raw);
char **parseAttr(char *attr_raw);
char *trimWhitespace(char *str);
int *scan_table_grade(char *query, grade_table *grades);
char *query_table_grade(char *attributes, char *query, grade_table *grades_list);
int insert_to_table_grade(char *val_raw, char *attr_raw, grade_table *grades_list);

#endif