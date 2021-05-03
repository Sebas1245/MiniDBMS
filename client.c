#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8888

int main(int argc, char const *argv[])
{
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char *hello = "Hello from client";
  char clientReply[1024] = {0};
  char serverMsg[1024] = {0};

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Socket creation error \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
  {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\nConnection Failed \n");
    return -1;
  }
  printf("Connection established\n");

  // Chat with server
  int isLoggedIn = 0;
  while (1)
  {
    if (!isLoggedIn)
    {
      printf("Log In with your user and password (format: user;password): ");
      scanf("%s", clientReply);
      send(sock, clientReply, sizeof(clientReply), 0);
      if (recv(sock, serverMsg, sizeof(serverMsg), 0) > 0)
      {
        printf("Server: %s\n", serverMsg);
        isLoggedIn = atoi(serverMsg);
        if (!isLoggedIn)
          printf("Authentication error, please try again.\n");

        else
          printf("Welcome to your MiniDBMS!\n");
      }
      continue;
    }

    printf("Remember you must follow the following syntax (no spaces):\n");
    printf("- Select:\n");
    printf(
        "\tselect;columnName,columnName2 | *(to print all "
        "columns);tableName;[optional "
        "where clause]columnName,operator,conditionValue\n");
    printf("- Insert:\n");
    printf("\tinsert;tableName;columnName1,columName2 | *;value1,value2\n");
    printf("- Join:\n");
    printf("\tjoin;columnName,columnName2 | *;tableName1,tableName2\n");
    printf("Now, type your query:\n");
    scanf("%s", clientReply);
    send(sock, clientReply, sizeof(clientReply), 0);
    if (recv(sock, serverMsg, sizeof(serverMsg), 0) > 0)
    {
      printf("Query was: %s\n", serverMsg);
    }
  }

  return 0;
}