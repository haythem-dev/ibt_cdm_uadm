#ifndef POS1G_PPBRANCH_INCLUDED
#define POS1G_PPBRANCH_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPBRANCH_NAME     30

/* ----- Feldpositionen ------ */

#define P_PPBRANCH_BRANCHID 0
#define P_PPBRANCH_NAME     1

#define POSANZ_PPBRANCH     2

/* ----- Struktur ------ */

struct PPBRANCH  {
       short   BRANCHID ;
       char    NAME     [31];
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPBRANCH \
       "sc30"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPBRANCH  38

/* ----- Projektion ------ */

#define PROJECTION_PPBRANCH \
       "BRANCHID," \
       "NAME"

/* ----- From ------ */

#define FROM_PPBRANCH \
       "PPBRANCH"

#endif   /* POS1G_PPBRANCH_INCLUDED */

