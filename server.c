#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <limits.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

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

int main()
{
    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    char *message;

    printf("Starting daemonize\n");
    daemonize();
    syslog(LOG_NOTICE, "Server daemon running.");

    while (1)
    {
        //TODO: Insert daemon code here.
        //Create socket
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_desc == -1)
        {
            syslog(LOG_ERR, "Could not create socket");
            return 1;
        }

        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_family = AF_INET;
        server.sin_port = htons(8888);

        //Bind
        if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            syslog(LOG_ERR, "connect error");
            return 1;
        }
        syslog(LOG_NOTICE, "bind done, server running");
        listen(socket_desc, 3);
        syslog(LOG_NOTICE, "Waiting for incoming connections...");
        c = sizeof(struct sockaddr_in);
        new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
        while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
        {
            syslog(LOG_NOTICE, "Connection accepted");

            //Reply to the client
            message = "Hello Client , I have received your connection. But I have to go now, bye\n";
            write(new_socket, message, strlen(message));
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
