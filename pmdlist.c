// Program Information ////////////////////////////////////////////////////////
/**
 * @file pmdlist.c
 *
 * @brief creates a singly linked list for program meta data nodes
 *
 *
 * @details
 *
 * @version 0.1 01/27/2018
 *
 * @Note Requires  pmdlist.h.
   @Note Heavily modified from Michael Leverington linkedlist.c

 *
 *
 */
// header files

#include "pmdlist.h"



// supporting function implementations
int isInString(char *strOfChar, char charToTest)
{
    const char NULL_CHAR = '\0';
    char isInTmpStr = ' ';
    char *strIter = strOfChar;

    while(isInTmpStr != NULL_CHAR){

        isInTmpStr = *strIter++;
        if(isInTmpStr == charToTest){

          return 1;
        }

    }

    return 0;

}


//add node onto tail of list allways add to end of list
struct ProgramMetaDataNode *addNode( struct ProgramMetaDataNode *tailNode,
                                     struct ProgramMetaDataNode *newNode )
   {


    // check for viable node
    if( tailNode != NULL )
      {

        tailNode->next = addNode( tailNode->next, newNode );


        return tailNode;
       }

    // assume empty tail node
    tailNode = makeNode( newNode->commandLetter, newNode->operationString,
                                                            newNode->cycleTime);

    return tailNode;
   }

// function eliminates redundant code in addNode
struct ProgramMetaDataNode *makeNode( char commandLetter, char *operationString,
                                                                  int cycleTime )
   {
    struct ProgramMetaDataNode *newNode
                   = ( struct ProgramMetaDataNode * )
                      malloc( sizeof( struct ProgramMetaDataNode ) );

    strcpy( newNode->operationString, operationString);
    newNode->commandLetter = commandLetter;
    newNode->cycleTime = cycleTime;
    newNode->next = NULL;

    return newNode;
   }


struct ProgramMetaDataNode *clearList( struct ProgramMetaDataNode *headNode )
   {
    if( headNode == NULL )
       {
        return NULL;
       }

    if( headNode->next != NULL )
       {
        clearList( headNode->next );
       }

    free( headNode );

    headNode = NULL;

    return headNode;
   }

int getDataFromFile( struct ProgramMetaDataNode **headPtr, char *fileName )
{

  //constants, including externally declared constants
  const int MAX_STR_LEN = 120;
  const int NOT_AT_FILE_END = 0;
  const char READ_ONLY_FLAG[] = "r";
  const char NULL_CHAR = '\0';
  const char SEMICOLON = ';';
  const char SPACE = ' ';
  const char LBRACKET = '(';
  const char RBRACKET = ')';
  const char PERIOD = '.';

  typedef int Boolean;
  Boolean True = 1;
  Boolean False = 0;
  FILE *fileAccessPtr;
  int strIndex, charAsInt,leftBracketIndex,rightBracketIndex;
  char strBuffer[ MAX_STR_LEN ],
  cycleTime[ MAX_STR_LEN],
  tempFileLine0[ 2048 ],
  tempFileLine1[ 2048 ],
  tempFileLine2[ 2048 ],
  tempFileLine3[ 2048 ];
  char commandLetterDefinition[] = "SAPMIO";
  Boolean continueRun = True;
  struct ProgramMetaDataNode *tempNodePtr
                 = (struct ProgramMetaDataNode *)malloc( sizeof( struct ProgramMetaDataNode ) );

  fileAccessPtr = fopen( fileName, READ_ONLY_FLAG );

  if( fileAccessPtr != NULL )
  {

  //get first line
  fscanf(fileAccessPtr,"%s %s %s %s",tempFileLine0,tempFileLine1,tempFileLine2,tempFileLine3);
  //printf("begining to read: %s %s %s %s\n",tempFileLine0,tempFileLine1,tempFileLine2,tempFileLine3);

  while( continueRun == True )
      {
       charAsInt = fgetc( fileAccessPtr );

       // clear white space prior to data line
       while( feof( fileAccessPtr ) == NOT_AT_FILE_END
                && charAsInt <= (int)( SPACE ) )
          {
           charAsInt = fgetc( fileAccessPtr );
          }



       if( feof( fileAccessPtr ) == NOT_AT_FILE_END )
          {

           // get name
           strIndex = 0;
           leftBracketIndex = 0;
           rightBracketIndex = 0;
           while( charAsInt != (int)( SEMICOLON ) && charAsInt != (int)(PERIOD))
              {


               strBuffer[ strIndex ] = (char)( charAsInt );

               if( charAsInt == (int)( LBRACKET))
               {
                 leftBracketIndex = strIndex;
                 ////printf("left bracket index: %d\n", leftBracketIndex);
               }
               else if( charAsInt == (int)( RBRACKET))
               {
                 rightBracketIndex = strIndex;
                 ////printf("right bracket index: %d\n", rightBracketIndex);

               }

               strIndex++;

               strBuffer[ strIndex ] = NULL_CHAR;

               charAsInt = fgetc( fileAccessPtr );
              }

          ////printf("begining semicolon sweep:\n%s\n", strBuffer);
           // do things to strBuffer
           //check for command letter
           if(isInString(commandLetterDefinition, (char)strBuffer[0]))
           {
             tempNodePtr-> commandLetter = strBuffer[0];

           }
           else
           {
             free(tempNodePtr);
             return INCOMPLETE_FILE_ERR;
           }
           int tmpcount = 0;

           //check for good right bracket
           if(rightBracketIndex > 1)
           {
             for(int i=leftBracketIndex+1;i< rightBracketIndex;i++)
             {
               tempNodePtr->operationString[tmpcount++] = strBuffer[i];

             }
             //null terminate the opString
             tempNodePtr->operationString[tmpcount] = NULL_CHAR;

             tmpcount = 0;
             for(int i=rightBracketIndex+1;i< strlen(strBuffer)+1;i++)
             {
               cycleTime[tmpcount++] = strBuffer[i];

             }
             //check for good cycleTime
             if(cycleTime != NULL || cycleTime[0] != SEMICOLON)
             {
               tempNodePtr->cycleTime = atoi(cycleTime);
             }
             else
             {
               free(tempNodePtr);
               return INCOMPLETE_FILE_ERR ;
             }
           }
           else
           {
              free(tempNodePtr);
              return INCOMPLETE_FILE_ERR ;
           }

           // load to linked list
           *headPtr = addNode( *headPtr, tempNodePtr );

           //we found the end of file with a period end nicely
           if(charAsInt == (int)(PERIOD))
           {
             continueRun = False;
             ////printf("found PERIOD\n");
             fclose( fileAccessPtr );

             free( tempNodePtr );

             return NO_ERROR_MSG;
           }
          }

       else
          {
           fclose( fileAccessPtr );

           free( tempNodePtr );

           return END_FILE_FOUND_MSG;
          }

       charAsInt = fgetc( fileAccessPtr );
      }
    }

    else
    {
      free(tempNodePtr);
      return FILE_OPEN_ERR;
    }

  free(tempNodePtr);
  return UNKNOWN_ERR;
}
