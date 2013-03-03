// ---------------------------------------------------------------------------------------------------------------------------------
//  _____            _  _____ _               _       _     
// |  __ \          | |/ ____| |             | |     | |    
// | |__) | __ _  __| | (___ | |__   ___  ___| |_    | |__  
// |  _  / / _` |/ _` |\___ \| '_ \ / _ \/ _ \ __|   | '_ \ 
// | | \ \| (_| | (_| |____) | | | |  __/  __/ |_  _ | | | |
// |_|  \_\\__,_|\__,_|_____/|_| |_|\___|\___|\__|(_)|_| |_|
//                                                          
//                                                          
//
// Description:
//
//   Radiosity processor's main dialog
//
// Notes:
//
//   Best viewed with 8-character tabs and (at least) 132 columns
//
// History:
//
//   09/25/2001 by Paul Nettle: Original creation
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

#ifndef	_H_RADSHEET
#define _H_RADSHEET

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "GeneralPage.h"
#include "DatabasePage.h"
#include "LightmapPage.h"
#include "PostPage.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	RadSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(RadSheet)

public:
				RadSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
				RadSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
virtual				~RadSheet();

	//{{AFX_VIRTUAL(RadSheet)
	//}}AFX_VIRTUAL

inline		GeneralPage &	generalPage()		{return _generalPage;}
inline	const	GeneralPage &	generalPage() const	{return _generalPage;}
inline		DatabasePage &	databasePage()		{return _databasePage;}
inline	const	DatabasePage &	databasePage() const	{return _databasePage;}
inline		LightmapPage &	lightmapPage()		{return _lightmapPage;}
inline	const	LightmapPage &	lightmapPage() const	{return _lightmapPage;}
inline		PostPage &	postPage()		{return _postPage;}
inline	const	PostPage &	postPage() const	{return _postPage;}

protected:
	//{{AFX_MSG(RadSheet)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

		GeneralPage	_generalPage;
		DatabasePage	_databasePage;
		LightmapPage	_lightmapPage;
		PostPage	_postPage;
		void		addPages();
public:
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
#endif // _H_RADSHEET
// ---------------------------------------------------------------------------------------------------------------------------------
// RadSheet.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
