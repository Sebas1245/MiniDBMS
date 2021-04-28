#include <arpa/inet.h>  //inet_addr
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //strlen
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>  //write

char cwd[FILENAME_MAX];

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

int main() {
  int socket_desc, new_socket, c;
  struct sockaddr_in server, client;
  char clientMsg[1024] = {0};
  char serverReply[1024] = {0};

  printf("Starting daemonize\n");
  daemonize();
  syslog(LOG_NOTICE, "Server daemon running.");

  while (1) {
    // TODO: Insert daemon code here.
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
      syslog(LOG_ERR, "Could not create socket");
      return 1;
    }

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    // Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
      syslog(LOG_ERR, "connect error");
      return 1;
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
        if (!isLoggedIn) {
          isLoggedIn = 1;
          snprintf(serverReply, sizeof(serverReply), "%d", isLoggedIn);
          send(new_socket, serverReply, sizeof(serverReply), 0);
          continue;
        }

        snprintf(serverReply, sizeof(serverReply), "Welcome");
        send(new_socket, serverReply, sizeof(serverReply), 0);
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
