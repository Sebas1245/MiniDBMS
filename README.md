# MiniDBMS
File based mini DBMS 


### Compilation and Execution
This project was designed to be run on Linux. 
Run `make` and this will automatically compile the server file according to instructions on Makefile. This then generates the compiled file as `server.o`. To execute the file, run:


```
./server.o
```
The client is compiled like any regular C source file:
```
gcc client.c -o client.o
```


### JSON Parser
In this project [microjson](https://gitlab.com/esr/microjson.git) was  used to read a .JSON file containing the database's data and convert it into C objects. 

### SQL-like syntax
Queries made in this DBMS must follow the following syntax: 
* select;columnName,columnName2 | *;tableName;none | columnName,operator,conditionValue; 
* insert;tableName;columnName1,columName2 | *;value1,value2;
* join;columnName,columnName2 | *;tableName1,tableName2;

Note: * (to print all columns)
Note: in a `select` query, a `none` clause needs to be specified if there are no conditions on the query. 

It's important to know beforehand the exact names of the tables and their attributes since MiniDBMS is case-sensitive. 

#### Some Examples

* select;fname,lname;students;cumgrade,<,70;  //shows first and last name of the students with a cumulative grade lower than 70.
* insert;grades;*;245,543,Physics II,Spring 2020,98.3; //inserts new records in the table grades and provides values for all columns
* join;fname,lname;students,grades; // shows first and last name who have information in the grades table

More examples of queries for MiniDBMS can be found and executed in [test.c](test.c)

