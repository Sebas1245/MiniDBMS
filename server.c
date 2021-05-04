#include <arpa/inet.h> //inet_addr
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h> //write
#include <stddef.h>
#include "jsonparser/structsmappings.h"

char cwd[FILENAME_MAX];

static void daemonize()
{
    pid_t pid;
    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    /*TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(027);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir(getcwd(cwd, sizeof(cwd)));

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
    {
        close(x);
    }

    /* Open the log file */
    openlog("MiniDBMS", LOG_PID, LOG_DAEMON);
}

void kill_handler(int signal)
{
    syslog(LOG_NOTICE, "Server terminated.");
    closelog();
    exit(0);
}

int main(int argc, char *argv[])
{
    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    char clientMsg[1024] = {0};
    char serverReply[1024] = {0};
    char configCreds[1024];

    // load config file data
    FILE *configFile = fopen("./config.txt", "r");
    if (configFile == NULL)
    {
        printf("Error opening config file");
        exit(1);
    }
    else
    {
        fgets(configCreds, sizeof(configCreds), configFile);
        fclose(configFile);
        printf("Read from config file %s\n", configCreds);
    }
    printf("Starting daemonize\n");
    daemonize();
    syslog(LOG_NOTICE, "Server daemon running.");

    while (1)
    {
        // TODO: Insert daemon code here.
        // Create socket
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_desc == -1)
        {
            syslog(LOG_ERR, "Could not create socket");
            exit(1);
        }
        syslog(LOG_NOTICE, "Socket created successfully");
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_family = AF_INET;
        server.sin_port = htons(8888);

        // Bind
        if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            syslog(LOG_NOTICE, "connect error");
            exit(1);
        }
        syslog(LOG_NOTICE, "bind done, server running");
        listen(socket_desc, 3);
        syslog(LOG_NOTICE, "Waiting for incoming connections...");
        c = sizeof(struct sockaddr_in);

        while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
        {
            syslog(LOG_NOTICE, "Connection accepted");

            int isLoggedIn = 0;
            while (recv(new_socket, clientMsg, sizeof(clientMsg), 0) > 0)
            {
                // TODO: Login (se recibe usuario y contrasena en formato:
                // user;password)
                syslog(LOG_NOTICE, clientMsg);
                if (!isLoggedIn)
                {
                    syslog(LOG_NOTICE, "configCreds = %s", configCreds);
                    if (strcmp(clientMsg, configCreds) == 0)
                    {
                        syslog(LOG_NOTICE, "config credentials match with client credentials %s = %s", clientMsg, configCreds);
                        isLoggedIn = 1;
                        snprintf(serverReply, sizeof(serverReply), "%d", isLoggedIn);
                        send(new_socket, serverReply, sizeof(serverReply), 0);
                        continue;
                    }
                    else
                    {
                        syslog(LOG_NOTICE, "config credentials DON'T match with client credentials %s != %s", clientMsg, configCreds);
                        snprintf(serverReply, sizeof(serverReply), "%d", isLoggedIn);
                        send(new_socket, serverReply, sizeof(serverReply), 0);
                    }
                }
                else
                {

                    // LOAD DATABASE INFORMATION INTO AN ARRAY OF OBJECTS
                    FILE *fStudents;
                    FILE *fGrade;

                    fStudents = fopen("tables/students.json", "r");
                    fGrade = fopen("tables/grades.json", "r");
                    // Object to store all students records
                    student_table *students = malloc(sizeof(student_table));
                    // Object to store all grades records
                    grade_table *grades = malloc(sizeof(grade_table));

                    char json_line[1000] = "";
                    char line[1000];

                    if (fStudents == NULL) {
                        printf("Could not open students file");
                        return 1;
                    }

                    if (fGrade == NULL){
                        printf("Could not open grades file");
                        return 1;
                    }

                    // json read by json_student_read must be in one line
                    while (fgets(line, sizeof(line), fStudents)) {
                        strcat(json_line, line);
                    }

                    // function that convers json objects into C objects
                    int statusS = json_student_read(json_line, students);

                    memset(line,0,1000);
                    memset(json_line,0,1000);
                    
                    // json read by json_student_read must be in one line
                    while (fgets(line, sizeof(line), fGrade)) {
                        strcat(json_line, line);
                    }

                    // function that convers json objects into C objects
                    int statusG = json_grade_read(json_line, grades);

                    // TO ACCESS RECORDS OF STUDENTS -> students.students.records[i]
                    // TO ACCESS RECORDS OF GRADES -> grades.grade_records[i]

                    if (statusS != 0) {
                        puts(json_error_string(statusS));
                        return 1;
                    }else if(statusG != 0 ){
                        puts(json_error_string(statusG));
                    }

                    fclose(fGrade);
                    fclose(fStudents);

                    // TODO: return query results

                    snprintf(serverReply, sizeof(serverReply), "%s", clientMsg);
                    send(new_socket, serverReply, sizeof(serverReply), 0);



                    //Commiting Changes to the DB
                    int commitStudents = commitToDBStudents(students);
                    if (commitStudents != 0 ) printf("error commiting changes to the database");

                    int commitGrades = commitToDBGrades(grades);
                    if(commitGrades != 0) printf("error commiting changes to the database");
                    

                }
            }
        }
        if (new_socket < 0)
        {
            syslog(LOG_ERR, "accept failed");
            exit(1);
        }
        if (signal(SIGTERM, kill_handler) == SIG_ERR)
        {
            syslog(LOG_NOTICE, "Error terminating process.");
            exit(1);
        }
    }
}
