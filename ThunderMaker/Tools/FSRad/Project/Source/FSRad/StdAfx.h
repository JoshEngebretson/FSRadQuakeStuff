// ---------------------------------------------------------------------------------------------------------------------------------
//   _____ _       _           __          _     
//  / ____| |     | |   /\    / _|        | |    
// | (___ | |_  __| |  /  \  | |___  __   | |__  
//  \___ \| __|/ _` | / /\ \ |  _\ \/ /   | '_ \ 
//  ____) | |_| (_| |/ ____ \| |  >  <  _ | | | |
// |_____/ \__|\__,_/_/    \_\_| /_/\_\(_)|_| |_|
//                                               
//                                               
//
// Description:
//
//   Used by MSDEV for precompiled headers and standard windows/MFC includes
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

#ifndef	_H_STDAFX
#define _H_STDAFX

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// ---------------------------------------------------------------------------------------------------------------------------------
// Includes specific to this application
// ---------------------------------------------------------------------------------------------------------------------------------

#include "../../Include/fstl/fstl"
#include "../../Include/geom/geom"

//{{AFX_INSERT_LOCATION}}
#endif // _H_STDAFX
// ---------------------------------------------------------------------------------------------------------------------------------
// StdAfx.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
