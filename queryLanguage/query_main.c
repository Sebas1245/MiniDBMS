#include "../jsonparser/structsmappings.h"
#include "query_main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))
#define ATTR_MAX_STUDENTS 7
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
        strcpy(attr[0], "student_id");
        strcpy(attr[1], "fname");
        strcpy(attr[2], "lname");
        strcpy(attr[3], "sex");
        strcpy(attr[4], "status");
        strcpy(attr[5], "sholarship");
        strcpy(attr[6], "semester");
        strcpy(attr[7], "cumgrade");
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

// Input: Student Table, Query Struct
// Retuns: Array of Indexes
int *scan_table_student(char *query_raw, student_table *students)
{
    static int r[1000];
    int counter = 0;
    if (strcmp(query_raw, "") ==0) {
        int l;
        for (l =0 ; l < students->cant; l++) {
            r[l]= l;
        } 
        return r;
    }
    char **query = parseQuery(query_raw);
    int i, j;

    for (i = 0; i < students->cant; i++)
    {

        if (strcmp(query[0], "student_id") == 0)
        {
            if (strcmp(query[1], "<") == 0)
            {
                if (students->student_records[i].student_id < atoi(query[2]))
                {
                    r[counter] = i;
                    counter++;
                }
            }
            if (strcmp(query[1], "=") == 0)
            {
                if (students->student_records[i].student_id == atoi(query[2]))
                {
                    r[counter] = i;
                    counter++;
                }
            }
            if (strcmp(query[1], ">") == 0)
            {
                if (students->student_records[i].student_id > atoi(query[2]))
                {
                    r[counter] = i;
                    counter++;
                }
            }
        }
        if (strcmp(query[0], "fname") == 0)
        {
            if (strcmp(students->student_records[i].fname, query[2]) == 0)
            {
                r[counter] = i;
                counter++;
            }
        }
        if (strcmp(query[0], "lname") == 0)
        {
            if (strcmp(students->student_records[i].lname, query[2]) == 0)
            {
                r[counter] = i;
                counter++;
            }
        }
        if (strcmp(query[0], "sex") == 0)
        {
            if (strcmp(&students->student_records[i].sex, query[2]) == 0)
            {
                r[counter] = i;
                counter++;
            }
        }
        if (strcmp(query[0], "status") == 0)
        {
            if (strcmp(students->student_records[i].status, query[2]) == 0)
            {
                r[counter] = i;
                counter++;
            }
        }
        if (strcmp(query[0], "scholarship") == 0)
        {
            if (students->student_records[i].scholarship == atoi(query[2]))
            {
                r[counter] = i;
                counter++;
            }
        }
        if (strcmp(query[0], "semester") == 0)
        {
            if (strcmp(query[1], "<") == 0)
            {
                if (students->student_records[i].semester < atoi(query[2]))
                {
                    r[counter] = i;
                    counter++;
                }
            }
            if (strcmp(query[1], "=") == 0)
            {
                if (students->student_records[i].semester == atoi(query[2]))
                {
                    r[counter] = i;
                    counter++;
                }
            }
            if (strcmp(query[1], ">") == 0)
            {
                if (students->student_records[i].semester > atoi(query[2]))
                {
                    r[counter] = i;
                    counter++;
                }
            }
        }
        if (strcmp(query[0], "cumgrade") == 0)
        {
            double cumgrade;
            sscanf(query[2], "%lf", &cumgrade);

            if (strcmp(query[1], "<") == 0)
            {
                if (students->student_records[i].cumgrade < atoi(query[2]))
                {
                    r[counter] = i;
                    counter++;
                }
            }
            if (strcmp(query[1], "=") == 0)
            {
                if (students->student_records[i].cumgrade == atoi(query[2]))
                {
                    r[counter] = i;
                    counter++;
                }
            }
            if (strcmp(query[1], ">") == 0)
            {
                if (students->student_records[i].cumgrade > atoi(query[2]))
                {
                    r[counter] = i;
                    counter++;
                }
            }
        }
    }

    return r;
}

/*
* select;fname,lname;students;cumgrade,<,70  //shows first and last name of the students with a cumulative grade lower than 70.
Inputs:  student_list, 
        attributes: fname,lname
        query: cumgrade,<,70
Outputs: 
    string of student with attributes;
*/

char *query_table_student(char *attributes, char *query, student_table *students_list)
{
    char *result = malloc(10000);
    int *p = scan_table_student(query, students_list);
    char **attr = parseAttr(attributes);
    int i, j;
    int len = ARRAY_LEN(p) - 1;
    for (i = 0; i < len; i++)
    {
        for (j = 0; j < sizeof(attr) / sizeof(char); j++)
        {
            if (strcmp(attr[j], "student_id") == 0)
            {
                char student_id[50];
                sprintf(student_id, "%d", students_list->student_records[p[i]].student_id);
                strcat(result, student_id);
                strcat(result, "\t");
            }
            if (strcmp(attr[j], "fname") == 0)
            {
                strcat(result, students_list->student_records[p[i]].fname);
                strcat(result, "\t");
            }
            if (strcmp(attr[j], "lname") == 0)
            {
                strcat(result, students_list->student_records[p[i]].lname);
                strcat(result, "\t");
            }
            if (strcmp(attr[j], "sex") == 0)
            {
                strcat(result, &students_list->student_records[p[i]].sex);
                strcat(result, "\t");
            }
            if (strcmp(attr[j], "status") == 0)
            {
                strcat(result, students_list->student_records[p[i]].status);
                strcat(result, "\t");
            }
            if (strcmp(attr[j], "scholarship") == 0)
            {
                strcat(result, students_list->student_records[p[i]].scholarship ? "YES" : "NO");
                strcat(result, "\t");
            }
            if (strcmp(attr[j], "semester") == 0)
            {
                char semester[50];
                sprintf(semester, "%d", students_list->student_records[p[i]].semester);
                strcat(result, semester);
                strcat(result, "\t");
            }
            if (strcmp(attr[j], "cumgrade") == 0)
            {
                char s[50] = {0};
                sprintf(s, "%lf", students_list->student_records[p[i]].cumgrade);
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
Inputs:  student_list, 
        attributes: fname,lname | * 
        val: 245,543,Physics II,Spring 2020,98.3 
Outputs: 
    string of student with attributes;
*/
int insert_to_table_student(char *val_raw, char *attr_raw, student_table *students_list)
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
    int size = students_list->cant;
    // TODO: Handle Mismatch of size between val and attr
    if (!(sizeof(val) / sizeof(char) == sizeof(attr) / sizeof(char)) {
        return 1;
    }
    // TODO: Handle Student Table Capacity
    if (size == 1000) {
        return 1;
    }
    for (i = 0; i < sizeof(attr) / sizeof(char); i++)
    {
        printf("'%s'\n", attr[i]);
        printf("'%s'\n", val[i]);
        if (strcmp(attr[i], "student_id") == 0)
        {
            students_list->student_records[size].student_id = atoi(val[i]);
            printf("%s", atoi(val[i]));
        }
        // if (strcmp(attr[i], "fname") == 0)
        // {
        //     strcpy(students_list->student_records[size].fname, val[i]);
        // }
        // if (strcmp(attr[i], "lname") == 0)
        // {
        //     strcpy(students_list->student_records[size].lname, val[i]);
        // }
        // if (strcmp(attr[i], "sex") == 0)
        // {
        //     strcpy(&students_list->student_records[size].sex, val[i]);
        // }
        // if (strcmp(attr[i], "status") == 0)
        // {
        //     strcpy(students_list->student_records[size].status, val[i]);
        // }
        // if (strcmp(attr[i], "scholarship") == 0)
        // {
        //     students_list->student_records[size].scholarship = atoi(val[i]);
        // }
        // if (strcmp(attr[i], "semester") == 0)
        // {
        //     students_list->student_records[size].semester = atoi(val[i]);
        // }
        // if (strcmp(attr[i], "cumgrade") == 0)
        // {
        //     double cumgrade;
        //     sscanf(val[1], "%lf", &cumgrade);
        //     students_list->student_records[size].cumgrade = cumgrade;
        // }
    }
    students_list->cant++;
    return 0;
}