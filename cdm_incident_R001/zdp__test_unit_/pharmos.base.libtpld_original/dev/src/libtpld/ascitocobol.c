/******************************************************************************/
/*     Hauptmodul: lorbas                                                     */
/*         Source: ascitocobol.c                                              */
/*          Autor: A. Schmidt-Rehschuh                                        */
/*       Erstellt: 19.07.1995                                                 */
/*        Sprache: C                                                          */
/*        Erzeugt: manuell                                                    */
/*        Aufgabe: Abwicklung Ascii-Satz nach Cobol bringen                   */
/*                                                                            */
/******************************************************************************/

/*** INCLUDE ******************************************************************/

  #include <stdio.h>
  #include <memory.h>
  #include <stdlib.h>
  #include "vtcodum.h"

/*** DEFINE *******************************************************************/

  #include "define.h"

/*** STRUKTUR *****************************************************************/

  #include "struct.h"

/*** GLOBALE ******************************************************************/

  extern int debug_enabled;

/*** DECLARATION **************************************************************/

 #include "allgemein.h"
 #include "ascitocobol.h"

 static int  AsciToCobol( int *, char *, char *, int, int, int, int, int );
 static void DebCobolToAsci( char *, int );         /* zur Kontrolle (Debug)  */
 static unsigned SetBits( unsigned, unsigned );

/*** EXTERNE DECLARATION ******************************************************/

  extern int PruefeAbfrage( char *, char *, int );

 /***** WANDEL-COBOL *********************************************************/

 int WandelCobol( char *ergebnis, char *satz, struct AUFB d[], int anzfelder )
 {
   char *fun = "WandelCobol";
   char inhalt[256];
   int  feldlaenge;
   int  nachkomma;
   int  i;
   int  zaehler_len = 0;
   int  satzpos = 0;

   DEBUG_PRINT( fun, "ist aufgerufen", 0, 0 );

   for ( i = 0; i < anzfelder; i++ )
       {
       nachkomma  = HoleNachkomma( d[i].typ, d[i].nachkomma );
       feldlaenge = HoleFeldlaenge( d[i].typ, d[i].laenge );

       AsciToCobol( &satzpos, satz, inhalt, feldlaenge, d[i].laenge, \
		    d[i].typ, nachkomma, d[i].negativ );

       if ( i == 0 )
	  {
	  memcpy( ergebnis, inhalt, feldlaenge );
	  zaehler_len = 0;
	  }
	  else
	  memcpy( ergebnis + zaehler_len, inhalt, feldlaenge );

       zaehler_len = zaehler_len + feldlaenge;
       }

   return zaehler_len;
 }
 /***** ASCI-TO-COBOL ********************************************************/

 static int AsciToCobol( int *satzpos, char *satz, char *phost, int len,
			 int urlen, int art, int nk, int minus )
 {
   int rc = OK;
   int me = NO;
   int  i;
   int  ze = 0;
   int  zaehler = 0;
   int pos;
   unsigned wert1;
   unsigned wert2;
   unsigned char *pp;
   unsigned short halbwort;
   char packwert[256];
   char msg[L_TEXT];
   char *fun = "AsciToCobol";

   pos = *satzpos;
   *satzpos = *satzpos + urlen;
   pp = (unsigned char *) phost;

   if ( debug_enabled )
      {
      me = YES;
      debug_enabled = NO;
      }

   DEBUG_SECTION
   sprintf( msg, "pos %d urlen %d len %d art %c nk %d -> spos %d",
	    pos, urlen, len, art, nk, *satzpos );
   DEBUG_PRINT( fun, msg, 0, 0 )
   sprintf( msg, "String:%*.*s -> %d\n", urlen, urlen, satz + pos, urlen );
   DEBUG_PRINT( fun, msg, 0, 0 )
   END_DEBUG_SECTION

   switch(art)
      {
      case EBCDIC: for ( i = 0; i < len; i++ )
		       pp[i] = satz[pos+i];
		   StoEbcd( (char *) pp, len ); break;
      case STRINO: for ( i = 0; i < len; i++ )
		       pp[i] = satz[pos+i];
		   break;
      case CHARAC: for ( i = 0; i < len; i++ )
		       pp[i] = satz[pos+i];
		   break;
      case NUMERC: for ( i = 0; i < len; i++ )
		       pp[i] = satz[pos+i];
		   break;
      case PACKED: memcpy( packwert, satz + pos, urlen );
		   DEBUG_SECTION
		   sprintf( msg, "P_NACH:%d %*.*s\n",
			    urlen, urlen, urlen, packwert );
		   DEBUG_PRINT( fun, msg, 0, 0 )
		   END_DEBUG_SECTION

		   if ( packwert[0] == '-' )
		   {
			  minus = 1;
			  packwert[0] = '0';
		   }
		   else if ( packwert[0] == '+' )
		   {
			  packwert[0] = '0';
		   }
		   
		   for ( i = 0; i < urlen; i++ )
		   {
	           wert1 = packwert[i++];
		       wert2 = packwert[i];

		       if ( ++zaehler == len )
			   {
			      if ( minus )
			         wert2 = 0x0d;
			      else
			         wert2 = 0x0f;
			      pp[ze] = SetBits( wert1, wert2 ) & 0xFF;
			      break;
			   }
		       pp[ze++] = SetBits( wert1, wert2 ) & 0xFF;
		   }
		   break;
      case HALBWO: memcpy( (char *) pp, satz + pos, urlen );
		   pp[urlen] = STR_ENDE;
		   halbwort = atoi( (char *) pp ) & 0xFFFF;
		   pp[0] = halbwort >> 8;
		   pp[1] = halbwort & 0xFF;
		   break;

      case VOLLWO: memcpy( pp, satz + pos, urlen );
		   pp[urlen] = STR_ENDE;
		   wert1 = atoi( (char *) pp );
		   pp[0] = (wert1 >> 24) & 0xFF;
		   pp[1] = (wert1 >> 16) & 0xFF;
		   pp[2] = (wert1 >> 8)  & 0xFF;
		   pp[3] = wert1 & 0xFF;
		   break;
      default    : rc = ERROR; break;
      }

      DEBUG_SECTION
      if ( art == PACKED )
	 DebCobolToAsci( (char *) pp, len );
      END_DEBUG_SECTION

   if ( me )
      debug_enabled = YES;

   return rc;
 }

 /***** SET-BITS *************************************************************/

 static unsigned SetBits( unsigned wert1, unsigned wert2  )
 {
   return ( (( wert1 << 4 ) & 0xf0) | ( wert2 & 0x0f ) );
 }

 /***** COBOL-TO-ASCI (DEBUG) ************************************************/

 static void DebCobolToAsci( char *pp, int len )
 {
   char str[80];
   int  ch;
   int  vz;
   int  feldlen;
   int  zz = 0;
   int  i;
   char msg[L_TEXT];
   char *fun = "DebCobolToAsci";

   str[zz++] = BLANC;
   feldlen = 1;
   for ( i = 0; i < len; i++ )
       {
       ch = pp[i];
       str[zz++]  = ( ch & 0xf0 ) >> 4 | '0';
       feldlen ++;
       if ( i < ( len -1 ) )
	  {
	  str[zz++] =  (ch & 0x0f) | '0';
	  feldlen ++;
	  }
       else
	  {
	  vz = ch & 0x0f;
	  if (vz == 0x0d || vz == 0x0b)
	     str[0] = '-';
	     else
	     str[0] = '+';      /* neu */
	  }
       }
   str[zz] = STR_ENDE;
   sprintf( msg, "EntPackt: %d -> %s\n", zz, str );
   DEBUG_PRINT( fun, msg, 0, 0 )
   return;
 }
