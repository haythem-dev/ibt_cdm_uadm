/*****************************************************************************/
/*                                                                           */
/*   Modul-Name            : lorbas                                          */
/*   Projekt/Produkt       : Fehlerbehandlung SQL                            */
/*   Hauptmodul            : lorbas.c                                        */
/*   Quelle                : allgemein.c                                     */
/*   Sprache               : c                                               */
/*                                                                           */
/*   Autorname             : Schmidt-Rehschuh, Alexander                     */
/*   Abt./Tel.-Nr.         : Zentrale Programmierung / 644                   */
/*   Datum letzte Aenderung: 24.09.1996                                      */
/*   Freigabedatum         :                                                 */
/*   Betriebssystem        : SCO Unix                                        */
/*                                                                           */
/*   Kurzbeschreibung      : Modul beinhaltet allgemein nutzbare Funktionen  */
/*                                                                           */
/*****************************************************************************/

/******** INCLUDE ************************************************************/

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 #ifdef _AIX
 #include <unistd.h>
 #endif

#ifdef _MSC_VER
#include <process.h>
#define popen _popen
#define pclose _pclose
#define getpid _getpid
#endif
 
 #include <time.h>
 #include "struct.h"

/******** DEFINE *************************************************************/

/*  #include "define.h" */   /* hajo jetzt in allgemein.h */
 #define  BIT_VERBUNDPOS_GEBUCHT   10
 #define  BIT_VERBUNDPOS_INAUFTRAG 14
 #define  PS_COMMAND   "ps -ef | grep %s | awk -v P=%s '{ if ( P == $8 ) print $2 }'"

/******** GLOBALE ************************************************************/

 extern int debug_enabled;

/******** DECLARATION ********************************************************/

 #define ALLGEMEIN
 #include "allgemein.h"

/******** CONVERT-TIME *******************************************************/

 void ConvertTime( char *spTime, const char* spInTime, const int iLen )
 {
   long lTime;

   if ( (lTime=atol(spInTime)) > 240000 ) /* must be written with millsec */
   {
      sprintf( spTime, "%0*ld", iLen, lTime / 1000 );  /* takeaway the millisec */
   }
   else
   {
       sprintf( spTime, "%0*ld", iLen, lTime );
   }
   return;
 }

/******** HOLE-DATUM-ZEIT ****************************************************/

 void HoleDatumZeit( char *str, int rc )
 {
   struct tm *localtime();
   struct tm *d;
   time_t hour1;

   hour1 = time(&hour1);
   d = localtime(&hour1);

   switch( rc )
   {
     case HOLE_DATUM:
          sprintf( str, "%02d.%02d.%02d", d->tm_mday, d->tm_mon + 1, (d->tm_year+1900)%100 );
          break;
     case HOLE_UHRZEIT:
          sprintf( str, "%02d:%02d:%02d", d->tm_hour, d->tm_min, d->tm_sec );
          break;
     case HOLE_DATUM_ZEIT:
          sprintf( str, "%02d.%02d.%02d %02d:%02d:%02d",
                        d->tm_mday, d->tm_mon + 1, (d->tm_year+1900)%100,
                        d->tm_hour, d->tm_min, d->tm_sec );
          break;
   }
   return;
 }

/******** HOLE-DATUM *********************************************************/

 void HoleDatum( char *str, int tausender )
 {
   struct tm *localtime();
   struct tm *d;
   time_t hour1;
   short sDate;
   const char *fun = "HoleDatum";

   printf("Allgemein::Holedatum[ str ]: > %s %s\n", str, "<");
   printf("Allgemein::Holedatum[ tausender ]: > %d %s\n", tausender, "<");
   
   
   hour1 = time(&hour1);
   d = localtime(&hour1);
  
   sDate = (d->tm_year + 1900) & 0xFFFF;
   printf("Allgemein::Holedatum VOR dem if [ sDate ]: > %d %s\n", sDate, "<");

   if ( tausender )
      sprintf( str, "%04d%02d%02d", sDate, d->tm_mon + 1, d->tm_mday );
   else
      sprintf( str, "%02d%02d%02d", sDate%100, d->tm_mon + 1, d->tm_mday );
  
    printf("Allgemein::Holedatum NACH dem if [ sDate ]: > %d %s\n", sDate, "<");

   DEBUG_PRINT( fun, str, atoi(str), 0 )      
   return;
 }


/******** HOLE-ZEIT **********************************************************/

 void HoleZeit( char *str, int rc )
 {
   struct tm *localtime();
   struct tm *d;
   static short ms = 0;
   time_t hour1;

   hour1 = time(&hour1);
   d = localtime(&hour1);

   if ( rc == MIT_SEKUNDE )
   {
      sprintf( str, "%02d%02d%02d%03d", d->tm_hour, d->tm_min, d->tm_sec, ms );
      if ( ms++ > 999 )
         ms = 0;
   }
   else
      sprintf( str, "%02d%02d", d->tm_hour, d->tm_min );
   return;
 }

/******** SETZE-DATUM-OHNE-JAHR **********************************************/

 void SetzeDatumOhneJahr( char *datum, int len )
 {
   if ( atol( datum ) > 991231 && atol( datum ) != 0 )
   {
      char huelse[L_INTEGER];
      memcpy( huelse, datum + 2, len );
      memset( datum, BLANC, len );
      memcpy( datum, huelse, len );
   }

   return;
 }

/******** SETZE-DATUM-MIT-JAHR ***********************************************/

 void SetzeDatumMitJahr( char *datum, int len )
 {
   struct tm *localtime();
   struct tm *d;
   time_t hour1;

   hour1 = time(&hour1);
   d = localtime(&hour1);

   datum[len] = STR_ENDE;
   if ( atol( datum ) <= 701231 && atol( datum ) != 0 )
   {
      int iHHMM = atol(datum) % 10000;
      sprintf( datum, "%04ld%04d", 2000 + (atol( datum )/10000)%1000, iHHMM );
   }
   else if ( atol( datum ) <= 991231 && atol( datum ) != 0 )
   {
      int iHHMM = atol(datum) % 10000;
      sprintf( datum, "%04d%04d", d->tm_year + 1900, iHHMM );
   }

  return;
 }

/******** HOLE-TAGES-TYP *****************************************************/

 void HoleTagesTyp( char *str, int rc  )
 {
   const char *fun = "HoleTagesTyp";

   DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

   switch ( rc )
   {
     case 1: strcpy( str, TYP_MONTAG ); break;
     case 2: strcpy( str, TYP_DIENSTAG ); break;
     case 3: strcpy( str, TYP_MITTWOCH ); break;
     case 4: strcpy( str, TYP_DONNERSTAG ); break;
     case 5: strcpy( str, TYP_FREITAG ); break;
     case 6: strcpy( str, TYP_SAMSTAG ); break;
     case 7: strcpy( str, TYP_SONNTAG ); break;
   }

   return;
 }

/******** SETZE JA NEIN SCHALTER *********************************************/

 void SetzeJaNeinSchalter( char *ptr )
 {
    if ( *ptr != BLANC )
       *ptr = *ptr == EIN ? JA : NEIN;

    return;
 }

/******** SETZE EINS NULL SCHALTER *******************************************/

 void SetzeEinsNullSchalter( char *ptr )
 {
    if ( *ptr != BLANC )
       *ptr = *ptr == JA ? EIN : AUS;

    return;
 }

/*** STRING-RECHTSBUENDIG *****************************************************/

  char *Rbuendig( char *str, struct AUFB *d )
  {
    int i = 0;
    const char *fun = "StringRechtsbuendig";
    char huelse[80] = {0};

    /* save original beginning of str */
    char * org_str = str;

    if ( d->negativ == JA_NEIN )
       *str = *str == ZAHL_NULL ? NEIN : JA;
  
    if ( d->typ == PACKED || d->typ == STRINO ||
         d->typ == HALBWO || d->typ == VOLLWO || d->typ == NUMERC )
    {
       if ( *str == MINUS )
       {
          d->negativ = YES;
		  /*skip leading minus character*/
          str++;
       }
       else
       {
            d->negativ = NO;
            if ( *str == PLUS )
			{
				/*skip leading plus character*/
                str++;
			}
       }
       
	   /*look last non-blanc non-end character in string*/
       for ( i = ( d->laenge - 1 ); i >= 0; i-- )
       {
           while(str[i] == STR_ENDE  && i >= 0)
           {
		 /* str[i] = BLANC; */
			 /* since setting str[i] to BLANC has only one effect: ignoring terminating \0
				let's try something else: */
			--i;
	   }
           if ( str[i] != BLANC )
              break;
       }

       if ( (++i) < d->laenge )
       {
			memcpy( huelse + (d->laenge - i), str, i );
			/*fill the beginning of huelse with '0'*/
			memset( huelse, '0', d->laenge - i );
			memcpy( org_str, huelse, d->laenge );
       }

       if ( d->nachkomma > 0 )
       {
			/*make sure string hat d->nachkomma after digit places*/
			/*1 look for delimter*/
			int decimal_digits = 0;
			int position_decimal_delim = -1;
			for ( i = (int)strlen(org_str); i >= 0; --i)
			{
				if (org_str[i] == STR_ENDE)
				{
					/*do not count \0*/
					continue;					
				}
				if (org_str[i] == PUNKT || org_str[i] == KOMMA)
				{
					break;
				}
				++decimal_digits;
			}
			/* i is position of delim */
			
			position_decimal_delim = i;
			
			if (decimal_digits < d->nachkomma)
			{
				/*action is required*/
				
				/*clear huelse of previous values*/
				memset(huelse, '\0', sizeof(huelse));
				/*find out how much decimal digits are missing*/				
				int missing_decimal_digits = d->nachkomma - decimal_digits;
				
				memcpy(huelse, org_str, d->laenge);
				/*append missing_decimal_digits to huelse after position position_decimal_delim + decimal_digits*/
				/*huelse + position_decimal_delim + decimal_digits is the position of the lase valid character in string */
				memset(huelse + position_decimal_delim + decimal_digits + 1, '0', missing_decimal_digits);
				/*copy the last d-laenge bytes from huelse to org_str - AufbereitenString will take care of non number characters*/
				/*since huelse has been cleared, using strlen will work (needs \0 terminated string)*/
				size_t raw_str_len = strlen(huelse);
				int offset = 0;
				
				/*since raw_str_len can at most be 80, it's safe casting it to int*/
				if ((int)raw_str_len > d->laenge)
				{
					offset = (int)raw_str_len - d->laenge;
				}
				
				memcpy(org_str, huelse + offset, d->laenge);
				
			}
			/*AufbereitenString moves string one byte to the left in case of too long string*/
			AufbereitenString( org_str, d->laenge );
       }
    }

    DEBUG_SECTION
    sprintf( huelse, "Nach(%c):#%-*.*s#", d->typ, d->laenge, d->laenge, str );
    DEBUG_PRINT( fun, huelse, d->laenge, i );
    END_DEBUG_SECTION
    
    return org_str;
  }

 /***** AUFBEREITEN-STRING ***************************************************/

 void AufbereitenString( char *str, int len )
 {
   int  i;
   int  zaehler;
   char huelse[L_TEXT] = {0};
   const char *fun = "AufbereitenString";

   DEBUG_PRINT( fun, "wird aufgerufen", 0, 0 )
  
   if ( len > L_TEXT )
   {
      #ifdef _XCICS
      char error_msg[81];
      sprintf( error_msg, "Länge überschritten: %d", len );
      PruefeAbfrage( fun, error_msg, FEHLER );
      #endif
      return;
   }

   zaehler = len - 1;
   for ( i = len; i >= 0; i-- )
   {
       if ( str[i] == STR_ENDE || str[i] == BLANC )
          continue;
       else if ( str[i] == PLUS || str[i] == KOMMA || str[i] == PUNKT )
          huelse[0] = ZAHL_NULL;
       else
          huelse[zaehler--] = str[i];
   }
   memcpy( str, huelse, len );
   return;
 }

 /***** STR-TO-BITVEKTOR *****************************************************/

 Bitleiste StrToBitvektor( char *str, Bitleiste *bitv )
 {
   int len;
   int i;
   const char *fun = "StrToBitvektor";

   DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

   if ( (len = (int)strlen(str)) > L_BITLEISTE )
      len = L_BITLEISTE;

   *bitv = 0;
   for ( i = 0; i < len; i++ )
   {
       if ( str[i] == '1')
       {
          *bitv <<= 1;
          *bitv |= 1;
       }
       else
       {
          *bitv <<= 1;
          *bitv |= 0;
       }
   }
   return *bitv;
 }

 /***** BITVEKTOR-TO-STR *****************************************************/

 int BitvektorToStr( char *str, int slen, Bitleiste bitv )
 {
   int i;
   Bitleiste bit;
   const char *fun = "BitvektorToStr";

   DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

   if ( slen < L_BITLEISTE )
      return ERROR;

   *str = STR_ENDE;
   bit = bitv;

   for ( i = 0; i < L_BITLEISTE; i++ )
   {
       if ( bit % 2 )
          str[L_BITLEISTE - 1 - i] = EIN;
       else
          str[L_BITLEISTE - 1 - i] = AUS;
       bit = bit >> 1;
   }
   str[L_BITLEISTE] = STR_ENDE;
   return OK;
 }

/***** IST-VERBUND-POS ******************************************************/

 int IstVerbundPos( Bitleiste bitl )
 {
   char str[L_BITLEISTE + 1];
   int rc = 0;
   const char *fun = "IstVerbundPos";
   
   DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

   BitvektorToStr( str, L_BITLEISTE, bitl );
  
   if ( str[BIT_VERBUNDPOS_GEBUCHT] == '1' )
      rc = atoi( TYP_VERBUND );
   else if( str[BIT_VERBUNDPOS_INAUFTRAG] == '1' )
      rc = atoi( TYP_BATCH );
   else rc = 0;
   DEBUG_PRINT( fun, "Verbund (y>0/n=0):", rc, 0 )
  
   return rc;
 }

 /***** IST-VERBUND-POS ******************************************************/

 int IstOpiat( Bitleiste bitl )
 {
   char str[L_BITLEISTE + 1];
   const char *fun = "IstOpiat";
   
   DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

   BitvektorToStr( str, L_BITLEISTE, bitl );
  
   if ( str[BIT_OPIAT] == '1' )
      return 1;
   return 0;
 }
 
/***** HOLE-FELDLAENGE ******************************************************/

 int HoleFeldlaenge( int typ, int laenge )
 {
   if ( typ == PACKED )
      laenge = ( laenge + 1 ) / 2;
   if ( typ == HALBWO )
      laenge = 2;
   if ( typ == VOLLWO )
      laenge = 4;
   if ( typ == LONGST )
      laenge = 4;
   return laenge;
 }

 /***** HOLE-NACHKOMMA *******************************************************/

 int HoleNachkomma( int typ, int nachkomma )
 {
   if ( typ != PACKED )
      nachkomma = 0;

   return nachkomma;
 }

/*** HOLE-KD-AUFTRAG-STATUS ***************************************************/

  char HoleKdAuftragStatus( char *status_leiste, int len, int typ )
  {
    static char merker[L_TEXT + 1];
    const char *fun = "HoleKdAuftragStatus";

    DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

    if ( typ == HOLE )
       {
       memcpy( status_leiste, merker, len );
       DEBUG_SECTION
       status_leiste[len] = STR_ENDE;
       DEBUG_PRINT( "HOLE (STATUS)", status_leiste, 0, 0 )
       END_DEBUG_SECTION
       return ZAHL_NULL;
       }

    if ( typ == SCHREIBE )
       {
       DEBUG_SECTION
       status_leiste[len] = STR_ENDE;
       DEBUG_PRINT( "SCHREIBE (STATUS)", status_leiste, 0, 0 )
       END_DEBUG_SECTION
       memcpy( merker, status_leiste, len );
       }
    
    
    if ( status_leiste[AUFTRAG_FAKTURIERT] == EIN &&
         status_leiste[AUFTRAG_STORNIERT] == EIN )
       return KZ_AUFTRAG_STORNIERT_TAG;
    else if ( status_leiste[AUFTRAG_NICHT_AENDERBAR] == EIN )
       return KZ_AUFTRAG_NICHT_AENDERBAR;
    else if ( status_leiste[AUFTRAG_STORNIERT] == EIN )
       return KZ_AUFTRAG_STORNIERT;
    else if ( status_leiste[AUFTRAG_FAKTURIERT] == EIN )
       return KZ_AUFTRAG_FAKTURIERT;
    else if ( status_leiste[AUFTRAG_FAKTUR_BEREIT] == EIN )
       return KZ_AUFTRAG_FAKTUR_BEREIT;
    else if ( status_leiste[KOMMI_ABGESCHLOSSEN] == EIN )
       return KZ_KOMMI_ABGESCHLOSSEN;
    else if ( status_leiste[KOMMI_BEGONNEN] == EIN )
       return KZ_KOMMI_BEGONNEN;
    else if ( status_leiste[ERFASSUNG_ABGESCHLOSSEN] == EIN )
       return KZ_ERFASSUNG_ABGESCHLOSSEN;
    else if ( status_leiste[ERFASSUNG_BEGONNEN] == EIN )
       return KZ_ERFASSUNG_BEGONNEN;
    else return KZ_ERFASSUNG_BEGONNEN; /*'9';*/
  }

/*** SETZE-KD-AUFTRAG-STATUS **************************************************/

  void SetzeKdAuftragStatus( char *status_leiste, char *status )
  {
    const char *fun = "SetzeKdAuftragStatus";

    DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

    if ( *status == KZ_AUFTRAG_STORNIERT_TAG )
       status_leiste[AUFTRAG_STORNIERT] = EIN;
    else if ( *status == KZ_AUFTRAG_NICHT_AENDERBAR )
       status_leiste[AUFTRAG_NICHT_AENDERBAR] = EIN;
    else if ( *status == KZ_AUFTRAG_STORNIERT )
       status_leiste[AUFTRAG_STORNIERT] = EIN;
    else if ( *status == KZ_AUFTRAG_FAKTURIERT )
       status_leiste[AUFTRAG_FAKTURIERT] = EIN;
    else if ( *status == KZ_AUFTRAG_FAKTUR_BEREIT )
       status_leiste[AUFTRAG_FAKTUR_BEREIT] = EIN;
    else if ( *status == KZ_KOMMI_ABGESCHLOSSEN )
       status_leiste[KOMMI_ABGESCHLOSSEN] = EIN;
    else if ( *status == KZ_KOMMI_BEGONNEN )
       status_leiste[KOMMI_BEGONNEN] = EIN;
    else if ( *status == KZ_ERFASSUNG_ABGESCHLOSSEN )
       status_leiste[ERFASSUNG_ABGESCHLOSSEN] = EIN;
    else if ( *status == KZ_ERFASSUNG_BEGONNEN )
       status_leiste[ERFASSUNG_BEGONNEN] = EIN;
    else return;

    return;
  }

/*** RECHNE-VOLUMEN ***********************************************************/

  int RechneVolumen( char *laenge, char *hoehe, char *breite, char *erg, int iSize )
  {
    int volumen;
    char d[L_INTEGER];
    const char *fun = "RechneVolumen";

    DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

    volumen = ( ( atoi( laenge ) * atoi( hoehe ) * atoi( breite ) ) / 1000 );
    if ( volumen == 0 )
       volumen = STANDARD_VOLUMEN;

    sprintf( d, "%d", volumen );
    memset( erg, ' ', iSize ); 
    memcpy( erg, d, strlen( d ) );

    return volumen;
  }

/*** SETZE-POSNR-PLUS-ZEHN ****************************************************/

  int SetzePosNrPlusZehn( char *posnr, int len )
  {
    char *huelse;
    const char *fun = "SetzePosPlusZehn";

    DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

    if ( ( huelse = (char *)malloc( ( sizeof( char ) * len ) + 1 ) ) == NULL )
       return ERROR;

    sprintf( huelse, "%*.*s", len, len, posnr );
    sprintf( posnr, "%0*d", len, atoi( huelse ) * TEXT_POSNR );
    free( huelse );

    return OK;
  }

/*** SETZE-POSNR-PLUS-EINS ****************************************************/

  int SetzePosNrPlusEins( char *posnr, int len )
  {
    char *huelse;
    const char *fun = "SetzePosPlusZehn";

    DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

    if ( ( huelse = (char *)malloc( ( sizeof( char ) * len ) + 1 ) ) == NULL )
       return ERROR;

    sprintf( huelse, "%*.*s", len, len, posnr );
    sprintf( posnr, "%0*d", len, atoi( huelse ) + 1 );
    free( huelse );

    return OK;
  }

/*** HOLE-POSNR-MINUS-ZEHN ****************************************************/

  int HolePosNrMinusZehn( char *posnr, char *posnrold, int len )
  {
    char *huelse;
    const char *fun = "HolePosNrMinusZehn";

    DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

    if ( ( huelse = (char *)malloc( ( sizeof( char ) * len ) + 1 ) ) == NULL )
       return ERROR;

    sprintf( huelse, "%*.*s", len, len, posnrold );
    if ( atoi( huelse ) % TEXT_POSNR != 0 )
       return KEIN_UPDATE;

    sprintf( posnr, "%0*d", len, atoi( huelse ) / TEXT_POSNR );
    free( huelse );

    return OK;
  }

/*** HOLE-POSNR-MINUS-ZEHN ****************************************************/

  int HoleSchluesselMinusZehn( char *posnr )
  {
    int rc;
    const char *fun = "HoleSchluesselMinusZehn";

    DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

    if ( atoi( posnr ) < TEXT_POSNR )
       return atoi( posnr );

    if ( ( rc = atoi( posnr ) % TEXT_POSNR ) != 0 && rc != 1 )
       return ERROR;

    if ( rc == 1 )
       rc = atoi( posnr ) - 1;
       else
       rc = atoi( posnr );

    return ( rc / TEXT_POSNR );
  }

/*** HOLE-DBMONEY *************************************************************/

  char HoleDbmoney( void )
  {
    char *ch;
    const char *fun = "HoleDbmoney";

    DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

    ch = (char *)getenv( "DBMONEY" );

#ifdef UNSER_UEBERFLIEGER
    if ( *ch != KOMMA && *ch != PUNKT )
       *ch = PUNKT;

    return( *ch );
#else
    if ( ch != NULL )
    {
       if ( *ch != KOMMA && *ch != PUNKT )
	  *ch = PUNKT;
       return *ch;
    }
    return PUNKT;
#endif
  }

/*** PROZESS-AKTIV ************************************************************/

  int ProzessAktiv( char *prozess )
  {
    FILE *pipe;
    int pid;
    int count = 0;
    int i = 0;
    int ch;
    char buf[12];
    char Command[256];
    const char *fun = "ProzessAktiv";

    DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

    pid = getpid();
    sprintf( Command, PS_COMMAND, BaseName( prozess ), BaseName( prozess ) );

    if ( ( pipe = popen( Command, "r" ) ) == NULL )
       return NO;

    while ( (ch = fgetc(pipe)) != EOF)
    {
         if ( ( buf[i++] = (char)ch ) == '\n' )
         {
            buf[i - 1] = STR_ENDE;
            if ( pid != atoi( buf ) )
            {
               count++;
               printf( "Prozess ist mit Pid %d schon aktiv\n", atoi( buf ) );
            }
            i = 0;
         }
    }
    pclose( pipe );
    return count;
  }
  
/*** BASENAME *****************************************************************/

  char *BaseName( char *path )
  {
    int i = 0;
    int len;
    const char *fun = "BaseName";

    DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )

    len = (int)strlen( path );

    while( path[len - ( i + 1 )] != '/' && len - i++ > 1 );
    return( path + len - i );
  }

/*** HOLE-BATCH ***************************************************************/

  void HoleBatch( char *buf, char *value )
  {
     if ( !memcmp( value, "VB", 2 ) )
        strcpy( buf, "1" );
     else if ( !memcmp( value, "CL", 2 ) )
        strcpy( buf, "2" );
     else if ( !memcmp( value, "ER", 2 ) )
        strcpy( buf, "3" );
     else if ( !memcmp( value, "LA", 2 ) )
        strcpy( buf, "4" );
     else if ( !memcmp( value, "LV", 2 ) )
        strcpy( buf, "5" );
     else
        strcpy( buf, "0" );
     return;
  }

  /*** SETZE-BATCH **************************************************************/

  void SetzeBatch( char *buf, char *value, int len )
  {
     char *ptr;
     switch( atoi( value ) )
     {
     case  1:  ptr = "VB    "; break;
     case  2:  ptr = "CL    "; break;
     case  3:  ptr = "ER    "; break;
     case  4:  ptr = "LA    "; break;
     case  5:  ptr = "LV    "; break;
     default:  ptr = "      "; break;
     }
     
     memcpy( buf, ptr, len );
     return;        
  }
