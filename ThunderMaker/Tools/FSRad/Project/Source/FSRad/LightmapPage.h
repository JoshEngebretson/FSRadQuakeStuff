// ---------------------------------------------------------------------------------------------------------------------------------
//  _      _       _     _                         _____                       _     
// | |    (_)     | |   | |                       |  __ \                     | |    
// | |     _  __ _| |__ | |_ _ __ ___   __ _ _ __ | |__) | __ _  __ _  ___    | |__  
// | |    | |/ _` | '_ \| __| '_ ` _ \ / _` | '_ \|  ___/ / _` |/ _` |/ _ \   | '_ \ 
// | |____| | (_| | | | | |_| | | | | | (_| | |_) | |    | (_| | (_| |  __/ _ | | | |
// |______|_|\__, |_| |_|\__|_| |_| |_|\__,_| .__/|_|     \__,_|\__, |\___|(_)|_| |_|
//            __/ |                         | |                  __/ |               
//           |___/                          |_|                 |___/                
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

#ifndef	_H_LIGHTMAPPAGE
#define _H_LIGHTMAPPAGE

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------

class	LightmapPage : public CPropertyPage
{
	DECLARE_DYNCREATE(LightmapPage)

public:
				LightmapPage();
				~LightmapPage();

	//{{AFX_DATA(LightmapPage)
	enum { IDD = IDD_LIGHTMAP_PAGE };
	CEdit	vTexelsPerUnitEdit;
	CEdit	uTexelsPerUnitEdit;
	CEdit	lightmapWidthEdit;
	CEdit	lightmapHeightEdit;
	UINT	lightmapHeight;
	UINT	lightmapWidth;
	float	uTexelsPerUnit;
	float	vTexelsPerUnit;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(LightmapPage)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(LightmapPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
#endif // _H_LIGHTMAPPAGE
// ---------------------------------------------------------------------------------------------------------------------------------
// LightmapPage.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
