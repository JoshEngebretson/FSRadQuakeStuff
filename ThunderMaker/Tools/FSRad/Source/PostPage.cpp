// ---------------------------------------------------------------------------------------------------------------------------------
//  _____            _   _____                                        
// |  __ \          | | |  __ \                                       
// | |__) | ___  ___| |_| |__) | __ _  __ _  ___      ___ _ __  _ __  
// |  ___/ / _ \/ __| __|  ___/ / _` |/ _` |/ _ \    / __| '_ \| '_ \ 
// | |    | (_) \__ \ |_| |    | (_| | (_| |  __/ _ | (__| |_) | |_) |
// |_|     \___/|___/\__|_|     \__,_|\__, |\___|(_) \___| .__/| .__/ 
//                                     __/ |             | |   | |    
//                                    |___/              |_|   |_|    
//
// Description:
//
//   Radiosity processor's configuration
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
#include "PostPage.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(PostPage, CPropertyPage)

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(PostPage, CPropertyPage)
	//{{AFX_MSG_MAP(PostPage)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	PostPage::PostPage() : CPropertyPage(PostPage::IDD)
{
	//{{AFX_DATA_INIT(PostPage)
	bAmbient = 0;
	gAmbient = 0;
	rAmbient = 0;
	gamma = 220;
	//}}AFX_DATA_INIT
}

// ---------------------------------------------------------------------------------------------------------------------------------

	PostPage::~PostPage()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PostPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PostPage)
	DDX_Control(pDX, IDC_GAMMA_VALUE, gammaValueEdit);
	DDX_Control(pDX, IDC_GAMMA, gammaSlider);
	DDX_Control(pDX, IDC_CLAMPING, clampingList);
	DDX_Control(pDX, IDC_AMBIENT_COLOR_BOX, ambientColorBox);
	DDX_Control(pDX, IDC_AMBIENT_R_BOX, rAmbientBox);
	DDX_Control(pDX, IDC_AMBIENT_G_BOX, gAmbientBox);
	DDX_Control(pDX, IDC_AMBIENT_B_BOX, bAmbientBox);
	DDX_Control(pDX, IDC_AMBIENT_G, gAmbientSlider);
	DDX_Control(pDX, IDC_AMBIENT_R, rAmbientSlider);
	DDX_Control(pDX, IDC_AMBIENT_B, bAmbientSlider);
	DDX_Slider(pDX, IDC_AMBIENT_B, bAmbient);
	DDX_Slider(pDX, IDC_AMBIENT_G, gAmbient);
	DDX_Slider(pDX, IDC_AMBIENT_R, rAmbient);
	DDX_Slider(pDX, IDC_GAMMA, gamma);
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	PostPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	gammaSlider.SetRange(100, 340);
	gammaSlider.ClearTics();
	{for (unsigned int i = 100; i <= 340; i += 10) gammaSlider.SetTic(i);}
	gammaSlider.SetPos(123); // Strange windows thing.. for some reason, we gotta do this. :/
	gammaSlider.SetPos(theApp.GetProfileInt("Options", "gamma", 220));

	rAmbientSlider.SetRange(0, 255);
	rAmbientSlider.SetPos(theApp.GetProfileInt("Options", "rAmbient", 0));
	gAmbientSlider.SetRange(0, 255);
	gAmbientSlider.SetPos(theApp.GetProfileInt("Options", "gAmbient", 0));
	bAmbientSlider.SetRange(0, 255);
	bAmbientSlider.SetPos(theApp.GetProfileInt("Options", "bAmbient", 0));

	clampingList.SetCurSel(theApp.GetProfileInt("Options", "clamping", 0));

	OnHScroll(0, 0, 0);

	if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000))
	{
		// Nothing.. this is just simpler to understand :)
	}
	else
	{
		rAmbientSlider.EnableWindow(FALSE);
		gAmbientSlider.EnableWindow(FALSE);
		bAmbientSlider.EnableWindow(FALSE);
	}

	SetTimer(123, 100, NULL);
	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PostPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	char	temp[MAX_PATH];

	sprintf(temp, "%.2f", static_cast<float>(gammaSlider.GetPos()) / 100.0f);
	gammaValueEdit.SetWindowText(temp);

	int	r = rAmbientSlider.GetPos();
	int	g = gAmbientSlider.GetPos() << 8;
	int	b = bAmbientSlider.GetPos() << 16;

	// Ambient red box
	{
		CDC	&dc = *rAmbientBox.GetDC();

		CRect	rect;
		rAmbientBox.GetClientRect(rect);

		CBrush	brush;
		brush.CreateSolidBrush(COLORREF(r));
		dc.FillRect(rect, &brush);

		brush.DeleteObject();
		rAmbientBox.ReleaseDC(&dc);
	}

	// Ambient green box
	{
		CDC	&dc = *gAmbientBox.GetDC();

		CRect	rect;
		gAmbientBox.GetClientRect(rect);

		CBrush	brush;
		brush.CreateSolidBrush(COLORREF(g));
		dc.FillRect(rect, &brush);

		brush.DeleteObject();
		gAmbientBox.ReleaseDC(&dc);
	}

	// Ambient blue box
	{
		CDC	&dc = *bAmbientBox.GetDC();

		CRect	rect;
		bAmbientBox.GetClientRect(rect);

		CBrush	brush;
		brush.CreateSolidBrush(COLORREF(b));
		dc.FillRect(rect, &brush);

		brush.DeleteObject();
		bAmbientBox.ReleaseDC(&dc);
	}

	// Ambient color box
	{
		CDC	&dc = *ambientColorBox.GetDC();

		CRect	rect;
		ambientColorBox.GetClientRect(rect);

		CBrush	brush;
		brush.CreateSolidBrush(COLORREF(r|g|b));
		dc.FillRect(rect, &brush);

		brush.DeleteObject();
		ambientColorBox.ReleaseDC(&dc);
	}

	if (pScrollBar) CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	PostPage::OnKillActive() 
{
	// Save parameters

	if (UpdateData())
	{
		theApp.WriteProfileInt("Options", "rAmbient", rAmbient);
		theApp.WriteProfileInt("Options", "gAmbient", gAmbient);
		theApp.WriteProfileInt("Options", "bAmbient", bAmbient);

		theApp.WriteProfileInt("Options", "gamma", gamma);

		theApp.WriteProfileInt("Options", "clamping", clampingList.GetCurSel());
	}

	return CPropertyPage::OnKillActive();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PostPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	OnHScroll(0, 0, 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PostPage.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
