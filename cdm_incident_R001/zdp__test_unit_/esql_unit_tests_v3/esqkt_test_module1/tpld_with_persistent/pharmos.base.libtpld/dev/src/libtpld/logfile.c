/*****************************************************************************/
/*                                                                           */
/*   header     : logfile.c                                                  */
/*   Author     : A. Schmidt-Rehschuh (644)                                  */
/*   made       : 04.04.97                                                   */
/*   language   : C                                                          */
/*                                                                           */
/*   declaration: the default focus use:									 */
/*                $LOG_PATH enviroment variable for directory                */
/*                 wws.log for logfile                                        */
/*                                                                           */
/*                for changing default use the function                      */
/*                SetFileName and SetDirName in front                        */
/*                                                                           */
/*                use the function WriteLog similar the function printf      */
/*                the first three deliveries are forcible:                   */
/*                -> WriteLog( "main", errno, "start Prog" ); or             */
/*                -> WriteLog( "main", errno, "%s", "start Prog" );          */
/*                                                                           */
/*****************************************************************************/

/*** INCLUDE *****************************************************************/

 #include <stdio.h>
 #include <time.h>
 #include <errno.h>
 #include <stdarg.h>
 #include <string.h>
 #include <signal.h>
 #include <malloc.h>
 #include <stdlib.h>
 
 #ifdef _AIX
 #include <unistd.h>
 #endif

#ifdef _MSC_VER
#include <process.h>
#define getpid _getpid
#endif
 
 #include "logfile.h"

/*** DEFINE ******************************************************************/

 #define DEFAULT_LOGFILE "wws.log"                                          
 #define GET_FREE 		 (char *)1
 #define GET_VALUE 	     "###"
 #define LEN_VALUE       3  
 #define T_REFERENCE     'H'
 #define T_WARNING       'W'
 #define T_ERROR         'F'
 #define L_STRLEN		 256

/*** DECLARATION MEMBER-FUNCTION *********************************************/

 static char GetTypeOfReference( int iErrno );
 static char *GetFileName( void );
 static char *GetTimeStamp( void );
 static char *GetProgramName( void );
 
/*** SET-PROGRAM-NAME ********************************************************/

 char *SetProgramName( const char *spProg )                 
 {                                   
   int i;                                             
   static char *spzProgName = (char *)0;
   
   if ( spProg == GET_FREE )
   {
      free( spzProgName );
      return (char *)0;
   }
   else if ( !memcmp( spProg, GET_VALUE, LEN_VALUE ) && spzProgName )
   {
      return spzProgName;
   }

   if ( spProg == (char *)0 )
   {
      spProg = "logfile";
   }
   
   const char* p = spProg + strlen( spProg ) - 1;                       
   for( i = (int)strlen( spProg ) - 1; i >= 0; i--, p-- )       
   {                                                  
      if ( *p == '/' || *p == '\\' )                  
      {                                               
         spProg = p + 1;                                 
         break;                                       
      }                                               
   }                                                  
                                                      
   if ( spzProgName == (char *)0 )
   {
      spzProgName = (char *)malloc( ( strlen( spProg ) + 1 ) * sizeof( char ) );
      strcpy( spzProgName, spProg );
   }
   else if ( memcmp( spzProgName, spProg, strlen(spProg) ) )
   {
      free( spzProgName );
      spzProgName = (char *)malloc( ( strlen( spProg ) + 1 ) * sizeof( char ) );
      strcpy( spzProgName, spProg );
   }
   
   return spzProgName;                                        
 }                                                     	 

/*** SET-FILE-NAME ***********************************************************/

 char *SetFileName( const char *spFile )                                          
 {                                                                          
   int iLen;
   static char *spzFileName = (char *)0;

   if ( spFile == GET_FREE )
   {
      free( spzFileName );
      return (char *)0;
   }

   if ( spFile == NULL )
      iLen = 0;
   else
      iLen = (int)strlen(spFile);

   if ( iLen == 0 && spzFileName == (char *)0 )
      spzFileName = DEFAULT_LOGFILE;
   else if ( iLen != 0 && spzFileName == (char *)0 )
   {
      spzFileName = (char *)malloc( (iLen * sizeof( char )) + 1 );
      if ( spzFileName == (char *)0 )
         return (char *)0;
      strcpy( spzFileName, spFile );
   }
   else if ( iLen != 0 && spzFileName != (char *)0 )
      strcpy( spzFileName, spFile );

   return spzFileName;
                                                     
 }

/*** SET-DIR-NAME ************************************************************/

 char *SetDirName( char *spDir )                                          
 {                                                                          
   int iLen;
   static char *spzDirName = (char *)0;

   if ( spDir == GET_FREE )
   {
      free( spzDirName );
      return (char *)0;
   }

   if ( spDir == NULL)
      iLen = 0;
   else
      iLen = (int)strlen(spDir);

   if ( iLen == 0 && spzDirName == (char *)0 )
   {
      if ( ( spzDirName = (char *)getenv( "LOG_PATH" ) ) == (char *)0 )
      spzDirName = ".";
   }
   else if ( iLen != 0 && spzDirName == (char *)0 )
   {
      spzDirName = (char *)malloc( (iLen * sizeof( char )) + 1 );
      if ( spzDirName == (char *)0 )
         return (char *)0;
      strcpy( spzDirName, spDir );
   }

   return spzDirName;
                                                      
 }

/*** SET-NUMBER-OF-BRANCH *****************************************************/

 int SetNumberOfBranch( int iNumber )                                          
 {                                                                          
   static int iBranch = 0;                                                
                                                                           
   if ( iNumber == 0 && iBranch == 0 )      
      {
	  char *str;
	  if ( ( str = getenv( "BRANCH_NO" ) ) == NULL )
	     return -1;
	  if ( ( iBranch = atoi( str ) ) == 0 )
	     return -1;
      }
   else if ( iNumber > 0 ) 
      iBranch = iNumber;
                                                                           
   return iBranch;                                                      
 }

/*** GET-CURRENT-DATE ********************************************************/

 long GetCurrentDate( void )
 {
   char szTime[9];
   struct tm *ptrTm;
   time_t lmSec;

   lmSec = time( NULL );
   ptrTm = localtime( &lmSec );

   sprintf( szTime, "%04d%02d%02d",
	        ptrTm->tm_year + 1900, 
			ptrTm->tm_mon + 1,
			ptrTm->tm_mday );
   return atol( szTime );
 }

/*** GET-CURRENT-TIME ********************************************************/

 long GetCurrentTime( void )
 {
   char szTime[9];
   struct tm *ptrTm;
   time_t lmSec;

   lmSec = time( NULL );
   ptrTm = localtime( &lmSec );

   sprintf( szTime, "%02d%02d%02d",
	        ptrTm->tm_hour, 
			ptrTm->tm_min,
			ptrTm->tm_sec );
   return atol( szTime );
 }

/*** END-LOG *****************************************************************/

 void EndLog( void )
 {
   SetFileName( GET_FREE );
   SetDirName( GET_FREE );
   SetProgramName( GET_FREE );
   return;
 }

/*** WRITE-LOG ***************************************************************/

 int WriteLog( const char *spFunctionName, int iErrno, const char *spForm, ... )
 {
   FILE *fpFile;
   va_list ptr_vaList;
  
   if ( ( fpFile = fopen( GetFileName(), "a" ) ) == (FILE *)0 )
	  return -1;
   
   va_start( ptr_vaList, spForm );
   fprintf( fpFile, "%c %02d %s pid=%05d %-8.8s %s %d ", 
	        GetTypeOfReference( iErrno ),
	        SetNumberOfBranch( 0 ),
			GetTimeStamp(),
			getpid(),
			GetProgramName(),
			spFunctionName,
			iErrno );
   vfprintf( fpFile, spForm, ptr_vaList );
   fprintf( fpFile, "\n" ); 
   va_end( ptr_vaList );
   fclose( fpFile );
   
   return 1;
 }

/*****************************************************************************/
/***** MEMBER-FUNCTION *******************************************************/
/*****************************************************************************/

/***** GET-TIME-STAMP ********************************************************/

 static char *GetTimeStamp( void )
 {
   static char szTimeStr[21];
   struct tm *ptrTm;
   time_t lmSec;

   lmSec = time( NULL );
   ptrTm = localtime( &lmSec );

   sprintf( szTimeStr, "%02d.%02d.%02d %02d:%02d:%02d",
	   ptrTm->tm_mday, ptrTm->tm_mon + 1, (ptrTm->tm_year+1900)%100,
	   ptrTm->tm_hour, ptrTm->tm_min, ptrTm->tm_sec );

   return szTimeStr;
 }

/***** GET-TIME-STAMP ********************************************************/

 static char GetTypeOfReference( int iErrno )
 {
  char cCh;	

  cCh = iErrno < 0 ? T_ERROR : ( iErrno > 0 ? T_WARNING : T_REFERENCE );
  return cCh;
 }

/***** GET-FILE-NAME *********************************************************/

 static char *GetFileName( void )
 {
   static char spzFileName[L_STRLEN];
   
   if ( strlen( strcpy( spzFileName, SetDirName( (char *)0 ) ) ) > L_STRLEN )
	  return (char *)0;
   if ( strlen( strcat( spzFileName, "/") ) > L_STRLEN )
	  return (char *)0;
   if ( strlen( strcat( spzFileName, SetFileName( (char *)0 ) ) ) > L_STRLEN )
	  return (char *)0;
	 
   return spzFileName;
 }

/***** GET-FILE-NAME *********************************************************/

 static char *GetProgramName( void )
 {
   return SetProgramName( GET_VALUE );
 }

/***** END *******************************************************************/
