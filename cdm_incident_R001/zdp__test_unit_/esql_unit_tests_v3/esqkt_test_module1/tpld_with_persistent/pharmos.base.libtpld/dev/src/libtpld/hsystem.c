

/************************************************************************************/
/*       Systemaufruf mit returncode               hsystem.c                        */
/************************************************************************************/

/*** INCLUDE ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>

#ifdef _AIX
#include <unistd.h>
#endif

#ifdef _MSC_VER
#include <process.h>
#define execvp _execvp
int fork() { return -1; }
int wait(int* c) { c; return -1; }
#endif

#include "hsystfkt.h"

/*** DEFINE *************************************************************************/

#define ANZPAR 20

/*** H-SYSTEM ************************************************************************/

int h_system( char *prstr )
{
	char *pr;
    char *prend;
    char *pra;
    char *exarg[ANZPAR];
    int  sohn;
    int  retstat = 0;
    int  i;

    if ( prstr == (char *)0 || *prstr == '\0')
	   return( -99 );
    
	pr = prstr;
    while ( !( *pr == '<' || *pr == '>' || *pr == '|' || *pr == '\0' ) ) pr++;
    
	if ( *pr )
       {
	   if ( ( retstat = system( prstr ) ) == 256 )
          retstat = -100-2;
	      else
	      {
	      retstat /= 256;
	      if ( retstat >= 128 ) retstat -= 256;
	      }
	   return( retstat );
       }
    
	prend = pr;
    pr = prstr;
    i = 0;
    while ( *pr && i < ANZPAR )
      { 
	  while ( *pr && *pr == ' ' ) pr++;
	  if ( *pr != '\0' )
	     { 
		 pra = pr;
	     while ( *pr && *pr != ' ' ) pr++;
	     if ( *pr == ' ' )
			*pr++ = '\0';
	     exarg[i++] = pra;
	     }
      }
    
	if ( i < ANZPAR )
       {
	   exarg[i] = (char *)0;
	   switch( sohn = fork() )
	     {
	     case -1: retstat = -100 - errno; break;
	     case  0: if ( execvp( exarg[0], exarg ) == -1 ) 
					 exit( -100-errno );
		          break;
	     default: while (wait(&retstat) != sohn) ;
		          break;
	     }
       }
       else
       { 
	   retstat = -100*256;
       }
    
	pr = prstr;
    while ( pr < prend )
      { 
	  if ( *pr == '\0' )
		 *pr = ' ';
      pr++;
      }

    retstat /= 256;
    if ( retstat >= 128 )
	   retstat -= 256;
  
    return( retstat );
  }
