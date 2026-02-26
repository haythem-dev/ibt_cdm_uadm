/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 30.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef _PDLGRIANALYSISLISTCRTL_H__________
#define _PDLGRIANALYSISLISTCRTL_H__________

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BRightAnalysis;
class BConRightAnalysis;

/*----------------------------------------------------------------------------*/
//  Class Declaration
/*----------------------------------------------------------------------------*/
class PDlgRiAnalysisListCtrl : public TVConListCtrl<BConRightAnalysis, BRightAnalysis>
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
