// ---------------------------------------------------------------------------------------------------------------------------------
//  _____            _   _____                       _     
// |  __ \          | | |  __ \                     | |    
// | |__) | ___  ___| |_| |__) | __ _  __ _  ___    | |__  
// |  ___/ / _ \/ __| __|  ___/ / _` |/ _` |/ _ \   | '_ \ 
// | |    | (_) \__ \ |_| |    | (_| | (_| |  __/ _ | | | |
// |_|     \___/|___/\__|_|     \__,_|\__, |\___|(_)|_| |_|
//                                     __/ |               
//                                    |___/                
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

#ifndef	_H_POSTPAGE
#define _H_POSTPAGE

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------

class	PostPage : public CPropertyPage
{
	DECLARE_DYNCREATE(PostPage)

public:
				PostPage();	
				~PostPage();

	//{{AFX_DATA(PostPage)
	enum { IDD = IDD_POST_PAGE };
	CEdit	gammaValueEdit;
	CSliderCtrl	gammaSlider;
	CComboBox	clampingList;
	CStatic	ambientColorBox;
	CStatic	rAmbientBox;
	CStatic	gAmbientBox;
	CStatic	bAmbientBox;
	CSliderCtrl	gAmbientSlider;
	CSliderCtrl	rAmbientSlider;
	CSliderCtrl	bAmbientSlider;
	int		bAmbient;
	int		gAmbient;
	int		rAmbient;
	int		gamma;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(PostPage)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(PostPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
#endif // _H_POSTPAGE
// ---------------------------------------------------------------------------------------------------------------------------------
// PostPage.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
