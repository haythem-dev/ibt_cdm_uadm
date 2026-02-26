#ifndef POS1G_PPREGION_INCLUDED
#define POS1G_PPREGION_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPREGION_NAME     30

/* ----- Feldpositionen ------ */

#define P_PPREGION_REGIONID 0
#define P_PPREGION_NAME     1

#define POSANZ_PPREGION     2

/* ----- Struktur ------ */

struct PPREGION  {
       short   REGIONID ;
       char    NAME     [31];
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPREGION \
       "sc30"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPREGION  38

/* ----- Projektion ------ */

#define PROJECTION_PPREGION \
       "REGIONID," \
       "NAME"

/* ----- From ------ */

#define FROM_PPREGION \
       "PPREGION"

#endif   /* POS1G_PPREGION_INCLUDED */

