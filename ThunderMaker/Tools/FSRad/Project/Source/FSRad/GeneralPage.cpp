// ---------------------------------------------------------------------------------------------------------------------------------
//   _____                            _ _____                                        
//  / ____|                          | |  __ \                                       
// | |  __  ___ _ __   ___ _ __  __ _| | |__) | __ _  __ _  ___      ___ _ __  _ __  
// | | |_ |/ _ \ '_ \ / _ \ '__|/ _` | |  ___/ / _` |/ _` |/ _ \    / __| '_ \| '_ \ 
// | |__| |  __/ | | |  __/ |  | (_| | | |    | (_| | (_| |  __/ _ | (__| |_) | |_) |
//  \_____|\___|_| |_|\___|_|   \__,_|_|_|     \__,_|\__, |\___|(_) \___| .__/| .__/ 
//                                                    __/ |             | |   | |    
//                                                   |___/              |_|   |_|    
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
#include "GeneralPage.h"
#include "RadSheet.h"
#include "RadGen.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(GeneralPage, CPropertyPage)

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(GeneralPage, CPropertyPage)
	//{{AFX_MSG_MAP(GeneralPage)
	ON_BN_CLICKED(IDC_LEAVE_RESULTS, OnLeaveResults)
	ON_BN_CLICKED(IDC_EXIT_WHEN_FINISHED, OnExitWhenFinished)
	ON_BN_CLICKED(IDC_MAX_ITERATIONS, OnMaxIterations)
	ON_BN_CLICKED(IDC_GO, OnGo)
	ON_BN_CLICKED(IDC_ADAPTIVE_PATCHES, OnAdaptivePatches)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	GeneralPage::GeneralPage() : CPropertyPage(GeneralPage::IDD)
{
	//{{AFX_DATA_INIT(GeneralPage)
	leaveResults = FALSE;
	exitWhenFinished = FALSE;
	convergence = 1;
	areaLightMultiplier = 1;
	maxIterations = FALSE;
	maxIterationsCount = 1;
	subdivisionU = 4;
	subdivisionV = 4;
	ambientTerm = TRUE;
	useNusselt = FALSE;
	directLight = FALSE;
	adaptivePatches = FALSE;
	adaptiveThreshold = 15;
	adaptiveMaxU = 128;
	adaptiveMaxV = 128;
	pointLightMultiplier = 0.0f;
	//}}AFX_DATA_INIT
}

// ---------------------------------------------------------------------------------------------------------------------------------

	GeneralPage::~GeneralPage()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	GeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GeneralPage)
	DDX_Control(pDX, IDC_POINT_LIGHT_MULTIPLIER, pointLightMultiplierEdit);
	DDX_Control(pDX, IDC_ADAPTIVE_THRESHOLD, adaptiveThresholdEdit);
	DDX_Control(pDX, IDC_ADAPTIVE_X_STATIC, adaptiveXStatic);
	DDX_Control(pDX, IDC_ADAPTIVE_THRESHOLD_STATIC, adaptiveThresholdStatic);
	DDX_Control(pDX, IDC_ADAPTIVE_PATCHES, adaptivePatchesButton);
	DDX_Control(pDX, IDC_ADAPTIVE_MAX_V, adaptiveMaxVEdit);
	DDX_Control(pDX, IDC_ADAPTIVE_MAX_U, adaptiveMaxUEdit);
	DDX_Control(pDX, IDC_ADAPTIVE_MAX_STATIC, adaptiveMaxStatic);
	DDX_Control(pDX, IDC_DIRECT_LIGHT_ONLY, directLightButton);
	DDX_Control(pDX, IDC_ENABLE_NUSSELT, useNusseltButton);
	DDX_Control(pDX, IDC_SUBDIVISION_V, subdivisionVEdit);
	DDX_Control(pDX, IDC_SUBDIVISION_U, subdivisionUEdit);
	DDX_Control(pDX, IDC_ENABLE_AMBIENT_TERM, ambientTermButton);
	DDX_Control(pDX, IDC_MAX_ITERATIONS_COUNT, maxIterationsCountEdit);
	DDX_Control(pDX, IDC_MAX_ITERATIONS, maxIterationsButton);
	DDX_Control(pDX, IDC_AREA_LIGHT_MULTIPLIER, areaLightMultiplierEdit);
	DDX_Control(pDX, IDC_CONVERGENCE, convergenceEdit);
	DDX_Control(pDX, IDC_EXIT_WHEN_FINISHED, exitWhenFinishedButton);
	DDX_Control(pDX, IDC_LEAVE_RESULTS, leaveResultsButton);
	DDX_Check(pDX, IDC_LEAVE_RESULTS, leaveResults);
	DDX_Check(pDX, IDC_EXIT_WHEN_FINISHED, exitWhenFinished);
	DDX_Text(pDX, IDC_CONVERGENCE, convergence);
	DDX_Text(pDX, IDC_AREA_LIGHT_MULTIPLIER, areaLightMultiplier);
	DDV_MinMaxUInt(pDX, areaLightMultiplier, 1, 4000000000);
	DDX_Check(pDX, IDC_MAX_ITERATIONS, maxIterations);
	DDX_Text(pDX, IDC_MAX_ITERATIONS_COUNT, maxIterationsCount);
	DDV_MinMaxUInt(pDX, maxIterationsCount, 0, 4000000000);
	DDX_Text(pDX, IDC_SUBDIVISION_U, subdivisionU);
	DDV_MinMaxUInt(pDX, subdivisionU, 1, 1024);
	DDX_Text(pDX, IDC_SUBDIVISION_V, subdivisionV);
	DDV_MinMaxUInt(pDX, subdivisionV, 1, 1024);
	DDX_Check(pDX, IDC_ENABLE_AMBIENT_TERM, ambientTerm);
	DDX_Check(pDX, IDC_ENABLE_NUSSELT, useNusselt);
	DDX_Check(pDX, IDC_DIRECT_LIGHT_ONLY, directLight);
	DDX_Check(pDX, IDC_ADAPTIVE_PATCHES, adaptivePatches);
	DDX_Text(pDX, IDC_ADAPTIVE_THRESHOLD, adaptiveThreshold);
	DDX_Text(pDX, IDC_ADAPTIVE_MAX_U, adaptiveMaxU);
	DDV_MinMaxUInt(pDX, adaptiveMaxU, 1, 1024);
	DDX_Text(pDX, IDC_ADAPTIVE_MAX_V, adaptiveMaxV);
	DDV_MinMaxUInt(pDX, adaptiveMaxV, 1, 1024);
	DDX_Text(pDX, IDC_POINT_LIGHT_MULTIPLIER, pointLightMultiplier);
	DDV_MinMaxFloat(pDX, pointLightMultiplier, 0.f, 1.e+009f);
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	GeneralPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// General parameters

	exitWhenFinishedButton.SetCheck(theApp.GetProfileInt("Options", "exitWhenFinished", 1));
	leaveResultsButton.SetCheck(theApp.GetProfileInt("Options", "leaveResults", 0));
	maxIterationsButton.SetCheck(theApp.GetProfileInt("Options", "enableMaxIterations", 0));
	ambientTermButton.SetCheck(theApp.GetProfileInt("Options", "ambientTerm", 1));
	useNusseltButton.SetCheck(theApp.GetProfileInt("Options", "useNusselt", 0));
	directLightButton.SetCheck(theApp.GetProfileInt("Options", "directLightOnly", 0));
	adaptivePatchesButton.SetCheck(theApp.GetProfileInt("Options", "adaptivePatchSubdivision", 1));

	char	text[MAX_PATH];

	sprintf(text, "%u", theApp.GetProfileInt("Options", "subdivisionU", 4));
	subdivisionUEdit.SetWindowText(text);

	sprintf(text, "%u", theApp.GetProfileInt("Options", "adaptiveMaxSubdivisionU", 128));
	adaptiveMaxUEdit.SetWindowText(text);

	sprintf(text, "%u", theApp.GetProfileInt("Options", "subdivisionV", 4));
	subdivisionVEdit.SetWindowText(text);

	sprintf(text, "%u", theApp.GetProfileInt("Options", "adaptiveMaxSubdivisionV", 128));
	adaptiveMaxVEdit.SetWindowText(text);

	sprintf(text, "%u", theApp.GetProfileInt("Options", "adaptiveThreshold", 15));
	adaptiveThresholdEdit.SetWindowText(text);

	sprintf(text, "%u", theApp.GetProfileInt("Options", "convergence", 1));
	convergenceEdit.SetWindowText(text);

	sprintf(text, "%u", theApp.GetProfileInt("Options", "maxIterations", 1024));
	maxIterationsCountEdit.SetWindowText(text);

	sprintf(text, "%u", theApp.GetProfileInt("Options", "areaLightMultiplier", 1000000));
	areaLightMultiplierEdit.SetWindowText(text);

	sprintf(text, "%s", theApp.GetProfileString("Options", "pointLightMultiplier", "0.6"));
	pointLightMultiplierEdit.SetWindowText(text);

	OnExitWhenFinished();
	OnMaxIterations();
	OnAdaptivePatches();

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	GeneralPage::OnLeaveResults() 
{
	theApp.WriteProfileInt("Options", "leaveResults", leaveResultsButton.GetCheck());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	GeneralPage::OnExitWhenFinished() 
{
	BOOL	flag = exitWhenFinishedButton.GetCheck();
	theApp.WriteProfileInt("Options", "exitWhenFinished", flag);
	leaveResultsButton.EnableWindow(!flag);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	GeneralPage::OnMaxIterations() 
{
	BOOL	flag = maxIterationsButton.GetCheck();
	theApp.WriteProfileInt("Options", "enableMaxIterations", flag);
	maxIterationsCountEdit.EnableWindow(flag);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	GeneralPage::OnAdaptivePatches() 
{
	BOOL	flag = adaptivePatchesButton.GetCheck();
	theApp.WriteProfileInt("Options", "adaptivePatchSubdivision", flag);
	adaptiveThresholdStatic.EnableWindow(flag);
	adaptiveXStatic.EnableWindow(flag);
	adaptiveMaxUEdit.EnableWindow(flag);
	adaptiveMaxVEdit.EnableWindow(flag);
	adaptiveThresholdEdit.EnableWindow(flag);
	adaptiveMaxStatic.EnableWindow(flag);
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	GeneralPage::OnKillActive() 
{
	saveSettings();
	return CPropertyPage::OnKillActive();
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	GeneralPage::saveSettings()
{
	// Save settings

	if (!UpdateData()) return false;

	char	temp[MAX_PATH];

	sprintf(temp, "%.5f", pointLightMultiplier);
	theApp.WriteProfileString("Options", "pointLightMultiplier", temp);

	sprintf(temp, "%u", maxIterationsCount);
	theApp.WriteProfileInt("Options", "maxIterations", maxIterationsCount);

	theApp.WriteProfileInt("Options", "convergence", convergence);
	theApp.WriteProfileInt("Options", "adaptiveThreshold", adaptiveThreshold);
	theApp.WriteProfileInt("Options", "areaLightMultiplier", areaLightMultiplier);
	theApp.WriteProfileInt("Options", "subdivisionU", subdivisionU);
	theApp.WriteProfileInt("Options", "adaptiveMaxSubdivisionU", adaptiveMaxU);
	theApp.WriteProfileInt("Options", "subdivisionV", subdivisionV);
	theApp.WriteProfileInt("Options", "adaptiveMaxSubdivisionV", adaptiveMaxV);
	theApp.WriteProfileInt("Options", "ambientTerm", ambientTerm);
	theApp.WriteProfileInt("Options", "useNusselt", useNusselt);
	theApp.WriteProfileInt("Options", "directLightOnly", directLightButton.GetCheck());

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	GeneralPage::OnGo() 
{
	// Save the settings

	if (!saveSettings()) return;

	// Setup the radiosity generator

	RadGen		radGen;
	RadSheet *	parent = static_cast<RadSheet *>(GetParent());
	if (!parent)
	{
		AfxMessageBox("Internal error - unable to get parent dialog");
		return;
	}

	// Read the parms from the registry

	radGen.readDefaultParms();

	// This isn't stored in the registry... it's UI-specific

	radGen.leaveResults() = (!exitWhenFinished && leaveResults) ? true:false;

	// "Do yo thang"

	parent->ShowWindow(SW_HIDE);

	radGen.go(this);

	parent->ShowWindow(SW_SHOW);

	// Exit now?

	if (exitWhenFinished) parent->SendMessage(WM_COMMAND, IDOK);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// GeneralPage.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
