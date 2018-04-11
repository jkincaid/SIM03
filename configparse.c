// Program Information ////////////////////////////////////////////////////////
/**
 * @file configparse.c
 *
 * @brief parses sim config creates nice struct for config file
 *
 *
 * @details
 *
 * @version 0.1 01/27/2018
 *
 * @Note Requires configparse.h, pmdlist.h.
 *
 *
 */


//HEADER files
#include "configparse.h"
#include "pmdlist.h"



#include <ctype.h>

// Return a pointer to the trimmed string
char *string_trim_inplace(char *s) {
  while (isspace((unsigned char) *s)) s++;
  if (*s) {
    char *p = s;
    while (*p) p++;
    while (isspace((unsigned char) *(--p)));
    p[1] = '\0';
  }
  return s;
}

//compares up to the first colon
int logParamStringCompare(const char *logDefinitionStr, char *stringToTest)
{

  const char CP_COLON = ':';


  int stringTestIndex = 0;
  char tmpChar = ' ';
  while(tmpChar != CP_COLON)
  {
    if(logDefinitionStr[stringTestIndex] != stringToTest[stringTestIndex] )
      return CONFIG_VALUE_ERR;
    tmpChar = stringToTest[stringTestIndex++];
  }

  return NO_ERROR_MSG;

}

int getScheduleCode(char *scheduleCodeStr)
{
  const int SCHEDULE_CODE_COUNT = 6;
  const char *CPU_SCHEDULING_CODES[] =
    {"NONE", "FCFS-N", "SJF-N", "SRTF-P", "FCFS-P", "RR-P"};


   //printf("%s", scheduleCodeStr);

   int scheduleCodeCount = 0;
   for( ;scheduleCodeCount < SCHEDULE_CODE_COUNT; scheduleCodeCount++)
   {

      if(!strcmp(CPU_SCHEDULING_CODES[scheduleCodeCount],&scheduleCodeStr[1]))
      {
        break;
      }
   }
   if(scheduleCodeCount == SCHEDULE_CODE_COUNT) scheduleCodeCount = 7;
   return scheduleCodeCount;

}

int parseAndPackConfig(struct ConfigFile *cFileStruct,
  char *fileLineStr,char *configValueStr, int configFileLineNum)
{

  const char *logFileDefinition[] =
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

  if(!logParamStringCompare(logFileDefinition[configFileLineNum],fileLineStr)){

    //debug
    //printf("%d :%s <==> %s ::: %s\n", configFileLineNum,logFileDefinition[configFileLineNum], fileLineStr,configValueStr);
    switch(configFileLineNum)
    {
      case 1:
      {  //convert string to correct type

        int tmpConfigValue;
        tmpConfigValue = atoi(configValueStr);

        if(tmpConfigValue <=10 && tmpConfigValue >= 0)
        {

          //set variable
          cFileStruct->version = (int) tmpConfigValue;
        }
        else
        {
          return CONFIG_VALUE_ERR;
        }
        break;
      }

      case 2:
      {
        if(configValueStr != NULL)
        {
          //convert string to correct type

          //set variable
          strcpy(cFileStruct->filePath,string_trim_inplace(configValueStr));
        }
        else
        {
          return CONFIG_VALUE_ERR;
        }
        break;
      }

      case 3:
      {
        int tmpScheduleCode;
        tmpScheduleCode = getScheduleCode(configValueStr);
        //printf("cpuScheduleCode --> %d\n" , tmpScheduleCode);

        if(tmpScheduleCode >= 0 && tmpScheduleCode <6 )
        {
          if(tmpScheduleCode == 0) tmpScheduleCode =1;
          //set variable
          cFileStruct->cpuScheduleCode = (int) tmpScheduleCode;
        }
        else
        {
          return CONFIG_VALUE_ERR;
        }
        break;
      }

      case 4:
      {
        int tmpConfigValue;
        tmpConfigValue = atoi(configValueStr);

        if(tmpConfigValue <=100 && tmpConfigValue >= 0)
        {

          //set variable
          cFileStruct->quantumTime = (int) tmpConfigValue;
        }
        else
        {
          return CONFIG_VALUE_ERR;
        }
        break;
      }

      case 5:
      {
        int tmpConfigValue;
        tmpConfigValue = atoi(configValueStr);

        if(tmpConfigValue <=1048576 && tmpConfigValue >= 0)
        {

          //set variable
          cFileStruct->memoryAvail = (int) tmpConfigValue;
        }
        else
        {
          return CONFIG_VALUE_ERR;
        }
        break;
      }

      case 6:
      {
        int tmpConfigValue;
        tmpConfigValue = atoi(configValueStr);

        if(tmpConfigValue <=1000 && tmpConfigValue >= 0)
        {

          //set variable
          cFileStruct->processorCycleTime_msec = (int) tmpConfigValue;
        }
        else
        {
          return CONFIG_VALUE_ERR;
        }
        break;
      }

      case 7:
      {

        int tmpConfigValue;
        tmpConfigValue = atoi(configValueStr);

        if(tmpConfigValue <=10000 && tmpConfigValue >= 0)
        {

          //set variable
          cFileStruct->ioCycleTime_msec = (int) tmpConfigValue;
        }
        else
        {
          return CONFIG_VALUE_ERR;
        }
        break;
      }

      case 8:
      {
        if(configValueStr != NULL)
        {
          //convert string to correct type

          //set variable
          strcpy(cFileStruct->logTo,string_trim_inplace(configValueStr));
        }
        else
        {
          return CONFIG_VALUE_ERR;
        }
        break;
      }

      case 9:
      {
        if(configValueStr != NULL)
        {
          //convert string to correct type

          //set variable
          strcpy(cFileStruct->logFilePath,string_trim_inplace(configValueStr));
        }
        else
        {
          return CONFIG_VALUE_ERR;
        }
        break;
      }

    default:
      break;
    }


  }
  else
  {
    return CONFIG_VALUE_ERR;
  }

  return NO_ERROR_MSG;
}


int getConfigDataFromFile( struct ConfigFile *cFileStruct, char *fileName )
{

  //constants, including externally declared constants
  const int CP_MAX_STR_LEN = 120;
  const int CP_NOT_AT_FILE_END = 0;
  const char CP_READ_ONLY_FLAG[] = "r";
  const char CP_NULL_CHAR = '\0';
  const char CP_NEW_LINE = '\n';
  const char CP_COLON = ':';
  const char CP_SPACE = ' ';



  FILE *fileAccessPtr;
  int strIndex, charAsInt,colonIndex,tmpcount,lineNum,returnCode=0;
  char strBuffer[ CP_MAX_STR_LEN ],
  configValue[CP_MAX_STR_LEN];
  int continueRun = 1;



  fileAccessPtr = fopen( fileName, CP_READ_ONLY_FLAG );

  if( fileAccessPtr != NULL )
     {

    lineNum = 0;
     while( continueRun == 1 )
         {
          charAsInt = fgetc( fileAccessPtr );

          // clear white CP_SPACE prior to data line
          while( feof( fileAccessPtr ) == CP_NOT_AT_FILE_END
                   && charAsInt <= (int)( CP_SPACE ) )
             {
              charAsInt = fgetc( fileAccessPtr );
             }



          if( feof( fileAccessPtr ) == CP_NOT_AT_FILE_END )
             {

              // get name
              strIndex = 1;  //because we have to cycle past the new line
              colonIndex = 0;
              while( charAsInt != (int)( CP_NEW_LINE ))
                 {


                  strBuffer[ strIndex ] = (char)( charAsInt );

                  if( charAsInt == (int)( CP_COLON))
                  {
                    colonIndex = strIndex;

                    //printf("CP_COLON index at: %d\n", colonIndex);
                  }


                  strIndex++;
                  strBuffer[ strIndex ] = CP_NULL_CHAR;


                  charAsInt = fgetc( fileAccessPtr );
                 }


               tmpcount = 0;
               if(strBuffer != NULL)
               {

                 for(int i=colonIndex+1;i< strlen(strBuffer)+1;i++)
                 {
                   configValue[tmpcount++] = strBuffer[i];

                 }
               }
               //null terminate configValue
               configValue[tmpcount] = CP_NULL_CHAR;



            if(colonIndex)
            {
              //

              returnCode = parseAndPackConfig(cFileStruct,strBuffer,configValue,lineNum);
            }
            if(returnCode)
               {

                 fclose( fileAccessPtr );
                 printf("CONFIG_VALUE_ERR occured at line : %d\n", lineNum );
                 // free( tempConfigFile );

                 return CONFIG_VALUE_ERR;

               }

              lineNum++;


              //
              //
              // //we found the end of file with a CP_PERIOD end nicely
              // if(charAsInt == (int)(CP_PERIOD))
              // {
              //   continueRun = 0;
              //   printf("found CP_PERIOD\n");
              //   fclose( fileAccessPtr );
              //
              //
              //
              //   return NO_ERROR_MSG;
              // }
             }

          else
             {
              fclose( fileAccessPtr );



              return END_FILE_FOUND_MSG;
             }

          charAsInt = fgetc( fileAccessPtr );
          strBuffer[0] = (char) charAsInt;
          //set first character after finding new line
         }

  }

  else
     {
      return FILE_OPEN_ERR;
     }

  return UNKNOWN_ERR;

}
