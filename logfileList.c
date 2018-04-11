//dependencies
#include "logfileList.h"



//add node onto tail of list allways add to end of list
struct LogFileNode *addLogFileNode( struct LogFileNode *tailNode,
                                     char * newLogFileStr )
   {


    // check for viable node
    if( tailNode != NULL )
      {

        tailNode->next = addLogFileNode( tailNode->next, newLogFileStr );


        return tailNode;
       }

    // assume empty tail node
    tailNode = makeLogFileNode( newLogFileStr);

    return tailNode;
   }

// function eliminates redundant code in addNode
struct LogFileNode *makeLogFileNode(char * newLogFileStr)
   {
    struct LogFileNode *newNode
                   = ( struct LogFileNode * )
                      malloc( sizeof( struct LogFileNode ) );


    strcpy(newNode->logStr,newLogFileStr);

    newNode->next = NULL;


    return newNode;
   }


struct LogFileNode *clearLogFileList( struct LogFileNode *headNode )
   {
    if( headNode == NULL )
       {
        return NULL;
       }

    if( headNode->next != NULL )
       {
        clearLogFileList( headNode->next );
       }


    free( headNode );

    headNode = NULL;

    return headNode;
   }
