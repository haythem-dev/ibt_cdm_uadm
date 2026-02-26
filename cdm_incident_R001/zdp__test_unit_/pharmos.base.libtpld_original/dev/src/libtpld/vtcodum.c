/* ----------------------------------------------------------- *
 *  FSCMAIL:        ASCCI <--> EBCDIC              codum.c     *
 * ----------------------------------------------------------- */

 /***** INCLUDE **************************************************************/

 #include <stdio.h>
 #include "vtcodum.h"

 /***** DEFINE ***************************************************************/

 #include "define.h"

 /***** GLOBALE **************************************************************/

 static unsigned char eatab[256];
 static int eatab_def = 0;
 static unsigned char aetab[] =                         /* 0x00 - 0X0F */
   {  0x00, 0x01, 0x02, 0x03, 0x37, 0x2D, 0x2E, 0x2F,
      0x16, 0x05, 0x15, 0x08, 0x0C, 0x0D, 0x0E, 0x0F,  /* 0x25 -> 0x15 */
                                                       /* 0x10 - 0X1F */
      0x10, 0x11, 0x12, 0x13, 0x3C, 0x3D, 0x32, 0x26,
      0x18, 0x19, 0x3F, 0x27, 0x1C, 0x1D, 0x1E, 0x1F,
                                                       /* 0x20 - 0X2F */
      0x40, 0x5A, 0x7F, 0x7B, 0x5B, 0x6C, 0x50, 0x7D,
      0x4D, 0x5D, 0x5C, 0x4E, 0x6B, 0x60, 0x4B, 0x61,
                                                       /* 0x30 - 0X3F */
      0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
      0xF8, 0xF9, 0x7A, 0x5E, 0x4C, 0x7E, 0x6E, 0x6F,
                                                       /* 0x40 - 0X4F */
      0x7C, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
      0xC8, 0xC9, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6,
                                                       /* 0x50 - 0X5F */
      0xD7, 0xD8, 0xD9, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6,
      0xE7, 0xE8, 0xE9, 0xAD, 0xE0, 0xBD, 0x6A, 0x6D,  /* 0xBB->0xAD/0xBC->0xE0  */
                                                       /* 0x60 - 0X6F */
      0x4A, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
      0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
                                                       /* 0x70 - 0X7F */
      0x97, 0x98, 0x99, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6,
      0xA7, 0xA8, 0xA9, 0xC0, 0x4F, 0xD0, 0xA1, 0x07,  /* 0xFB ->0xC0 / 0xFD->0xD0 / 0xFF->0xA1*/
     };

 /***** GLOBALE **************************************************************/

 extern int debug_enabled;

 static int isExtendedCharset = 0;

 /***** DECLARATION **********************************************************/

 void CheckAbdruckbar( unsigned char * );
 void CheckAbdruckbarZahl( unsigned char * );

 int Fgetcs( char *, int, FILE * );
 static unsigned char ToEbcd( unsigned char );
 static unsigned char ToAsciTab( unsigned char );
 unsigned char ToAsci( unsigned char );
/* void StoEbcd( char *str, int lng );  */
 void StoAsci( char *str, int lng );

 /****************************************************************************/
 void setExtendedCharSet(int isExtended)
 {
     isExtendedCharset = isExtended;
 }

 /***** FUNKTION: CHECK-ABDRUCKBAR *******************************************/

 void CheckAbdruckbar( unsigned char *cp )
 {
   unsigned char ch = *cp;

   if ( ch < ' ' )
      *cp = ' ';
   else
   {
       // allow extended character set
       if (isExtendedCharset > 0)
           return;

       if (ch >= 0x80)
           *cp = ' ';
   }
 }

 /***** FUNKTION: CHECK-ABDRUCKBAR-ZAHL **************************************/

 void CheckAbdruckbarZahl( unsigned char *cp )
 {
   unsigned char ch = *cp;

   if ( ch < '0' || ch > '9' )
      *cp = '0';
 }

 /***** FUNKTION: FGETCS *****************************************************/

 int Fgetcs( char *cp, int len, FILE *fp )
 {
   int i;
   int c;

   for( i = 1; i <= len; i++ )
      {
      if ( ( c = fgetc( fp ) ) == EOF)
    	 return 0;

      *cp++ = (char)c;
      }
   return len;
 }

 /***** FUNKTION: TO_EBC *****************************************************/

 static unsigned char ToEbcd( unsigned char ze )
 {
   return( aetab[(int)ze] );
 }

 /***** FUNKTION: TO_ASC_TAB *************************************************/

 static unsigned char ToAsciTab(unsigned char ze)
 {
   size_t i = 0;

   while( i < sizeof( aetab ) )
   {
     if ( aetab[i] == ze )
    	return( (unsigned char)i );
     i++;
   }
   return(0x00);
 }

 /***** FUNKTION: TO_ASC *****************************************************/

 unsigned char ToAsci( unsigned char ze )
 {
   size_t i;
   if ( eatab_def )
      return( eatab[(int)ze] );
   else
   {
      for ( i = 0; i < sizeof( eatab ); i++ )
          eatab[i] = ToAsciTab( (unsigned char)i );
      eatab_def = 1;
      return( eatab[(int)ze] );
   } 
 }

 /***** FUNKTION: STO_EBCD ***************************************************/

 void StoEbcd( char *str, int lng )
 {
   int i;

   for ( i = 0; i < lng; i++ )
       str[i] = ToEbcd( str[i] );
   return;
 }

 /***** FUNKTION: STO_ASC ****************************************************/

 void StoAsci( char *str, int lng )
 {
   int i;
   for (i=0; i<lng; i++)
       str[i] = ToAsci( str[i] );

   return;
 }
