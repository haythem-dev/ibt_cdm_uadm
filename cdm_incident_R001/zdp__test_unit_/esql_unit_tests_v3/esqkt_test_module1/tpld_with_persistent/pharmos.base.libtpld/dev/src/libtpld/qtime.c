/******************************************************************************/
/*     Hauptmodul: shmem                                                      */
/*         Source: qtime.h                                                    */
/*          Autor: A. Schmidt-Rehschuh/W. Dittrich                            */
/*       Erstellt: 10.09.1996                                                 */
/*        Sprache: C                                                          */
/*        Erzeugt: manuell                                                    */
/*        Aufgabe: Tagesdatum/Uhrzeit liefern                                 */
/*                                                                            */
/******************************************************************************/

/*** INCLUDE ******************************************************************/

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <memory.h>
 #include "qtime.h"

/*** STRUCTUR *****************************************************************/

 struct HTIME {
    char dayS[4];
    char mon[4];
    char day[3];
    char time[9];
    char year[5];
    char ret[2];
    };

/*** DEFINITION ***************************************************************/

 #define MONTH  "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep",\
		"Oct", "Nov", "Dec"
 #define MONAT  "Jan", "Feb", "M{r", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep",\
		"Okt", "Nov", "Dez"

 #define HOUR   1
 #define MIN    2
 #define SECOND 3

/*** DECLARATION **************************************************************/

 static int Month( char * );
 static int Time( char *, int );

/*** TIME-OF-DAY **************************************************************/

 extern timeStr *TimeOfDay( time_t msec  )
 {
   static struct tmd d;
   struct HTIME h;
   char *str;
   int  i;
   int  len;
   char ch;

   str = ctime( &msec );
   
   len = (int)strlen( str );

   for ( i = 0; i < len; i++ )
   {
       if ( ( ch = *(str + i) ) == ' ' && *(str + i + 1) == ' ' )
       {
		  *(str + i) = '\0';
          *(str + i + 1) = '0';
	   }
	   else if ( ( ch = *(str + i) ) == ' ' || ch == '\0' )
		  *(str + i) = '\0';
   }

   memcpy( (char *)&h, str, len );
   d.day  = atoi( h.day );
   d.mon  = Month( h.mon );
   d.year = atoi( h.year );
   d.hour = Time( h.time, 1 );
   d.min  = Time( h.time, 2 );
   d.sec  = Time( h.time, 3 );

   return( &d );
 }

/*** TIME-OF-DAY **************************************************************/

 extern void DateTimeStr( dtime_s Str, timeStr *t )
 {
   sprintf( Str, "%02d/%02d/%02d %02d:%02d:%02d",  t->day, t->mon,
						   t->year,t->hour,
						   t->min, t->sec );
   return;
 }

/*** DAY-NUMBER-OF-WEEK *******************************************************/

 extern long DayNumberOfWeek( int date, int *dayNo )
 {
   struct tm d;          
   long msec;             
   char str[20];
   char hh[3];

   sprintf( str, "%08d", date );
   sprintf( hh, "%4.4s", str );
   d.tm_year = atoi( hh ) - 1900;        
   sprintf( hh, "%2.2s", str + 4 );
   d.tm_mon  = atoi( hh ) - 1;         
   sprintf( hh, "%2.2s", str + 6 );
   d.tm_mday = atoi( hh );        
   d.tm_sec  = 0;          
   d.tm_min  = 0;          
   d.tm_hour = 12;         
                          
   msec = (long)mktime( &d );     
                              
   *dayNo = d.tm_wday;

   return( msec );
 }
 
/*** GET-NEW-DATE *************************************************************/

 extern time_t GetNewDate( int date, const int iAddDay )
 {
   struct tm d;          
   char str[20];
   char hh[3];

   sprintf( str, "%08d", date );
   sprintf( hh, "%4.4s", str );
   d.tm_year = atoi( hh ) - 1900;        
   sprintf( hh, "%2.2s", str + 4 );
   d.tm_mon  = atoi( hh ) - 1;         
   sprintf( hh, "%2.2s", str + 6 );
   d.tm_mday = atoi( hh );        
   d.tm_sec  = 0;          
   d.tm_min  = 0;          
   d.tm_hour = 12;         
   return SetSecondToDate( (long)mktime( &d ) + (86400*iAddDay) );
 }

/*** ADD-TO-DATE *************************************************************/

 extern time_t AddToDate( int date, const int iAddDay )
 {
   struct tm d;          
   char str[20];
   char hh[3];

   sprintf( str, "%08d", date );
   sprintf( hh, "%4.4s", str );
   d.tm_year = atoi( hh ) - 1900;        
   sprintf( hh, "%2.2s", str + 4 );
   d.tm_mon  = atoi( hh ) - 1;         
   sprintf( hh, "%2.2s", str + 6 );
   d.tm_mday = atoi( hh );        
   d.tm_sec  = 0;          
   d.tm_min  = 0;          
   d.tm_hour = 12;         
   return SetSecondToDate( mktime( &d ) + (86400*iAddDay) );
 }

/*** SUB-TO-DATE *************************************************************/

 extern time_t SubToDate( int date, const int iAddDay )
 {
   struct tm d;          
   char str[20];
   char hh[3];

   sprintf( str, "%08d", date );
   sprintf( hh, "%4.4s", str );
   d.tm_year = atoi( hh ) - 1900;        
   sprintf( hh, "%2.2s", str + 4 );
   d.tm_mon  = atoi( hh ) - 1;         
   sprintf( hh, "%2.2s", str + 6 );
   d.tm_mday = atoi( hh );        
   d.tm_sec  = 0;          
   d.tm_min  = 0;          
   d.tm_hour = 12;         
   return SetSecondToDate( mktime( &d ) - (86400*iAddDay) );
 }

/*** GetCurrentDate **********************************************************/

 extern time_t GetMachineDate( void )
 {
   time_t lDate;
   struct tm *pTime;        
   time_t tTime;
    
   tTime = time( 0 );   
   pTime = localtime( &tTime );  
   lDate = (pTime->tm_year + 1900) * 10000 +   
           (pTime->tm_mon + 1) * 100 + pTime->tm_mday;	   
   return lDate;
 }
 
/*** SetSecondToDate *********************************************************/

 extern time_t SetSecondToDate( time_t time ) 
 {                                                                               
   time_t lDate;                                                                  
   struct tm *ptmTime;                                                                 
                                                                                
   ptmTime = localtime( &time );                                               
   lDate = (ptmTime->tm_year + 1900) * 10000 +
           (ptmTime->tm_mon + 1) * 100 + ptmTime->tm_mday;                      
                                                                                
   return lDate;                                                                                           
 }

/*** GetUnixTime *************************************************************/

 extern void GetUnixTime( tTime *dtTime )
 { 
   struct tm *tmTime;        
   time_t tTimeSet;
   
   tTimeSet = time(0L);   
   tmTime  = localtime( &tTimeSet ); 

   dtTime->iHour   = tmTime->tm_hour;
   dtTime->iMin    = tmTime->tm_min;
   dtTime->iSec    = tmTime->tm_sec;
   dtTime->iHHMM   = (tmTime->tm_hour * 100) + tmTime->tm_min;
   dtTime->iHHMMSS = (tmTime->tm_hour * 10000) + (tmTime->tm_min * 100)
	                 + tmTime->tm_sec;

   return;
 }

 /*** GetUnixSecond **********************************************************/

 extern time_t SetTimeToSecond( const long lDate, const long lTime )
 {
   struct tm tmTime;                                               
   time_t lSecond;
  
   tmTime.tm_year  = (lDate / 10000) - 1900;                                   
   tmTime.tm_mon   = ((lDate % 10000) / 100) - 1;                                          
   tmTime.tm_mday  = lDate % 100;                                             
   tmTime.tm_hour  = lTime / 10000;                                             
   tmTime.tm_min   = (lTime % 10000) / 100;                                              
   tmTime.tm_sec   = lTime % 100;                                              
   tmTime.tm_isdst = -1;    
   
   if( ( lSecond = mktime( &tmTime ) ) == -1 )                                       
       tmTime.tm_wday=7;                                           
   return lSecond;			           /* 00:00:00 and date from current day */
 }

/*** MONTH ********************************************************************/

 static int Month( char *mon )
 {
   char *Month[] = { MONTH };
   char *Monat[] = { MONAT };
   int i;

   for ( i = 1; i < 13; i++ )
   {
        if ( !strcmp( mon, Month[i - 1] ) )
           return i;
   }
   for ( i = 1; i < 13; i++ )
   {
       if ( !strcmp( mon, Monat[i - 1] ) )
      	  return i;
   }

   return 0;
 }

/*** TIME *********************************************************************/

 static int Time( char *time, int st )
 {
   char hh[3];

   switch( st )
     {
     case HOUR  : memcpy( hh, time, 2 ); break;
     case MIN   : memcpy( hh, time + 3, 2 ); break;
     case SECOND: memcpy( hh, time + 6, 2 ); break;
     default    : return 0;
     }

   hh[2] = '\0';

   return( atoi( hh ) );
 }
