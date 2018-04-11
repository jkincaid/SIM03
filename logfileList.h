//singly linked list of strings
//header files
#include "pmdlist.h"
#include <stdio.h>  // I/O
#include <stdlib.h>  // malloc
#include <string.h>  // string management

struct LogFileNode
  {

    char logStr[120];

    struct LogFileNode *next;

  };



struct LogFileNode *addLogFileNode( struct LogFileNode *headNode, char * newLogFileStr);
struct LogFileNode *clearLogFileList( struct LogFileNode *headNode );
struct LogFileNode *makeLogFileNode( char * newLogFileStr);
