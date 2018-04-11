
#include "programlist.h"


int getProgramMetaDataListLength(struct ProgramMetaDataNode *head,int *processCyclesCount, int *ioCyclesCount,int *memoryCyclesCount)
  {
    int totalNodes = 0;

    struct ProgramMetaDataNode *tmpPtr = head;

    //printf("\ngetProgramMetaDataListLength:\n" );
    while( tmpPtr -> next != NULL)
    {
      char commandChar = tmpPtr->commandLetter;
      //printf("\nchecking for cycles %c %s\n", commandChar, tmpPtr->operationString);
      if( commandChar == 'I' || commandChar == 'O') *ioCyclesCount += tmpPtr->cycleTime;
      else if (commandChar == 'P') *processCyclesCount += tmpPtr->cycleTime;
      else if (commandChar == 'M') *memoryCyclesCount += tmpPtr->cycleTime;
      else if (commandChar == 'A' && !strcmp(tmpPtr->operationString,"end"))
      {
        //printf("\n total nodes: %d\n", totalNodes+1);
        return ++totalNodes;
      }


      tmpPtr = tmpPtr -> next;
      totalNodes++;

    }

    return totalNodes;

  }

//add node onto tail of list allways add to end of list
struct ProgramListNode *addProgramNode( struct ProgramListNode *tailNode,
                                     struct ProgramListNode *newNode )
   {


    // check for viable node
    if( tailNode != NULL )
      {

        tailNode->next = addProgramNode( tailNode->next, newNode );


        return tailNode;
       }

    // assume empty tail node
    tailNode = makeProgramNode( newNode->programHead);
    tailNode->processID = newNode->processID;
    tailNode->approxTotalProgramTime_mSec = newNode->approxTotalProgramTime_mSec;

    return tailNode;
   }

// function eliminates redundant code in addNode
struct ProgramListNode *makeProgramNode(struct ProgramMetaDataNode *programHead)
   {
    struct ProgramListNode *newNode
                   = ( struct ProgramListNode * )
                      malloc( sizeof( struct ProgramListNode ) );



    int  pCycles = 0;
    int  ioCycles = 0;
    int  memCycles = 0;


    newNode->totalNodes = getProgramMetaDataListLength(programHead,&pCycles,&ioCycles,&memCycles);
    newNode->approxTotalProgramTime_mSec = 0;
    newNode->totalProcessCycles = pCycles;
    newNode->totalIOcycles = ioCycles;
    newNode->totalMemCycles = memCycles;
    newNode->programHead = programHead;
    newNode->next = NULL;


    return newNode;
   }

struct ProgramListNode *returnShortestProgram(struct ProgramListNode *headNode)
{
  struct ProgramListNode *tmpPtr = headNode;
  struct ProgramListNode *tmpShortestPtr = ( struct ProgramListNode * )
     malloc( sizeof( struct ProgramListNode ) );


  tmpShortestPtr = headNode;

  while( tmpPtr != NULL)
  {

    if( tmpPtr->approxTotalProgramTime_mSec <= tmpShortestPtr->approxTotalProgramTime_mSec)
    {
        tmpShortestPtr = tmpPtr;
    }


    tmpPtr = tmpPtr -> next;


  }

  return tmpShortestPtr;

}

//returns the head of the list with item removed
struct ProgramListNode *removeProgramNodeByProcessID( struct ProgramListNode *headNode, int processID)
  {

    struct ProgramListNode *tmpPtr = headNode;
    struct ProgramListNode *prevPtr = headNode;

    while( tmpPtr != NULL)
    {

      if( tmpPtr->processID == processID)
      {
        if(tmpPtr == headNode)
        {
          if(tmpPtr->next != NULL)
           tmpPtr = tmpPtr->next;
          else
          {
            tmpPtr = NULL;
          }
          //free(headNode);
          return tmpPtr;
        }
        else
        {
          prevPtr->next = tmpPtr->next;
          //free(tmpPtr);
          return headNode;

        }
      }
      prevPtr = tmpPtr;
      tmpPtr = tmpPtr -> next;


    }

    return headNode;
  }


struct ProgramListNode *clearProgramList( struct ProgramListNode *headNode )
   {
    if( headNode == NULL )
       {
        return NULL;
       }

    if( headNode->next != NULL )
       {
        clearProgramList( headNode->next );
       }

    free( headNode );

    headNode = NULL;

    return headNode;
   }
