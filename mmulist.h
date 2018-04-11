//singly linked list of strings
//header files
#include "pmdlist.h"
#include <stdio.h>  // I/O
#include <stdlib.h>  // malloc
#include <string.h>  // string management

struct MMUControllerNode
  {

    int processID;
    int segmentIndex;
    int memBase;
    int memTotal;

    struct MMUControllerNode *next;

  };



struct MMUControllerNode *addMMUControllerNode( struct MMUControllerNode *tailNode,struct MMUControllerNode *newNode );
struct MMUControllerNode *clearMMUControllerList( struct MMUControllerNode *headNode );
struct MMUControllerNode *makeMMUControllerNode(struct MMUControllerNode *oldNodePtr);
