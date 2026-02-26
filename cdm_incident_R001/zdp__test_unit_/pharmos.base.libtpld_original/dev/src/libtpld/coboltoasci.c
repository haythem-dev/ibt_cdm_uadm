/******************************************************************************/
/*     Hauptmodul: lorbas                                                     */
/*         Source: coboltoasci.c                                              */
/*          Autor: A. Schmidt-Rehschuh                                        */
/*       Erstellt: 19.07.1995                                                 */
/*        Sprache: C                                                          */
/*        Erzeugt: manuell                                                    */
/*        Aufgabe: Abwicklung Cobol-Satz nach Ascii bringen                   */
/*                                                                            */
/******************************************************************************/

/*** INCLUDE ******************************************************************/

  #include <stdio.h>
  #include <memory.h>  /* by hajo */

/*** VTCODUM ******************************************************************/

  #include "vtcodum.h" /* by hajo */

/*** DEFINE *******************************************************************/

  #include "define.h"

/*** STRUKTUR *****************************************************************/

  #include "struct.h"

/*** GLOBALE ******************************************************************/

  extern int debug_enabled;

/*** DECLARATION **************************************************************/

 static int  CobolToAsci( int *, char *, char *, int, int, int, int );

/*** EXTERNE DECLARATION ******************************************************/

 #include "allgemein.h"
 #include "coboltoasci.h"   /* by hajo */

 /***** WANDEL-ASCI ***********************************************************/

 int WandelAsci( char *ergebnis, char *satz, struct AUFB d[], int anzfelder )
 {
   char inhalt[256];
   int  feldlaenge;
   int  nachkomma;
   int  i;
   int  zaehler_len = 0;
   int  satzpos = 0;
   int  laenge;
   char msg[L_TEXT];
   char fun[] = "WandelAsci";

   DEBUG_PRINT( fun, "ist aufgerufen", 0, 0 );

   for ( i = 0; i < anzfelder; i++ )
   {
       nachkomma  = HoleNachkomma( d[i].typ, d[i].nachkomma );
       feldlaenge = HoleFeldlaenge( d[i].typ, d[i].laenge );

       laenge = CobolToAsci( &satzpos, satz, inhalt, feldlaenge, d[i].laenge,
			                 d[i].typ, nachkomma );

       DEBUG_SECTION
       sprintf( msg, "#%s# -> %d -> %d", inhalt, laenge, d[i].laenge );
       DEBUG_PRINT( fun, msg, 0, 0 );
       END_DEBUG_SECTION

       if ( i == 0 )
	   {
	      memcpy( ergebnis, inhalt, laenge );
	      zaehler_len = 0;
	   }
	   else
	      memcpy( ergebnis + zaehler_len, inhalt, laenge );

       zaehler_len = zaehler_len + laenge;
   }

   DEBUG_PRINT( fun, ergebnis, zaehler_len, 0 );
   return zaehler_len;
 }
 /***** COBOL-TO-ASCI ********************************************************/

 static int CobolToAsci( int *satzpos, char *satz, char *phost, int len,
			             int urlen, int art, int nk )
 {
   int  i;
   int  z;
   int  ch;
   int  vz;
   int  pos;
   int  feldlen = 1;
   int  me = NO;
   char *pp;
   char *hphost;
   unsigned char muster;
   unsigned short halbwort;
   char binaerwert[17];
   static char Komma = BLANC;
   char fun[] = "CobolToAsci";

   if ( Komma == BLANC )
      Komma = HoleDbmoney();

   pos = *satzpos;
   *satzpos = *satzpos + len;
   pp = phost;

   if ( debug_enabled )
   {
      me = YES;
      debug_enabled = NO;
   }

   DEBUG_SECTION
   char msg[128];
   sprintf( msg, "pos %d urlen %d len %d art %c nk %d -> spos %d",
            pos, urlen, len, art, nk, *satzpos );
   DEBUG_PRINT( fun, msg, 0, 0 )
   sprintf( msg, "String:%*.*s -> %d", urlen, urlen, satz + pos, urlen );
   DEBUG_PRINT( fun, msg, 0, 0 )
   END_DEBUG_SECTION

   switch(art)
   {
     case CHARAC:
		  for ( i = 0; i < len; i++ )
		  {
		      *pp = satz[pos+i];
		      CheckAbdruckbar( (unsigned char *) pp );
		      pp++;
		      feldlen++;
		  }
		  *pp = STR_ENDE;
		  break;
     case STRINO:
		  for ( i = 0; i < len; i++ )
		  {
		      *pp = satz[pos+i];
		      CheckAbdruckbar( (unsigned char *) pp );
		      pp++;
		      feldlen++;
		  }
		  *pp = STR_ENDE;
		  break;
     case NUMERC:
		  for ( i = 0; i < len; i++ )
		  {
		      *pp = satz[pos+i];
		      CheckAbdruckbarZahl( (unsigned char *) pp );
		      pp++;
		      feldlen++;
		  }
		  *pp = STR_ENDE;
		  break;
     case EBCDIC:
		  for ( i = 0; i < len; i++ )
		  {
		      *pp = ToAsci( satz[pos+i] );
		      CheckAbdruckbar( (unsigned char *) pp );
		      pp++;
		      feldlen++;
		  }
		  *pp = STR_ENDE;
		  break;
     case PACKED:
		  *pp++ = BLANC;
		  feldlen = 1;
		  for ( i = 0; i < len; i++ )
		  {
		      ch = satz[pos+i];
		      *pp++ = ( ch & 0xf0 ) >> 4 | '0';
		      feldlen ++;
		      if ( i < ( len -1 ) )
			  {
		         *pp++ =  (ch & 0x0f) | '0';
			     feldlen ++;
			  }
			  else
			  {
	     		 vz = ch & 0x0f;
			     if (vz == 0x0d || vz == 0x0b)
			     {
			        *phost = '-';
			     }
			     else if (vz == 0x0a || vz == 0x0c || vz == 0x0e || vz == 0x0f )
				 {
			       *phost = '+';      /* neu */
			     }
				 else
				 {
			       memset(phost, '0', len);
				   phost[len-1] = STR_ENDE;
				   if ( nk != 0 )
					  feldlen++;
				   return feldlen;
				 }
			  }
		  } /* end for */

		  *pp = STR_ENDE;
		  if ( nk != 0)
		  {
		     hphost = phost;
		     hphost += feldlen;
		     *(hphost+1) = STR_ENDE;   /* neu */
		     while ( nk-- > 0 )
			 {
			    *hphost = *(hphost-1);
			    hphost--;
			 }
		     *hphost = Komma;  /* ','; */
		     feldlen++;
		  }
		  break;
     case BITMST:
		  for ( z = 0; z < len; z++ )
		  {
		      muster = satz[pos + z];
		      for ( i = 1; i <= 8; i++ )
			  {
			      if ( muster & 0x80 )
			         *pp++ = '1';
			      else
			         *pp++ = '0';
			      muster = muster << 1;
			  }
		  }
		  *pp = STR_ENDE;
		  break;
     case LONGST:
		  for ( i = 0; i < 6; i++ )
		  {
			  binaerwert[i] = 0;
		  }
		  for ( i = 0; i < len; i++ )
		  {
			  binaerwert[i] = satz[pos+i];
		  }

          {
              // strict aliasing: avoid type-punning
              long tmp;
              memcpy(&tmp, binaerwert, sizeof(long));

              sprintf( pp, "%011ld", tmp);
          }

		  feldlen = 11;
		  break;
     case BINAER:
		  for ( i = 0; i < 6; i++ )
		  {
			  binaerwert[i] = 0;
		  }
		  for ( i = 0; i < len; i++ )
		  {
			  binaerwert[len-1-i] = satz[pos+i];
		  }

          {
              // strict aliasing: avoid type-punning
              long tmp;
              memcpy(&tmp, binaerwert, sizeof(long));

              sprintf( pp, "%010ld", tmp);
          }

		  feldlen = 10;
	      break;
     case HALBWO:
		  memcpy( pp, satz + pos, len );
		  pp[len] = STR_ENDE;
		  halbwort = *(short *) pp;
		  sprintf( pp, "%0*d", urlen, halbwort );
		  feldlen = urlen + 1;
		  break;
     case VOLLWO:
		  for ( i = 0; i < len; i++ )
		  {
			  binaerwert[i] = 0;
		  }
		  for ( i = 0; i < len; i++ )
		  {
			  binaerwert[i] = satz[pos+i];
		  }

          {
              // strict aliasing: avoid type-punning
              long tmp;
              memcpy(&tmp, binaerwert, sizeof(long));

		      sprintf( pp, "%0*ld", urlen, tmp);
          }

	      feldlen = urlen + 1;
	  	  break;
     default:
		  return ERROR;
   }

   if ( me )
      debug_enabled = YES;

   return feldlen;
 }
