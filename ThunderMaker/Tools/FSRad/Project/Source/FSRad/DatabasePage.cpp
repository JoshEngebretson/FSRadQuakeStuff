// ---------------------------------------------------------------------------------------------------------------------------------
//  _____        _         _                    _____                                        
// |  __ \      | |       | |                  |  __ \                                       
// | |  | | __ _| |_  __ _| |__   __ _ ___  ___| |__) | __ _  __ _  ___      ___ _ __  _ __  
// | |  | |/ _` | __|/ _` | '_ \ / _` / __|/ _ \  ___/ / _` |/ _` |/ _ \    / __| '_ \| '_ \ 
// | |__| | (_| | |_| (_| | |_) | (_| \__ \  __/ |    | (_| | (_| |  __/ _ | (__| |_) | |_) |
// |_____/ \__,_|\__|\__,_|_.__/ \__,_|___/\___|_|     \__,_|\__, |\___|(_) \___| .__/| .__/ 
//                                                            __/ |             | |   | |    
//                                                           |___/              |_|   |_|    
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
#include "DatabasePage.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(DatabasePage, CPropertyPage)

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(DatabasePage, CPropertyPage)
	//{{AFX_MSG_MAP(DatabasePage)
	ON_BN_CLICKED(IDC_OUTPUT_RAW_BROWSE, OnOutputRawBrowse)
	ON_BN_CLICKED(IDC_OUTPUT_RAW_FLAG, OnOutputRawFlag)
	ON_BN_CLICKED(IDC_OUTPUT_OCT_FLAG, OnOutputOctFlag)
	ON_BN_CLICKED(IDC_OUTPUT_OCT_BROWSE, OnOutputOctBrowse)
	ON_BN_CLICKED(IDC_INPUT_BROWSE, OnInputBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	DatabasePage::DatabasePage() : CPropertyPage(DatabasePage::IDD)
{
	//{{AFX_DATA_INIT(DatabasePage)
	gaussianResolution = 8;
	minSplitRange = 5.0f;
	maxDepth = 50;
	minRadius = 5.0f;
	threshold = 50000;
	outputRawDirectory = _T("");
	inputFilename = _T("");
	outputOctFilename = _T("");
	outputOct = FALSE;
	gReflectivity = 0.0f;
	bReflectivity = 0.0f;
	rReflectivity = 0.0f;
	//}}AFX_DATA_INIT
}

// ---------------------------------------------------------------------------------------------------------------------------------

	DatabasePage::~DatabasePage()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	DatabasePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DatabasePage)
	DDX_Control(pDX, IDC_REFLECTIVITY_R, rReflectivityEdit);
	DDX_Control(pDX, IDC_REFLECTIVITY_G, gReflectivityEdit);
	DDX_Control(pDX, IDC_REFLECTIVITY_B, bReflectivityEdit);
	DDX_Control(pDX, IDC_OUTPUT_OCT_FLAG, outputOctButton);
	DDX_Control(pDX, IDC_OUTPUT_OCT_FILENAME, outputOctFilenameEdit);
	DDX_Control(pDX, IDC_OUTPUT_OCT_BROWSE, outputOctBrowseButton);
	DDX_Control(pDX, IDC_INPUT_FILENAME, inputFilenameEdit);
	DDX_Control(pDX, IDC_INPUT_BROWSE, inputFilenameBrowseButton);
	DDX_Control(pDX, IDC_OUTPUT_RAW_BROWSE, outputRawDirectoryBrowseButton);
	DDX_Control(pDX, IDC_OUTPUT_RAW_FLAG, outputRawDirectoryButton);
	DDX_Control(pDX, IDC_GAUSSIAN_RESOLUTION, gaussianResolutionEdit);
	DDX_Control(pDX, IDC_MIN_SPLIT_RANGE, minSplitRangeEdit);
	DDX_Control(pDX, IDC_MAX_DEPTH, maxDepthEdit);
	DDX_Control(pDX, IDC_MIN_RADIUS, minRadiusEdit);
	DDX_Control(pDX, IDC_THRESHOLD, thresholdEdit);
	DDX_Control(pDX, IDC_OUTPUT_RAW_DIRECTORY, outputRawDirectoryEdit);
	DDX_Text(pDX, IDC_GAUSSIAN_RESOLUTION, gaussianResolution);
	DDV_MinMaxUInt(pDX, gaussianResolution, 2, 512);
	DDX_Text(pDX, IDC_MIN_SPLIT_RANGE, minSplitRange);
	DDV_MinMaxFloat(pDX, minSplitRange, 0.f, 50.f);
	DDX_Text(pDX, IDC_MAX_DEPTH, maxDepth);
	DDV_MinMaxUInt(pDX, maxDepth, 0, 10000000);
	DDX_Text(pDX, IDC_MIN_RADIUS, minRadius);
	DDV_MinMaxFloat(pDX, minRadius, 0.f, 1.e+007f);
	DDX_Text(pDX, IDC_THRESHOLD, threshold);
	DDV_MinMaxUInt(pDX, threshold, 1, 10000000);
	DDX_Text(pDX, IDC_OUTPUT_RAW_DIRECTORY, outputRawDirectory);
	DDX_Text(pDX, IDC_INPUT_FILENAME, inputFilename);
	DDX_Text(pDX, IDC_OUTPUT_OCT_FILENAME, outputOctFilename);
	DDX_Check(pDX, IDC_OUTPUT_OCT_FLAG, outputOct);
	DDX_Text(pDX, IDC_REFLECTIVITY_G, gReflectivity);
	DDV_MinMaxFloat(pDX, gReflectivity, 0.f, 100.f);
	DDX_Text(pDX, IDC_REFLECTIVITY_B, bReflectivity);
	DDV_MinMaxFloat(pDX, bReflectivity, 0.f, 100.f);
	DDX_Text(pDX, IDC_REFLECTIVITY_R, rReflectivity);
	DDV_MinMaxFloat(pDX, rReflectivity, 0.f, 100.f);
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	DatabasePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// Desktop folder

	char	desktopPath[MAX_PATH];
	memset(desktopPath, 0, sizeof(desktopPath));
	SHGetSpecialFolderPath(NULL, desktopPath, CSIDL_DESKTOPDIRECTORY, 0);

	// Octree parameters

	char	text[MAX_PATH];

	sprintf(text, "%d", theApp.GetProfileInt("Options", "OctreePolysPerNode", 50000));
	thresholdEdit.SetWindowText(text);

	minRadiusEdit.SetWindowText(theApp.GetProfileString("Options", "OctreeMinRadius", "5.0"));

	sprintf(text, "%d", theApp.GetProfileInt("Options", "OctreeMaxDepth", 50));
	maxDepthEdit.SetWindowText(text);

	// BSP parameters

	sprintf(text, "%d", theApp.GetProfileInt("Options", "BSPGaussianResolution", 8));
	gaussianResolutionEdit.SetWindowText(text);

	minSplitRangeEdit.SetWindowText(theApp.GetProfileString("Options", "BSPMinSplitRange", "5.0"));

	rReflectivityEdit.SetWindowText(theApp.GetProfileString("Options", "rReflectivity", "0.78"));
	gReflectivityEdit.SetWindowText(theApp.GetProfileString("Options", "gReflectivity", "0.78"));
	bReflectivityEdit.SetWindowText(theApp.GetProfileString("Options", "bReflectivity", "0.78"));

	// Input/Output parameters

	outputRawDirectoryEdit.SetWindowText(theApp.GetProfileString("Options", "outputRawDirectory", desktopPath));
	outputRawDirectoryButton.SetCheck(theApp.GetProfileInt("Options", "outputRawDirectoryFlag", 0));
	outputOctButton.SetCheck(theApp.GetProfileInt("Options", "outputOctFileFlag", 1));

	fstl::string	defaultOctOutput = fstl::string(desktopPath) + "\\output.oct";
	outputOctFilenameEdit.SetWindowText(theApp.GetProfileString("Options", "outputOctFilename", defaultOctOutput.asArray()));
	inputFilenameEdit.SetWindowText(theApp.GetProfileString("Options", "inputFilename", ""));

	OnOutputRawFlag();
	OnOutputOctFlag();
	
	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	DatabasePage::OnOutputRawBrowse() 
{
	BROWSEINFO	bi;
	memset(&bi, 0, sizeof(bi));

	bi.hwndOwner = GetSafeHwnd();
	bi.lpszTitle = "Select output folder";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST	idl = SHBrowseForFolder(&bi);
	if (idl)
	{
		char	path[MAX_PATH];
		SHGetPathFromIDList(idl, path);
		outputRawDirectoryEdit.SetWindowText(path);
		theApp.WriteProfileString("Options", "outputRawDirectory", path);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	DatabasePage::OnOutputRawFlag() 
{
	BOOL	flag = outputRawDirectoryButton.GetCheck();
	theApp.WriteProfileInt("Options", "outputRawDirectoryFlag", flag);

	outputRawDirectoryBrowseButton.EnableWindow(flag);
	outputRawDirectoryEdit.EnableWindow(flag);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	DatabasePage::OnOutputOctFlag() 
{
	BOOL	flag = outputOctButton.GetCheck();
	theApp.WriteProfileInt("Options", "outputOctFileFlag", flag);

	outputOctBrowseButton.EnableWindow(flag);
	outputOctFilenameEdit.EnableWindow(flag);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	DatabasePage::OnOutputOctBrowse() 
{
	CString	str;
	outputOctFilenameEdit.GetWindowText(str);

	char		fname[256];
	strcpy(fname, (LPCSTR) str);

	char		filters[] = "OCT files (*.oct)\0*.oct\0All files (*.*)\0*.*\0\0";
	OPENFILENAME	of;
	memset(&of, 0, sizeof(OPENFILENAME));

	of.lStructSize  = sizeof(OPENFILENAME);
	of.hwndOwner    = GetSafeHwnd();
	of.lpstrFilter  = filters;
	of.nFilterIndex = 1;
	of.lpstrFile    = fname;
	of.nMaxFile     = sizeof(fname);
	of.lpstrTitle   = "Save RAD output";
	of.Flags        = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_OVERWRITEPROMPT;

	if (!GetSaveFileName(&of)) return;

	outputOctFilenameEdit.SetWindowText(of.lpstrFile);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	DatabasePage::OnInputBrowse() 
{
	char		fname[512];
	memset(fname, 0, sizeof(fname));
	char		filters[] =	"Entropy files (*.ent)\0"
					"*.ent\0"
					"OCT files (*.oct)\0"
					"*.oct\0"
					"ASE files (*.ase)\0"
					"*.ase\0"
					"All files (*.*)\0*.*\0\0";
	OPENFILENAME	of;
	memset(&of, 0, sizeof(OPENFILENAME));

	of.lStructSize  = sizeof(OPENFILENAME);
	of.hwndOwner    = GetSafeHwnd();
	of.lpstrFilter  = filters;
	of.nFilterIndex = 1;
	of.lpstrFile    = fname;
	of.nMaxFile     = sizeof(fname);
	of.lpstrTitle   = "Choose a geometry file";
	of.Flags        = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_EXPLORER;
	of.lpstrDefExt  = "ent";

	CString	str;
	inputFilenameEdit.GetWindowText(str);
	of.lpstrInitialDir = str;

	if (!GetOpenFileName(&of)) return;

	inputFilenameEdit.SetWindowText(of.lpstrFile);
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	DatabasePage::OnKillActive() 
{
	// Save parameters

	if (UpdateData())
	{
		char	temp[MAX_PATH];

		// Reflectivity

		sprintf(temp, "%.5f", rReflectivity);
		theApp.WriteProfileString("Options", "rReflectivity", temp);

		sprintf(temp, "%.5f", gReflectivity);
		theApp.WriteProfileString("Options", "gReflectivity", temp);

		sprintf(temp, "%.5f", bReflectivity);
		theApp.WriteProfileString("Options", "bReflectivity", temp);


		// Octree parameters

		theApp.WriteProfileInt("Options", "OctreePolysPerNode", threshold);
		sprintf(temp, "%.5f", minRadius);
		theApp.WriteProfileString("Options", "OctreeMinRadius", temp);
		theApp.WriteProfileInt("Options", "OctreeMaxDepth", maxDepth);

		// BSP parameters

		theApp.WriteProfileInt("Options", "BSPGaussianResolution", gaussianResolution);
		sprintf(temp, "%.5f", minSplitRange);
		theApp.WriteProfileString("Options", "BSPMinSplitRange", temp);

		// Input/Output parameters

		theApp.WriteProfileString("Options", "outputRawDirectory", outputRawDirectory);
		theApp.WriteProfileString("Options", "inputFilename", inputFilename);
		theApp.WriteProfileString("Options", "outputOctFilename", outputOctFilename);
	}
	
	return CPropertyPage::OnKillActive();
}

// ---------------------------------------------------------------------------------------------------------------------------------
// DatabasePage.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
