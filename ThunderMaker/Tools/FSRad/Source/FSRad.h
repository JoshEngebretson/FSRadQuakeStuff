// ---------------------------------------------------------------------------------------------------------------------------------
//  ______  _____ _____            _     _     
// |  ____|/ ____|  __ \          | |   | |    
// | |__  | (___ | |__) | __ _  __| |   | |__  
// |  __|  \___ \|  _  / / _` |/ _` |   | '_ \ 
// | |     ____) | | \ \| (_| | (_| | _ | | | |
// |_|    |_____/|_|  \_\\__,_|\__,_|(_)|_| |_|
//
// Description:
//
//   Application entry point
//
// Notes:
//
//   Best viewed with 8-character tabs and (at least) 132 columns
//
// History:
//
//   06/04/2001 by Paul Nettle: Original creation
//
// Restrictions & freedoms pertaining to usage and redistribution of this software:
//
//   This software is 100% free. If you use this software (in part or in whole) you must credit the author. This software may not be
//   re-distributed (in part or in whole) in a modified form without clear documentation on how to obtain a copy of the original
//   work. You may not use this software to directly or indirectly cause harm to others. This software is provided as-is and without
//   warrantee -- Use at your own risk. For more information, visit HTTP://www.FluidStudios.com/
//
// Copyright 2002, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_FSRAD
#define _H_FSRAD

#ifdef FSRAD_WINDOWS_DISABLE

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "resource.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class RadApp : public CWinApp
{
public:
				RadApp();

	//{{AFX_VIRTUAL(RadApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RadApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern RadApp theApp;
#endif

//{{AFX_INSERT_LOCATION}}
#endif // _H_FSRAD
// ---------------------------------------------------------------------------------------------------------------------------------
// FSRad.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
