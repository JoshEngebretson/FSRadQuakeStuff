// ---------------------------------------------------------------------------------------------------------------------------------
//  ______  _____ _____            _                      
// |  ____|/ ____|  __ \          | |                     
// | |__  | (___ | |__) | __ _  __| |     ___ _ __  _ __  
// |  __|  \___ \|  _  / / _` |/ _` |    / __| '_ \| '_ \ 
// | |     ____) | | \ \| (_| | (_| | _ | (__| |_) | |_) |
// |_|    |_____/|_|  \_\\__,_|\__,_|(_) \___| .__/| .__/ 
//                                           | |   | |    
//                                           |_|   |_|    
// Description:
//
//   Application entry point
//
// Notes:
//
//   Best viewed with 8-character tabs and (at least) 132 columns
//
// History:
//
//   06/04/2001 by Paul Nettle: Original creation
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
#include "RadSheet.h"
#include "HelpDialog.h"
#include "RadGen.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

RadApp theApp;

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(RadApp, CWinApp)
	//{{AFX_MSG_MAP(RadApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	RadApp::RadApp()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	bool	runCommandLineArgs(const char * commandLine)
{
	// Create a radgen object and tell it to read the default parms from the registry...

	RadGen		radGen;
	radGen.readDefaultParms();

	// fstl::string form, plesae

	fstl::string	cmdLine = commandLine;

	// Strip out the program filename...

	if (cmdLine[0] == '\"')
	{
		int	idx = cmdLine.find("\"", 1);
		if (idx >= 0)
		{
			cmdLine.erase(0, idx+2); // +2 will include the space after it
		}
	}
	else
	{
		int	idx = cmdLine.find(" ");
		if (idx >= 0) cmdLine.erase(0, idx+1);
		if (idx < 0) cmdLine.erase();
	}

	// Start processing parameters
	unsigned int	parmsProcessed = 0;

	try
	{
		for (;;)
		{
			// Get this parameter

			fstl::string	thisParm = cmdLine.findWord(parmsProcessed, ' ');

			// Done with parms?

			if (!thisParm.length()) break;

			// Skip the first token, if it's a dash or slash

			if (thisParm[0] == '/' || thisParm[0] == '-') thisParm.erase(0, 1);

			// Make sure that's not ALL we got! :)

			if (!thisParm.length()) throw fstl::string("Invalid command line parm... no token");

			// Parse the parameter -- we'll be using 1- and 2-character commands, so we can't use switch/case :/

			if (thisParm.substring(0, 3) == "lw=")
			{
				radGen.lightmapWidth() = atoi(thisParm.substring(3).asArray());
			}
			else if (thisParm.substring(0, 3) == "lh=")
			{
				radGen.lightmapHeight() = atoi(thisParm.substring(3).asArray());
			}
			else if (thisParm.substring(0, 5) == "utpu=")
			{
				radGen.uTexelsPerUnit() = 1 / static_cast<float>(atof(thisParm.substring(5).asArray()));
			}
			else if (thisParm.substring(0, 5) == "vtpu=")
			{
				radGen.vTexelsPerUnit() = 1 / static_cast<float>(atof(thisParm.substring(5).asArray()));
			}
			else if (thisParm.substring(0, 9) == "otthresh=")
			{
				radGen.octreeThreshold() = atoi(thisParm.substring(9).asArray());
			}
			else if (thisParm.substring(0, 11) == "otmaxdepth=")
			{
				radGen.octreeMaxDepth() = atoi(thisParm.substring(11).asArray());
			}
			else if (thisParm.substring(0, 12) == "otminradius=")
			{
				radGen.octreeMinRadius() = static_cast<float>(atof(thisParm.substring(12).asArray()));
			}
			else if (thisParm.substring(0, 9) == "reflectr=")
			{
				radGen.defaultReflectivity().r() = static_cast<float>(atof(thisParm.substring(9).asArray()));
			}
			else if (thisParm.substring(0, 9) == "reflectg=")
			{
				radGen.defaultReflectivity().g() = static_cast<float>(atof(thisParm.substring(9).asArray()));
			}
			else if (thisParm.substring(0, 9) == "reflectb=")
			{
				radGen.defaultReflectivity().b() = static_cast<float>(atof(thisParm.substring(9).asArray()));
			}
			else if (thisParm.substring(0, 12) == "bspminsplit=")
			{
				radGen.bspMinSplitRange() = static_cast<float>(atof(thisParm.substring(12).asArray()));
			}
			else if (thisParm.substring(0, 8) == "bspgaus=")
			{
				radGen.bspGaussianResolution() = atoi(thisParm.substring(8).asArray());
			}
			else if (thisParm.substring(0, 10) == "rawfolder=")
			{
				radGen.writeRawLightmapsFolder() = thisParm.substring(10).asArray();
			}
			else if (thisParm.substring(0, 9) == "writeraw=")
			{
				radGen.writeRawLightmaps() = atoi(thisParm.substring(9).asArray()) ? true:false;
			}
			else if (thisParm.substring(0, 9) == "writeoct=")
			{
				radGen.writeOctFile() = atoi(thisParm.substring(9).asArray()) ? true:false;
			}
			else if (thisParm.substring(0, 8) == "octfile=")
			{
				radGen.writeOctFilename() = thisParm.substring(8).asArray();
			}
			else if (thisParm.substring(0, 7) == "infile=")
			{
				radGen.inputFilename() = thisParm.substring(7).asArray();
			}
			else if (thisParm.substring(0, 12) == "convergence=")
			{
				radGen.convergence() = static_cast<unsigned int>(atoi(thisParm.substring(12).asArray()));
			}
			else if (thisParm.substring(0, 17) == "usemaxiterations=")
			{
				radGen.maxIterations() = atoi(thisParm.substring(17).asArray()) ? true:false;
			}
			else if (thisParm.substring(0, 14) == "maxiterations=")
			{
				radGen.maxIterationsCount() = atoi(thisParm.substring(14).asArray());
			}
			else if (thisParm.substring(0, 9) == "areamult=")
			{
				radGen.areaLightMultiplier() = atoi(thisParm.substring(9).asArray());
			}
			else if (thisParm.substring(0, 10) == "pointmult=")
			{
				radGen.pointLightMultiplier() = static_cast<float>(atof(thisParm.substring(10).asArray()));
			}
			else if (thisParm.substring(0, 5) == "subu=")
			{
				radGen.subdivisionU() = atoi(thisParm.substring(5).asArray());
			}
			else if (thisParm.substring(0, 5) == "subv=")
			{
				radGen.subdivisionV() = atoi(thisParm.substring(5).asArray());
			}
			else if (thisParm.substring(0, 11) == "useambterm=")
			{
				radGen.ambientTerm() = atoi(thisParm.substring(11).asArray()) ? true:false;
			}
			else if (thisParm.substring(0, 11) == "usenusselt=")
			{
				radGen.useNusselt() = atoi(thisParm.substring(11).asArray()) ? true:false;
			}
			else if (thisParm.substring(0, 11) == "directonly=")
			{
				radGen.directLightOnly() = atoi(thisParm.substring(11).asArray()) ? true:false;
			}
			else if (thisParm.substring(0, 10) == "adaptmaxu=")
			{
				radGen.adaptiveMaxSubdivisionU() = atoi(thisParm.substring(10).asArray());
			}
			else if (thisParm.substring(0, 10) == "adaptmaxv=")
			{
				radGen.adaptiveMaxSubdivisionV() = atoi(thisParm.substring(10).asArray());
			}
			else if (thisParm.substring(0, 9) == "useadapt=")
			{
				radGen.adaptivePatchSubdivision() = atoi(thisParm.substring(9).asArray()) ? true:false;
			}
			else if (thisParm.substring(0, 12) == "adaptthresh=")
			{
				radGen.adaptiveThreshold() = static_cast<unsigned int>(atoi(thisParm.substring(12).asArray()));
			}
			else if (thisParm.substring(0, 6) == "gamma=")
			{
				radGen.gamma() = static_cast<float>(atof(thisParm.substring(6).asArray()));
			}
			else if (thisParm.substring(0, 5) == "ambr=")
			{
				radGen.ambient().r() = static_cast<float>(atof(thisParm.substring(5).asArray()));
			}
			else if (thisParm.substring(0, 5) == "ambg=")
			{
				radGen.ambient().g() = static_cast<float>(atof(thisParm.substring(5).asArray()));
			}
			else if (thisParm.substring(0, 5) == "ambb=")
			{
				radGen.ambient().b() = static_cast<float>(atof(thisParm.substring(5).asArray()));
			}
			else if (thisParm.substring(0, 14) == "clampsaturate=")
			{
				radGen.clamping() = RadGen::ClampSaturate;
			}
			else if (thisParm.substring(0, 10) == "clampnone=")
			{
				radGen.clamping() = RadGen::ClampSaturate;
			}
			else if (thisParm.substring(0, 12) == "clampretain=")
			{
				radGen.clamping() = RadGen::ClampRetain;
			}
			else
			{
				throw fstl::string("Invalid or unknown parameter:  ") + thisParm;
			}

			// We got one...

			parmsProcessed++;
		}
	}
	catch (const fstl::string err)
	{
		fstl::string	e = err + "\n\nClick OK for help...";
		AfxMessageBox(e.asArray());

		// Give them some help

		HelpDialog	dlg;
		dlg.DoModal();

		// Bail without running the process

		return true;
	}

	// If no command line parms, bail

	if (!parmsProcessed) return false;

	// Run the process

	radGen.go();

	// Exit, telling the caller the process was already run

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	RadApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application manifest specifies use of ComCtl32.dll version 6 or
	// later to enable visual styles.  Otherwise, any window creation will fail.

	InitCommonControls();

	// We're using a statically linked MFC DLL

	AfxEnableControlContainer();
	//Enable3dControlsStatic();

	// "This is who we are" (Millenium fans go nutz here :)

	SetRegistryKey("Fluid Studios");
	free(const_cast<void *>(static_cast<const void *>(theApp.m_pszProfileName)));
	theApp.m_pszProfileName = _tcsdup(_T("Fluid Studios Radiosity Processor"));

	// Check for command line arguments

	if (runCommandLineArgs(GetCommandLine()))
	{
		// We just ran the program, exit now...

		return FALSE;
	}

	// Do the dialog thang...

#ifdef	_DEBUG
	RadSheet dlg("Fluid Studios Radiosity Processor (DEBUG)");
#else
	RadSheet dlg("Fluid Studios Radiosity Processor");
#endif
	dlg.DoModal();
	return FALSE;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// FSRad.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

