/******************************************************************************/
/*     Hauptmodul: lorbas                                                     */
/*         Source: qsort.c                                                    */
/*          Autor: A. Schmidt-Rehschuh/O. Kaerger                             */
/*       Erstellt: 06.12.1995                                                 */
/*        Sprache: C                                                          */
/*        Erzeugt: manuell                                                    */
/*        Aufgabe: stellt den Sort fuer Lorbasfunktionalitaet bereit.         */
/*      Uebergabe: int Sortnr, void Pointer                                   */
/*                                                                            */
/******************************************************************************/

/*** INCLUDE ******************************************************************/

 #include <stdlib.h>
 #include <stdio.h>
 #include "memory.h"

#ifdef  WIRKLICH_2_MAL     /* hajo */
 #include "qsort.h"
#endif

/*** DEFINE *******************************************************************/
 
 #include "define.h"                              /* allg. Defines            */
 #include "qsortdef.h"  /* vor qsort.h */         /* Defines/Struct/Makro sort*/

/*** GLOBALE ******************************************************************/

 extern int debug_enabled;

/*** DEKLARATION **************************************************************/

 #include "qsort.h"                               /* Declaration fuer qsort   */


/*** SORT-OPEN ****************************************************************/

 int SortOpen( int sortnum, int lsatz, int lsort )
 {
   char *fun = "SortOpen";
   (void) fun;

   #ifdef _XCICS
   DEBUG_PRINT( fun, "wurde aufgerufen", sortnum, lsort );
   #endif
   if ( sortnum < 0 || sortnum > 9)
   {
      #ifdef _XCICS
      PruefeAbfrage( fun, MSG( M_PARAMETER_UNBEKANNT ), M_PARAMETER_UNBEKANNT );
      #endif
      return ERROR;
   }

   if ( sort[sortnum].Frei == 0)
      {
      SortClose( sortnum );
      }

   sort[sortnum].Frei = 0;
   sort[sortnum].SatzLaenge = lsatz;
   sort[sortnum].Vergleichsbereich = lsort;
   sort[sortnum].pFirstPos = NULL;
   sort[sortnum].pLastPos = NULL;

   return OK;
}                             

/*** SORT-CLOSE ***************************************************************/

 int SortClose( int sortnum )
 {
   char *fun = "SortClose";
   (void) fun;
   char *ptr;

   #ifdef _XCICS
   DEBUG_PRINT( fun, "wurde aufgerufen", 0, 0 )
   #endif
   if ( sort[sortnum].Frei == 0 ) /* in Benutzung */
      {
      while ( sort[sortnum].pFirstPos != NULL )
	    {
	    ptr = *(char **)( sort[sortnum].pFirstPos + L_POINTER );
	    free( (void *)( sort[sortnum].pFirstPos ) );
	    sort[sortnum].pFirstPos = ptr;
	    }
      sort[sortnum].Frei = 1;
      }

   return OK;
 }

/*** SORT *********************************************************************/

 int Sort( void )
 {
   /* Da wir sortiert einfuegen, ist diese Funktion unnoetig */
   return OK;
 }

/*** SORT-PUT *****************************************************************/

 int SortPut( int sortnum, char *Pos )
 {
   char *fun = "SortPut";
   (void) fun;
   char *ptrNewPos;
   char *ptrPos  = sort[sortnum].pFirstPos;
   char *ptrVorg = NULL;
	
   #ifdef _XCICS
   DEBUG_PRINT( fun, Pos, sortnum, 0 )
   #endif
   ptrNewPos = malloc( (L_POINTER * sizeof(char)) + sizeof(char *) );
   if ( ptrNewPos == NULL )
   {
   #ifdef _XCICS
      PruefeAbfrage( fun, "Kein Speicher mehr frei!", ERROR );
      DEBUG_PRINT( fun, "Kein Speicher mehr frei!", sortnum, 0 )
   #endif
      return ERROR;
   }

   /* ohne die vier letzten Bytes! */

   if ( ADR_OFFSET( sortnum ) > L_POINTER )
   {
      #ifdef _XCICS
	  PruefeAbfrage( fun, MSG( M_FALSCHE_LAENGE ), M_FALSCHE_LAENGE );
      #endif
      return ERROR;
   }

   memcpy( ptrNewPos, (char *)Pos, ADR_OFFSET( sortnum ) );

   if ( sort[sortnum].pFirstPos == NULL )             /* Liste war leer       */
      {
      sort[sortnum].pLastPos  = ptrNewPos;
      sort[sortnum].pFirstPos = ptrNewPos;
      *(char **)( sort[sortnum].pFirstPos + L_POINTER ) = NULL;
      return OK;
      }

   if ( memcmp( sort[sortnum].pLastPos, ptrNewPos,
		sort[sortnum].Vergleichsbereich ) <= 0 )
      {
      /* einfuegen am Ende */
      *(char **)( sort[sortnum].pLastPos + L_POINTER ) = ptrNewPos;
      *(char **)( ptrNewPos + L_POINTER ) = NULL;
      sort[sortnum].pLastPos = ptrNewPos;
      return OK;
      }

   do      /* Einfuegestelle suchen */
      {
      if ( memcmp( ptrPos, ptrNewPos, sort[sortnum].Vergleichsbereich ) > 0 )
	 break;

      ptrVorg = ptrPos;
      ptrPos = *(char **)( ptrPos + L_POINTER );
      } while ( ptrPos != NULL );

   if ( ptrVorg == NULL )   /* an erster Stelle einfügen */
      sort[sortnum].pFirstPos = ptrNewPos;
      else
      *(char **)( ptrVorg + L_POINTER ) = ptrNewPos;

   *(char **)( ptrNewPos + L_POINTER ) = ptrPos;

   return OK;
 }


/*** SORT-GET *****************************************************************/

#ifdef QSORT        /*  ifdef by hajo */
 int SortGet( int sortnum, void *Mem )
#else
 int SortGet( int sortnum, char *Mem )
#endif
 {
   char *fun = "SortGet";
   (void) fun;
   char *ptr;
   
   #ifdef _XCICS
   DEBUG_PRINT( fun, "wurde aufgerufen", sortnum, 0 )
   #endif
   if ( sort[sortnum].pFirstPos == NULL )
      return S_EOF;

   memcpy( (char*)Mem, sort[sortnum].pFirstPos, (int)ADR_OFFSET( sortnum ) );
   ptr = *(char**)( sort[sortnum].pFirstPos + L_POINTER );

   free( (void*)( sort[sortnum].pFirstPos ) );
   sort[sortnum].pFirstPos = ptr;

   return OK;
 }
