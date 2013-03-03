// ---------------------------------------------------------------------------------------------------------------------------------
//  _      _       _     _                         _____                                        
// | |    (_)     | |   | |                       |  __ \                                       
// | |     _  __ _| |__ | |_ _ __ ___   __ _ _ __ | |__) | __ _  __ _  ___      ___ _ __  _ __  
// | |    | |/ _` | '_ \| __| '_ ` _ \ / _` | '_ \|  ___/ / _` |/ _` |/ _ \    / __| '_ \| '_ \ 
// | |____| | (_| | | | | |_| | | | | | (_| | |_) | |    | (_| | (_| |  __/ _ | (__| |_) | |_) |
// |______|_|\__, |_| |_|\__|_| |_| |_|\__,_| .__/|_|     \__,_|\__, |\___|(_) \___| .__/| .__/ 
//            __/ |                         | |                  __/ |             | |   | |    
//           |___/                          |_|                 |___/              |_|   |_|    
//
// Description:
//
//   Lightmap parameters
//
// Notes:
//
//   Best viewed with 8-character tabs and (at least) 132 columns
//
// History:
//
//   10/12/2001 by Paul Nettle: Original creation
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
#include "LightmapPage.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(LightmapPage, CPropertyPage)

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(LightmapPage, CPropertyPage)
	//{{AFX_MSG_MAP(LightmapPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	LightmapPage::LightmapPage() : CPropertyPage(LightmapPage::IDD)
{
	//{{AFX_DATA_INIT(LightmapPage)
	lightmapHeight = 128;
	lightmapWidth = 128;
	uTexelsPerUnit = 0.0625f;
	vTexelsPerUnit = 0.0625f;
	//}}AFX_DATA_INIT
}

// ---------------------------------------------------------------------------------------------------------------------------------

	LightmapPage::~LightmapPage()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	LightmapPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LightmapPage)
	DDX_Control(pDX, IDC_LIGHTMAP_TPU_V, vTexelsPerUnitEdit);
	DDX_Control(pDX, IDC_LIGHTMAP_TPU_U, uTexelsPerUnitEdit);
	DDX_Control(pDX, IDC_LIGHTMAP_WIDTH, lightmapWidthEdit);
	DDX_Control(pDX, IDC_LIGHTMAP_HEIGHT, lightmapHeightEdit);
	DDX_Text(pDX, IDC_LIGHTMAP_HEIGHT, lightmapHeight);
	DDV_MinMaxUInt(pDX, lightmapHeight, 1, 8192);
	DDX_Text(pDX, IDC_LIGHTMAP_WIDTH, lightmapWidth);
	DDV_MinMaxUInt(pDX, lightmapWidth, 1, 8192);
	DDX_Text(pDX, IDC_LIGHTMAP_TPU_U, uTexelsPerUnit);
	DDX_Text(pDX, IDC_LIGHTMAP_TPU_V, vTexelsPerUnit);
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	LightmapPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	uTexelsPerUnitEdit.SetWindowText(theApp.GetProfileString("Options", "uTexelsPerUnitInverse", "16"));
	vTexelsPerUnitEdit.SetWindowText(theApp.GetProfileString("Options", "vTexelsPerUnitInverse", "16"));

	char	text[MAX_PATH];

	sprintf(text, "%d", theApp.GetProfileInt("Options", "lightmapWidth", 128));
	lightmapWidthEdit.SetWindowText(text);

	sprintf(text, "%d", theApp.GetProfileInt("Options", "lightmapHeight", 128));
	lightmapHeightEdit.SetWindowText(text);

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	LightmapPage::OnKillActive() 
{
	// Save parameters

	if (UpdateData())
	{
		theApp.WriteProfileInt("Options", "lightmapWidth", lightmapWidth);
		theApp.WriteProfileInt("Options", "lightmapHeight", lightmapHeight);

		char	temp[MAX_PATH];

		sprintf(temp, "%.5f", uTexelsPerUnit);
		theApp.WriteProfileString("Options", "uTexelsPerUnitInverse", temp);

		sprintf(temp, "%.5f", vTexelsPerUnit);
		theApp.WriteProfileString("Options", "vTexelsPerUnitInverse", temp);
	}

	return CPropertyPage::OnKillActive();
}

// ---------------------------------------------------------------------------------------------------------------------------------
// LightmapPage.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
