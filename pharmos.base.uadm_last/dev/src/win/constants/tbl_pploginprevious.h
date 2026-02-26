#ifndef ZPCUA1_PPLOGINPREVIOUS_INCLUDED
#define ZPCUA1_PPLOGINPREVIOUS_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPLOGINPREVIOUS_GLOBALUSER     20
#define L_PPLOGINPREVIOUS_LANGUAGEID     2
#define L_PPLOGINPREVIOUS_COUNTRY        2

/* ----- Feldpositionen ------ */

#define P_PPLOGINPREVIOUS_APPLICATIONID  0
#define P_PPLOGINPREVIOUS_GLOBALUSER     1
#define P_PPLOGINPREVIOUS_USERID         2
#define P_PPLOGINPREVIOUS_LANGUAGEID     3
#define P_PPLOGINPREVIOUS_BRANCHREGIONID 4
#define P_PPLOGINPREVIOUS_COUNTRY        5

#define POSANZ_PPLOGINPREVIOUS           6

/* ----- Struktur ------ */

struct PPLOGINPREVIOUS  {
       short   APPLICATIONID  ;
       char    GLOBALUSER     [21];
       short   USERID         ;
       char    LANGUAGEID     [3];
       short   BRANCHREGIONID ;
       char    COUNTRY        [3];
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPLOGINPREVIOUS \
       "sc20sc2sc2"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPLOGINPREVIOUS  48

/* ----- Projektion ------ */

#define PROJECTION_PPLOGINPREVIOUS \
       "APPLICATIONID," \
       "GLOBALUSER," \
       "USERID," \
       "LANGUAGEID," \
       "BRANCHREGIONID," \
       "COUNTRY"

/* ----- Projektion fuer Joins ------ */

#define JOIN_PROJECTION_PPLOGINPREVIOUS \
       "PPLOGINPREVIOUS.APPLICATIONID," \
       "PPLOGINPREVIOUS.GLOBALUSER," \
       "PPLOGINPREVIOUS.USERID," \
       "PPLOGINPREVIOUS.LANGUAGEID," \
       "PPLOGINPREVIOUS.BRANCHREGIONID," \
       "PPLOGINPREVIOUS.COUNTRY"

/* ----- From ------ */

#define FROM_PPLOGINPREVIOUS \
       "PPLOGINPREVIOUS"

#endif   /* ZPCUA1_PPLOGINPREVIOUS_INCLUDED */

