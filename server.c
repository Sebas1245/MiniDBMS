#include <arpa/inet.h>  //inet_addr
#include <limits.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //strlen
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>  //write

#include "jsonparser/structsmappings.h"
#include "queryLanguage/query_main.h"
#include "queryLanguage/query_main_grades.h"

char cwd[FILENAME_MAX];

char **parseLine(char *line_raw) {
  char **query = (char **)malloc(sizeof(char *) * 8);
  int j;
  for (j = 0; j < 8; j++) {
    query[j] = (char *)malloc(50 * sizeof(char));
    memset(query[j], '\0', sizeof(query[j]));
  }

  char line[200];
  strcpy(line, line_raw);
  char delim[] = ";";
  char *ptr = strtok(line, delim);
  int i = 0;
  while (ptr != NULL) {
    strcpy(query[i++], ptr);
    ptr = strtok(NULL, delim);
  }
  memset(line, '\0', sizeof(line));
  return query;
}

static void daemonize() {
  pid_t pid;
  /* Fork off the parent process */
  pid = fork();

  /* An error occurred */
  if (pid < 0) exit(EXIT_FAILURE);

  /* Success: Let the parent terminate */
  if (pid > 0) exit(EXIT_SUCCESS);

  /* On success: The child process becomes session leader */
  if (setsid() < 0) exit(EXIT_FAILURE);

  /* Catch, ignore and handle signals */
  /*TODO: Implement a working signal handler */
  signal(SIGCHLD, SIG_IGN);
  signal(SIGHUP, SIG_IGN);

  /* Fork off for the second time*/
  pid = fork();

  /* An error occurred */
  if (pid < 0) exit(EXIT_FAILURE);

  /* Success: Let the parent terminate */
  if (pid > 0) exit(EXIT_SUCCESS);

  /* Set new file permissions */
  umask(027);

  /* Change the working directory to the root directory */
  /* or another appropriated directory */
  chdir(getcwd(cwd, sizeof(cwd)));

  /* Close all open file descriptors */
  int x;
  for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
    close(x);
  }

  /* Open the log file */
  openlog("MiniDBMS", LOG_PID, LOG_DAEMON);
}

void kill_handler(int signal) {
  syslog(LOG_NOTICE, "Server terminated.");
  closelog();
  exit(0);
}

int main(int argc, char *argv[]) {
  int socket_desc, new_socket, c;
  struct sockaddr_in server, client;
  char clientMsg[1024];
  char serverReply[10024];
  char configCreds[1024];

  // load config file data
  FILE *configFile = fopen("./config.txt", "r");
  if (configFile == NULL) {
    printf("Error opening config file");
    exit(1);
  } else {
    fgets(configCreds, sizeof(configCreds), configFile);
    fclose(configFile);
    printf("Read from config file %s\n", configCreds);
  }
  printf("Starting daemonize\n");
  daemonize();
  syslog(LOG_NOTICE, "Server daemon running.");

  while (1) {
    // TODO: Insert daemon code here.
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
      syslog(LOG_ERR, "Could not create socket");
      exit(1);
    }
    syslog(LOG_NOTICE, "Socket created successfully");
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    // Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
      syslog(LOG_NOTICE, "connect error");
      exit(1);
    }
    syslog(LOG_NOTICE, "bind done, server running");
    listen(socket_desc, 3);
    syslog(LOG_NOTICE, "Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while ((new_socket = accept(socket_desc, (struct sockaddr *)&client,
                                (socklen_t *)&c))) {
      syslog(LOG_NOTICE, "Connection accepted");

      int isLoggedIn = 0;
      while (recv(new_socket, clientMsg, sizeof(clientMsg), 0) > 0) {
        // TODO: Login (se recibe usuario y contrasena en formato:
        // user;password)
        syslog(LOG_NOTICE, clientMsg);
        if (!isLoggedIn) {
          syslog(LOG_NOTICE, "configCreds = %s", configCreds);
          if (strcmp(clientMsg, configCreds) == 0) {
            syslog(LOG_NOTICE,
                   "config credentials match with client credentials %s = %s",
                   clientMsg, configCreds);
            isLoggedIn = 1;
            snprintf(serverReply, sizeof(serverReply), "%d", isLoggedIn);
            send(new_socket, serverReply, sizeof(serverReply), 0);
            continue;
          } else {
            syslog(LOG_NOTICE,
                   "config credentials DON'T match with client credentials %s "
                   "!= %s",
                   clientMsg, configCreds);
            snprintf(serverReply, sizeof(serverReply), "%d", isLoggedIn);
            send(new_socket, serverReply, sizeof(serverReply), 0);
          }
        } else {
          // LOAD DATABASE INFORMATION INTO AN ARRAY OF OBJECTS
          FILE *fStudents;
          FILE *fGrade;

          fStudents = fopen("tables/students.json", "r");
          fGrade = fopen("tables/grades.json", "r");
          // Object to store all students records
          student_table *students = malloc(sizeof(student_table));
          // Object to store all grades records
          grade_table *grades = malloc(sizeof(grade_table));

          size_t nJsonLine = 10000;
          char *json_line = malloc(nJsonLine);
          memset(json_line, '\0', nJsonLine);
          size_t nLine = 1000;
          char *line = malloc(nLine);
          memset(line, '\0', nLine);

          if (fStudents == NULL) {
            printf("Could not open students file");
            return 1;
          }

          if (fGrade == NULL) {
            printf("Could not open grades file");
            return 1;
          }

          // json read by json_student_read must be in one line_st
          while (fgets(line, nLine, fStudents)) {
            strcat(json_line, line);
          }

          // function that convers json objects into C objects
          int statusS = json_student_read(json_line, students);

          memset(json_line, '\0', nJsonLine);
          memset(line, '\0', nLine);

          // json read by json_student_read must be in one line
          while (fgets(line, nLine, fGrade)) {
            strcat(json_line, line);
          }

          // function that convers json objects into C objects
          int statusG = json_grade_read(json_line, grades);

          // TO ACCESS RECORDS OF STUDENTS -> students.students.records[i]
          // TO ACCESS RECORDS OF GRADES -> grades.grade_records[i]

          if (statusS != 0) {
            puts(json_error_string(statusS));
            return 1;
          }
          if (statusG != 0) {
            puts(json_error_string(statusG));
            return 1;
          }

          memset(json_line, '\0', nJsonLine);
          memset(line, '\0', nLine);
          free(json_line);
          free(line);

          fclose(fGrade);
          fclose(fStudents);

          // TODO: return query results
          char **query = parseLine(clientMsg);
          char *result = malloc(10024);
          memset(result, '\0', sizeof(result));
          if (strcmp(query[0], "select") == 0) {
            if (strcmp(query[2], "students") == 0) {
              query_table_student(query[1], query[3], students, result);
              snprintf(serverReply, sizeof(serverReply), "%s", result);
            } else if (strcmp(query[2], "grades") == 0) {
              query_table_grade(query[1], query[3], grades, result);
              snprintf(serverReply, sizeof(serverReply), "%s", result);
            } else {
              snprintf(serverReply, sizeof(serverReply), "Wrong select: %s",
                       clientMsg);
            }
          } else if (strcmp(query[0], "insert") == 0) {
            if (strcmp(query[1], "students") == 0) {
              insert_to_table_student(query[3], query[2], students);
            } else if (strcmp(query[1], "grades") == 0) {
              insert_to_table_grade(query[3], query[2], grades);
            } else {
              snprintf(serverReply, sizeof(serverReply), "Wrong insert: %s",
                       clientMsg);
            }
          } else if (strcmp(query[0], "join") == 0) {
            if (strcmp(query[2], "students,grades") == 0) {
              join_student_with_grades(query[1], students, grades, result);
              snprintf(serverReply, sizeof(serverReply), "%s", result);
            } else if (strcmp(query[2], "grades,students") == 0) {
              join_grades_with_students(query[1], students, grades, result);
              snprintf(serverReply, sizeof(serverReply), "%s", result);
            } else {
              snprintf(serverReply, sizeof(serverReply), "Wrong join: %s",
                       clientMsg);
            }
          } else {
            snprintf(serverReply, sizeof(serverReply), "Wrong query: %s",
                     clientMsg);
          }

          send(new_socket, serverReply, sizeof(serverReply), 0);

          memset(result, '\0', sizeof(result));
          for (int j = 0; j < 8; j++) {
            memset(query[j], '\0', sizeof(query[j]));
            free(query[j]);
          }
          free(result);
          free(query);
          // Commiting Changes to the DB
          int commitStudents = commitToDBStudents(students);
          if (commitStudents != 0) {
            printf("error commiting changes to the database");
            return 1;
          }

          int commitGrades = commitToDBGrades(grades);
          if (commitGrades != 0) {
            printf("error commiting changes to the database");
            return 1;
          }

          free(students);
          free(grades);
        }

        memset(clientMsg, '\0', sizeof(clientMsg));
        memset(serverReply, '\0', sizeof(serverReply));
      }
    }
    if (new_socket < 0) {
      syslog(LOG_ERR, "accept failed");
      exit(1);
    }
    if (signal(SIGTERM, kill_handler) == SIG_ERR) {
      syslog(LOG_NOTICE, "Error terminating process.");
      exit(1);
    }
  }
}
