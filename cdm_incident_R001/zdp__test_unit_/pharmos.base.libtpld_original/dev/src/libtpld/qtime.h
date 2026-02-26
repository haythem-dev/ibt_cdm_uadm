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

 #ifndef QTIME_H_INCLUDED
 #define QTIME_H_INCLUDED

/*** INCLUD *******************************************************************/

 #include <time.h>

/*** DEFINE *******************************************************************/

 #define L_DATETIME   18

/*** EIGENE STRUCTUR (DATENTYP) ***********************************************/

 typedef struct tmd  {
   int day;
   int mon;
   int year;
   int hour;
   int min;
   int sec;
   } timeStr;

 typedef struct {									
	 int iHour;
	 int iMin;
	 int iSec;
	 int iHHMM;
	 int iHHMMSS;
 } tTime;											/* Datumsstruktur DataW.*/

 typedef char dtime_s[L_DATETIME];

 #ifdef __CPLUS__
 extern "C" {
 timeStr *TimeOfDay( time_t msec  );
 void DateTimeStr( dtime_s Str, timeStr *t );
 long DayNumberOfWeek( int date, int *dayNo );
 void GetUnixTime( tTime *dtTime );
 time_t SetSecondToDate( time_t time ); 
 time_t SetTimeToSecond( const long lDate, const long lTime );
 time_t GetNewDate( int date, const int iAddDay );
 time_t SubToDate( int date, const int iAddDay );
 time_t AddToDate( int date, const int iAddDay );
 time_t GetMachineDate( void );
 }
 #else
 extern timeStr *TimeOfDay( time_t msec  );
 extern void DateTimeStr( dtime_s Str, timeStr *t );
 extern long DayNumberOfWeek( int date, int *dayNo );
 extern void GetUnixTime( tTime *dtTime );
 extern time_t SetSecondToDate( time_t time ); 
 extern time_t SetTimeToSecond( const long lDate, const long lTime );
 extern time_t GetNewDate( int date, const int iAddDay );
 extern time_t SubToDate( int date, const int iAddDay );
 extern time_t AddToDate( int date, const int iAddDay );
 extern time_t GetMachineDate( void );
 #endif

 #endif /* QTIME_H_INCLUDED */
