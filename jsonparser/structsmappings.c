#include <string.h>
#include <stdio.h>
#include "structsmappings.h"


int json_student_read(const char *buf,student_table *obj_list) {  //toma un arreglo de objetos en json

    /* Mapping of JSON attributes to my_object's struct members */
    const struct json_attr_t json_attrs_subobject[] = {
        {"student_id", t_integer, STRUCTOBJECT(student_t, student_id)},
        {"fname", t_string, STRUCTOBJECT(student_t, fname),
                .len = sizeof(obj_list->student_records[0].fname)},
        {"lname", t_string, STRUCTOBJECT(student_t, lname),
                .len = sizeof(obj_list->student_records[0].lname)},
        {"sex", t_character, STRUCTOBJECT(student_t, sex)},
        {"status", t_string, STRUCTOBJECT(student_t, status),
                .len = sizeof(obj_list->student_records[0].status)},
        {"scholarship", t_boolean, STRUCTOBJECT(student_t,  scholarship)},
        {"semester", t_integer, STRUCTOBJECT(student_t, semester)},
        {"cumgrade", t_real, STRUCTOBJECT(student_t, cumgrade)},
        {NULL},
    };

    /* Mapping of JSON attributes to my_object_list's struct members */
    const struct json_attr_t json_attrs_objects[] = {
        {"class", t_check, .dflt.check = "OBJECTS"},
        {"students", t_array, STRUCTARRAY(obj_list->student_records,
                json_attrs_subobject,
                &(obj_list->cant))},
        {NULL},
    };

    memset(obj_list, '\0', sizeof(*obj_list));
    return json_read_object(buf, json_attrs_objects, NULL);
}

int json_grade_read(const char *buf, grade_table *obj_list) {  //toma un arreglo de objetos en json

    /* Mapping of JSON attributes to my_object's struct members */
    const struct json_attr_t json_attrs_subobject[] = {
        {"enrollment_id", t_integer, STRUCTOBJECT(grade_t, enrollment_id)},
        {"student_id", t_integer, STRUCTOBJECT(grade_t, student_id)},
        {"course", t_string, STRUCTOBJECT(grade_t, course),
               .len = sizeof(obj_list->grade_records[0].course)},
        {"school_term", t_string, STRUCTOBJECT(grade_t, school_term),
               .len = sizeof(obj_list->grade_records[0].school_term)},
        {"grade", t_real, STRUCTOBJECT(grade_t, grade)},
        
        {NULL},
    };

    /* Mapping of JSON attributes to my_object_list's struct members */
    const struct json_attr_t json_attrs_objects[] = {
        {"class", t_check, .dflt.check = "OBJECTS"},
        {"grades", t_array, STRUCTARRAY(obj_list->grade_records,
                json_attrs_subobject,
                &(obj_list->cant))},
        {NULL},
    };

    memset(obj_list, '\0', sizeof(*obj_list));
    return json_read_object(buf, json_attrs_objects, NULL);
}

int commitToDBStudents(student_table *obj_list){
    int i;
    FILE *ptr  = fopen("tables/students.json", "w");
    fputs("{\"students\": [\n", ptr );
    for(int i=0;i<obj_list->cant - 1;i++){
        fprintf(ptr, "\t{\"student_id\": %d, \"fname\": \"%s\", \"lname\": \"%s\", \"sex\": \"%c\", \"status\": \"%s\", \"scholarship\": %d, \"semester\": %d, \"cumgrade\": %f},\n",
            obj_list->student_records[i].student_id, 
            obj_list->student_records[i].fname, 
            obj_list->student_records[i].lname, 
            obj_list->student_records[i].sex, 
            obj_list->student_records[i].status, 
            obj_list->student_records[i].scholarship,
            obj_list->student_records[i].semester,
            obj_list->student_records[i].cumgrade   
        ); 
    }
    fprintf(ptr, "\t{\"student_id\": %d, \"fname\": \"%s\", \"lname\": \"%s\", \"sex\": \"%c\", \"status\": \"%s\", \"scholarship\": %d, \"semester\": %d, \"cumgrade\": %f}\n",
            obj_list->student_records[obj_list->cant-1].student_id, 
            obj_list->student_records[obj_list->cant-1].fname, 
            obj_list->student_records[obj_list->cant-1].lname, 
            obj_list->student_records[obj_list->cant-1].sex, 
            obj_list->student_records[obj_list->cant-1].status, 
            obj_list->student_records[obj_list->cant-1].scholarship,
            obj_list->student_records[obj_list->cant-1].semester,
            obj_list->student_records[obj_list->cant-1].cumgrade   
    ); 

    
    fputs("]}", ptr);
    fclose(ptr);
    return 0;
}


int commitToDBGrades(grade_table *obj_list){
    int i;
    FILE *ptr  = fopen("tables/grades.json", "w");
    fputs("{\"grades\": [\n", ptr );
    for(int i=0;i<obj_list->cant - 1;i++){
        fprintf(ptr, "\t{\"enrollment_id\": %d, \"student_id\": %d, \"course\": \"%s\", \"school_term\": \"%s\", \"grade\": %f},\n",
            obj_list->grade_records[i].enrollment_id, 
            obj_list->grade_records[i].student_id, 
            obj_list->grade_records[i].course, 
            obj_list->grade_records[i].school_term, 
            obj_list->grade_records[i].grade  
        ); 
    }
    fprintf(ptr, "\t{\"enrollment_id\": %d, \"student_id\": %d, \"course\": \"%s\", \"school_term\": \"%s\", \"grade\": %f}\n",
            obj_list->grade_records[obj_list->cant-1].enrollment_id, 
            obj_list->grade_records[obj_list->cant-1].student_id, 
            obj_list->grade_records[obj_list->cant-1].course, 
            obj_list->grade_records[obj_list->cant-1].school_term, 
            obj_list->grade_records[obj_list->cant-1].grade 
    ); 
    fputs("]}", ptr);
    fclose(ptr);

    return 0;
}
