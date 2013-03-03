// ---------------------------------------------------------------------------------------------------------------------------------
//  _____            _  _____ _               _                        
// |  __ \          | |/ ____| |             | |                       
// | |__) | __ _  __| | (___ | |__   ___  ___| |_      ___ _ __  _ __  
// |  _  / / _` |/ _` |\___ \| '_ \ / _ \/ _ \ __|    / __| '_ \| '_ \ 
// | | \ \| (_| | (_| |____) | | | |  __/  __/ |_  _ | (__| |_) | |_) |
// |_|  \_\\__,_|\__,_|_____/|_| |_|\___|\___|\__|(_) \___| .__/| .__/ 
//                                                        | |   | |    
//                                                        |_|   |_|    
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

#include "stdafx.h"
#include "FSRad.h"
#include "RadSheet.h"
#include "HelpDialog.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(RadSheet, CPropertySheet)

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(RadSheet, CPropertySheet)
	//{{AFX_MSG_MAP(RadSheet)
	//}}AFX_MSG_MAP
//	ON_WM_HELPINFO()
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	RadSheet::RadSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	addPages();
}

// ---------------------------------------------------------------------------------------------------------------------------------

	RadSheet::RadSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	addPages();
}

// ---------------------------------------------------------------------------------------------------------------------------------

	RadSheet::~RadSheet()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	RadSheet::OnInitDialog()
{
	CPropertySheet::OnInitDialog();

	HICON	hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	RadSheet::addPages()
{
	AddPage(&generalPage());
	AddPage(&databasePage());
	AddPage(&lightmapPage());
	AddPage(&postPage());
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	RadSheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case	HELP_FORCEFILE:
			HelpDialog	dlg;
			dlg.DoModal();
			return 1;
	}

	return CPropertySheet::OnCommand(wParam, lParam);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// RadSheet.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
