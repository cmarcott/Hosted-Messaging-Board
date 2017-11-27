#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/*************************************************************************
Name: mallocBuffer
Purpose: allocates a string *, while initializing the string with "\0"
**************************************************************************/
char * mallocBuffer(int bSize) {
  char * buffer;
  buffer = malloc(sizeof(char)*bSize+1);
  strcpy(buffer, "\0");
  return buffer;
}

/*************************************************************************
Name: reallocBuffer
Purpose: Used to realloc a string *, and sets final char to '\0'
**************************************************************************/
char * reallocBuffer(char * buffer, int bSize) {
  buffer = realloc(buffer, sizeof(char)*bSize+1);
  buffer[bSize] = '\0';
  return buffer;
}

/*************************************************************************
Name: addBufferStringYesChar
Purpose: Used in the initial parsing of information from a file.
         Adds a buffer string to the double pointer string array storing each token
         from the file in it's own string. The entire parsedStorage array is dynamically
         allocated so it is necessary
**************************************************************************/
char ** addBufferStringYesChar(char ** parsedStorage, char * buffer, int * bufferSize, int * parsedSize, char toAdd) {
  int pSize, bSize;
  pSize = *parsedSize;
  bSize = *bufferSize;
  pSize++;
  bSize++;
  buffer = reallocBuffer(buffer, bSize);
  parsedStorage = realloc(parsedStorage, sizeof(char*)*(pSize)+1);
  buffer[bSize-1] = toAdd;
  parsedStorage[pSize-1] = buffer;

  bSize = 0;
  *parsedSize = pSize;
  *bufferSize = bSize;
  return parsedStorage;
}

/*************************************************************************
Name: addBufferStringNoChar
Purpose: Used for the same purpose as above function, except it does not append
         a character to the end before appending buffer to double string array
**************************************************************************/
char ** addBufferStringNoChar(char ** parsedStorage, char * buffer, int * bufferSize, int * parsedSize) {
  int pSize, bSize;
  pSize = *parsedSize;
  bSize = *bufferSize;
  pSize++;

  parsedStorage = realloc(parsedStorage, sizeof(char*)*(pSize)+1);
  parsedStorage[pSize-1] = buffer;

  bSize = 0;
  *parsedSize = pSize;
  *bufferSize = bSize;
  return parsedStorage;
}

bool checkIfWhitespace(char * string) {
  if ((strcmp(string, " ") != 0) && (strcmp(string, "\f") != 0) && (strcmp(string, "\v") != 0) && (strcmp(string, "\r") != 0) && (strcmp(string, "\t") != 0) && (strcmp(string, "\n") != 0)) {
    return false;
  } else {
    return true;
  }
}
/*************************************************************************
Name: Parser
Purpose: Parser Function, parses all information from the file using a switch
         statement, and calls a converter function which does what is necessary
         to the information to make it compile as a .c program
**************************************************************************/
char ** Parser(int argc, char *argv[], int * ListSize) {
  char currentChar;
  char ** parsedStorage;
  char * buffer;
  int bSize, storSize, parsedSize;
  FILE * inputFile;

  inputFile = fopen(argv[1], "r");

  bSize = storSize = parsedSize = 0;
  currentChar = ' ';

  parsedStorage = malloc(sizeof(char*)*storSize+1);
  buffer = mallocBuffer(bSize+1);

  do {
    if (inputFile == NULL) {
      printf("File was not found...Exiting program\n");
      free(parsedStorage);
      free(buffer);
      return NULL;
    }
    currentChar = fgetc(inputFile);
    if (feof(inputFile)) {
      break;
    }
    /*Switch statement that parses information from the file*/
    switch (currentChar) {
      case ' ':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, ' ');
        buffer = mallocBuffer(bSize);
        break;
      case '\n':
      /*Checks for empty buffer string, if it is not empty adds it on to the char ** array parsedStorage
        and begins a new one which it adds on as well*/
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '\n');
        bSize = 0;
        buffer = mallocBuffer(bSize);
        break;
      case '\r':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '\r');
        buffer = mallocBuffer(bSize);
      break;
      case '\t':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '\t');
        buffer = mallocBuffer(bSize);
      break;
      case '\v':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '\v');
        buffer = mallocBuffer(bSize);
        break;
      case '\f':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '\f');
        buffer = mallocBuffer(bSize);
        break;
      case '.':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '.');
        buffer = mallocBuffer(bSize);
      break;
      case '(':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '(');
        buffer = mallocBuffer(bSize);
      break;
      case '<':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '<');
        buffer = mallocBuffer(bSize);
      break;
      case '>':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '>');
        buffer = mallocBuffer(bSize);
      break;
      case ',':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, ',');
        buffer = mallocBuffer(bSize);
      break;
      case '=':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '=');
        buffer = mallocBuffer(bSize);
      break;
      case ')':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, ')');
        buffer = mallocBuffer(bSize);
      break;
      case '"':
      /*This switch case is for quotes, it will add entire comments on as one token*/
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '"');
        buffer = mallocBuffer(bSize);
        currentChar = fgetc(inputFile);
		    if (feof(inputFile)) {
		      break;
		    }
        bSize++;
        buffer = reallocBuffer(buffer, bSize);
        buffer[bSize-1] = currentChar;
        if (currentChar == '"') {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
          continue;
        }
        currentChar = fgetc(inputFile);
        if (feof(inputFile)) {
          break;
        }
        while ((currentChar != '"')) {
          bSize++;
          buffer = reallocBuffer(buffer, bSize);
          buffer[bSize-1] = currentChar;
          currentChar = fgetc(inputFile);
          if (feof(inputFile)) {
            break;
          }
        }
        parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
        buffer = mallocBuffer(bSize);
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '"');
        buffer = mallocBuffer(bSize);
        break;
      default:
        /* The default case is to just add on a character to the token*/
        bSize++;
        buffer = reallocBuffer(buffer, bSize);
        buffer[bSize-1] = currentChar;
        break;
    }
  } while (1);
  	if (strcmp(buffer, "\0") != 0) {
      parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
      buffer = mallocBuffer(bSize);
    }
  /*Calls the main converter*/
  /*Convert(parsedStorage, parsedSize, outputFileString);*/

  /*Various Frees*/
  free(buffer);
  fclose(inputFile);
  *ListSize = parsedSize;
  return parsedStorage;
}

void Conv2HTML(char ** parsedStorage, int parsedSize, char * filename) {
	char * htmlString;
  char * theToken;
  char * PageName;
  bool fleft, booltextfield, booltextfile;
	int htmlSize, i, hargsize;
	int count, rvalueCount, tripletnum, hiddentripletnum;
  char headerSize[50], headerText[450], buttonName[150], buttonLink[450], linkText[450], linkLink[450];
  char imageLink[450], imgWidth[50], imgHeight[50], radioFilename[100], radioName[100], radioValue[50][100];
  char inputAction[150], inputName[50][150], inputText[50][150], inputValue[50][150], inputrows[50][150], inputcols[50][150], elementValue[150], element[100];
  char textfield[500], cmd[250], hiddenName[50][150], hiddenValue[50][150], varname[100], varvalue[100], hsize[50], harg[50][200], executableCommand[100], textfilename[100];
  /*FILE *outputFile = fopen("output2.php", "w");*/

	htmlSize = count = rvalueCount = tripletnum = hiddentripletnum = hargsize = 0;
  fleft = booltextfield = booltextfile = false;
	htmlString = mallocBuffer(htmlSize+1);
  PageName = strtok(filename, ".");

  htmlSize = htmlSize + strlen("<HTML>\n<HEAD>\n<TITLE>") + strlen(PageName) + strlen("</TITLE>\n\n<BODY>\n\n");
  htmlString = reallocBuffer(htmlString, htmlSize);

  strcpy(htmlString, "<HTML>\n<HEAD>\n<TITLE>");
  strcat(htmlString, PageName);
  strcat(htmlString, "</TITLE>\n\n<BODY>\n\n");

	while (count < parsedSize) {

		if (strcmp(parsedStorage[count], ".") == 0) {
      if (count+3 < parsedSize) {
  			if ((strcmp(parsedStorage[count+1], "h") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            strcpy(headerSize, "\0");
            strcpy(headerText, "\0");
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "size") == 0) {
                while (strcmp(parsedStorage[count], "=") != 0) {
                  count++;
                }
                count++;
                while ((strcmp(parsedStorage[count], ",") != 0) && (strcmp(parsedStorage[count], ")") != 0)) {
                  if (checkIfWhitespace(parsedStorage[count]) == false) {
                    strcat(headerSize, parsedStorage[count]);
                  }
                  count++;
                }
              } else if (strcmp(parsedStorage[count], "text") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(headerText, parsedStorage[count]);
                }
              }
              count++;
            }
            if (strcmp(headerSize, "\0") == 0) {
              strcpy(headerSize, "3");
            }
            if (strcmp(headerText, "\0") == 0) {
              strcpy(headerText, "HEADING");
            }
            htmlSize = htmlSize + strlen("<h>") + strlen(headerSize) + strlen(headerSize) + strlen(headerText) + strlen("</h>\n\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "<h");
            strcat(htmlString, headerSize);
            strcat(htmlString, ">");
            strcat(htmlString, headerText);
            strcat(htmlString, "</h");
            strcat(htmlString, headerSize);
            strcat(htmlString, ">\n\n");
        } else if ((strcmp(parsedStorage[count+1], "b") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            fleft = false;
            strcpy(buttonName, "\0");
            strcpy(buttonLink, "\0");
            hiddentripletnum = 0;
            for (i=0; i<50; i++) {
              strcpy(hiddenValue[i], "\0");
              strcpy(hiddenName[i], "\0");
            }
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "name") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(buttonName, parsedStorage[count]);
                }
              } else if (strcmp(parsedStorage[count], "link") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(buttonLink, parsedStorage[count]);
                }
              } else if (strcmp(parsedStorage[count], "hide") == 0) {
                while (strcmp(parsedStorage[count], "name") != 0) {
                  count++;
                }
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(hiddenName[hiddentripletnum], parsedStorage[count]);
                }
                while (strcmp(parsedStorage[count], "value") != 0) {
                  count++;
                }
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(hiddenValue[hiddentripletnum], parsedStorage[count]);
                }
                hiddentripletnum++;
              } else if (strcmp(parsedStorage[count], "fleft") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                fleft = true;
              }
              count++;
            }
            if (strcmp(buttonName, "\0") == 0) {
              strcpy(buttonName, "Default Button");
            }
            if (strcmp(buttonLink, "\0") == 0) {
              strcpy(buttonLink, "http://www.google.com");
            }
            htmlSize = htmlSize+strlen("<form method = \"POST\" action=\"")+strlen(buttonLink)+strlen("\">\n  <input type=\"submit\" value=\"")+strlen(buttonName)+strlen("\"/>\n</form>\n");
            if (fleft) {
              htmlSize += strlen("style = 'float: left' ");
            }
            for (i=0; i<hiddentripletnum; i++) {
              htmlSize += strlen("  ");
              htmlSize += strlen(" <input type=\"hidden\" name=\"");
              htmlSize += strlen(hiddenName[i]);
              htmlSize += strlen("\" value=\"");
              htmlSize += strlen(hiddenValue[i]);
              htmlSize += strlen("\">\n");
            }
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "<form ");
            if (fleft) {
              strcat(htmlString, "style = 'float: left' ");
            }
            strcat(htmlString, "method = \"POST\" action=\"");
            strcat(htmlString, buttonLink);
            strcat(htmlString, "\">\n");
            for (i=0; i<hiddentripletnum; i++) {
              strcat(htmlString, "  ");
              strcat(htmlString, " <input type=\"hidden\" name=\"");
              strcat(htmlString, hiddenName[i]);
              strcat(htmlString, "\" value=\"");
              strcat(htmlString, hiddenValue[i]);
              strcat(htmlString, "\">\n");
            }
            strcat(htmlString, "<input type=\"submit\" value=\"");
            strcat(htmlString, buttonName);
            strcat(htmlString, "\"/>\n");
            strcat(htmlString,"</form>\n");
        } else if ((strcmp(parsedStorage[count+1], "t") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            strcpy(textfield, "\0");
            strcpy(textfilename, "\0");
            booltextfile = false;
            booltextfield = false;
            count += 3;
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "text") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  booltextfield = true;
                  strcat(textfield, parsedStorage[count]);
                }
              } else if (strcmp(parsedStorage[count], "file") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(textfilename, parsedStorage[count]);
                  booltextfile = true;
                }
              }
              count++;
            }
            if (strcmp(textfield, "\0") == 0) {
              strcpy(textfield, "Default Text");
            }
            if (booltextfield) {
              htmlSize += strlen(textfield);
              htmlString = reallocBuffer(htmlString, htmlSize);
              strcat(htmlString, textfield);
            } if (booltextfile) {
              htmlSize += strlen("<?php echo file_get_contents( \"");
              htmlSize += strlen(textfilename);
              htmlSize += strlen("\" );?>\n");
              htmlString = reallocBuffer(htmlString, htmlSize);
              strcat(htmlString, "<?php echo file_get_contents( \"");
              strcat(htmlString, textfilename);
              strcat(htmlString, "\" );?>\n");
            }
        } else if ((strcmp(parsedStorage[count+1], "d") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            while (strcmp(parsedStorage[count], ")") != 0) {
              count++;
            }
            htmlSize = htmlSize+strlen("\n<hr>\n\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "\n<hr>\n\n");
        } else if ((strcmp(parsedStorage[count+1], "e") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            strcpy(executableCommand,"\0");
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "exe") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(executableCommand, parsedStorage[count]);
                }
              }
              count++;
            }
            htmlSize += strlen(executableCommand)+strlen(executableCommand)+strlen(executableCommand);
            htmlSize += strlen("\n<?php\n$cmd = './';\nexec($cmd,$output,$status);\nif($status) {\n  $cmd = './bin';\n  exec($cmd,$output,$status);\n  if ($status) {\n    $cmd = '/bin/';\n    exec($cmd,$output,$status);\n    if ($status) {\n      echo 'Command Failed';\n    } else {\n      foreach($output as $line) {\n        echo $line;\n      }");
            htmlSize += strlen("    }\n  } else {\n    foreach($output as $line) {\n      echo $line;\n    }\n  }\n} else {\n  foreach($output as $line) {\n    echo $line;\n  }\n}\n?>\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "\n<?php\n$cmd = './");
            strcat(htmlString, executableCommand);
            strcat(htmlString, "';\nexec($cmd,$output,$status);\nif($status) {\n  $cmd = './bin");
            strcat(htmlString, executableCommand);
            strcat(htmlString, "';\n  exec($cmd,$output,$status);\n  if ($status) {\n    $cmd = '/bin/");
            strcat(htmlString, executableCommand);
            strcat(htmlString, "';\n    exec($cmd,$output,$status);\n    if ($status) {\n      echo 'Command Failed';\n    } else {\n      foreach($output as $line) {\n        echo $line;\n      }");
            strcat(htmlString, "    }\n  } else {\n    foreach($output as $line) {\n      echo $line;\n    }\n  }\n} else {\n  foreach($output as $line) {\n    echo $line;\n  }\n}\n?>\n");
        } else if ((strcmp(parsedStorage[count+1], "i") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            strcpy(inputAction, "\0");
            tripletnum = 0;
            hiddentripletnum = 0;
            for (i=0; i<50; i++) {
            	strcpy(inputText[i], "\0");
            	strcpy(inputValue[i], "\0");
            	strcpy(inputName[i], "\0");
              strcpy(hiddenValue[i], "\0");
              strcpy(hiddenName[i], "\0");
            }
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "action") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(inputAction, parsedStorage[count]);
                }
              } else if (strcmp(parsedStorage[count], "text") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(inputText[tripletnum], parsedStorage[count]);
                }
                while (strcmp(parsedStorage[count], "name") != 0) {
                  count++;
                }
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(inputName[tripletnum], parsedStorage[count]);
                }
                while (strcmp(parsedStorage[count], "value") != 0) {
                  count++;
                }
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(inputValue[tripletnum], parsedStorage[count]);
                }
                tripletnum++;
              } else if (strcmp(parsedStorage[count], "hide") == 0) {
                while (strcmp(parsedStorage[count], "name") != 0) {
                  count++;
                }
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(hiddenName[hiddentripletnum], parsedStorage[count]);
                }
                while (strcmp(parsedStorage[count], "value") != 0) {
                  count++;
                }
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(hiddenValue[hiddentripletnum], parsedStorage[count]);
                }
                hiddentripletnum++;
              }
              count++;
            }
            htmlSize += strlen("<form method=\"Post\" action=\"")+strlen(inputAction)+strlen("\">\n");
            for (i=0; i<tripletnum; i++) {
            	htmlSize+=strlen("  ")+strlen(inputText[i])+strlen(" <input type=\"text\" name=\"")+strlen(inputName[i])+strlen("\" value=\"")+strlen(inputValue[i])+strlen("\"><br>\n");
            }
            for (i=0; i<hiddentripletnum; i++) {
              htmlSize+=strlen("  ")+strlen(" <input type=\"hidden\" name=\"")+strlen(hiddenName[i])+strlen("\" value=\"")+strlen(hiddenValue[i])+strlen("\"><br>\n");
            }
            htmlSize+=strlen("  <input type=\"submit\" value=\"Submit\">\n")+strlen("</form>\n\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
           	strcat(htmlString, "<form method=\"Post\" action=\"");
           	strcat(htmlString, inputAction);
           	strcat(htmlString, "\">\n");
           	for (i=0; i<tripletnum; i++) {
           		strcat(htmlString, "  ");
           		strcat(htmlString, inputText[i]);
           		strcat(htmlString, " <input type=\"text\" name=\"");
           		strcat(htmlString, inputName[i]);
           		strcat(htmlString, "\" value=\"");
           		strcat(htmlString, inputValue[i]);
           		strcat(htmlString, "\"><br>\n");
           	}
            for (i=0; i<hiddentripletnum; i++) {
              strcat(htmlString, "  ");
              strcat(htmlString, " <input type=\"hidden\" name=\"");
              strcat(htmlString, hiddenName[i]);
              strcat(htmlString, "\" value=\"");
              strcat(htmlString, hiddenValue[i]);
              strcat(htmlString, "\">\n");
            }
           	strcat(htmlString, "  <input type=\"submit\" value=\"Submit\">\n</form>\n\n");
        /* Post Program Page Flag */
        } else if ((strcmp(parsedStorage[count+1], "u") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            strcpy(inputAction, "\0");
            tripletnum = 0;
            hiddentripletnum = 0;
            for (i=0; i<50; i++) {
              strcpy(inputText[i], "\0");
              strcpy(inputrows[i], "\0");
              strcpy(inputcols[i], "\0");
              strcpy(inputName[i], "\0");
              strcpy(hiddenValue[i], "\0");
              strcpy(hiddenName[i], "\0");
            }
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "action") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(inputAction, parsedStorage[count]);
                }
              } else if (strcmp(parsedStorage[count], "text") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(inputText[tripletnum], parsedStorage[count]);
                }
                while (strcmp(parsedStorage[count], "name") != 0) {
                  count++;
                }
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(inputName[tripletnum], parsedStorage[count]);
                }
                while (strcmp(parsedStorage[count], "rows") != 0) {
                  count++;
                }
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(inputrows[tripletnum], parsedStorage[count]);
                }
                while (strcmp(parsedStorage[count], "cols") != 0) {
                  count++;
                }
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(inputcols[tripletnum], parsedStorage[count]);
                }
                tripletnum++;
              } else if (strcmp(parsedStorage[count], "hide") == 0) {
                while (strcmp(parsedStorage[count], "name") != 0) {
                  count++;
                }
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(hiddenName[hiddentripletnum], parsedStorage[count]);
                }
                while (strcmp(parsedStorage[count], "value") != 0) {
                  count++;
                }
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(hiddenValue[hiddentripletnum], parsedStorage[count]);
                }
                hiddentripletnum++;
              }
              count++;
            }
            htmlSize += strlen("<form method=\"Post\" action=\"")+strlen(inputAction)+strlen("\">\n");
            for (i=0; i<tripletnum; i++) {
              htmlSize+=strlen("  ")+strlen(inputText[i])+strlen(" <input type=\"text\" name=\"")+strlen(inputName[i])+strlen("\" rows=\"")+strlen(inputrows[i])+strlen("\" cols=\"")+strlen(inputcols[i])+strlen("\"></input><br>\n");
            }
            for (i=0; i<hiddentripletnum; i++) {
              htmlSize+=strlen("  ")+strlen(" <input type=\"hidden\" name=\"")+strlen(hiddenName[i])+strlen("\" value=\"")+strlen(hiddenValue[i])+strlen("\"><br>\n");
            }
            htmlSize+=strlen("  <input type=\"submit\" value=\"Submit\">\n")+strlen("</form>\n\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "<form method=\"Post\" action=\"");
            strcat(htmlString, inputAction);
            strcat(htmlString, "\">\n");
            for (i=0; i<tripletnum; i++) {
              strcat(htmlString, "  ");
              strcat(htmlString, inputText[i]);
              strcat(htmlString, " <textarea type=\"text\" name=\"");
              strcat(htmlString, inputName[i]);
              strcat(htmlString, "\" rows=\"");
              strcat(htmlString, inputrows[i]);
              strcat(htmlString, "\" cols=\"");
              strcat(htmlString, inputcols[i]);
              strcat(htmlString, "\"></textarea><br>\n");
            }
            for (i=0; i<hiddentripletnum; i++) {
              strcat(htmlString, "  ");
              strcat(htmlString, " <input type=\"hidden\" name=\"");
              strcat(htmlString, hiddenName[i]);
              strcat(htmlString, "\" value=\"");
              strcat(htmlString, hiddenValue[i]);
              strcat(htmlString, "\">\n");
            }
            strcat(htmlString, "  <input type=\"submit\" value=\"Submit\">\n</form>\n\n");
        } else if ((strcmp(parsedStorage[count+1], "l") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            strcpy(linkText,"\0");
            strcpy(linkLink,"\0");
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "text") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(linkText, parsedStorage[count]);
                }
              } else if (strcmp(parsedStorage[count], "link") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(linkLink, parsedStorage[count]);
                }
              }
              count++;
            }
            htmlSize += strlen("<a href=\"")+strlen(linkLink)+strlen("> ")+strlen(linkText)+strlen("\"> </a>\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "<a href=\"");
            strcat(htmlString, linkLink);
            strcat(htmlString, "> ");
            strcat(htmlString, linkText);
            strcat(htmlString, "\"> </a>\n");
        } else if ((strcmp(parsedStorage[count+1], "p") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            strcpy(imgWidth, "\0");
            strcpy(imgHeight, "\0");
            strcpy(imageLink, "\0");
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "image") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(imageLink, parsedStorage[count]);
                }
              } else if (strcmp(parsedStorage[count], "size") == 0) {
                while (strcmp(parsedStorage[count], "=") != 0) {
                  count++;
                }
              	count++;
              	theToken = strtok(parsedStorage[count], "x");
                strcat(imgWidth, theToken);
                theToken = strtok(NULL, "\0");
                strcat(imgHeight, theToken);
                break;
                count++;
	            }
              count++;
            }
            if (strcmp(imgWidth, "\0") == 0) {
              strcpy(imgWidth, "100");
            }
            if (strcmp(imgHeight, "\0") == 0) {
              strcpy(imgHeight, "100");
            }
            htmlSize += strlen("<img src=\"")+strlen(imageLink)+strlen("\" width=\"")+strlen(imgWidth)+strlen("\" height=\"")+strlen(imgHeight)+strlen("\">\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "<img src=\"");
            strcat(htmlString, imageLink);
            strcat(htmlString, "\" width=\"");
            strcat(htmlString, imgWidth);
            strcat(htmlString, "\" height=\"");
            strcat(htmlString, imgHeight);
            strcat(htmlString, "\">\n");
        } else if ((strcmp(parsedStorage[count+1], "r") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            strcpy(radioFilename, "\0");
            strcpy(radioName, "\0");
            for (i=0; i<50; i++) {
            	strcpy(radioValue[i], "\0");
            }
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "action") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(radioFilename, parsedStorage[count]);
                }
              } else if (strcmp(parsedStorage[count], "name") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(radioName, parsedStorage[count]);
                }
              } else if (strcmp(parsedStorage[count], "value") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(radioValue[rvalueCount], parsedStorage[count]);
                  rvalueCount++;
                }
              }
              count++;
            }
	          htmlSize+=strlen("\n<form method=\"Post\" action=\"")+strlen(radioFilename)+strlen("\">\n");
	          for (i=0; i<rvalueCount; i++) {
	          	htmlSize+=strlen("  <input type=\"radio\" name=\"\" value=\"\"/> <br>\n ")+strlen(radioName)+strlen(radioValue[i])+strlen(radioValue[i]);
	          }
	          htmlSize+=strlen("  <input type=\"submit\" value=\"Submit\">");
	          htmlSize+=strlen("</form>\n\n")+strlen(" checked");
	          htmlString = reallocBuffer(htmlString, htmlSize);
	          strcat(htmlString, "\n<form method=\"Post\" action=\"");
	          strcat(htmlString, radioFilename);
	          strcat(htmlString, "\">\n");
	          for (i=0; i<rvalueCount; i++) {
	          	strcat(htmlString, "  <input type=\"radio\" name=\"");
	          	strcat(htmlString, radioName);
	          	strcat(htmlString, "\" value=\"");
	          	strcat(htmlString, radioValue[i]);
	          	if (i == 0) strcat(htmlString, "\" checked/> ");
	          	else strcat(htmlString, "\"/> ");
	          	strcat(htmlString, radioValue[i]);
	          	strcat(htmlString, "<br>\n");
	          }
	          strcat(htmlString, "  <input type=\"submit\" value=\"Submit\">");
	          strcat(htmlString, "</form>\n\n");
        /*Style custom tag*/
        } else if ((strcmp(parsedStorage[count+1], "s") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            strcpy(element,"\0");
            strcpy(elementValue,"\0");
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "element") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(element, parsedStorage[count]);
                }
              } else if (strcmp(parsedStorage[count], "elementValue") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(elementValue, parsedStorage[count]);
                }
              }
              count++;
            }
            htmlSize += strlen("<style> ")+strlen(element)+strlen(" {")+strlen(elementValue)+strlen("; }\n</style>\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "<style> ");
            strcat(htmlString, element);
            strcat(htmlString, " {");
            strcat(htmlString, elementValue);
            strcat(htmlString, "; }\n</style>\n");
        /* Add Author custom tag*/
        } else if ((strcmp(parsedStorage[count+1], "a") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            strcpy(cmd, "\0");
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "cmd") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                while (strcmp(parsedStorage[count], "END") != 0) {
                  if (strcmp(parsedStorage[count], ",") == 0) {
                    strcat(cmd, ".");
                  } else if (strcmp(parsedStorage[count], "\"") == 0) {
                    strcat(cmd, " ");
                  } else {
                    strcat(cmd, parsedStorage[count]);
                  }
                  count++;
                }
              }
              count++;
            }
            htmlSize += strlen("unset($output);\nunset($cmd);\n$cmd =")+strlen(cmd)+strlen(";\nexec($cmd, $output, $status);\nif ($status) {\n  echo $status;\n  echo \"exec failed\";\n} else {\n  foreach($output as $line)\n    echo $line;\n}\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "unset($output);\nunset($cmd);\n$cmd =");
            strcat(htmlString, cmd);
            strcat(htmlString, ";\nexec($cmd, $output, $status);\nif ($status) {\n  echo $status;\n  echo \"exec failed\";\n} else {\n  foreach($output as $line)\n    echo $line;\n}\n");
        /* Python Find Streams Program Custom Tag */
        } else if ((strcmp(parsedStorage[count+1], "y") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            strcpy(cmd, "\0");
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "cmd") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                while (strcmp(parsedStorage[count], "END") != 0) {
                  if (strcmp(parsedStorage[count], ",") == 0) {
                    strcat(cmd, ".");
                  } else if (strcmp(parsedStorage[count], "\"") == 0) {
                    strcat(cmd, " ");
                  } else {
                    strcat(cmd, parsedStorage[count]);
                  }
                  count++;
                }
              }
              count++;
            }
            htmlSize += strlen("unset($output);\nunset($cmd);\n$cmd =")+strlen(cmd)+strlen(";\nexec($cmd, $output, $status);\nif ($status) {\n  echo $status;\n  echo \"exec failed\";\n} else {\n  foreach($line as $line)\n    echo $line;\n}\nif ($line != 'The user is not included in any streams... Please Try Again' && $line != 'There are currently no streams in the Database...Please return to the previous page' && $line != 'You need to specify an author... Please Try Again') {\n");
            htmlSize += strlen("echo <<<_END\n<input type=\"radio\" name=\"stream\" value=\"all\" />all<br>\n<input type=\"hidden\" name=\"username\" value=\"$uname\">\n<input type=\"hidden\" name=\"flag\" value=\"st@rt\">\n<input type=\"submit\" value=\"Choose Stream\"></form>\n_END;\n}\n\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "unset($output);\nunset($cmd);\n$cmd =");
            strcat(htmlString, cmd);
            strcat(htmlString, ";\nexec($cmd, $output, $status);\nif ($status) {\n  echo $status;\n  echo \"exec failed\";\n} else {\n  foreach($output as $line)\n    echo $line;\n}\nif ($line != 'The user is not included in any streams... Please Try Again' && $line != 'There are currently no streams in the Database...Please return to the previous page' && $line != 'You need to specify an author... Please Try Again') {\n");
            strcat(htmlString, "echo <<<_END\n<input type=\"radio\" name=\"stream\" value=\"all\" />all<br>\n<input type=\"hidden\" name=\"username\" value=\"$uname\">\n<input type=\"hidden\" name=\"flag\" value=\"st@rt\">\n<input type=\"submit\" value=\"Choose Stream\"></form>\n_END;\n}\n\n");
        /* Opening PHP */
        } else if ((strcmp(parsedStorage[count+1], "o") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            while (strcmp(parsedStorage[count], ")") != 0) {
              count++;
            }
            htmlSize = htmlSize+strlen("\n<?php\n\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "\n<?php\n\n");
        /* Closing PHP */
        } else if ((strcmp(parsedStorage[count+1], "c") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            while (strcmp(parsedStorage[count], ")") != 0) {
              count++;
            }
            htmlSize = htmlSize+strlen("\n?><br>\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "\n?><br>\n");
        /* Tag for adding hidden variables to end of open form and closing it */
        } else if ((strcmp(parsedStorage[count+1], "k") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            while (strcmp(parsedStorage[count], ")") != 0) {
              count++;
            }
            htmlSize = htmlSize+strlen("\necho <<<_END\n\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "\necho <<<_END\n\n");
        } else if ((strcmp(parsedStorage[count+1], "w") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            while (strcmp(parsedStorage[count], ")") != 0) {
              count++;
            }
            htmlSize = htmlSize+strlen("\n_END;\n\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "\n_END;\n\n");
        /* Custom tag for variables */
        } else if ((strcmp(parsedStorage[count+1], "f") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            strcpy(varname,"\0");
            strcpy(varvalue,"\0");
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "varname") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(varname, parsedStorage[count]);
                }
              } else if (strcmp(parsedStorage[count], "varvalue") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(varvalue, parsedStorage[count]);
                }
              }
              count++;
            }
            htmlSize += strlen(varname)+strlen(" = ")+strlen(varvalue)+strlen(";\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, varname);
            strcat(htmlString, " = ");
            strcat(htmlString, varvalue);
            strcat(htmlString, ";\n");
            /* Special case output a header tag from php using arguments */
        } else if ((strcmp(parsedStorage[count+1], "j") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            strcpy(hsize,"\0");
            hargsize = 0;
            for (i=0; i<50; i++) {
              strcpy(harg[i], "\0");
            }
            strcpy(elementValue,"\0");
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "size") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(hsize, parsedStorage[count]);
                }
              } else if (strcmp(parsedStorage[count], "arg") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                if (strcmp(parsedStorage[count], "\"") != 0) {
                  strcat(harg[hargsize], parsedStorage[count]);
                  hargsize++;
                }
              }
              count++;
            }
            if (strcmp(hsize, "\0") == 0) {
              strcpy(hsize, "3");
            }
            htmlSize = htmlSize + strlen("echo \"<h>\" .     ") + strlen(hsize) + strlen(hsize);
            for (i=0; i<hargsize; i++) {
              htmlSize += strlen(harg[i]);
              htmlSize += strlen(".");
            }
            htmlSize += strlen(". \"</h\"\n\n") + strlen("\";");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "echo \"<h");
            strcat(htmlString, hsize);
            strcat(htmlString, ">\" . ");
            for (i=0; i<hargsize; i++) {
              if (i!=0) strcat(htmlString, ".");
              if (harg[i][0] != '$') strcat(htmlString, "\"");
              strcat(htmlString, harg[i]);
              if (harg[i][0] != '$') strcat(htmlString, "\"");
            }
            strcat(htmlString, ". \"</h");
            strcat(htmlString, hsize);
            strcat(htmlString, ">");
            strcat(htmlString, "\";\n\n");
        /*Post Output Custom Tag*/
        } else if ((strcmp(parsedStorage[count+1], "m") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            strcpy(cmd, "\0");
            while (strcmp(parsedStorage[count], ")") != 0) {
              if (strcmp(parsedStorage[count], "cmd") == 0) {
                while (strcmp(parsedStorage[count], "\"") != 0) {
                  count++;
                }
                count++;
                while (strcmp(parsedStorage[count], "END") != 0) {
                  if (strcmp(parsedStorage[count], ",") == 0) {
                    strcat(cmd, ".");
                  } else if (strcmp(parsedStorage[count], "\"") == 0) {
                    strcat(cmd, " ");
                  } else {
                    strcat(cmd, parsedStorage[count]);
                  }
                  count++;
                }
              }
              count++;
            }
            htmlSize += strlen("unset($output);\nunset($cmd);\n$cmd =")+strlen(cmd)+strlen(";\nexec($cmd, $output, $status);\nif ($status) {\n  echo \"Exec Error \";\necho $status;\necho \"<br>\";\necho \"Unfortunately the Post was not able to be submitted. A common reason for this would be an invalid character<br>\";\necho \"copied from another website such as Wikipedia. Note the Readme for acceptable strings.\";\n} else {\n  foreach($output as $line)\n    echo $line;\n}\necho\"<br><br>\";\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "unset($output);\nunset($cmd);\n$cmd =");
            strcat(htmlString, cmd);
            strcat(htmlString, ";\nexec($cmd, $output, $status);\nif ($status) {\n  echo \"Exec Error \";\necho $status;\necho \"<br>\";\necho \"Unfortunately the Post was not able to be submitted. A common reason for this would be an invalid character<br>\";\necho \"copied from another website such as Wikipedia. Note the Readme for acceptable strings.\";\n} else {\n  foreach($output as $line)\n    echo $line;\n}\necho\"<br><br>\";\n");
        } else if ((strcmp(parsedStorage[count+1], "g") == 0) && (strcmp(parsedStorage[count+2], "(") == 0)) {
            count += 3;
            while (strcmp(parsedStorage[count], ")") != 0) {
              count++;
            }
            htmlSize = htmlSize+strlen("\n  function mynl2br($text) { \n     return strtr($text, array(\"\\r\\n\" => ' -5G5 ', \"\\r\" => ' -5G5 ', \"\\n\" => ' -5G5 ')); \n}\n\n");
            htmlString = reallocBuffer(htmlString, htmlSize);
            strcat(htmlString, "\n  function mynl2br($text) { \n     return strtr($text, array(\"\\r\\n\" => ' -5G5 ', \"\\r\" => ' -5G5 ', \"\\n\" => ' -5G5 ')); \n}\n\n");
        } else {
          htmlSize += strlen(parsedStorage[count]);
          htmlString = reallocBuffer(htmlString, htmlSize);
          strcat(htmlString, parsedStorage[count]);
        }
      } else {
        printf("File Too Short\n");
      }
    } else {
      htmlSize += strlen(parsedStorage[count]);
      htmlString = reallocBuffer(htmlString, htmlSize);
      strcat(htmlString, parsedStorage[count]);
    }
		count++;
	}
  htmlSize += strlen("\n</BODY>\n</HTML>");
  htmlString = reallocBuffer(htmlString, htmlSize);
  strcat(htmlString, "\n</BODY>\n</HTML>");

  printf("%s\n", htmlString);
  /*fprintf(outputFile, "%s\n", htmlString);*/
  free(htmlString);
}
int main(int argc, char *argv[]) {
	char ** parsedStorage;
	int parsedSize;
	int i;

  if (argc < 2) {
    printf("You Must Specify a File Name... Exiting\n");
    return -1;
  }
  parsedStorage = Parser(argc, argv, &parsedSize);
  if (parsedStorage == NULL) {
  	return -1;
  }

  /*for(i=0; i<parsedSize; i++) {
  	printf("%s\n", parsedStorage[i]);
  }*/

  Conv2HTML(parsedStorage, parsedSize, argv[1]);

  /*Various Frees*/
  for(i=0; i<parsedSize; i++) {
    free(parsedStorage[i]);
  }
  free(parsedStorage);
  return 0;

}
