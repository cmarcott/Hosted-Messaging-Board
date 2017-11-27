#include "stream.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mysql/mysql.h>

#define HOSTNAME  "dursley"
#define USERNAME  "cmarcott"
#define PASSWORD  "0892486"
#define DATABASE  "cmarcott"

/* This function has the ability to create new files and post to the stream */
void updateStream(struct userPost *st) {
	MYSQL mysql;
  	char query[512];

	mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
        DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host\n");
    }
    strcpy(query, "\0");

	strcat(query, "insert into posts values ('','");
	strcat(query, st->streamname);
	strcat(query, "','");
	strcat(query, st->username);
	strcat(query, "','");
	strcat(query, st->date);
	strcat(query, "','");
	strcat(query, st->text);
	strcat(query, "')");
	if(mysql_query(&mysql, query)){
	  printf("Could not create post\n");
	}

	mysql_close(&mysql);
}

/* This function adds a user to the selected streams, creating the correct files
   if necessary */
void addUser(char *username, char *list) {
	char streamListArray[30][200];
	int listlen, i, k, j;
	
	MYSQL mysql;
	MYSQL_RES *result;
  	char query[512];

	listlen = strlen(list);
	k=j=0;
	for (i=0; i<listlen; i++) {
		if (list[i] == ',') {
			streamListArray[k][j] = '\0';
			k++;
			j=0;
			continue;
		}
		if (list[i] == ' ') {
			continue;
		}
		if (list[i] == '\n') {
			break;
		}
		streamListArray[k][j] = list[i];
		j++;
	}
	streamListArray[k][j] = '\0';

    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
        DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host\n");
    }

	for (i=0; i<=k; i++) {
		strcpy(query, "\0");

		/*insert user to stream*/
		strcat(query, "insert into users values ('','");
		strcat(query, streamListArray[i]);
		strcat(query, "','");
		strcat(query, username);
		strcat(query, "','0')");
		if(mysql_query(&mysql, query)){
		  printf("Could not create user\n");
		}

		strcpy(query, "\0");
		strcat(query, "SELECT * FROM streams WHERE stream = '");
		strcat(query, streamListArray[i]);
		strcat(query, "'");

		mysql_query(&mysql, query);
		result=mysql_store_result(&mysql);
		if(mysql_num_rows(result) != 0) {
			continue;
		} else {
			strcpy(query, "\0");
			/*insert stream into stream table*/
			strcat(query, "insert into streams values ('','");
			strcat(query, streamListArray[i]);
			strcat(query, "')");

			if(mysql_query(&mysql, query)){
			  printf("Could not create stream\n");
			}
		}
		strcpy(query, "\0");
	}
	mysql_close(&mysql);
}

/* This function has the ability to remove a user from selected streams */
void removeUser(char *username, char *list) {
	int listlen, j, i, k;
	char streamListArray[30][200];

	MYSQL mysql;
  	char query[512];

	listlen = strlen(list);
	k = j = i = 0;
	for (i=0; i<listlen; i++) {
		if (list[i] == ',') {
			streamListArray[k][j] = '\0';
			k++;
			j=0;
			continue;
		}
		if (list[i] == ' ') {
			continue;
		}
		if (list[i] == '\n') {
			break;
		}
		streamListArray[k][j] = list[i];
		j++;
	}
	streamListArray[k][j] = '\0';

    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
        DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host\n");
    }

	for (i=0; i<=k; i++) {
		strcpy(query, "\0");

		strcat(query, "delete from users where username = '");
		strcat(query, username);
		strcat(query, "' AND stream = '");
		strcat(query, streamListArray[i]);
		strcat(query, "'");
		if(mysql_query(&mysql, query)){
		  printf("Could not Remove User\n");
		}

	}
	mysql_close(&mysql);
}
