//dependencies
#include "mmulist.h"



//add node onto tail of list allways add to end of list
struct MMUControllerNode *addMMUControllerNode( struct MMUControllerNode *tailNode,
                                     struct MMUControllerNode *newNode )
   {


    // check for viable node
    if( tailNode != NULL )
      {

        tailNode->next = addMMUControllerNode( tailNode->next, newNode );


        return tailNode;
       }

    // assume empty tail node
    tailNode = makeMMUControllerNode( newNode);

    return tailNode;
   }

// function eliminates redundant code in addNode
struct MMUControllerNode *makeMMUControllerNode(struct MMUControllerNode *oldNodePtr)
   {
    struct MMUControllerNode *newNode
                   = ( struct MMUControllerNode * )
                      malloc( sizeof( struct MMUControllerNode ) );


    newNode->processID = oldNodePtr->processID;
    newNode->segmentIndex = oldNodePtr->segmentIndex;
    newNode->memBase = oldNodePtr->memBase;
    newNode->memTotal = oldNodePtr->memTotal;

    newNode->next = NULL;


    return newNode;
   }


struct MMUControllerNode *clearMMUControllerList( struct MMUControllerNode *headNode )
   {
    if( headNode == NULL )
       {
        return NULL;
       }

    if( headNode->next != NULL )
       {
        clearMMUControllerList( headNode->next );
       }


    free( headNode );

    headNode = NULL;

    return headNode;
   }
