#ifndef PCUA1_PPUSERTYPE_INCLUDED
#define PCUA1_PPUSERTYPE_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPUSERTYPE_NAME    150
#define L_PPUSERTYPE_DESC    254

/* ----- Feldpositionen ------ */

#define P_PPUSERTYPE_ID      0
#define P_PPUSERTYPE_NAME    1
#define P_PPUSERTYPE_DESC    2

#define POSANZ_PPUSERTYPE    3

/* ----- Struktur ------ */

struct PPUSERTYPE
{
    short    USERTYPEID;
    char     NAME[151];
    char     DESC[255];
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPUSERTYPE \
       "sc150c254"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPUSERTYPE  412

/* ----- Projektion ------ */

#define PROJECTION_PPUSERTYPE \
       "USERTYPEID," \
       "NAME," \
       "DESCRIPTION"

/* ----- From ------ */

#define FROM_PPUSERTYPE \
       "PPUSERTYPE"

#endif   /* PCUA1_PPUSERTYPE_INCLUDED */
