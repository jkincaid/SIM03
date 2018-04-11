
#include "configparse.h"
#include "pmdlist.h"
#include "programlist.h"
#include "simtimer.h"
#include "logfileList.h"
#include "mmulist.h"
#include <pthread.h>


const int STD_STR_LEN = 60;
const int MAX_STR_LEN = 120;
const char SPACE = ' ';

void *callRunTimer(void *timeUsec)
{

  //printf("\ncallRunTimer init for time : %d\n", *(int*)timeUsec);
  runTimer(*(int*)timeUsec);
  pthread_exit(NULL);
}
int blockedtimerThread(int timeInMilliSeconds)
{
  int returnCode =0;
  pthread_t timerRunThread[1];
  void * status;

  returnCode = pthread_create(&timerRunThread[0],NULL,callRunTimer, (void *)&timeInMilliSeconds);

  if(returnCode)
  {
    printf("ERROR, return code from pthread_create() is %d\n", returnCode);

  }
  else
  {
    returnCode = pthread_join(timerRunThread[0], &status);

  }


  return returnCode;

}

void doLogChoice(struct LogFileNode * head, char* logStr,int logChoice){

  if(!logChoice){

    printf("%s", logStr);

  }
  else
  {
    head = addLogFileNode(head, logStr);
    if(logChoice == 2) printf("%s", logStr);

  }

}

int testConfigParse(char *configFilePath){

  int resultCode;
  struct ConfigFile *tempConfigFile  = (struct ConfigFile *)malloc( sizeof( struct ConfigFile ) );

  //one function to rule them all
  resultCode = getConfigDataFromFile(tempConfigFile,configFilePath);

  const char *logFileDefinitionMain[] =
  {
    "Start Simulator Configuration File",
    "Version/Phase:",
    "File Path:",
    "CPU Scheduling Code:",
    "Quantum Time (cycles):",
    "Memory Available (KB):",
    "Processor Cycle Time (msec):",
    "I/O Cycle Time (msec):",
    "Log To:",
    "Log File Path:",
    "End Simulator Configuration File."
  };

  if(resultCode == NO_ERROR_MSG || resultCode == END_FILE_FOUND_MSG){
    int displayCount=0;

    printf("\n%s                    \n", logFileDefinitionMain[displayCount++]);
    printf("%-30s                    : %i\n", logFileDefinitionMain[displayCount++], tempConfigFile->version);
    printf("%-30s                    : %s\n", logFileDefinitionMain[displayCount++], tempConfigFile->filePath);
    printf("%-30s                    : %i\n", logFileDefinitionMain[displayCount++], tempConfigFile->cpuScheduleCode);
    printf("%-30s                    : %i\n", logFileDefinitionMain[displayCount++], tempConfigFile->quantumTime);
    printf("%-30s                    : %i\n", logFileDefinitionMain[displayCount++], tempConfigFile->memoryAvail);
    printf("%-30s                    : %i\n", logFileDefinitionMain[displayCount++], tempConfigFile->processorCycleTime_msec);
    printf("%-30s                    : %i\n", logFileDefinitionMain[displayCount++], tempConfigFile->ioCycleTime_msec);
    printf("%-30s                    : %s\n", logFileDefinitionMain[displayCount++], tempConfigFile->logTo);
    printf("%-30s                    : %s\n", logFileDefinitionMain[displayCount++], tempConfigFile->logFilePath);
    printf("%-30s                 \n", logFileDefinitionMain[displayCount++]);



  }
  else
  {
    printf("\nERROR MESSAGE RECIEVED CONFIG PARSE : %d --> %s\n", resultCode, configFilePath);
    free(tempConfigFile);
    return resultCode;
  }

  free(tempConfigFile);

  return NO_ERROR_MSG;


}

int testMetaDataParse(char *metaDataFilePath)
{
  struct ProgramMetaDataNode *workingPtr, *headPtr = NULL;

  int resultCode;

  //one function to rule them all
  resultCode = getDataFromFile(&headPtr,metaDataFilePath);

  //printf("\nERROR CODE FROM MDFP %d --> %s\n", resultCode, metaDataFilePath);
  if(!resultCode)
  {
    workingPtr = headPtr;

    while( workingPtr != NULL )
       {
        printf( "\nThe data item component letter is: %c\n"\
                  "The data item operation string is: %s\n"\
                  "The data item cycle time is      : %i\n",
                        workingPtr->commandLetter, workingPtr->operationString, workingPtr->cycleTime );

        workingPtr = workingPtr->next;
       }

  }
  else
  {
    printf("ERROR MESSAGE RECIEVED METADATAPARSE: %d --> %s\n", resultCode, metaDataFilePath);
    headPtr = clearList(headPtr);
    free(headPtr);
    return resultCode;
  }

  headPtr = clearList(headPtr);
  free(workingPtr);
  free(headPtr);
  return NO_ERROR_MSG;

}

int testConfigParseAndMetaDataFile(char *configFilePath){

  int resultCode;
  struct ConfigFile *tempConfigFile
                    = (struct ConfigFile *)malloc( sizeof( struct ConfigFile ) );
  char timeString[STD_STR_LEN];
  accessTimer(ZERO_TIMER,timeString);
  printf("TIME: %s, System Start", timeString);
  //one function to rule them all
  resultCode = getConfigDataFromFile(tempConfigFile,configFilePath);

  const char *logFileDefinitionMain[] =
  {
    "Start Simulator Configuration File",
    "Version/Phase:",
    "File Path:",
    "CPU Scheduling Code:",
    "Quantum Time (cycles):",
    "Memory Available (KB):",
    "Processor Cycle Time (msec):",
    "I/O Cycle Time (msec):",
    "Log To:",
    "Log File Path:",
    "End Simulator Configuration File."
  };

  if(resultCode == NO_ERROR_MSG || resultCode == END_FILE_FOUND_MSG){
    int displayCount=0;

    printf("\n%s                    \n", logFileDefinitionMain[displayCount++]);
    printf("%-30s                    : %i\n", logFileDefinitionMain[displayCount++], tempConfigFile->version);
    printf("%-30s                    : %s\n", logFileDefinitionMain[displayCount++], tempConfigFile->filePath);
    printf("%-30s                    : %i\n", logFileDefinitionMain[displayCount++], tempConfigFile->cpuScheduleCode);
    printf("%-30s                    : %i\n", logFileDefinitionMain[displayCount++], tempConfigFile->quantumTime);
    printf("%-30s                    : %i\n", logFileDefinitionMain[displayCount++], tempConfigFile->memoryAvail);
    printf("%-30s                    : %i\n", logFileDefinitionMain[displayCount++], tempConfigFile->processorCycleTime_msec);
    printf("%-30s                    : %i\n", logFileDefinitionMain[displayCount++], tempConfigFile->ioCycleTime_msec);
    printf("%-30s                    : %s\n", logFileDefinitionMain[displayCount++], tempConfigFile->logTo);
    printf("%-30s                    : %s\n", logFileDefinitionMain[displayCount++], tempConfigFile->logFilePath);
    printf("%-30s                 \n", logFileDefinitionMain[displayCount++]);


    accessTimer(LAP_TIMER,timeString);
    printf("TIME: %s, 0S: LOADING PROGRAMS", timeString);

    resultCode = testMetaDataParse(tempConfigFile->filePath);


    printf("\nERROR MESSAGE RECIEVED: %d --> %s\n", resultCode, tempConfigFile->filePath);


  }
  else
  {
    printf("\nERROR MESSAGE RECIEVED CONFIG PARSE : %d --> %s\n", resultCode, tempConfigFile->filePath);
    free(tempConfigFile);
    return resultCode;
  }

  free(tempConfigFile);

  return NO_ERROR_MSG;


}

int doSJFN(int scheduleCode,
  struct ConfigFile * cFileStruct,struct ProgramListNode * programListPtr,
                                  struct LogFileNode * logFileListPtr,
                                  struct MMUControllerNode *mmuControllerListPtr,
                                  int logChoice)
{
  int returnCode = 0;
  char timeString[STD_STR_LEN];
  char tempLogStr[MAX_STR_LEN];

  accessTimer(LAP_TIMER, timeString);
  sprintf(tempLogStr,"TIME: %s, OS: INITIALIZING SJF-N Strategy \n", timeString);
  doLogChoice(logFileListPtr,tempLogStr,logChoice);


  struct ProgramListNode *tempProgramListNode = (struct ProgramListNode *)
          malloc(sizeof(struct ProgramListNode));

  tempProgramListNode = programListPtr;
  //sort the list

  struct ProgramListNode *tmpShortestProgram = (struct ProgramListNode *)
                    malloc(sizeof(struct ProgramListNode));

  struct ProgramListNode *SJFProgramListNode = (struct ProgramListNode *)
                    malloc(sizeof(struct ProgramListNode));

  tmpShortestProgram = returnShortestProgram(tempProgramListNode);
  //
  SJFProgramListNode = tmpShortestProgram;
  //
  tempProgramListNode = removeProgramNodeByProcessID(tempProgramListNode, tmpShortestProgram->processID);




  while(tempProgramListNode != NULL)
  {

    tmpShortestProgram = returnShortestProgram(tempProgramListNode);
    //printf("shortest prog is : %d\n", tmpShortestProgram->processID);
    SJFProgramListNode = addProgramNode(SJFProgramListNode,tmpShortestProgram);
    tempProgramListNode = removeProgramNodeByProcessID(tempProgramListNode,tmpShortestProgram->processID );

  }

  tempProgramListNode = SJFProgramListNode;

  while( tempProgramListNode != NULL)
  {

    struct ProgramMetaDataNode *tmpPtr = tempProgramListNode->programHead;

    int processNodeCount = 0;
    accessTimer(LAP_TIMER, timeString);
    sprintf(tempLogStr,"TIME: %s, OS: PROCESS %d set in Running State\n", timeString, tempProgramListNode->processID);
    doLogChoice(logFileListPtr,tempLogStr,logChoice);


    //printf("\ngetProgramMetaDataListLength:\n" );
    while( processNodeCount < tempProgramListNode->totalNodes)
    {
      char commandChar = tmpPtr->commandLetter;
      //printf("\nchecking for cycles %c %s\n", commandChar, tmpPtr->operationString);
      if( commandChar == 'I' || commandChar == 'O')
      {
        accessTimer(LAP_TIMER, timeString);
        sprintf(tempLogStr,"TIME: %s, PROCESS %d: %s %c start \n", timeString,tempProgramListNode->processID,tmpPtr->operationString, commandChar);
        doLogChoice(logFileListPtr,tempLogStr,logChoice);

        blockedtimerThread((cFileStruct->ioCycleTime_msec * tmpPtr->cycleTime));

        accessTimer(LAP_TIMER, timeString);
        sprintf(tempLogStr,"TIME: %s, PROCESS %d: %s %c end \n", timeString,tempProgramListNode->processID,tmpPtr->operationString, commandChar);
        doLogChoice(logFileListPtr,tempLogStr,logChoice);


      }
      else if (commandChar == 'P')
      {
        accessTimer(LAP_TIMER, timeString);
        sprintf(tempLogStr,"TIME: %s, PROCESS %d: %s %c start \n", timeString,tempProgramListNode->processID,tmpPtr->operationString, commandChar);
        doLogChoice(logFileListPtr,tempLogStr,logChoice);

        blockedtimerThread((cFileStruct->processorCycleTime_msec * tmpPtr->cycleTime));

        accessTimer(LAP_TIMER, timeString);
        sprintf(tempLogStr,"TIME: %s, PROCESS %d: %s %c end \n", timeString,tempProgramListNode->processID,tmpPtr->operationString, commandChar);
        doLogChoice(logFileListPtr,tempLogStr,logChoice);


      }
      else if (commandChar == 'M')
      {


        char memStr[9];
        char segmentStr[3];
        char memBaseStr[4];
        char memTotalStr[4];

        sprintf(memStr,"%d",tmpPtr->cycleTime);

        if((int)strlen(memStr)==7)
        {

          sprintf(segmentStr,"%.*s",1,memStr);
          sprintf(memBaseStr,"%.*s",3,memStr+1);
          sprintf(memTotalStr,"%.*s",3,memStr+4);

        }else if((int)strlen(memStr) ==8)
        {
          sprintf(segmentStr,"%.*s",2,memStr);
          sprintf(memBaseStr,"%.*s",3,memStr+2);
          sprintf(memTotalStr,"%.*s",3,memStr+5);
        }

        //printf("\nmem alloc in str %s %s %s %d\n",segmentStr,memBaseStr,memTotalStr, (int)strlen(memStr));

        struct MMUControllerNode * tmpNewMMUNode = (struct MMUControllerNode *)
                                malloc(sizeof(struct MMUControllerNode));

        tmpNewMMUNode->processID = tempProgramListNode->processID;
        tmpNewMMUNode->segmentIndex = (int)atoi(segmentStr);
        tmpNewMMUNode->memBase = (int)atoi(memBaseStr);
        tmpNewMMUNode->memTotal = (int)atoi(memTotalStr);

        accessTimer(LAP_TIMER, timeString);
        sprintf(tempLogStr,"TIME: %s, PROCESS %d: MMU %s %d/%d/%d %c start \n", timeString,tempProgramListNode->processID,tmpPtr->operationString,tmpNewMMUNode->segmentIndex, tmpNewMMUNode->memBase,tmpNewMMUNode->memTotal, commandChar);
        doLogChoice(logFileListPtr,tempLogStr,logChoice);

        int segFault = 1;

        blockedtimerThread(500);

        if(!strcmp(tmpPtr->operationString,"allocate"))
        {
          int isEmpty = 1;

          struct MMUControllerNode * tmpLoopMMUPtr = mmuControllerListPtr;

          while(tmpLoopMMUPtr != NULL)
          {
                if(tmpNewMMUNode->segmentIndex == tmpLoopMMUPtr->segmentIndex &&
                tmpNewMMUNode->memBase == tmpLoopMMUPtr->memBase )
                {
                  isEmpty = 0;
                }

              tmpLoopMMUPtr = tmpLoopMMUPtr->next;
          }

          if(isEmpty)
          {
            if((tmpNewMMUNode->memBase+tmpNewMMUNode->memTotal) <= cFileStruct->memoryAvail)
            {

              mmuControllerListPtr = addMMUControllerNode(mmuControllerListPtr,tmpNewMMUNode);

              accessTimer(LAP_TIMER, timeString);
              sprintf(tempLogStr,"TIME: %s, PROCESS %d: MMU %s SUCCESS! \n", timeString,tempProgramListNode->processID,tmpPtr->operationString);
              doLogChoice(logFileListPtr,tempLogStr,logChoice);
              segFault = 0;

            }
            else
            {
              accessTimer(LAP_TIMER, timeString);
              sprintf(tempLogStr,"TIME: %s, PROCESS %d: MMU %s FAIL! \n", timeString,tempProgramListNode->processID,tmpPtr->operationString);
              doLogChoice(logFileListPtr,tempLogStr,logChoice);

            }
          }
          else
          {
            accessTimer(LAP_TIMER, timeString);
            sprintf(tempLogStr,"TIME: %s, PROCESS %d: MMU %s FAIL! \n", timeString,tempProgramListNode->processID,tmpPtr->operationString);
            doLogChoice(logFileListPtr,tempLogStr,logChoice);

          }

        }
        else if(!strcmp(tmpPtr->operationString,"access"))
        {
          int pidFound = 0;

          struct MMUControllerNode * tmpLoopMMUPtr = mmuControllerListPtr;

          while(tmpLoopMMUPtr != NULL)
          {
              if(tmpLoopMMUPtr->processID == tempProgramListNode->processID)
              {
                if(tmpNewMMUNode->segmentIndex == tmpLoopMMUPtr->segmentIndex &&
                tmpNewMMUNode->memBase == tmpLoopMMUPtr->memBase &&
                tmpNewMMUNode->memTotal == tmpLoopMMUPtr->memTotal)
                {
                  pidFound = 1;
                }
              }
              tmpLoopMMUPtr = tmpLoopMMUPtr->next;
          }


          if(pidFound)
          {
            accessTimer(LAP_TIMER, timeString);
            sprintf(tempLogStr,"TIME: %s, PROCESS %d: MMU %s SUCCESS! \n", timeString,tempProgramListNode->processID,tmpPtr->operationString);
            doLogChoice(logFileListPtr,tempLogStr,logChoice);
            segFault = 0;
          }
          else
          {
            accessTimer(LAP_TIMER, timeString);
            sprintf(tempLogStr,"TIME: %s, PROCESS %d: MMU %s FAIL! \n", timeString,tempProgramListNode->processID,tmpPtr->operationString);
            doLogChoice(logFileListPtr,tempLogStr,logChoice);
          }
        }

        if(segFault)
        {
          accessTimer(LAP_TIMER, timeString);
          sprintf(tempLogStr,"TIME: %s, PROCESS %d: Segmentation Fault - Process ended \n", timeString,tempProgramListNode->processID);
          doLogChoice(logFileListPtr,tempLogStr,logChoice);
          processNodeCount = tempProgramListNode->totalNodes;
          returnCode = 1;
        }

      }
      else if (commandChar == 'A' && !strcmp(tmpPtr->operationString,"end"))
      {

      }

      tmpPtr = tmpPtr->next;
      processNodeCount++;

    }
    accessTimer(LAP_TIMER, timeString);
    sprintf(tempLogStr,"TIME: %s, OS: PROCESS %d set in EXIT\n", timeString, tempProgramListNode->processID);
    doLogChoice(logFileListPtr,tempLogStr,logChoice);


    tempProgramListNode = tempProgramListNode->next;
  }

  free(tempProgramListNode);
  return returnCode;
}

int doFCFSN(int scheduleCode,
  struct ConfigFile * cFileStruct,struct ProgramListNode * programListPtr,
                                  struct LogFileNode * logFileListPtr,
                                  struct MMUControllerNode *mmuControllerListPtr,
                                  int logChoice)
{
  int returnCode = 0;
  char timeString[STD_STR_LEN];
  char tempLogStr[MAX_STR_LEN];

  accessTimer(LAP_TIMER, timeString);
  sprintf(tempLogStr,"TIME: %s, OS: INITIALIZING FCFS-N Strategy \n", timeString);
  doLogChoice(logFileListPtr,tempLogStr,logChoice);



  struct ProgramListNode *tempProgramListNode = (struct ProgramListNode *)
          malloc(sizeof(struct ProgramListNode));

  tempProgramListNode = programListPtr;

  while( tempProgramListNode != NULL)
  {

    struct ProgramMetaDataNode *tmpPtr = tempProgramListNode->programHead;

    int processNodeCount = 0;
    accessTimer(LAP_TIMER, timeString);
    sprintf(tempLogStr,"TIME: %s, OS: PROCESS %d set in Running State\n", timeString, tempProgramListNode->processID);
    doLogChoice(logFileListPtr,tempLogStr,logChoice);


    //printf("\ngetProgramMetaDataListLength:\n" );
    while( processNodeCount < tempProgramListNode->totalNodes)
    {
      char commandChar = tmpPtr->commandLetter;
      //printf("\nchecking for cycles %c %s\n", commandChar, tmpPtr->operationString);
      if( commandChar == 'I' || commandChar == 'O')
      {
        accessTimer(LAP_TIMER, timeString);
        sprintf(tempLogStr,"TIME: %s, PROCESS %d: %s %c start \n", timeString,tempProgramListNode->processID,tmpPtr->operationString, commandChar);
        doLogChoice(logFileListPtr,tempLogStr,logChoice);

        blockedtimerThread((cFileStruct->ioCycleTime_msec * tmpPtr->cycleTime));

        accessTimer(LAP_TIMER, timeString);
        sprintf(tempLogStr,"TIME: %s, PROCESS %d: %s %c end \n", timeString,tempProgramListNode->processID,tmpPtr->operationString, commandChar);
        doLogChoice(logFileListPtr,tempLogStr,logChoice);


      }
      else if (commandChar == 'P')
      {
        accessTimer(LAP_TIMER, timeString);
        sprintf(tempLogStr,"TIME: %s, PROCESS %d: %s %c start \n", timeString,tempProgramListNode->processID,tmpPtr->operationString, commandChar);
        doLogChoice(logFileListPtr,tempLogStr,logChoice);

        blockedtimerThread((cFileStruct->processorCycleTime_msec * tmpPtr->cycleTime));

        accessTimer(LAP_TIMER, timeString);
        sprintf(tempLogStr,"TIME: %s, PROCESS %d: %s %c end \n", timeString,tempProgramListNode->processID,tmpPtr->operationString, commandChar);
        doLogChoice(logFileListPtr,tempLogStr,logChoice);


      }
      else if (commandChar == 'M')
      {
        char memStr[9];
        char segmentStr[3];
        char memBaseStr[4];
        char memTotalStr[4];

        sprintf(memStr,"%d",tmpPtr->cycleTime);


        if((int)strlen(memStr)==7)
        {

          sprintf(segmentStr,"%.*s",1,memStr);
          sprintf(memBaseStr,"%.*s",3,memStr+1);
          sprintf(memTotalStr,"%.*s",3,memStr+4);

        }else if((int)strlen(memStr) ==8)
        {
          sprintf(segmentStr,"%.*s",2,memStr);
          sprintf(memBaseStr,"%.*s",3,memStr+2);
          sprintf(memTotalStr,"%.*s",3,memStr+5);
        }

        //printf("\nmem alloc in str %s %s %s %d\n",segmentStr,memBaseStr,memTotalStr, (int)strlen(memStr));

        struct MMUControllerNode * tmpNewMMUNode = (struct MMUControllerNode *)
                                malloc(sizeof(struct MMUControllerNode));

        tmpNewMMUNode->processID = tempProgramListNode->processID;
        tmpNewMMUNode->segmentIndex = (int)atoi(segmentStr);
        tmpNewMMUNode->memBase = (int)atoi(memBaseStr);
        tmpNewMMUNode->memTotal = (int)atoi(memTotalStr);

        accessTimer(LAP_TIMER, timeString);
        sprintf(tempLogStr,"TIME: %s, PROCESS %d: MMU %s %d/%d/%d %c start \n", timeString,tempProgramListNode->processID,tmpPtr->operationString,tmpNewMMUNode->segmentIndex, tmpNewMMUNode->memBase,tmpNewMMUNode->memTotal, commandChar);
        doLogChoice(logFileListPtr,tempLogStr,logChoice);

        int segFault = 1;

        blockedtimerThread(500);

        if(!strcmp(tmpPtr->operationString,"allocate"))
        {
          int isEmpty = 1;

          struct MMUControllerNode * tmpLoopMMUPtr = mmuControllerListPtr;

          while(tmpLoopMMUPtr != NULL)
          {
                if(tmpNewMMUNode->segmentIndex == tmpLoopMMUPtr->segmentIndex &&
                tmpNewMMUNode->memBase == tmpLoopMMUPtr->memBase )
                {
                  isEmpty = 0;
                }

              tmpLoopMMUPtr = tmpLoopMMUPtr->next;
          }

          if(isEmpty)
          {
            if((tmpNewMMUNode->memBase+tmpNewMMUNode->memTotal) <= cFileStruct->memoryAvail)
            {

              mmuControllerListPtr = addMMUControllerNode(mmuControllerListPtr,tmpNewMMUNode);

              accessTimer(LAP_TIMER, timeString);
              sprintf(tempLogStr,"TIME: %s, PROCESS %d: MMU %s SUCCESS! \n", timeString,tempProgramListNode->processID,tmpPtr->operationString);
              doLogChoice(logFileListPtr,tempLogStr,logChoice);
              segFault = 0;

            }
            else
            {
              accessTimer(LAP_TIMER, timeString);
              sprintf(tempLogStr,"TIME: %s, PROCESS %d: MMU %s FAIL! \n", timeString,tempProgramListNode->processID,tmpPtr->operationString);
              doLogChoice(logFileListPtr,tempLogStr,logChoice);

            }
          }
          else
          {
            accessTimer(LAP_TIMER, timeString);
            sprintf(tempLogStr,"TIME: %s, PROCESS %d: MMU %s FAIL! \n", timeString,tempProgramListNode->processID,tmpPtr->operationString);
            doLogChoice(logFileListPtr,tempLogStr,logChoice);

          }

        }
        else if(!strcmp(tmpPtr->operationString,"access"))
        {
          int pidFound = 0;

          struct MMUControllerNode * tmpLoopMMUPtr = mmuControllerListPtr;

          while(tmpLoopMMUPtr != NULL)
          {
              if(tmpLoopMMUPtr->processID == tempProgramListNode->processID)
              {
                if(tmpNewMMUNode->segmentIndex == tmpLoopMMUPtr->segmentIndex &&
                tmpNewMMUNode->memBase == tmpLoopMMUPtr->memBase &&
                tmpNewMMUNode->memTotal == tmpLoopMMUPtr->memTotal)
                {
                  pidFound = 1;
                }
              }
              tmpLoopMMUPtr = tmpLoopMMUPtr->next;
          }


          if(pidFound)
          {
            accessTimer(LAP_TIMER, timeString);
            sprintf(tempLogStr,"TIME: %s, PROCESS %d: MMU %s SUCCESS! \n", timeString,tempProgramListNode->processID,tmpPtr->operationString);
            doLogChoice(logFileListPtr,tempLogStr,logChoice);
            segFault = 0;
          }
          else
          {
            accessTimer(LAP_TIMER, timeString);
            sprintf(tempLogStr,"TIME: %s, PROCESS %d: MMU %s FAIL! \n", timeString,tempProgramListNode->processID,tmpPtr->operationString);
            doLogChoice(logFileListPtr,tempLogStr,logChoice);
          }
        }

        if(segFault)
        {
          accessTimer(LAP_TIMER, timeString);
          sprintf(tempLogStr,"TIME: %s, PROCESS %d: Segmentation Fault - Process ended \n", timeString,tempProgramListNode->processID);
          doLogChoice(logFileListPtr,tempLogStr,logChoice);
          processNodeCount = tempProgramListNode->totalNodes;
          returnCode = 1;
        }


      }
      else if (commandChar == 'A' && !strcmp(tmpPtr->operationString,"end"))
      {

      }

      tmpPtr = tmpPtr->next;
      processNodeCount++;

    }
    accessTimer(LAP_TIMER, timeString);
    sprintf(tempLogStr,"TIME: %s, OS: PROCESS %d set in EXIT\n", timeString, tempProgramListNode->processID);
    doLogChoice(logFileListPtr,tempLogStr,logChoice);


    tempProgramListNode = tempProgramListNode->next;
  }

  free(tempProgramListNode);

  return returnCode;
}

int doForScheduleCode(int scheduleCode,
  struct ConfigFile * cFileStruct,struct ProgramListNode * programListPtr,
                                  struct LogFileNode * logFileListPtr,struct MMUControllerNode *mmuControllerListPtr, int logChoice)
{

  int returnCode = 1;
  switch (scheduleCode) {
    case NONE:
    {
      //if none does FCFS because when congif is read none is fcfn
      break;
    }
    case FCFSN:
    {

      returnCode = doFCFSN(scheduleCode,
        cFileStruct, programListPtr, logFileListPtr,mmuControllerListPtr,logChoice);

      break;
    }
    case SJFN:
    {

      returnCode = doSJFN(scheduleCode,
        cFileStruct, programListPtr, logFileListPtr,mmuControllerListPtr,logChoice);

      break;
    }
    case SRTFP:
    {

      break;
    }
    case FCFSP:
    {

      break;
    }
    case RRP:
    {

      break;
    }
    default :
      break;

  }

  return returnCode;
}

int testMetaDataParseProgramList(char *metaDataFilePath, struct ConfigFile * cFileStruct,
                                                         struct LogFileNode * logFileNodePtr,
                                                         int logChoice)
{

  struct ProgramMetaDataNode *workingPtr, *headPtr = NULL;

  struct ProgramListNode *headProgramListNode = NULL;

  struct ProgramListNode *tempProgramListNode = (struct ProgramListNode *)
                    malloc(sizeof(struct ProgramListNode));

  struct MMUControllerNode *MMUControllerListPtr = NULL;




  int resultCode;

  char timeString[STD_STR_LEN];
  char tempLogStr[MAX_STR_LEN];


  accessTimer(LAP_TIMER, timeString);
  sprintf(tempLogStr,"TIME: %s, OS: LOADING METADATA\n", timeString);
  doLogChoice(logFileNodePtr,tempLogStr,logChoice);

  //one function to rule them all
  resultCode = getDataFromFile(&headPtr,metaDataFilePath);

  //printf("\nERROR CODE FROM MDFP %d --> %s\n", resultCode, metaDataFilePath);
  if(!resultCode)
  {
    workingPtr = headPtr;
    //accessTimer(ZERO_TIMER,timeString);
    while( workingPtr != NULL )
       {

         if(workingPtr->commandLetter == 'A' && !strcmp(workingPtr->operationString,"start"))
         {

           headProgramListNode = addProgramNode(headProgramListNode,makeProgramNode(workingPtr));

         }

        workingPtr = workingPtr->next;
       }

    accessTimer(LAP_TIMER, timeString);

    sprintf(tempLogStr,"TIME: %s, OS: ALL PROGRAMS LOADED\n", timeString);
    doLogChoice(logFileNodePtr,tempLogStr,logChoice);


    accessTimer(LAP_TIMER, timeString);

    sprintf(tempLogStr,"TIME: %s, OS: ANALYZING PROGRAM METADATA\n", timeString);
    doLogChoice(logFileNodePtr,tempLogStr,logChoice);

    tempProgramListNode = headProgramListNode;

    //
    int processCount = 0;
    while( headProgramListNode != NULL)
    {
      headProgramListNode->approxTotalProgramTime_mSec =
        (headProgramListNode->totalProcessCycles * cFileStruct->processorCycleTime_msec)
        + (headProgramListNode->totalIOcycles * cFileStruct->ioCycleTime_msec);
      headProgramListNode->processID = processCount++;

      headProgramListNode = headProgramListNode->next;
    }



    accessTimer(LAP_TIMER, timeString);

    sprintf(tempLogStr,"TIME: %s, OS: PROGRAM METADATA ANALYSYS COMPLETE\n", timeString);
    doLogChoice(logFileNodePtr,tempLogStr,logChoice);


    //for scheduling do the right thing

    resultCode = doForScheduleCode(cFileStruct->cpuScheduleCode,cFileStruct,tempProgramListNode,logFileNodePtr,MMUControllerListPtr,logChoice);

  }
  else
  {
    printf("ERROR MESSAGE RECIEVED METADATAPARSE: %d --> %s\n", resultCode, metaDataFilePath);
    headPtr = clearList(headPtr);
    free(headPtr);
    return resultCode;
  }

  headPtr = clearList(headPtr);
  free(workingPtr);
  free(headPtr);
  tempProgramListNode = clearProgramList(tempProgramListNode);
  headProgramListNode = clearProgramList(headProgramListNode);
  MMUControllerListPtr = clearMMUControllerList(MMUControllerListPtr);
  free(MMUControllerListPtr);
  free(tempProgramListNode);
  free(headProgramListNode);
  return NO_ERROR_MSG;

}

int testFCFSandSJF(char * configFilePath)
{
  int resultCode;
  struct ConfigFile *tempConfigFile
                    = (struct ConfigFile *)malloc( sizeof( struct ConfigFile ) );

  struct LogFileNode * tempLogFileNodePtr =
      (struct LogFileNode*)malloc( sizeof(struct LogFileNode));

  struct LogFileNode * headLogFilePtr;

  char tempLogStr[MAX_STR_LEN];

  char timeString[STD_STR_LEN];

  int logChoice;


  accessTimer(ZERO_TIMER,timeString);
  sprintf(tempLogStr,"TIME: %s, System Start\n", timeString);

  tempLogFileNodePtr = addLogFileNode(tempLogFileNodePtr, tempLogStr);
  headLogFilePtr = tempLogFileNodePtr;

  //one function to rule them all
  resultCode = getConfigDataFromFile(tempConfigFile,configFilePath);

  if(!strcmp(tempConfigFile->logTo,"Monitor")){

    logChoice = 0;
  }
  else if(!strcmp(tempConfigFile->logTo,"File"))
  {
    logChoice = 1;
  }
  else logChoice = 2;



  if(resultCode == NO_ERROR_MSG || resultCode == END_FILE_FOUND_MSG)
  {


    accessTimer(LAP_TIMER,timeString);
    sprintf(tempLogStr,"TIME: %s, 0S: LOADING PROGRAMS\n", timeString);
    doLogChoice(tempLogFileNodePtr,tempLogStr,logChoice);

    resultCode = testMetaDataParseProgramList(tempConfigFile->filePath, tempConfigFile, tempLogFileNodePtr, logChoice);

  }
  else
  {
    printf("\nERROR MESSAGE RECIEVED CONFIG PARSE : %d --> %s\n", resultCode, tempConfigFile->filePath);
    headLogFilePtr = clearLogFileList(headLogFilePtr);
    free(headLogFilePtr);
    free(tempConfigFile);
    free(tempLogFileNodePtr);
    return resultCode;
  }

  accessTimer(LAP_TIMER,timeString);
  sprintf(tempLogStr,"TIME: %s, SYSTEM EXIT\n", timeString);
  doLogChoice(tempLogFileNodePtr,tempLogStr,logChoice);

  if(logChoice){

    printf("\nWRITING LOG FILE ->> %s\n", tempConfigFile->logFilePath);
    //write log to file
    FILE *logfile;
    logfile = fopen(tempConfigFile->logFilePath,"w+");

    if(logfile)
    {
      while(tempLogFileNodePtr->next != NULL){

        fputs(tempLogFileNodePtr->logStr,logfile);
        tempLogFileNodePtr = tempLogFileNodePtr->next;
      }
    }
    else
    {
      printf("\nFAILED TO OPEN LOG FILE\n");
      return FILE_OPEN_ERR;
    }

    fclose(logfile);

  }

  headLogFilePtr = clearLogFileList(headLogFilePtr);
  free(headLogFilePtr);
  free(tempConfigFile);

  return NO_ERROR_MSG;


}

int main( int argc, char **argv )
   {

     //printf("argc: %d", argc);
     if(argc == 1)
     {

       printf("\nERROR INCORRECT PARAMETERS\n");


      }
      else if (argc == 3)
      {

        printf("BEGINING CONFIG TEST SIM01: %s\n\n", argv[1]);
        testConfigParse(argv[1]);

        printf("BEGINING METADATA TEST SIM01: %s\n\n", argv[2]);
        testMetaDataParse(argv[2]);

      }
      else if (argc == 2)
      {
        printf("\nBEGINING CONFIG TEST SIM01: %s\n\n", argv[1]);

        testFCFSandSJF(argv[1]);


      }
      else
      {
        printf("\nERROR INCORRECT PARAMETERS\n");
      }




    return 0;
  }
