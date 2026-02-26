/*****************************************************************************/
/*                                                                           */
/*   main-module: cstdio.c                                                   */
/*   header     : cstdio.h                                                   */
/*   Author     : A. Schmidt-Rehschuh (644)                                  */
/*   made       : 09.04.97                                                   */
/*   language   : C                                                          */
/*                                                                           */
/*   declaration: extension of standardfunction                              */
/*                                                                           */
/*****************************************************************************/

/*** DECLARATION *************************************************************/
#ifndef __CSTDIO__1234
#define __CSTDIO__1234

 #if defined(__CPLUS__) || defined(PPC543C50)
 extern "C" {
 #endif					

 /* stringhandling */ 
 int stoi( char *spPtr, int iLen );         /* str to int without \0  */
 int stol( char *spPtr, int iLen ); 		   /* str to long without \0 */
 char *szncpy( char *spPtr, char *spPart, int iLen ); /* strncpy + \0 */

 /* signalhandling */
 int Signal( void );                        /* activate signalhandling*/
 int Interrupt( int iSig );         /* reaction of signal if iSig = 0 */
                                           /* you can set sig. by your own if*/
                                           /* iSig != 0                      */ 
 /* timehandling */
 
 long GetDateOfDay( void ); 		          /* return date in yyyymmdd */
 #if defined(__CPLUS__) || defined(PPC543C50)
 }
 #endif					

#endif
/*** END-DECLARATION *********************************************************/
