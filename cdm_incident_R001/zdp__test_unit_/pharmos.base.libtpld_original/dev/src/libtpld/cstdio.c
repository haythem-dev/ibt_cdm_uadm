/*****************************************************************************/
/*                                                                           */
/*   main-module: pinload                                                    */
/*   header     : set.h                                                      */
/*   Author     : A. Schmidt-Rehschuh (644)                                  */
/*   made       : 20.03.97                                                   */
/*   language   : C                                                          */
/*                                                                           */
/*   declaration: insert/update from file into database.                     */
/*                data come from fu and ma (housno, articlebo, stockintrade) */
/*                                                                           */
/*****************************************************************************/

/***** INCLUDE ***************************************************************/

  #include <stdio.h>
  #include <stdlib.h>
  #include <memory.h>
  #include <signal.h>
  #include <errno.h>
  #include <time.h>
  #include "cstdio.h"
  #include "cprztype.h"

#ifdef _AIX
#ifdef __xlC__
typedef enum {false,true} bool;
#endif
#endif

/*** DECLARATION MEMBER-FUNCTION *********************************************/

  static bool CheckOnNumbers( char *spPtr, int iLen );
  static long GetNumberFromStr( char *spPtr, int iLen );
  static void Intrpt( int iSig );                   
  
/*****************************************************************************/
/* Stringhandling and convert-functions                                      */  
/*****************************************************************************/

/***** STOI (str to int without \0) ******************************************/
 			 
  int stoi( char *spPtr, int iLen )
  {
    if ( !CheckOnNumbers( spPtr, iLen ) )
       return atoi( spPtr );
    return (int)GetNumberFromStr( spPtr, iLen );
  }

/***** STOL (str to long without \0) *****************************************/

  int stol( char *spPtr, int iLen )
  {
    if ( !CheckOnNumbers( spPtr, iLen ) )
       return atol( spPtr );
    return (long)GetNumberFromStr( spPtr, iLen );
  }

/***** SZNCPY ( str to str with len plus \0) *********************************/

  char *szncpy( char *spPtr, char *spPart, int iLen )
  {
    memcpy( spPtr, spPart, iLen );
    *(spPtr + iLen) = '\0';

    return ( spPtr );
  }

/*****************************************************************************/
/* Signalhandling                                                            */  
/*****************************************************************************/

/***** SIGNAL ****************************************************************/
                                      
  int Signal( void )                     
  {                                      
#ifdef SIGHUP
    signal( SIGHUP,  SIG_IGN );
#endif
    signal( SIGINT,  SIG_IGN );

#ifdef SIGQUIT
    signal( SIGQUIT, SIG_IGN );
#endif

    if ( 0 != signal( SIGTERM, Intrpt ) )
	{
	   Interrupt( 0 );
       return 0;
	}  
	else
	  Interrupt( errno );   
    
	return errno;
  }                           

/***** INTERRUPT *************************************************************/

  int Interrupt( int iSig )
  {
    int iReturn;
    static int iInterrupt;

    iReturn = iInterrupt;
    iInterrupt = iSig;
    
    return iReturn;
  } 

/*****************************************************************************/
/* Timehandling and convert-functions                                        */  
/*****************************************************************************/

  extern long GetDateOfDay( void )
  {
	long lDate;
	struct tm *pTime;        
    time_t tTime;
    
	tTime = time( 0 );   
    pTime = localtime( &tTime );  
	lDate = (pTime->tm_year + 1900) * 10000 +   
            (pTime->tm_mon + 1) * 100 + pTime->tm_mday;	   
	return lDate;
  }

/*****************************************************************************/
/***** MEMBER-FUNCTION *******************************************************/
/*****************************************************************************/

/***** CHECK-ON-NUMBER *******************************************************/

  static bool CheckOnNumbers( char *spPtr, int iLen )
  {
    bool bResult = true;
	int  i;
 
	for ( i = *spPtr == '+'|| *spPtr == '-' ? 1 : 0; i < iLen; i++ )
	{
	if ( spPtr[i] < '0' || spPtr[i] > '9' )
	   {
	   spPtr[i] = '\0';
	   bResult = false;
	   break;
	   }
	}

    return bResult;
  }

/***** GET-NUMBER-FROM-STR ***************************************************/

  static long GetNumberFromStr( char *spPtr, int iLen )
  {
    long lResult;
    char *spStr;

    if ( ( spStr = (char *)malloc( (iLen+1) * sizeof(char) ) ) == (char *)0 )
       return -1;
    
	sprintf( spStr, "%*.*s", iLen, iLen, spPtr );
    lResult = atol( spStr );
    free( spStr );

    return lResult;
  }

/***** INTRPT (signalhandling intern ) ***************************************/

  static void Intrpt( int iSig )                   
  {                                      
    Interrupt( iSig );
    signal( SIGTERM, SIG_IGN );                             /* ignore signal */                                       
    signal( SIGTERM, Intrpt );               
  
    return;
  }                                      

/***** END *******************************************************************/
