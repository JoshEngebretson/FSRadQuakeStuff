// ---------------------------------------------------------------------------------------------------------------------------------
//  _____                                     _____  _                            
// |  __ \                                   |  __ \| |                           
// | |__) |_ __  ___   __ _ _ __  ___ ___ ___| |  | | | __ _      ___ _ __  _ __  
// |  ___/| '__|/ _ \ / _` | '__|/ _ \ __/ __| |  | | |/ _` |    / __| '_ \| '_ \ 
// | |    | |  | (_) | (_| | |  |  __/__ \__ \ |__| | | (_| | _ | (__| |_) | |_) |
// |_|    |_|   \___/ \__, |_|   \___|___/___/_____/|_|\__, |(_) \___| .__/| .__/ 
//                     __/ |                            __/ |        | |   | |    
//                    |___/                            |___/         |_|   |_|    
//
// Description:
//
//   Progress dialog
//
// Notes:
//
//   Best viewed with 8-character tabs and (at least) 132 columns
//
// History:
//
//   08/03/2001 by Paul Nettle: Original creation
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
#include "ProgressDlg.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(ProgressDlg, CDialog)
	//{{AFX_MSG_MAP(ProgressDlg)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	ProgressDlg::ProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ProgressDlg::IDD, pParent), _supressAreYouSure(false), _stopRequested(false), _paused(false), _runtimeStart(time(NULL)), _lastTime(0)
{
	//{{AFX_DATA_INIT(ProgressDlg)
	//}}AFX_DATA_INIT

	Create(IDD_PROGRESS, pParent);
}

// ---------------------------------------------------------------------------------------------------------------------------------

	ProgressDlg::~ProgressDlg()
{
	DestroyWindow();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ProgressDlg)
	DDX_Control(pDX, IDC_PAUSE, pauseButton);
	DDX_Control(pDX, IDC_TEXT10, text10);
	DDX_Control(pDX, IDC_TEXT9, text9);
	DDX_Control(pDX, IDC_TEXT8, text8);
	DDX_Control(pDX, IDC_TEXT7, text7);
	DDX_Control(pDX, IDC_TEXT6, text6);
	DDX_Control(pDX, IDC_TEXT5, text5);
	DDX_Control(pDX, IDC_TEXT4, text4);
	DDX_Control(pDX, IDC_TEXT3, text3);
	DDX_Control(pDX, IDC_TEXT2, text2);
	DDX_Control(pDX, IDC_TEXT1, text1);
	DDX_Control(pDX, IDC_LABEL10, label10);
	DDX_Control(pDX, IDC_LABEL9, label9);
	DDX_Control(pDX, IDC_LABEL8, label8);
	DDX_Control(pDX, IDC_LABEL7, label7);
	DDX_Control(pDX, IDC_LABEL6, label6);
	DDX_Control(pDX, IDC_LABEL5, label5);
	DDX_Control(pDX, IDC_LABEL4, label4);
	DDX_Control(pDX, IDC_LABEL3, label3);
	DDX_Control(pDX, IDC_LABEL2, label2);
	DDX_Control(pDX, IDC_LABEL1, label1);
	DDX_Control(pDX, IDC_RUNTIME_TEXT, runtimeText);
	DDX_Control(pDX, IDC_STOP, stopButton);
	DDX_Control(pDX, IDC_CURRENT_TASK_TEXT, currentTaskText);
	DDX_Control(pDX, IDC_CURRENT_TASK_PROGRESS, currentTaskProgress);
	DDX_Control(pDX, IDC_CURRENT_TASK_PERCENT, currentTaskPercent);
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	ProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	currentTaskProgress.SetRange(0, 1000);

	setCurrentStatus("");
	setCurrentPercent(0.0f);
	
	ShowWindow(SW_SHOW);

	HICON	hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::OnOK()
{
	return;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::OnCancel()
{
	OnStop();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::OnStop() 
{
	if (!supressAreYouSure() && AfxMessageBox("Are you sure you wish to stop processing?", MB_YESNO) != IDYES) return;

	stopRequested() = true;
	stopButton.EnableWindow(FALSE);
	pauseButton.EnableWindow(FALSE);
	setCurrentStatus("Stopping...");
	RedrawWindow();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::OnPause() 
{
	paused() = !paused();
	if (paused())
	{
		pauseButton.SetWindowText("Resume");
		runtimeText.SetWindowText("Paused");
		RedrawWindow();
	}
	else
	{
		pauseButton.SetWindowText("Pause");
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setCurrentStatus(const fstl::string & text)
{
	if (!stopRequested())
	{
		currentTaskText.SetWindowText(text.asArray());
	}

	allowBackgroundProcessing();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setCurrentPercent(const float percent)
{
	if (!stopRequested())
	{
		static	float	lastPercent = 23456.0f;
		if (percent != lastPercent)
		{
			lastPercent = percent;
			char	dsp[20];
			sprintf(dsp, "%.3f%%", percent);
			currentTaskPercent.SetWindowText(dsp);
			currentTaskProgress.SetPos(static_cast<int>(percent * 10.0f));
		}

		{
			char	disp[90];
			memset(disp, 0, sizeof(disp));
			currentTaskText.GetWindowText(disp, sizeof(disp));

			char	disp2[90];
#ifdef	_DEBUG
			sprintf(disp2, "%.2f%% - %s (DEBUG)", percent, disp);
#else
			sprintf(disp2, "%.2f%% - %s", percent, disp);
#endif
			SetWindowText(disp2);
		}
	}

	// Update the timer, if it has changed

	int	thisTime = time(NULL);
	if (thisTime != lastTime() && !paused())
	{
		lastTime() = thisTime;
		int	seconds = lastTime() - runtimeStart();
		int	minutes = seconds / 60; seconds -= minutes * 60;
		int	hours = minutes / 60; minutes -= hours * 60;
		char	dsp[90];
		sprintf(dsp, "%02d:%02d:%02d", hours, minutes, seconds);
		runtimeText.SetWindowText(dsp);
	}

	allowBackgroundProcessing();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setText1(const fstl::string & text)
{
	if (!stopRequested()) text1.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setText2(const fstl::string & text)
{
	if (!stopRequested()) text2.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setText3(const fstl::string & text)
{
	if (!stopRequested()) text3.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setText4(const fstl::string & text)
{
	if (!stopRequested()) text4.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setText5(const fstl::string & text)
{
	if (!stopRequested()) text5.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setText6(const fstl::string & text)
{
	if (!stopRequested()) text6.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setText7(const fstl::string & text)
{
	if (!stopRequested()) text7.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setText8(const fstl::string & text)
{
	if (!stopRequested()) text8.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setText9(const fstl::string & text)
{
	if (!stopRequested()) text9.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setText10(const fstl::string & text)
{
	if (!stopRequested()) text10.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setLabel1(const fstl::string & text)
{
	if (!stopRequested()) label1.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setLabel2(const fstl::string & text)
{
	if (!stopRequested()) label2.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setLabel3(const fstl::string & text)
{
	if (!stopRequested()) label3.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setLabel4(const fstl::string & text)
{
	if (!stopRequested()) label4.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setLabel5(const fstl::string & text)
{
	if (!stopRequested()) label5.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setLabel6(const fstl::string & text)
{
	if (!stopRequested()) label6.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setLabel7(const fstl::string & text)
{
	if (!stopRequested()) label7.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setLabel8(const fstl::string & text)
{
	if (!stopRequested()) label8.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setLabel9(const fstl::string & text)
{
	if (!stopRequested()) label9.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::setLabel10(const fstl::string & text)
{
	if (!stopRequested()) label10.SetWindowText(text.asArray());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::allowBackgroundProcessing()
{
	for(;;)
	{
		for (int i = 0; i < 10; i++)
		{
			MSG	msg;
			if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (!paused() || stopRequested()) break;

		Sleep(100);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ProgressDlg::resetStopped()
{
	stopRequested() = false;
	if (paused()) OnPause();
	stopButton.EnableWindow(TRUE);
	pauseButton.EnableWindow(TRUE);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ProgressDlg.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
