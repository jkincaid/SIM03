//header files
#include "pmdlist.h"
#include <stdio.h>  // I/O
#include <stdlib.h>  // malloc
#include <string.h>  // string management

struct ProgramListNode
  {
    int processID;
    int totalNodes;
    int approxTotalProgramTime_mSec;
    int totalProcessCycles;
    int totalIOcycles;
    int totalMemCycles;

    struct ProgramMetaDataNode *programHead;

    struct ProgramListNode *next;

  };


int getProgramMetaDataListLength(struct ProgramMetaDataNode *head, int *processCyclesCount, int *ioCyclesCount,int *memoryCyclesCount);

struct ProgramListNode *addProgramNode( struct ProgramListNode *headNode, struct ProgramListNode *newNode );
struct ProgramListNode *clearProgramList( struct ProgramListNode *headNode );
struct ProgramListNode *returnShortestProgram(struct ProgramListNode *headNode);
struct ProgramListNode *removeProgramNodeByProcessID( struct ProgramListNode *headNode, int processID);
struct ProgramListNode *makeProgramNode(struct ProgramMetaDataNode *programHead);
