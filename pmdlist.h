// Program Information ////////////////////////////////////////////////////////
/**
 * @file pmdlist.h
 *
 * @brief creates a singly linked list for program meta data nodes
 *
 *
 * @details
 *
 * @version 0.1 01/27/2018
 *
 * @Note Heavily modified from Michael Leverington linkedlist.c

 *
 *
 */

// header files
#include <stdio.h>  // I/O
#include <stdlib.h>  // malloc
#include <string.h>  // string management

#ifndef PMDLIST_H
#define PMDLIST_H
// Global Constants
extern const int STD_STR_LEN;
extern const int MAX_STR_LEN;



// Message codes for file access
enum MESSAGE_CODES { UNKNOWN_ERR = -1000, FILE_OPEN_ERR,
                   INCOMPLETE_FILE_ERR, NO_ERROR_MSG = 0,
                   END_FILE_FOUND_MSG, CONFIG_VALUE_ERR };

// global constants, forward declaration

// struct declaration
struct ProgramMetaDataNode
   {
    char commandLetter;
    char operationString[ 60 ];// hard-coded STD_STR_LEN since it is not declared yet
    int cycleTime;

    struct ProgramMetaDataNode *next;
   };


   // function prototypes
int isInString(char *strOfChar, char charToTest);
struct ProgramMetaDataNode *addNode( struct ProgramMetaDataNode *headNode, struct ProgramMetaDataNode *newNode );
struct ProgramMetaDataNode *clearList( struct ProgramMetaDataNode *headNode );
struct ProgramMetaDataNode *makeNode( char commandLetter, char *operationString,int cycleTime);
int getDataFromFile( struct ProgramMetaDataNode **headPtr, char *fileName );
#endif //PMDLIST_H
