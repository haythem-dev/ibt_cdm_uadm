/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 15.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef _PDLGRIGHTLISTCRTL_H__________
#define _PDLGRIGHTLISTCRTL_H__________

/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_COL_RIGHTNAME          0
#define VS_LC_COL_RIGHTDESC			 1
#define VS_LC_COL_RIGHTDESC_EN		 2
#define VS_LC_COL_RIGHTDESC_FR		 3
#define VS_LC_COL_RIGHTDESC_HR		 4

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BppAppRight;
class BConppAppRight;

/*----------------------------------------------------------------------------*/
//  Class Declaration
/*----------------------------------------------------------------------------*/
class PDlgRightListCtrl : public TVConListCtrl<BConppAppRight, BppAppRight>
{
public:
	virtual bool                 init                ();

private:
  	virtual void                 refreshItem         (const int iItem);
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
