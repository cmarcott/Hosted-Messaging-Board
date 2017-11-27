#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include "stream.h"
#include <mysql/mysql.h>

#define HOSTNAME  "dursley"
#define USERNAME  "cmarcott"
#define PASSWORD  "0892486"
#define DATABASE  "cmarcott"

bool CheckForExitCommand() {
  if (feof(stdin)) {
    return true;
  } else return false;
}

/* clear the contents of an array of char's */
void clrstr(char *buf){
  buf[0] = '\0';
}

int main(int argc, char * argv[]) {
  char username[300];
  char list[300];
  char finalList[300];
  char streamListArray[30][200];
  int finalStreams[30], finalStreamsCount;
  bool copy;
  int listlen, i, k, j, numStreams;
  bool notAllowed, cntld;
  
  MYSQL mysql;
  MYSQL_RES *result;/*
  MYSQL_ROW row;
  MYSQL_FIELD *field;
  */
  char query[512];

  notAllowed = cntld = copy = false;
  finalStreamsCount = k = j = numStreams = 0;

  /* Various checks for unacceptable input cases */

  if (argc < 3) {
		printf("**Need to specify an author...Please Try Again...\n");
		return 0;
	}

  /* Prepares program to call RemoveUser() */
  if (strcmp(argv[1], "-r") == 0) {
    if (argc < 4) {
      printf("**Need to specify an author for removal...Please Try Again\n");
      return 0;
    }
    /* Copy Username including spaces*/
    strcpy(username, "\0");
    for (i=2; i<argc; i++) {
      if (strcmp(argv[i], "-un1qu3Ph@s3") == 0) {
        break;
      }
      if (i != 2) {
        strcat(username, " ");
      }
      strcat(username, argv[i]);
    }
    if (strlen(username) > 65) {
      printf("\n**Username entered is too long, must not exceed 65 characters\n");
      printf("No users were added... Please Try Again\n");
      return 0;
    }
    /*printf("list streams: ");*/
    /*fgets(list, 300, stdin);*/
    strcpy(list, "\0");
    copy = false;
    for (i=0; i<argc; i++) {
      if (copy == true) {
        strcat(list, argv[i]);
      }
      if (strcmp(argv[i], "-un1qu3Ph@s3") == 0) {
        copy = true;
      }
    }
    strcat(list, "\n");

    cntld = CheckForExitCommand();
    if (cntld) {
      printf("\n**No post will be saved... Please try Again\n");
      return 0;
    }

    /* Check for already existing users by splitting streams into a char array then checking the user files*/
    listlen = strlen(list);
    for (i=0; i<listlen-1; i++) {
      if (list[i] == ',') {
        streamListArray[numStreams][j] = '\0';
        numStreams++;
        j=0;
        continue;
      }
      if (list[i] == ' ') {
        continue;
      }
      streamListArray[numStreams][j] = list[i];
      j++;
    }
    streamListArray[numStreams][j] = '\0';

        mysql_init(&mysql);
        mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
        if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
            DATABASE, 0, NULL, 0)) {
            printf("Could not connect to host\n");
        }

        clrstr(query);
        
        strcat(query, "create table posts (id int not null auto_increment,");
        strcat(query, "stream char(65),");
        strcat(query, "sender char(65),");
        strcat(query, "date char(65),");
        strcat(query, "text varchar(3000),");
        strcat(query, "primary key(id) )");

        mysql_query(&mysql, query);

        clrstr(query);
        
        strcat(query, "create table users (id int not null auto_increment,");
        strcat(query, "stream char(65),");
        strcat(query, "username char(65),");
        strcat(query, "postsread char(4),");
        strcat(query, "primary key(id) )");
        

        mysql_query(&mysql, query);
      
        clrstr(query);

        for (i=0; i<=numStreams; i++) {
          clrstr(query);
          strcat(query, "SELECT * FROM users WHERE username = '");
          strcat(query, username);
          strcat(query, "' AND stream = '");
          strcat(query, streamListArray[i]);
          strcat(query, "'");

          mysql_query(&mysql, query);
          result=mysql_store_result(&mysql);
          if(mysql_num_rows(result) != 0) {
              printf("Removing user %s from stream %s<br>\n", username, streamListArray[i]);
              finalStreams[finalStreamsCount] = i;
              finalStreamsCount++;
          } else {
              printf("User %s is not a part of stream %s\n<br>", username, streamListArray[i]);
          }
          
        }

        strcpy(finalList, "\0");

        for (k=0; k<=numStreams; k++) {
          for (i=0; i<finalStreamsCount; i++) {
            if (finalStreams[i] == k) {
              notAllowed = true;
            }
          }
          if (notAllowed) {
            if (strcmp(finalList, "\0") != 0) {
              strcat(finalList, ",");
            }
            strcat(finalList, streamListArray[k]);
          }
          notAllowed = false;
        }
        /*Removes user from selected Streams */
        if (strcmp(finalList, "\0") != 0) {
          mysql_close(&mysql);
          printf("<br>**User was removed from streams %s\n", finalList);
          removeUser(username, finalList);
        } else {
          printf("\n**User is not a part of any streams listed or streams do not exist\n\n");
        }
    return 0;

  } else {
    /* Prepares to call addUser() */

    /* Copy Username including spaces*/
    strcpy(username, "\0");
    for (i=1; i<argc; i++) {
      if ((i != 1) && (strcmp(argv[i], "-un1qu3Ph@s3") != 0)) strcat(username, " ");
      if (strcmp(argv[i], "-un1qu3Ph@s3") == 0) {
        break;
      }
      strcat(username, argv[i]);
      if (strcmp(argv[i], "Sender:") == 0) {
        printf("**Username entered cannot contain the word 'Sender:', please choose a different username\n");
        return 0;
      }
    }
    if (strlen(username) > 65) {
      printf("\n**Username entered is too long, must not exceed 65 characters\n");
      printf("No users were added... Please Try Again\n");
      return 0;
    }
    /*printf("list streams: ");*/
    strcpy(list, "\0");
    copy = false;
    for (i=2; i<argc; i++) {
      if (copy == true) {
        strcat(list, argv[i]);
      }
      if (strcmp(argv[i], "-un1qu3Ph@s3") == 0) {
        copy = true;
      }
    }
    strcat(list, "\n");

    if (strcmp(list, "\n") == 0) {
      printf("\n**No Stream was entered... Please try again\n\n");
      exit(0);
    }
    /*fgets(list, 300, stdin);*/
    /*Checks if the user cntl-d'd*/
    cntld = CheckForExitCommand();
    if (cntld) {
      printf("\n**No post will be saved... Please Try Again\n");
      return 0;
    }

    /* Check for already existing users by splitting streams into a char array then checking the user files*/
    listlen = strlen(list);
    for (i=0; i<listlen-1; i++) {
      if (list[i] == ',') {
        streamListArray[numStreams][j] = '\0';
        numStreams++;
        j=0;
        continue;
      }
      if (list[i] == ' ') {
        continue;
      }
      streamListArray[numStreams][j] = list[i];
      j++;
    }
    streamListArray[numStreams][j] = '\0';

    for (i=0; i<=numStreams; i++) {
      if (strlen(streamListArray[i]) > 65) {
        printf("\n**Stream Name entered is too long, must not exceed 65 characters\n");
        printf("No users were added... Please Try Again\n");
        return 0;
      }
      if (strcasecmp(streamListArray[i], "all") == 0) {
        printf("\n**Stream Name cannot be 'all' \n");
        printf("No users were added... Please Try Again\n");
        return 0;
      }
    }


    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
        DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host\n");
    }

    clrstr(query);
    /* Create Post Table*/
    strcat(query, "create table posts (id int not null auto_increment,");
    strcat(query, "stream char(65),");
    strcat(query, "sender char(65),");
    strcat(query, "date char(65),");
    strcat(query, "text varchar(3000),");
    strcat(query, "primary key(id) )");

    mysql_query(&mysql, query);

    clrstr(query);
    /*Create users table*/
    strcat(query, "create table users (id int not null auto_increment,");
    strcat(query, "stream char(65),");
    strcat(query, "username char(65),");
    strcat(query, "postsread char(4),");
    strcat(query, "primary key(id) )");
    

    mysql_query(&mysql, query);

    clrstr(query);
    /*Create streams table*/
    strcat(query, "create table streams (id int not null auto_increment,");
    strcat(query, "stream char(65),");
    strcat(query, "primary key(id) )");
    

    mysql_query(&mysql, query);

    clrstr(query);

    for (i=0; i<=numStreams; i++) {
      clrstr(query);
      strcat(query, "SELECT * FROM users WHERE username = '");
      strcat(query, username);
      strcat(query, "' AND stream = '");
      strcat(query, streamListArray[i]);
      strcat(query, "'");

      mysql_query(&mysql, query);
      result=mysql_store_result(&mysql);
      if(mysql_num_rows(result) != 0) {
          printf("User %s is already a part of stream %s\n<br>", username, streamListArray[i]);
          finalStreams[finalStreamsCount] = i;
          finalStreamsCount++;
      } else {
        printf("Adding user %s to stream %s<br>\n", username, streamListArray[i]);
      }
      
    }

    strcpy(finalList, "\0");

    /* Creates a final string of acceptable streams to be added to */
    for (k=0; k<=numStreams; k++) {
      for (i=0; i<finalStreamsCount; i++) {
        if (finalStreams[i] == k) {
          notAllowed = true;
        }
      }
      if (!notAllowed) {
        if (strcmp(finalList, "\0") != 0) strcat(finalList, ",");
        strcat(finalList, streamListArray[k]);
      }
      notAllowed = false;
    }

    if (strcmp(finalList, "\0") != 0) {
      mysql_close(&mysql);
      addUser(username, finalList);
      printf("\n<br>**User is now part of streams %s \n\n", finalList);
    } else {
      printf("\n<br>**User is already a part of all streams listed\n\n");
    }
  }
  return 0;
}
