/******************************************************************************/
/*     Hauptmodul: lorbas                                                     */
/*         Source: qsort.h                                                    */
/*          Autor: A. Schmidt-Rehschuh/O. Kaerger                             */
/*       Erstellt: 06.12.1995                                                 */
/*        Sprache: C                                                          */
/*        Erzeugt: manuell                                                    */
/*        Aufgabe: stellt die Declaration fuer Qsort bereit.                  */
/*      Uebergabe: int Sortnr, void Pointer                                   */
/*    Returnwerte: 0 = OK                                                     */
/*                 1 = S_EOF (not found/oder kein Satz mehr)                  */
/*                -1 = ERROR                                                  */
/*                                                                            */
/******************************************************************************/

/*** DEFINE (EOF-SORT) ********************************************************/

 #define S_EOF                  1

/*** DECLARATION **************************************************************/


 #ifdef QSORT
 int SortOpen( int sortnum, int lsatz, int lsort );
 int SortClose( int sortnum );
 int SortPut( int sortnum, char *Pos );
 int SortGet( int, void *Mem );
 int Sort( void );
 #else
 int SortOpen( int sortnum, int lsatz, int lsort );
 int SortClose( int sortnum );
 int SortPut( int sortnum, char *Pos );
 int SortGet( int, char *Mem );
 int Sort( void );
 #endif
