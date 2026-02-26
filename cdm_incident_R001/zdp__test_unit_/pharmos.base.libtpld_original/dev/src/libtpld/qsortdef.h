/******************************************************************************/
/*     Hauptmodul: lorbas                                                     */
/*         Source: qsortdef.h                                                 */
/*          Autor: A. Schmidt-Rehschuh/O. Kaerger                             */
/*       Erstellt: 06.12.1995                                                 */
/*        Sprache: C                                                          */
/*        Erzeugt: manuell                                                    */
/*        Aufgabe: stellt den Sort fuer Lorbasfunktionalitaet bereit.         */
/*      Uebergabe: int Sortnr, void Pointer                                   */
/*                                                                            */
/******************************************************************************/

/*** DEFINE (ERKENNUNG LOKALES MODUL) *****************************************/

 #define QSORT

/*** DEFINE (VORBELEGUNG STRUCTUR-ARRAY) **************************************/

 #define QSORT_SATZ  { YES, 0, 0, NULL,NULL }, \
		     { YES, 0, 0, NULL,NULL }, \
		     { YES, 0, 0, NULL,NULL }, \
		     { YES, 0, 0, NULL,NULL }, \
		     { YES, 0, 0, NULL,NULL }, \
		     { YES, 0, 0, NULL,NULL }, \
		     { YES, 0, 0, NULL,NULL }, \
		     { YES, 0, 0, NULL,NULL }, \
		     { YES, 0, 0, NULL,NULL }, \
		     { YES, 0, 0, NULL,NULL }

 #define ANZ_PARALLEL_SORT     10
 #define L_POINTER             1024

/*** MAKRO ********************************************************************/

 /*  #define ADR_OFFSET(X)      (int)(sort[X].SatzLaenge)  */
 #define ADR_OFFSET(X)      sort[X].SatzLaenge

/*** STRUKTUR( GLOBAL ) *******************************************************/

 static struct SORTIERUNG
 {
   int  Frei;                                   /* Yes = Sort ist frei        */
   int  Vergleichsbereich;                      /* Groesse des Vergleichs     */
   int  SatzLaenge;                             /* Satzlaenge                 */
   char *pFirstPos;                             /* merken der ersten Position */
   char *pLastPos;                              /* merken der letzten Position*/
 } sort[ANZ_PARALLEL_SORT] = { QSORT_SATZ };    /* z.Zt max. 10 Sorts         */
