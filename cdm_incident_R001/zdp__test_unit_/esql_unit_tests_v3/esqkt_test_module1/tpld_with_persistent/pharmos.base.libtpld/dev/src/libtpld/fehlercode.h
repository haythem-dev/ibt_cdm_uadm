/*****************************************************************************/
/*                                                                           */
/*   Hauptmodule: lorbas                                                     */
/*   Header     : fehlercode.h                                               */
/*   Autor      : A. Schmidt-Rehschuh (644)                                  */
/*   erstellt   :                                                            */
/*   Sprache    : C                                                          */
/*   erzeugt    : manuell                                                    */
/*                                                                           */
/*   Aufgabe    : Standard Fehlercode/Meldungen fuer Lorbas-Projekt          */
/*                                                                           */
/*****************************************************************************/

/*** DEFINE - FEHLER BEI DB-ZUGRIFF ******************************************/

 #define DB_FEHLER    -999

/*** DEFINE - FEHLERNUMMER/FEHLERTEXT ****************************************/

 static struct FEHLER_MLD fehler_mld[] = {
     { 152,    2, "Im Lorbas Pagebuffer ist keine Page mehr frei" },
     { 152,    7, "Funktion ist im Lorbas nicht bekannt" },
	 {  97,    8, "ungueltige Parameterliste" },
     {   1,   12, "logische Schluessel nicht gefunden" },
	 {  99,   13, "Index für Schluessel nicht geladen" },
	 {  98,   14, "Es ist keine Bitmap definiert" },
	 {  99,   15, "ungueltige Area-Definitionen" },
	 {  99,   18, "Tabelle passt nicht zu Satzart" },
     { 152,   20, "Enter fuer Membersatzart nicht erlaubt" },
     { 152,   22, "Komponente in DB-Beschreibung nicht definiert" },
     { 152,   23, "Enter für einen aktiven Satz abgesetzt" },
     {   2,   24, "Systemabbruch vorausgegangen" },
     { 152,   51, "Ungueltige Satz/Tabellennummer" },
	 {  99,   34, "Tabellen-Eintrag nicht gefunden" },
     {   1,   55, "Tabelle nicht aktiv" },
     {   2,   27, "Ende der Komponenten Tabelle ist erreicht" },
     {   2,   40, "Area ist voll belegt" },
     { 152,   41, "Area ist nicht benutzt" },
     { 152,   43, "Ungueltige Areaauswahl" },
     {   3,   46, "Impart: es sind noch andere Units aktiv" },
     {   3,   47, "Run-Unit ist aktiv. Es werden keine weiteren geoeffnet" },
     { 152,   73, "Ungueltige Komponentenart" },
	 {   3,   74, "Satz nicht gesperrt" },
     {   1,   75, "Satz als geloescht gekennzeichnet" },
     {   3,   76, "Satz bereits gesperrt" },
     {  99,   91, "Falsches Datum" },
     {  99,   92, "Depart Fehler" },
     {  96,   97, "Shutdown" }
 };
   /*    1, 37, "Tabelle nicht aktiv", */
   /*   98, 33, "Ungueltige Satz/Tabellennummer" */
   /*   alle auf hex umsetzten: */
   /*   98 = 152 */
   /*   37 = 55  */
   /*   33 = 51  */
   /*   1  =  1  */

/*** MAKRO - FEHLERNUMMER/FEHLERTEXT *****************************************/

 #define MSG(x)         fehler_mld[x - START_LORBAS_ERROR].error_mld
 #define ERR(x)         fehler_mld[x - START_LORBAS_ERROR].detail

/*** GLOBALE FEHLER-STRUCTUR *************************************************/

 #ifdef _MAIN
 struct FEHLER_MLD pcb;
 #else
 extern struct FEHLER_MLD pcb;
 #endif
