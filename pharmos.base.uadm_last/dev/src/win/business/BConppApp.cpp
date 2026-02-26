/******************************************************************************/
/**                                                                           */
/**   Desc   : Container zur Aufnahme aller Applikationen aus der Relation ppApplication*/
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 01.06.2004                                                     */
/**                                                                           */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
// Includes                                                                   
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppapplication.h>
#include <struct_application.h>
#include <SppApp.h>

#include "BppApp.h"
#include "BConppApp.h"


/**---------------------------------------------------------------------------*/
#define GIVEAPPDATA		((SppApp::SListAppProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConppApp, SppApp::SListAppProj, SppApp)

/**---------------------------------------------------------------------------*/
BConppApp::BConppApp()
{
}

/**---------------------------------------------------------------------------*/
BConppApp::~BConppApp()
{
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BConppApp::load()
{
	// set where condition
	//strcpy(GIVEAPPDATA->stListCond.TABTYPE, "T");

	if (TVConBsnObject<BppApp>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	// load holt sich per default nur bestimmte Anzahl an Datensätzen
	// load könnte auch so erweitert werden, dass bei erstem Aufruf alles sofort gelesen wird!

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
