/******************************************************************************/
/*                                                                            */
/*   Modul-Name            : lorbas                                           */
/*   Projekt/Produkt       : Allgemeine Funktionen (Deklaration)              */
/*   Hauptmodul            : lorbas.c                                         */
/*   Quelle                : allgemein.h                                      */
/*   Sprache               : c                                                */
/*                                                                            */
/*   Autorname             : Schmidt-Rehschuh, Alexander                      */
/*   Abt./Tel.-Nr.         : Zentrale Programmierung / 644                    */
/*   Datum letzte Aenderung: 14.12.1995                                       */
/*   Freigabedatum         :                                                  */
/*   Betriebssystem        : SCO Unix                                         */
/*                                                                            */
/*   Kurzbeschreibung      : Modul beinhaltet allgemein nutzbare Funktionen   */
/*                                                                            */
/******************************************************************************/

#include "define.h"  /* hajo wegen Bitleiste */


/******** DECLARATION *********************************************************/

 struct AUFB;

 #ifdef ALLGEMEIN
 char *Rbuendig( char *, struct AUFB * );          /* Str rechtsbuendig mit 0 */
 void AufbereitenString( char *, int );            /* Komma entfernen         */
 Bitleiste StrToBitvektor( char *, Bitleiste * );  /* Short in Vektor (1/0)   */
 int  BitvektorToStr( char *,int , Bitleiste );    /* Vektor (1/0) in Short   */
 void HoleDatumZeit( char *, int );                /* Datum/Uhrzeit liefern   */
 void HoleTagesTyp( char *, int );                 /* Tagestyp fuer DB-Abfrage*/
 void HoleDatum( char *, int );                    /* Datum fuer DB liefern   */
 void HoleZeit( char *, int );                          /* Uhrzeit fuer DB liefern */
 void SetzeJaNeinSchalter( char * );               /* Schalter auf J/N setzen */
 void SetzeEinsNullSchalter( char * );             /* Schalter auf 0/1 setzen */
 int  HoleNachkomma( int, int );                   /* Lesen Cobolsatzbeschr.  */
 int  HoleFeldlaenge( int, int );                  /* Lesen Cobolsatzbeschr.  */
 int  HoleDateiNummer( void );                     /* Dateinummer fuer Debug  */
 char HoleKdAuftragStat( char *, int );            /* Auftragstatus fuer Cobol*/
 void SetzeKdAuftragStatus( char *, char * );      /* Auftragstatus fuer Db   */
 int  RechneVolumen( char *,char *,char *,char *, int );/* Volumenberech.fuer Arti */
 int  SetzePosNrPlusZehn( char *, int );           /* Posummer * 10           */
 int  SetzePosNrPlusEins( char *, int );           /* erhoehte Posnr + 1      */
 int  HolePosNrMinusZehn( char *, char *, int );   /* Posnummer / 10 (char)   */
 int  HoleSchluesselMinusZehn( char * );           /* Posnummer / 10          */
 void SetzeDatumOhneJahr( char *, int );           /* Jahreszahl abziehen     */
 void SetzeDatumMitJahr( char *, int );            /* Jahreszahl setzen       */
 char HoleDbmoney( void );                         /* DBMONEY abfragen        */
 int  ProzessAktiv( char * );                      /* Ist Prozess schon aktiv */
 char *BaseName( char * );                         /* absoluten Pfad kuerzen  */
 int IstVerbundPos( Bitleiste );                   /* festellen ob Verbundpos */
 #else
 extern char *Rbuendig( char *, const struct AUFB * );
 extern void AufbereitenString( char *, int * );
 extern Bitleiste StrToBitvektor( char *, Bitleiste * );
 extern int  BitvektorToStr( char *, Bitleiste );
 extern void HoleDatumZeit( char *, int );
 extern void HoleTagesTyp( char *, int );
 extern void HoleDatum( char *, int );
 extern void HoleZeit( char * );
 extern void SetzeJaNeinSchalter( char * );
 extern void SetzeEinsNullSchalter( char * );
 extern int  HoleNachkomma( int, int );
 extern int  HoleFeldlaenge( int, int );
 extern int  HoleDateiNummer( void );
 extern char HoleKdAuftragStat( char *, int );
 extern void SetzeKdAuftragStatus( char *, char * );
 extern int  RechneVolumen( char *, char *, char *, char * );
 extern int  SetzePosNrPlusZehn( char *, int );
 extern int  SetzePosNrPlusEins( char *, int );
 extern int  HolePosNrMinusZehn( char *, char *, int );
 extern int  HolePosNrMinusEins( char * );
 extern void SetzeDatumMitJahr( char *, int );
 extern void SetzeDatumOhneJahr( char *, int );
 extern char HoleDbmoney( void );
 extern int  ProzessAktiv( char * );                  /* Ist Prozess schon aktiv */
 extern char *BaseName( char * );                     /* absoluten Pfad kuerzen  */
 extern int IstVerbundPos( Bitleiste );               /* festellen ob Verbundpos */
 #endif
