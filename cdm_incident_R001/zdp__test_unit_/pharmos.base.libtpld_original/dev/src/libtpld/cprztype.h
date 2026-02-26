/*****************************************************************************/
/*                                                                           */
/*   header     : cprztype.h                                                 */
/*   Author     : A. Schmidt-Rehschuh (644)                                  */
/*   made       : 08.04.97                                                   */
/*   language   : C                                                          */
/*                                                                           */
/*   declaration: data-types                                                 */
/*                macros                                                     */
/*                                                                           */
/*****************************************************************************/

/*** DATA-TYPES **************************************************************/
#ifndef __CPRZTYP__1234
#define __CPRZTYP__1234

#ifndef __cplusplus
 #ifdef __sun
 #ifndef _STDC_C99
 #define _STDC_C99
 #endif
 #endif
 
 #include <stdbool.h>
#endif

#ifndef FALSE
 #define FALSE false
#endif
#ifndef TRUE
 #define TRUE true
#endif
#if defined (PPC543C50) && defined (__BOOL__)
 #else
 #if !defined (__BOOL__) && !defined (WIN32)  
 #define __BOOL__
/* typedef enum { FALSE, TRUE } bool; */
 #endif
 #endif
 typedef enum { D_STRING = 0, D_SHORT, D_INT, D_LONG, D_FLOAT, D_DOUBLE, D_CHAR } dataTyp;

/*** MACROS  *****************************************************************/

#define BOOL(x) ( x != 0 ? false : true )

 /* for using the follow macro you have to add sqlca.h and logfile.h  */
 /* to your sourcefile.a. Don't use a String in the macro!            */
 /* for example: CHECKSQL( fun( 1, str ) )							  */

 #ifdef SQLCA_INCL 
 #define __ESQL_C__
 #endif

 #ifdef ESQL_C
 #define __ESQL_C__
 #endif

 #ifdef __ESQL_C__
 #define CHECKSQL(x)  if ( !x ){	\
                         WriteLog("x", sqlca.sqlcode, "error in function" );\
 					     return; }
 #define CHECKISQL(x) if ( !x ){	\
                         WriteLog("x", sqlca.sqlcode, "error in function" );\
						 return -1; }
 #define CHECKBSQL(x) if ( !x ){	\
                         WriteLog("x", sqlca.sqlcode, "error in function" );\
						 return FALSE; }
 #define SQLLOG(x,y)  if ( sqlca.sqlcode < 0 ) { \
                         WriteLog( x, sqlca.sqlcode, "/%d %s", sqlca.sqlerrd[1],\
						           y ); return; }
 #define SQLILOG(x,y) if ( sqlca.sqlcode < 0 ) { \
                         WriteLog( x, sqlca.sqlcode, "/%d %s", sqlca.sqlerrd[1],\
						           y ); return -1; }
 #define SQLBLOG(x,y) if ( sqlca.sqlcode < 0 ) { \
                         WriteLog( x, sqlca.sqlcode, "/%d %s", sqlca.sqlerrd[1],\
						           y ); return FALSE; }
 #endif
#endif
