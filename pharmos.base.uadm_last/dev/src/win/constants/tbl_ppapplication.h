#ifndef PCUA1_PPAPPLICATION_INCLUDED
#define PCUA1_PPAPPLICATION_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPAPPLICATION_NAME          30
#define L_PPAPPLICATION_DESCRIPTION   254
#define L_PPAPPLICATION_NAME_EN       30
#define L_PPAPPLICATION_NAME_FR       30
#define L_PPAPPLICATION_NAME_HR       30

/* ----- Feldpositionen ------ */

#define P_PPAPPLICATION_APPLICATIONID 0
#define P_PPAPPLICATION_NAME          1
#define P_PPAPPLICATION_DESCRIPTION   2
#define P_PPAPPLICATION_NAME_EN       3
#define P_PPAPPLICATION_NAME_FR       4
#define P_PPAPPLICATION_NAME_HR       5

#define POSANZ_PPAPPLICATION          6

/* ----- Struktur ------ */

struct PPAPPLICATION  {
       long    APPLICATIONID ;
       char    NAME          [31];
       char    DESCRIPTION   [255];
       char    NAME_EN       [31];
       char    NAME_FR       [31];
       char    NAME_HR       [31];
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPAPPLICATION \
       "ic30c254c30c30c30"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPAPPLICATION  391

/* ----- Projektion ------ */

#define PROJECTION_PPAPPLICATION \
       "APPLICATIONID," \
       "NAME," \
       "DESCRIPTION," \
       "NAME_EN," \
       "NAME_FR," \
       "NAME_HR"

/* ----- From ------ */

#define FROM_PPAPPLICATION \
       "PPAPPLICATION"

#endif   /* PCUA1_PPAPPLICATION_INCLUDED */

