
// header files
#include "pmdlist.h"
#include <stdio.h>  // I/O
#include <stdlib.h>  // malloc


// Message codes for file access

enum SCHEDULING_CODES { NONE, FCFSN, SJFN, SRTFP, FCFSP, RRP};

// struct declaration
struct ConfigFile
   {
     int version;
     char filePath[2048];
     int cpuScheduleCode;
     int quantumTime;
     int memoryAvail;
     int processorCycleTime_msec;
     int ioCycleTime_msec;
     char logTo[2048];
     char logFilePath[2048];


   };


   // function prototypes
int getScheduleCode(char *scheduleCodeStr);
int parseAndPackConfig(struct ConfigFile *cFileStruct, char *fileLineStr,char *configValueStr, int configFileIndex);

int getConfigDataFromFile( struct ConfigFile *cFileStruct, char *fileName );
