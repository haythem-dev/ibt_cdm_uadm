/*****************************************************************************/
/*                                                                           */
/*   Hauptmodule: lorbas                                                     */
/*   Header     : struct.h                                                   */
/*   Autor      : A. Schmidt-Rehschuh (644)                                  */
/*   erstellt   :                                                            */
/*   Sprache    : C                                                          */
/*   erzeugt    : manuell                                                    */
/*                                                                           */
/*   Aufgabe    : allgemeine Structuren                                      */
/*                                                                           */
/*****************************************************************************/


/*** STRUCTUR - COBOL-SATZART/TYP ********************************************/

 typedef struct AUFB {
   int typ;
   int laenge;
   int nachkomma;
   int negativ;
 } tCobData;

/*** STRUCTUR - PARAMETERLISTE ***********************************************/

 struct PARA {
   int setno;
   int buf;
   int key;
 };

/*** STRUCTUR - FEHLERNUMMER-CODE ********************************************/

 struct FEHLER_MLD {
    short status;
    short detail;
    char *error_mld;
 };

/*** STRUCTUR - TABELLEN-AUSWAHL (SATZ-ART-LORBAS) ***************************/

 struct TAB_ART {
    char *tabart;
    int  satznr;
    int  key;
 };

/*** STRUCTUR - TABELLEN-AUSWAHL (SATZ-ART-LORBAS) ***************************/

 struct FUN_ART {
    char *funart;
    int  funktionnr;
 };

/*** STRUCTUR - SORT-INFORMATIONEN *******************************************/

 struct SORT_ART {
    int sortnr;
    int len;                                      /* bis zu 10 moeglich      */
    int key;                                      /* laenge des Satzes       */
 };                                               /* laenge Sortierbereich   */

/*** STRUCTUR - KOPF-FUSS-TEXT-MERKEN ****************************************/

 struct KOPFTEXT {
    int auftragnr;
    int posnr;
    char buf[2048];
 };

/*** ENDE ********************************************************************/
