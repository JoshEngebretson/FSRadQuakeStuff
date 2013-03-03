// ---------------------------------------------------------------------------------------------------------------------------------
//  _____        _         _                    _____                       _     
// |  __ \      | |       | |                  |  __ \                     | |    
// | |  | | __ _| |_  __ _| |__   __ _ ___  ___| |__) | __ _  __ _  ___    | |__  
// | |  | |/ _` | __|/ _` | '_ \ / _` / __|/ _ \  ___/ / _` |/ _` |/ _ \   | '_ \ 
// | |__| | (_| | |_| (_| | |_) | (_| \__ \  __/ |    | (_| | (_| |  __/ _ | | | |
// |_____/ \__,_|\__|\__,_|_.__/ \__,_|___/\___|_|     \__,_|\__, |\___|(_)|_| |_|
//                                                            __/ |               
//                                                           |___/                
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

#ifndef	_H_DATABASEPAGE
#define _H_DATABASEPAGE

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------

class	DatabasePage : public CPropertyPage
{
	DECLARE_DYNCREATE(DatabasePage)

public:
				DatabasePage();
				~DatabasePage();

	//{{AFX_DATA(DatabasePage)
	enum { IDD = IDD_DATABASE_PAGE };
	CEdit	rReflectivityEdit;
	CEdit	gReflectivityEdit;
	CEdit	bReflectivityEdit;
	CButton	outputOctButton;
	CEdit	outputOctFilenameEdit;
	CButton	outputOctBrowseButton;
	CEdit	inputFilenameEdit;
	CButton	inputFilenameBrowseButton;
	CButton	outputRawDirectoryBrowseButton;
	CButton	outputRawDirectoryButton;
	CEdit	gaussianResolutionEdit;
	CEdit	minSplitRangeEdit;
	CEdit	maxDepthEdit;
	CEdit	minRadiusEdit;
	CEdit	thresholdEdit;
	CEdit	outputRawDirectoryEdit;
	UINT	gaussianResolution;
	float	minSplitRange;
	UINT	maxDepth;
	float	minRadius;
	UINT	threshold;
	CString	outputRawDirectory;
	CString	inputFilename;
	CString	outputOctFilename;
	BOOL	outputOct;
	float	gReflectivity;
	float	bReflectivity;
	float	rReflectivity;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(DatabasePage)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(DatabasePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnOutputRawBrowse();
	afx_msg void OnOutputRawFlag();
	afx_msg void OnOutputOctFlag();
	afx_msg void OnOutputOctBrowse();
	afx_msg void OnInputBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
#endif // _H_DATABASEPAGE
// ---------------------------------------------------------------------------------------------------------------------------------
// DatabasePage.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
