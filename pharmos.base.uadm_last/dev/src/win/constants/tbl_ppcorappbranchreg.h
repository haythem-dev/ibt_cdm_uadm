#ifndef POS1G_PPCORAPPBRANCHREG_INCLUDED
#define POS1G_PPCORAPPBRANCHREG_INCLUDED

/* ----- Feldlaengen ------ */


/* ----- Feldpositionen ------ */

#define P_PPCORAPPBRANCHREG_BRANCHID      0
#define P_PPCORAPPBRANCHREG_REGIONID      1
#define P_PPCORAPPBRANCHREG_APPLICATIONID 2

#define POSANZ_PPCORAPPBRANCHREG          3

/* ----- Struktur ------ */

struct PPCORAPPBRANCHREG  {
       short   BRANCHID      ;
       short   REGIONID      ;
       short   APPLICATIONID ;
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPCORAPPBRANCHREG \
       "sss"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPCORAPPBRANCHREG  21

/* ----- Projektion ------ */

#define PROJECTION_PPCORAPPBRANCHREG \
       "BRANCHID," \
       "REGIONID," \
       "APPLICATIONID"

/* ----- From ------ */

#define FROM_PPCORAPPBRANCHREG \
       "PPCORAPPBRANCHREG"

#endif   /* POS1G_PPCORAPPBRANCHREG_INCLUDED */

