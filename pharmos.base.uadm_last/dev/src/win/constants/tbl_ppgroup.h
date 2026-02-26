#ifndef POS1G_PPGROUP_INCLUDED
#define POS1G_PPGROUP_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPGROUP_NAME    30

/* ----- Feldpositionen ------ */

#define P_PPGROUP_NAME    0
#define P_PPGROUP_GROUPID 1

#define POSANZ_PPGROUP    2

/* ----- Struktur ------ */

struct PPGROUP  {
       char    NAME    [31];
       long    GROUPID ;
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPGROUP \
       "c30i"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPGROUP  43

/* ----- Projektion ------ */

#define PROJECTION_PPGROUP \
       "NAME," \
       "GROUPID"

/* ----- From ------ */

#define FROM_PPGROUP \
       "PPGROUP"

#endif   /* POS1G_PPGROUP_INCLUDED */

