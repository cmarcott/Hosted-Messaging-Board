#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stream.h"
#include <stdbool.h>
#include <time.h>
#include <mysql/mysql.h>

#define HOSTNAME  "dursley"
#define USERNAME  "cmarcott"
#define PASSWORD  "0892486"
#define DATABASE  "cmarcott"

void clrstr(char *buf){
  buf[0] = '\0';
}
class PostEntry;
struct userPost * InitializeUserPost();
void FreeUserPost(struct userPost * post);
char * InitializeString(int size);
char * ReallocString(char * string, int size);
void PrintUserPostStruct(struct userPost post);
char * CopyString(char * toBeCopied, char * toCopyTo);
bool CheckForExitCommand();
void ExitProgram(struct userPost * post);

/* PostEntry Class with the four functions */
class PostEntry {

	char * readInput() {
		char tempString[300];
		char * pointerString;
		bool cntlD;

		fgets(tempString, 300, stdin);
		cntlD = CheckForExitCommand();
		if (cntlD == false) pointerString = InitializeString(1);
		if (cntlD == false) pointerString = CopyString(tempString, pointerString);

		return pointerString;
	}

	char * formatEntry(char * toBeCopied, char * toCopyTo) {
		int tlen, stringLen;
		tlen = stringLen = 0;

		tlen = strlen(toBeCopied);
		stringLen = strlen(toCopyTo);
		stringLen = stringLen+tlen;
		toCopyTo = ReallocString(toCopyTo, stringLen);
		strcat(toCopyTo, toBeCopied);

		return toCopyTo;
	}

	char * getTimeDate() {
		char * timeString;
		char tempString[200];
		int len;
		time_t t;
		time(&t);
		struct tm *timeinfo = localtime(&t);

		strftime(tempString, sizeof(tempString), "%b. %d, %Y  %I:%M:%S %p", timeinfo);
		len = strlen(tempString);
		timeString = InitializeString(1);
		timeString = ReallocString(timeString, len);
		strcpy(timeString, tempString);

		return timeString;
	}

	/* This function will restrict the user from posting to a stream
	 that doesn't exist or one the user does not have permission to,
	 if none of those cases are true it will call updateStream() */
	void submitPost(struct userPost * post) {
		char * theToken;
		bool permission;
		MYSQL mysql;
  		MYSQL_RES *result;

  		char query[512];

		permission = false;

		theToken = strtok(post->streamname, "\n");
		if (theToken == NULL) {
			printf("\n**A stream was not entered\n");
			return;
		}

		mysql_init(&mysql);
        mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
        if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
            DATABASE, 0, NULL, 0)) {
            printf("Could not connect to host\n");
        }

	      clrstr(query);
	      strcat(query, "SELECT * FROM users WHERE username = '");
	      strcat(query, post->username);
	      strcat(query, "' AND stream = '");
	      strcat(query, post->streamname);
	      strcat(query, "'");

	      mysql_query(&mysql, query);
	      result=mysql_store_result(&mysql);
	      if(mysql_num_rows(result) != 0) {
	          permission = true;
	      } else {
	        permission = false;
	      }

		if (permission) {
			mysql_close(&mysql);
			updateStream(post);
			printf("You have successfully submitted your post, Please choose an option below to continue\n");
		} else {
			mysql_close(&mysql);
			printf("\n**You do not have permission to post to this stream... Exiting Program\n");
		}
	}
};

/* Various helper functions */

struct userPost * InitializeUserPost() {

	struct userPost * post;
	post = (struct userPost*)malloc(sizeof(struct userPost)+1);
	post->username = InitializeString(1);
	post->streamname = InitializeString(1);
	post->date = InitializeString(1);
	post->text = InitializeString(1);
	return post;
}

void FreeUserPost(struct userPost * post) {
	free(post->username);
	free(post->streamname);
	free(post->date);
	free(post->text);
	free(post);
}

char * InitializeString(int size) {
	char * string;
	string = (char*)malloc(sizeof(char)*size+1);
	strcpy(string, "\0");
	return string;
}

char * ReallocString(char * string, int size) {
	string = (char*)realloc(string, sizeof(char)*size+1);
	return string;
}

void PrintUserPostStruct(struct userPost post) {
	printf("\n\nusername: '%s'\n", post.username);
	printf("streamname: '%s'\n", post.streamname);
	printf("date: %s\n", post.date);
	printf("text: '%s'\n\n", post.text);
}

char * CopyString(char * toBeCopied, char * toCopyTo) {
	int tlen, stringLen;
	tlen = stringLen = 0;

	tlen = strlen(toBeCopied);
	stringLen = strlen(toCopyTo);
	stringLen = stringLen+tlen;
	toCopyTo = ReallocString(toCopyTo, stringLen);
	strcat(toCopyTo, toBeCopied);

	return toCopyTo;
}

bool CheckForExitCommand() {
	if (feof(stdin)) {
		return true;
	} else return false;
}

void ExitProgram(struct userPost * post) {

	FreeUserPost(post);
	exit(1);
}

/* This one tackles folding lines over 80 chars */
char * CheckToFoldLines(char * pointerString) {
	int length, numFolds, i, k;
	char array[100][85];
	length = strlen(pointerString);
	numFolds = length/80;
	for (i=0; i<100; i++) {
		strcpy(array[i], "\0");
	}
	if ((numFolds > 0) && (length != 81) && (length != 80)) {
		pointerString = ReallocString(pointerString, length+numFolds);
		k=0;
		for (i=0; i<=numFolds; i++) {
			k += 79;
			strncpy(array[i], &pointerString[k-79], k);
			array[i][79] = '\0';
		}
		strcpy(pointerString, "\0");
		for (i=0; i<=numFolds; i++) {
			strcat(pointerString, array[i]);
			if (i != numFolds) {
				strcat(pointerString, "\n");
			}
		}
		return pointerString;
	} else {
		return pointerString;
	}
}

int main(int argc, char * argv[]) {
	struct userPost * post;
	class PostEntry postClass;
	char * pointerString, * dateString;
	char tempUsername[300], tempStreamname[400];
	/*bool cntlD;*/
	int i, startedAt;
	bool startAdding;

	startAdding = false;
	startedAt = 0;
	post = InitializeUserPost();

	if (argc < 2) {
		printf("**Need to specify an author...Program Exiting...\n");
		FreeUserPost(post);
		return -1;
	}
	if (strcmp(argv[argc-1], "-un1qu3Fl@g") == 0) {
		printf("No Text was entered... Please Try Again\n");
		return 0;
	}
	strcpy(tempUsername, "\0");
	for (i=1; i<argc; i++) {
		if (strcmp(argv[i], "-un1qu3Ph@s3") == 0) {
			break;
		}
		if (i != 1) strcat(tempUsername, " ");
		strcat(tempUsername, argv[i]);
	}
	/* Add Username */
	post->username = postClass.formatEntry(tempUsername, post->username);

	/* Add Streamname */
	strcpy(tempStreamname, "\0");
	for (i=1; i<argc; i++) {
		if (strcmp(argv[i], "-un1qu3Fl@g") == 0) {
			break;
		}
		if ((i != startedAt) && (startAdding == true)) strcat(tempStreamname, " ");
		if (startAdding == true) {
			strcat(tempStreamname, argv[i]);
		}
		if (strcmp(argv[i], "-un1qu3Ph@s3") == 0) {
			startAdding = true;
			startedAt = i+1;
		}
	}
	startAdding = false;
	post->streamname = postClass.formatEntry(tempStreamname, post->streamname);
	pointerString = InitializeString(1);
	for (i=1; i<argc; i++) {
		if ((i != startedAt) && (startAdding == true)) pointerString = CopyString(" ", pointerString);
		if (startAdding == true) {
			if (strcmp(argv[i], "-5G5") == 0) {
				pointerString = CopyString("\n", pointerString);
				continue;
			}
			pointerString = CopyString(argv[i], pointerString);
		}
		if (strcmp(argv[i], "-un1qu3Fl@g") == 0) {
			startAdding = true;
			startedAt = i+1;
		}
	}
	pointerString = CopyString("\n", pointerString);
	dateString = postClass.getTimeDate();
	post->date = postClass.formatEntry(dateString, post->date);
	post->text = postClass.formatEntry(pointerString, post->text);
	postClass.submitPost(post);

	free(dateString);
	free(pointerString);
	FreeUserPost(post);

	return 0;
}
