
/*****************************************************************************/
/*                                                                           */
/*   header     : logfile.h                                                  */
/*   Author     : A. Schmidt-Rehschuh (644)                                  */
/*   made       : 04.04.97                                                   */
/*   language   : C                                                          */
/*                                                                           */
/*   declaration: the default focus use:									 */
/*                $BRANCH_NO enviroment variable for number of branch/house  */
/*                $LOG_PATH  enviroment variable for directory               */
/*                wws.log    for logfile                                     */
/*                                                                           */
/*                when BRANCH_NO not set, use ->SetNumberOfBranch            */
/*                for changing default use the function                      */
/*                ->SetFileName and ->SetDirName in front                    */
/*                                                                           */
/*                use the function WriteLog similar the function printf      */
/*                the first three deliveries are forcible:                   */
/*                -> WriteLog( "main", errno, "start Prog" ); or             */
/*                -> WriteLog( "main", errno, "%s", "start Prog" );          */
/* 				  set in type of front H, W or F (reference, warning, error) */
/*                                                                           */
/*****************************************************************************/

/*** DECLARATION *************************************************************/

#ifndef __LOGFILE__1234
#define __LOGFILE__1234

 #ifdef __CPLUS__
 extern "C" {
  int   WriteLog( const char *spFunctionName, int iErrno, const char *spForm, ... );
  int   SetNumberOfBranch( int iNumber );       /* set number of branche      */                                          
  char *SetFileName( const char *spFile );            /* set name of logfile        */                                                 
  char *SetDirName( char *spDir );              /* set directory for logfile  */                                                 
  char *SetProgramName( const char *spProg );			/* set the name of program    */
  void  EndLog( void );				            /* give alloc free            */
  long  GetCurrentDate( void );                 /* return yyyymmdd            */
  long  GetCurrentTime( void );					/* return hhmmss              */
 }
 #else
 extern int WriteLog( const char *spFunctionName, int iErrno, const char *spForm, ... );
 extern int   SetNumberOfBranch( int iNumber ); /* set number of branche      */                                          
 extern char *SetFileName( const char *spFile );      /* set name of logfile        */                                                 
 extern char *SetDirName( char *spDir );        /* set directory for logfile  */                                                 
 extern char *SetProgramName( const char *spProg );	/* set the name of program    */
 extern void  EndLog( void );				    /* give alloc free			  */
 extern long  GetCurrentDate( void );           /* return yyyymmdd            */
 extern long  GetCurrentTime( void );			/* return hhmmss              */
 #endif

#endif
/*** END OF DECLARATION ******************************************************/


