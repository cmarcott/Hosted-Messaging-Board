#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>


#define HOSTNAME  "dursley"
#define USERNAME  "cmarcott"
#define PASSWORD  "0892486"
#define DATABASE  "cmarcott"

void clrstr(char *buf){
  buf[0] = '\0';
}

void error(char *msg, MYSQL *mysql){
	printf("%s\n%s\n",msg,mysql_error(mysql));
	exit(1);
}

int main(int argc, char * argv[]) {

	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char * theToken;
	char streamlist[2000];
	int i;
	char query[512];

	if (argc < 2) {
		printf("**A flag needs to be entered with the program... for a list of avaliable options use the '-help' flag\n");
		exit(0);
	}

	mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
        DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host\n");
    }

    clrstr(query);
	
	if (strcmp(argv[1], "-users") == 0) {
		clrstr(query);
	    strcat(query, "select DISTINCT username from users");
		if(mysql_query(&mysql, query)){
			error("fail select users",&mysql);
		}
		if (!(res = mysql_store_result(&mysql))){
			error("fail store users",&mysql);
		}
		while ((row = mysql_fetch_row(res))) {
			for (i=0; i < mysql_num_fields(res); i++){
				printf("%s ", row[i]);
			}
			printf("\n");
		}
	} else if (strcmp(argv[1], "-streams") == 0) {
		clrstr(query);
		strcat(query, "select DISTINCT stream from streams");
		if(mysql_query(&mysql, query)){
			error("fail select streams",&mysql);
		}
		if (!(res = mysql_store_result(&mysql))){
			error("fail store streams",&mysql);
		}
		while ((row = mysql_fetch_row(res))) {
			for (i=0; i < mysql_num_fields(res); i++){
				printf("%s ", row[i]);
			}
			printf("\n");
		}
	} else if (strcmp(argv[1], "-posts") == 0) {
		clrstr(query);
		strcat(query, "select stream,sender,date,text from posts");
		if(mysql_query(&mysql, query)){
			error("fail select posts",&mysql);
		}
		if (!(res = mysql_store_result(&mysql))){
			error("fail store posts",&mysql);
		}
		while ((row = mysql_fetch_row(res))) {
			for (i=0; i < mysql_num_fields(res); i++){
				printf("%s \n", row[i]);
			}
			printf("\n");
		}
	} else if (strcmp(argv[1], "-clear") == 0) {

		clrstr(query);
		strcat(query, "drop table posts");
		mysql_query(&mysql, query);

		clrstr(query);
		strcat(query, "drop table users");
		mysql_query(&mysql, query);

		clrstr(query);
		strcat(query, "drop table streams");
		mysql_query(&mysql, query);

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
	} else if (strcmp(argv[1], "-reset") == 0) {

		clrstr(query);
		strcat(query, "drop table posts");
		mysql_query(&mysql, query);

		clrstr(query);
		strcat(query, "drop table users");
		mysql_query(&mysql, query);

		clrstr(query);
		strcat(query, "drop table streams");
		mysql_query(&mysql, query);

	} else if (strcmp(argv[1], "-help") == 0) {
		printf("Availiable flags are:\n'-posts' will show all posts in the database\n'-streams will list all streams in the database\n'-users' will show all users in the database\n'-clear' clears all information from the table\n'-reset' removes all tables from the database\n'-help' shows this help list\n");
	} else if (strcmp(argv[1], "-ch00seStr3am") == 0) {
		if (argc < 3) {
			exit(0);
		}
		clrstr(query);
		strcat(query, "select stream from users where username = '");
		strcat(query, argv[2]);
		strcat(query, "'");
		mysql_query(&mysql, query);
		if(!(res = mysql_store_result(&mysql))) {
			exit(0);
		}
		while ((row = mysql_fetch_row(res))) {
			for (i=0; i < mysql_num_fields(res); i++){
				printf("%s", row[i]);
			}
			printf("\n");
		}

	} else if (strcmp(argv[1], "-postsRead") == 0) {
		if (argc < 3) {
			exit(0);
		}
		clrstr(query);
		strcat(query, "select postsread from users where username = '");
		strcat(query, argv[2]);
		strcat(query, "' and stream = '");
		strcat(query, argv[3]);
		strcat(query, "'");
		mysql_query(&mysql, query);
		res = mysql_store_result(&mysql);
		while ((row = mysql_fetch_row(res))) {
			for (i=0; i < mysql_num_fields(res); i++){
				printf("%s", row[i]);
			}
			printf("\n");
		}

	} else if (strcmp(argv[1], "-vi3w") == 0) {
		if (argc < 3) {
			exit(0);
		}
		clrstr(query);
		strcat(query, "select sender,date,text from posts where stream = '");
		strcat(query, argv[2]);
		strcat(query, "'");
		mysql_query(&mysql, query);
		res = mysql_store_result(&mysql);
		while ((row = mysql_fetch_row(res))) {
			for (i=0; i < mysql_num_fields(res); i++){
				printf("%s\n", row[i]);
			}
			printf("-3ND\n");
		}

	} else if (strcmp(argv[1], "-updateR3ad") == 0) {
		if (argc < 3) {
			exit(0);
		}
		clrstr(query);
		strcat(query, "update users set postsread = '");
		strcat(query, argv[4]);
		strcat(query, "' where username ='");
		strcat(query, argv[2]);
		strcat(query, "' and stream = '");
		strcat(query, argv[3]);
		strcat(query, "'");
		mysql_query(&mysql, query);
		res = mysql_store_result(&mysql);
	} else if (strcmp(argv[1], "-inam3s0rt") == 0) {
		if (argc < 3) {
			exit(0);
		}
		clrstr(query);
		strcat(query, "select sender,date,text from posts where stream = '");
		strcat(query, argv[2]);
		strcat(query, "'");
		strcat(query, "order by sender");
		mysql_query(&mysql, query);
		res = mysql_store_result(&mysql);
		while ((row = mysql_fetch_row(res))) {
			for (i=0; i < mysql_num_fields(res); i++){
				printf("%s\n", row[i]);
			}
			printf("-3ND\n");
		}

	} else if (strcmp(argv[1], "-allStr3ams") == 0) {
		if (argc < 3) {
			exit(0);
		}
		strcpy(streamlist, "\0");
		theToken = strtok(argv[2], " "); 
		i=0;
		while(theToken != NULL) {
			if (i!=0) {
				strcat(streamlist, "' OR stream = '");
			}
			strcat(streamlist,theToken);
			theToken = strtok(NULL, " ");
			i++;
		}
		clrstr(query);
		strcat(query, "select stream,sender,date,text from posts where stream = '");
		strcat(query, streamlist);
		strcat(query, "'");
		mysql_query(&mysql, query);
		res = mysql_store_result(&mysql);
		while ((row = mysql_fetch_row(res))) {
			for (i=0; i < mysql_num_fields(res); i++){
				printf("%s\n", row[i]);
			}
			printf("-3ND");
		}

	} else if (strcmp(argv[1], "-anam3s0rt") == 0) {
		if (argc < 3) {
			exit(0);
		}
		strcpy(streamlist, "\0");
		theToken = strtok(argv[2], " "); 
		i=0;
		while(theToken != NULL) {
			if (i!=0) {
				strcat(streamlist, "' OR stream = '");
			}
			strcat(streamlist,theToken);
			theToken = strtok(NULL, " ");
			i++;
		}
		clrstr(query);
		strcat(query, "select stream,sender,date,text from posts where stream = '");
		strcat(query, streamlist);
		strcat(query, "'");
		strcat(query, " order by sender");
		mysql_query(&mysql, query);
		res = mysql_store_result(&mysql);
		while ((row = mysql_fetch_row(res))) {
			for (i=0; i < mysql_num_fields(res); i++){
				printf("%s\n", row[i]);
			}
			printf("-3ND");
		}

	} else if (strcmp(argv[1], "-allP0stsR3ad") == 0) {
		if (argc < 3) {
			exit(0);
		}
		clrstr(query);
		strcat(query, "select stream, postsread from users where username = '");
		strcat(query, argv[2]);
		strcat(query, "'");
		mysql_query(&mysql, query);
		res = mysql_store_result(&mysql);
		while ((row = mysql_fetch_row(res))) {
			for (i=0; i < mysql_num_fields(res); i++){
				printf("%s ", row[i]);
			}
			printf("\n");
		}

	} else {
		printf("**Flag '%s' is not an option, for a list of avaliable options use the '-help' flag \n", argv[1]);
	}

	return 0;
}