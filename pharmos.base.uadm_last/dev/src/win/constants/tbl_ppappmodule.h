#ifndef PCUA1_PPAPPMODULE_INCLUDED
#define PCUA1_PPAPPMODULE_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPAPPMODULE_NAME          30
#define L_PPAPPMODULE_NAME_EN       30
#define L_PPAPPMODULE_NAME_FR       30
#define L_PPAPPMODULE_NAME_HR       30

/* ----- Feldpositionen ------ */

#define P_PPAPPMODULE_APPLICATIONID 0
#define P_PPAPPMODULE_MODULEID      1
#define P_PPAPPMODULE_NAME          2
#define P_PPAPPMODULE_NAME_EN       3
#define P_PPAPPMODULE_NAME_FR       4
#define P_PPAPPMODULE_NAME_HR       5

#define POSANZ_PPAPPMODULE          6

/* ----- Struktur ------ */

struct PPAPPMODULE  {
       short   APPLICATIONID ;
       short   MODULEID      ;
       char    NAME          [31];
       char    NAME_EN       [31];
       char    NAME_FR       [31];
       char    NAME_HR       [31];
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPAPPMODULE \
       "ssc30c30c30c30"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPAPPMODULE  138

/* ----- Projektion ------ */

#define PROJECTION_PPAPPMODULE \
       "APPLICATIONID," \
       "MODULEID," \
       "NAME," \
       "NAME_EN," \
       "NAME_FR," \
       "NAME_HR"

/* ----- From ------ */

#define FROM_PPAPPMODULE \
       "PPAPPMODULE"

#endif   /* PCUA1_PPAPPMODULE_INCLUDED */

