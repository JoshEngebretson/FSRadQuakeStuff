// ---------------------------------------------------------------------------------------------------------------------------------
//   _____                            _ _____                       _     
//  / ____|                          | |  __ \                     | |    
// | |  __  ___ _ __   ___ _ __  __ _| | |__) | __ _  __ _  ___    | |__  
// | | |_ |/ _ \ '_ \ / _ \ '__|/ _` | |  ___/ / _` |/ _` |/ _ \   | '_ \ 
// | |__| |  __/ | | |  __/ |  | (_| | | |    | (_| | (_| |  __/ _ | | | |
//  \_____|\___|_| |_|\___|_|   \__,_|_|_|     \__,_|\__, |\___|(_)|_| |_|
//                                                    __/ |               
//                                                   |___/                
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

#ifndef	_H_GENERALPAGE
#define _H_GENERALPAGE

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------

class	GeneralPage : public CPropertyPage
{
				DECLARE_DYNCREATE(GeneralPage)

public:
				GeneralPage();
				~GeneralPage();

	//{{AFX_DATA(GeneralPage)
	enum { IDD = IDD_GENERAL_PAGE };
	CEdit	pointLightMultiplierEdit;
	CEdit	adaptiveThresholdEdit;
	CStatic	adaptiveXStatic;
	CStatic	adaptiveThresholdStatic;
	CStatic	adaptivePercentStatic;
	CButton	adaptivePatchesButton;
	CEdit	adaptiveMaxVEdit;
	CEdit	adaptiveMaxUEdit;
	CStatic	adaptiveMaxStatic;
	CButton	directLightButton;
	CButton	useNusseltButton;
	CButton	indirectSubdivisionButton;
	CEdit	subdivisionVEdit;
	CEdit	subdivisionUEdit;
	CButton	ambientTermButton;
	CEdit	maxIterationsCountEdit;
	CButton	maxIterationsButton;
	CEdit	areaLightMultiplierEdit;
	CEdit	convergenceEdit;
	CButton	exitWhenFinishedButton;
	CButton	leaveResultsButton;
	BOOL	leaveResults;
	BOOL	exitWhenFinished;
	UINT	convergence;
	UINT	areaLightMultiplier;
	BOOL	maxIterations;
	UINT	maxIterationsCount;
	UINT	subdivisionU;
	UINT	subdivisionV;
	BOOL	ambientTerm;
	BOOL	useNusselt;
	BOOL	directLight;
	BOOL	adaptivePatches;
	UINT	adaptiveThreshold;
	UINT	adaptiveMaxU;
	UINT	adaptiveMaxV;
	float	pointLightMultiplier;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(GeneralPage)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(GeneralPage)
	afx_msg void OnLeaveResults();
	afx_msg void OnExitWhenFinished();
	virtual BOOL OnInitDialog();
	afx_msg void OnMaxIterations();
	afx_msg void OnGo();
	afx_msg void OnAdaptivePatches();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

virtual		bool		saveSettings();
};

//{{AFX_INSERT_LOCATION}}
#endif // _H_GENERALPAGE
// ---------------------------------------------------------------------------------------------------------------------------------
// GeneralPage.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
