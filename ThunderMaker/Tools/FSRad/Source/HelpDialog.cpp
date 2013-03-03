// ---------------------------------------------------------------------------------------------------------------------------------
//  _    _      _       _____  _       _                                  
// | |  | |    | |     |  __ \(_)     | |                                 
// | |__| | ___| |_ __ | |  | |_  __ _| | ___   __ _      ___ _ __  _ __  
// |  __  |/ _ \ | '_ \| |  | | |/ _` | |/ _ \ / _` |    / __| '_ \| '_ \ 
// | |  | |  __/ | |_) | |__| | | (_| | | (_) | (_| | _ | (__| |_) | |_) |
// |_|  |_|\___|_| .__/|_____/|_|\__,_|_|\___/ \__, |(_) \___| .__/| .__/ 
//               | |                            __/ |        | |   | |    
//               |_|                           |___/         |_|   |_|    
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

#include "stdafx.h"
#include "FSRad.h"
#include "HelpDialog.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(HelpDialog, CDialog)
	//{{AFX_MSG_MAP(HelpDialog)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	HelpDialog::HelpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(HelpDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(HelpDialog)
	//}}AFX_DATA_INIT
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	HelpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HelpDialog)
	DDX_Control(pDX, IDC_HELPBROWSE, helpBrowser);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, okButton);
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	HelpDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	char	buf[90];
	GetTempPath(sizeof(buf) - 1, buf);

	// Our Help page uses an image, which is also in the resources...
	{
		fstl::string	tempFileName = fstl::string(buf) + "FSRadHelp.jpg";

		HRSRC	hrsc = FindResource(NULL, MAKEINTRESOURCE(IDR_RT_HELP), RT_RCDATA);
		if (!hrsc)
		{
			AfxMessageBox("Unable to find help resource");
			return TRUE;
		}

		DWORD	helpSize = SizeofResource(NULL, hrsc);
		if (!helpSize)
		{
			AfxMessageBox("Unable to locate help resource");
			return TRUE;
		}

		HGLOBAL	hHelp = LoadResource(NULL, hrsc);
		if (!hHelp)
		{
			AfxMessageBox("Unable to load help resource");
			return TRUE;
		}

		void *	helpMem = LockResource(hHelp);
		if (!helpMem)
		{
			AfxMessageBox("Unable to lock help resource");
			return TRUE;
		}

		FILE *	fp = fopen(tempFileName.asArray(), "wb");
		if (fp)
		{
			fwrite(helpMem, helpSize, 1, fp);
			fclose(fp);
		}
	}

	// Create and view the HTML page
	{
		fstl::string	tempFileName = fstl::string(buf) + "FSRadHelp.html";

		HRSRC	hrsc = FindResource(NULL, MAKEINTRESOURCE(IDR_HELP), RT_HTML);
		if (!hrsc)
		{
			AfxMessageBox("Unable to find help resource");
			return TRUE;
		}

		DWORD	helpSize = SizeofResource(NULL, hrsc);
		if (!helpSize)
		{
			AfxMessageBox("Unable to locate help resource");
			return TRUE;
		}

		HGLOBAL	hHelp = LoadResource(NULL, hrsc);
		if (!hHelp)
		{
			AfxMessageBox("Unable to load help resource");
			return TRUE;
		}

		void *	helpMem = LockResource(hHelp);
		if (!helpMem)
		{
			AfxMessageBox("Unable to lock help resource");
			return TRUE;
		}

		FILE *	fp = fopen(tempFileName.asArray(), "wb");
		if (fp)
		{
			fwrite(helpMem, helpSize, 1, fp);
			fclose(fp);

			COleVariant varEmpty;
			fstl::string	url("file://");
			url += tempFileName;
			helpBrowser.Navigate(tempFileName.asArray(), &varEmpty, &varEmpty, &varEmpty, &varEmpty);
		}
	}
	
	HICON	hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	HelpDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (!okButton.GetSafeHwnd()) return;

	CRect	clientRect;
	GetClientRect(clientRect);

	// Border...

	clientRect.left += 11;
	clientRect.top += 11;
	clientRect.right -= 11;
	clientRect.bottom -= 11;

	CRect	buttonRect;
	okButton.GetWindowRect(buttonRect);

        helpBrowser.MoveWindow(clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height() - buttonRect.Height() - 8);

	okButton.MoveWindow(clientRect.right - buttonRect.Width(), clientRect.bottom - buttonRect.Height(), buttonRect.Width(), buttonRect.Height());
}

// ---------------------------------------------------------------------------------------------------------------------------------
// HelpDialog.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
