// ---------------------------------------------------------------------------------------------------------------------------------
//  _    _      _       _____  _       _                 _     
// | |  | |    | |     |  __ \(_)     | |               | |    
// | |__| | ___| |_ __ | |  | |_  __ _| | ___   __ _    | |__  
// |  __  |/ _ \ | '_ \| |  | | |/ _` | |/ _ \ / _` |   | '_ \ 
// | |  | |  __/ | |_) | |__| | | (_| | | (_) | (_| | _ | | | |
// |_|  |_|\___|_| .__/|_____/|_|\__,_|_|\___/ \__, |(_)|_| |_|
//               | |                            __/ |          
//               |_|                           |___/           
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
//   10/16/2001 by Paul Nettle: Original creation
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
//{{AFX_INCLUDES()
#include "webbrowser.h"
#include "afxwin.h"
//}}AFX_INCLUDES

#ifndef	_H_HELPDIALOG
#define _H_HELPDIALOG

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------

class	HelpDialog : public CDialog
{
public:
				HelpDialog(CWnd* pParent = NULL);

	//{{AFX_DATA(HelpDialog)
	enum { IDD = IDD_HELP_DIALOG };
	WebBrowser	helpBrowser;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(HelpDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(HelpDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CButton okButton;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

//{{AFX_INSERT_LOCATION}}
#endif // _H_HELPDIALOG
// ---------------------------------------------------------------------------------------------------------------------------------
// HelpDialog.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
