#include "../jsonparser/structsmappings.h"
#include "query_main_grades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))
#define ATTR_MAX_grades 7
// Parse Query String
// Returns: Query Struct
// Query Struct : key and value

/*
Inputs: cumgrade,<,70  
*/
char **parseQuery(char *query_raw)
{
    char **query = (char **)malloc(sizeof(char *) * 100);
    int j;
    for (j = 0; j < 8; j++)
    {
        query[j] = (char *)malloc(50 * sizeof(char));
    }

    char line[200];
    strcpy(line, query_raw);
    char delim[] = ",";
    char *ptr = strtok(line, delim);
    int i = 0;
    while (ptr != NULL)
    {
        strcpy(query[i++], ptr);
        ptr = strtok(NULL, delim);
    }
    return query;
}

char *trimWhitespace(char *str)
{
    char *end;

    // Leading whitespace
    while (isspace(*str))
        str++;

    if (*str == 0)
        return str;

    // Trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end))
        end--;

    end[1] = '\0';
    return str;
}
// Parse Attribute String
// Returns: Array of Attribute7
/*
Inputs: fname,lname | *
*/
char **parseAttr(char *attr_raw)
{
    char **attr = (char **)malloc(sizeof(char *) * 8);

    int j;
    for (j = 0; j < 8; j++)
    {
        attr[j] = (char *)malloc(50 * sizeof(char));
    }

#ifdef DEBUG
    printf("'%s'\n", attr_raw);
    printf("'%d'\n", strcmp(attr_raw, "*"));
#endif

    if (strcmp(attr_raw, "*") == 0)
    {
        strcpy(attr[0], "enrollment_id");
        strcpy(attr[1], "grade_id");
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
    while (ptr != NULL)
    {
        strcpy(attr[i++], ptr);
        ptr = strtok(NULL, delim);
    }
    return attr;
}



// Input: grade Table, Query Struct
// Retuns: Array of Indexes
int *scan_table_grade(char *query_raw, grade_table *grades)
{
    static int r[1000];
    int counter = 0;
    char **query = parseQuery(query_raw);
    int i, j;

    for (i = 0; i < grades->cant; i++)
    {

        if (strcmp(query[0], "grade_id") == 0)
        {
            if (grades->grade_records[i].grade_id == atoi(query[2]))
            {
                r[counter] = i;
                counter++;
            }
        }
        if (strcmp(query[0], "enrollment_id") == 0)
        {
             if (grades->grade_records[i].enrollment_id == atoi(query[2]))
            {
                r[counter] = i;
                counter++;
            }
        }
        if (strcmp(query[0], "course") == 0)
        {
            if (strcmp(grades->grade_records[i].course, query[2]) == 0)
            {
                r[counter] = i;
                counter++;
            }
        }
        if (strcmp(query[0], "school_term") == 0)
        {
            if (strcmp(&grades->grade_records[i].school_term, query[2]) == 0)
            {
                r[counter] = i;
                counter++;
            }
        }
        if (strcmp(query[0], "grade") == 0)
        {
            double grade;
            sscanf(query[2], "%lf", &grade);
            if (grades->grade_records[i].grade == grade)
            {
                r[counter] = i;
                counter++;
            }
        }
    }

    return r;
}

/*
* select;fname,lname;grades;cumgrade,<,70  //shows first and last name of the grades with a cumulative grade lower than 70.
Inputs:  grade_list, 
        attributes: fname,lname
        query: cumgrade,<,70
Outputs: 
    string of grade with attributes;
*/

char *query_table_grade(char *attributes, char *query, grade_table *grades_list)
{
    char *result = malloc(10000);
    int *p = scan_table_grade(query, grades_list);
    char **attr = parseAttr(attributes);
    int i, j;
    int len = ARRAY_LEN(p) - 1;
    for (i = 0; i < len; i++)
    {
        for (j = 0; j < sizeof(attr) / sizeof(char); j++)
        {
            if (strcmp(attr[j], "grade_id") == 0)
            {
                char grade_id[50];
                sprintf(grade_id, "%d", grades_list->grade_records[p[i]].grade_id);
                strcat(result, grade_id);
                strcat(result, "\t");
            }
            if (strcmp(attr[j], "enrollment_id") == 0)
            {
                char enrollment_id[50];
                sprintf(grade_id, "%d", grades_list->grade_records[p[i]].enrollment_id);
                strcat(result, enrollment_id);
                strcat(result, "\t");
            }
            if (strcmp(attr[j], "course") == 0)
            {
                strcat(result, &grades_list->grade_records[p[i]].course);
                strcat(result, "\t");
            }
            if (strcmp(attr[j], "school_term") == 0)
            {
                strcat(result, grades_list->grade_records[p[i]].status);
                strcat(result, "\t");
            }
            if (strcmp(attr[j], "grade") == 0)
            {
                char s[50] = {0};
                sprintf(s, "%lf", grades_list->grade_records[p[i]].grade);
                strcat(result, s);
                strcat(result, "\t");
            }
        }
        const char *end = "\n";
        strcat(result, end);
    }

    return result;
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
int insert_to_table_grade(char *val_raw, char *attr_raw, grade_table *grades_list)
{
    char **val = parseQuery(val_raw);
    char **attr = parseAttr(attr_raw);

#ifdef DEBUG
    int y;
    for (y = 0; y < sizeof(attr) / sizeof(char); y++)
    {
        printf("'%s'\n", attr[y]);
        printf("'%s'\n", val[y]);
    }
#endif

    int i;
    int size = grades_list->cant;
    // TODO: Handle Mismatch of size between val and attr
    // TODO: Handle grade Table Capacity
    for (i = 0; i < sizeof(attr) / sizeof(char); i++)
    {
         printf("'%s'\n", attr[i]);
         printf("'%s'\n", val[i]);
        if (strcmp(attr[i], "grade_id") == 0)
        {
            grades_list->grade_records[size].grade_id = atoi(val[i]);
            printf("%s",atoi(val[i]));
        }
        // if (strcmp(attr[i], "fname") == 0)
        // {
        //     strcpy(grades_list->grade_records[size].fname, val[i]);
        // }
        // if (strcmp(attr[i], "lname") == 0)
        // {
        //     strcpy(grades_list->grade_records[size].lname, val[i]);
        // }
        // if (strcmp(attr[i], "sex") == 0)
        // {
        //     strcpy(&grades_list->grade_records[size].sex, val[i]);
        // }
        // if (strcmp(attr[i], "status") == 0)
        // {
        //     strcpy(grades_list->grade_records[size].status, val[i]);
        // }
        // if (strcmp(attr[i], "scholarship") == 0)
        // {
        //     grades_list->grade_records[size].scholarship = atoi(val[i]);
        // }
        // if (strcmp(attr[i], "semester") == 0)
        // {
        //     grades_list->grade_records[size].semester = atoi(val[i]);
        // }
        // if (strcmp(attr[i], "cumgrade") == 0)
        // {
        //     double cumgrade;
        //     sscanf(val[1], "%lf", &cumgrade);
        //     grades_list->grade_records[size].cumgrade = cumgrade;
        // }
    }
    grades_list->cant++;
    return 0;
}