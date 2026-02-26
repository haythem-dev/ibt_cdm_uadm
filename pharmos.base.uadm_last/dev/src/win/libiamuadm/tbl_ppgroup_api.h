#ifndef POS1G_PPGROUP_API_INCLUDED
#define POS1G_PPGROUP_API_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPGROUP_API_NAME    50

/* ----- Feldpositionen ------ */

#define P_PPGROUP_API_NAME    0
#define P_PPGROUP_API_GROUPID 1


#define POSANZ__PPGROUP_API    2

/* ----- Struktur ------ */

struct PPGROUP_API {
    char    NAME[51];
    long   GROUPID;
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPGROUP_API \
       "c50l"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPGROUP_API  63

/* ----- Projektion ------ */

#define PROJECTION_PPGROUP_API \
       "NAME," \
       "GROUPID"

/* ----- From ------ */

#define FROM_PPGROUP_API \
       "PPGROUP_API"

#endif   /* POS1G_PPGROUP_API_INCLUDED */