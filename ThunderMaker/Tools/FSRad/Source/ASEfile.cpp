// ---------------------------------------------------------------------------------------------------------------------------------
//                    ______ _ _                           
//     /\            |  ____(_) |                          
//    /  \   ___  ___| |__   _| | ___      ___ _ __  _ __  
//   / /\ \ / __|/ _ \  __| | | |/ _ \    / __| '_ \| '_ \ 
//  / ____ \\__ \  __/ |    | | |  __/ _ | (__| |_) | |_) |
// /_/    \_\___/\___|_|    |_|_|\___|(_) \___| .__/| .__/ 
//                                            | |   | |    
//                                            |_|   |_|    
//
// Description:
//
//   Class for loading ASE files from 3DS Max
//
// Notes:
//
//   Best viewed with 8-character tabs and (at least) 132 columns
//
// History:
//
//   03/15/2002 by Paul Nettle: Original creation
//
// Restrictions & freedoms pertaining to usage and redistribution of this software:
//
//   This software is 100% free. If you use this software (in part or in whole) you must credit the author. This software may not be
//   re-distributed (in part or in whole) in a modified form without clear documentation on how to obtain a copy of the original
//   work. You may not use this software to directly or indirectly cause harm to others. This software is provided as-is and without
//   warrantee -- Use at your own risk. For more information, visit HTTP://www.FluidStudios.com/
//
// Copyright 2002, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "FSRad.h"
#include "AseFile.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TRACE printf

// ---------------------------------------------------------------------------------------------------------------------------------

static	const char *	newline = "\r\n";
static	const char *	spaces = " \t\v\f\r\n";

// ---------------------------------------------------------------------------------------------------------------------------------

	AseFile::AseFile()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

	AseFile::~AseFile()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	AseFile::load(const fstl::string & filename)
{
	FILE *	fp = NULL;
	char *	buf = NULL;
	
	try
	{
		// Check the file length

		unsigned int	fileLength;
		struct stat st;
		if(stat(filename.asArray(), &st)) fileLength = 0;
		fileLength = st.st_size;
		if (!fileLength) throw "File has no length, or is not a regular file";

		// Allocate RAM

		buf = new char[fileLength+1];
		if (!buf) throw "Unable to allocate memory";
		memset(buf, 0, fileLength+1);

		// Open up the file

		fp = fopen(filename.asArray(), "rb");
		if (!fp) throw "Unable to open ASE file";

		// Read it in...

		if (fread(buf, fileLength, 1, fp) != 1) throw "Unable to read file contents";

		// Done...

		fclose(fp);
		fp = NULL;

		// Transfer it into a string

		fstl::string	contents = buf;
		delete[] buf;
		buf = NULL;

		unsigned int	sindex = 0;
		fstl::string	perr = parse(contents, sindex);
		if (perr.length())
		{
			static	fstl::string	err = "Parsing error: " + perr;
			throw err.asArray();
		}

		if (!meshes().size())
		{
			static	fstl::string	err = "ASE file contains no meshes";
			throw err.asArray();
		}
	}
	catch(const char *err)
	{
		if (err && *err)
		{
			static	fstl::string	msg = fstl::string("Unable to load ASE file:\n\n") + err;
			//AfxMessageBox(msg.asArray());
			assert(0);
		}

		if (fp) fclose(fp);
		delete[] buf;

		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

fstl::string	AseFile::parse(const fstl::string & contents, unsigned int & sindex)
{
	// Start scanning for keywords

	while(sindex < contents.length())
	{
		// Skip the whitespace

		int	idx = contents.findFirstNotOf(spaces, sindex);
		if (idx == -1) break;
		sindex = idx;

		// Just reached the end?

		if (sindex >= contents.length()) break;

		// Assume recursion for blocks

		if (contents[sindex] == '}')
		{
			sindex++;
			return "";
		}

		// Just reached the end?

		if (sindex >= contents.length()) break;

		// Make sure it's a keyword

		if (contents[sindex] != '*')
		{
			return "Invalid keyword found, starting with: \"" + contents.substring(sindex, 10) + "\"";
		}

		// Skip the asterisk

		sindex++;

		// Which keyword?

		idx = contents.findFirstOf(spaces, sindex);
		if (idx == -1) return "Keyword not properly terminated";

		fstl::string	keyword = contents.substring(sindex, idx-sindex);
		sindex = idx;

		// Get the parameters for this keyword

		idx = contents.findFirstOf(newline, sindex);
		if (idx == -1) idx = contents.length();
		idx -= sindex;

		fstl::string	parms = contents.substring(sindex, idx);
		sindex += idx;

		// Does this line end with a bracket?

		bool	bracket = false;
		
		if (parms.length())
		{
			bracket = parms[parms.length()-1] == '{';
			if (bracket) parms.erase(parms.length() - 1);
		}

		// Parse keywords...

		     if (!keyword.ncCompare("MATERIAL_LIST"))
		{
			// We don't really care about material lists, but we need to handle their braces so we can traverse into
			// them properly...

			TRACE("Processing MATERIAL_LIST...\n");

			if (findMatchingBrace(contents, sindex) < 0) return "Mismatched brace for MATERIAL_LIST";

			// Just go recursive...

			fstl::string err = parse(contents, sindex);
			if (err.length()) return err;
		}
		else if (!keyword.ncCompare("MATERIAL"))
		{
			// Start a new material...

			TRACE(" Processing MATERIAL #%d...\n", materials().size() + 1);

			sAseMat	m;
			m.props.diffuseColor = geom::Color3(0,0,0);
			materials() += m;

			if (findMatchingBrace(contents, sindex) < 0) return "Mismatched brace for MATERIAL";

			// Just go recursive...

			fstl::string err = parse(contents, sindex);
			if (err.length()) return err;
		}
		else if (!keyword.ncCompare("MATERIAL_DIFFUSE"))
		{
			// Get current material...

			TRACE("   Processing MATERIAL_DIFFUSE...\n");

			if (!materials().size()) return "Diffuse color appears outside of a material";
			sAseMat &	m = materials()[materials().size()-1];

			// Dealing with a submaterial?

			geom::Color3 *	c = &m.props.diffuseColor;

			if (m.submats.size())
			{
				sAseSubMat &	sm = m.submats[m.submats.size()-1];
				c = &sm.diffuseColor;
			}

			// Parse the color...
			//
			// *MATERIAL_DIFFUSE 0.5880 0.5880 0.5880

			sscanf(parms.asArray(), "%f %f %f", &c->r(), &c->g(), &c->b());
		}
		else if (!keyword.ncCompare("SUBMATERIAL"))
		{
			// Get current material...

			if (!materials().size()) return "Submaterial appears outside of a material";
			sAseMat &	m = materials()[materials().size()-1];

			// Start a new sub material...

			TRACE("  Processing SUBMATERIAL #%d...\n", m.submats.size() + 1);

			sAseSubMat	sm;
			sm.diffuseColor = m.props.diffuseColor;
			m.submats += sm;

			if (findMatchingBrace(contents, sindex) < 0) return "Mismatched brace for SUBMATERIAL";

			// Just go recursive...

			fstl::string err = parse(contents, sindex);
			if (err.length()) return err;
		}
		else if (!keyword.ncCompare("MAP_DIFFUSE"))
		{
			// We don't really care about map diffuses, but we need to handle their braces so we can traverse into
			// them properly...

			TRACE("   Processing MAP_DIFFUSE...\n");

			if (findMatchingBrace(contents, sindex) < 0) return "Mismatched brace for MAP_DIFFUSE";

			// Just go recursive...

			fstl::string err = parse(contents, sindex);
			if (err.length()) return err;
		}
		else if (!keyword.ncCompare("BITMAP"))
		{
			// Get current material...

			TRACE("   Processing BITMAP...\n");

			if (!materials().size()) return "Bitmap appears outside of a material";
			sAseMat &	m = materials()[materials().size()-1];

			// Dealing with a submaterial?

			fstl::string *	s = &m.props.bitmap;

			if (m.submats.size())
			{
				sAseSubMat &	sm = m.submats[m.submats.size()-1];
				s = &sm.bitmap;
			}

			// Parse the bitmap filename
			//
			// *BITMAP "C:\DOCUME~1\PAULNE~1\Desktop\lodge-12\Blok-Wall-Trim.tga"

			*s = parms;
			s->trim(" \r\n\f\v\t");
		}
		else if (!keyword.ncCompare("GEOMOBJECT"))
		{
			// We don't really care about geomobjects, but we need to handle their braces so we can traverse into
			// them properly...

			TRACE("Processing GEOMOBJECT...\n");

			if (findMatchingBrace(contents, sindex) < 0) return "Mismatched brace for GEOMOBJECT";

			// Just go recursive...

			fstl::string err = parse(contents, sindex);
			if (err.length()) return err;
		}
		else if (!keyword.ncCompare("MESH"))
		{
			// Start a new Mesh...

			TRACE(" Processing MESH #%d...\n", meshes().size() + 1);

			sAseMesh	m;
			meshes() += m;

			if (findMatchingBrace(contents, sindex) < 0) return "Mismatched brace for MESH";

			// Just go recursive...

			fstl::string err = parse(contents, sindex);
			if (err.length()) return err;
		}
		else if (!keyword.ncCompare("MESH_VERTEX_LIST"))
		{
			TRACE("  Processing MESH_VERTEX_LIST...\n");
			if (findMatchingBrace(contents, sindex) < 0) return "Mismatched brace for MESH_VERTEX_LIST";

			// Just go recursive...

			fstl::string err = parse(contents, sindex);
			if (err.length()) return err;
		}
		else if (!keyword.ncCompare("MESH_VERTEX"))
		{
			// Get current mesh...

			if (!meshes().size()) return "Vertex appears outside of a mesh";
			sAseMesh &	m = meshes()[meshes().size()-1];

			// Parse the vertex...
			//
			// *MESH_VERTEX 0 17.8389 -30.0832 14.2214

			geom::Point3	v;
			int		dummy;
			sscanf(parms.asArray(), "%d %f %f %f", &dummy, &v.x(), &v.y(), &v.z());
			m.verts += v;
		}
		else if (!keyword.ncCompare("MESH_FACE_LIST"))
		{
			TRACE("  Processing MESH_FACE_LIST...\n");
			if (findMatchingBrace(contents, sindex) < 0) return "Mismatched brace for MESH_FACE_LIST";

			// Just go recursive...

			fstl::string err = parse(contents, sindex);
			if (err.length()) return err;
		}
		else if (!keyword.ncCompare("MESH_FACE"))
		{
			// Get current mesh...

			if (!meshes().size()) return "Face appears outside of a mesh";
			sAseMesh &	m = meshes()[meshes().size()-1];

			// Parse the face...
			//
			// *MESH_FACE 0: A: 1 B: 19 C: 18 AB: 0 BC: 1 CA: 1 *MESH_SMOOTHING *MESH_MTLID 1

			int	dummy;
			int	a, b, c;
			sscanf(parms.asArray(), "%d: A: %d B: %d C: %d", &dummy, &a, &b, &c);
			m.faces += a;
			m.faces += b;
			m.faces += c;

			// Get the material ID, if it exists

			int	matid = 0;
			int	idx = parms.find("MESH_MTLID");
			if (idx >= 0)
			{
				fstl::string	matstr = parms.substring(idx+11);
				matid = atoi(matstr.asArray());
			}

			m.matids += matid;
		}
		else if (!keyword.ncCompare("MESH_TVERTLIST"))
		{
			TRACE("  Processing MESH_TVERTLIST...\n");
			if (findMatchingBrace(contents, sindex) < 0) return "Mismatched brace for MESH_TVERTLIST";

			// Just go recursive...

			fstl::string err = parse(contents, sindex);
			if (err.length()) return err;
		}
		else if (!keyword.ncCompare("MESH_TVERT"))
		{
			// Get current mesh...

			if (!meshes().size()) return "TVertex appears outside of a mesh";
			sAseMesh &	m = meshes()[meshes().size()-1];

			// Parse the textured vertex...
			//
			// *MESH_TVERT 0 17.8389 -30.0832 14.2214

			geom::Point3	v;
			int		dummy;
			sscanf(parms.asArray(), "%d %f %f %f", &dummy, &v.x(), &v.y(), &v.z());
			m.tverts += v;
		}
		else if (!keyword.ncCompare("MESH_TFACELIST"))
		{
			TRACE("  Processing MESH_TFACELIST...\n");
			if (findMatchingBrace(contents, sindex) < 0) return "Mismatched brace for MESH_TFACELIST";

			// Just go recursive...

			fstl::string err = parse(contents, sindex);
			if (err.length()) return err;
		}
		else if (!keyword.ncCompare("MESH_TFACE"))
		{
			// Get current mesh...

			if (!meshes().size()) return "TFace appears outside of a mesh";
			sAseMesh &	m = meshes()[meshes().size()-1];

			// Parse the face...
			//
			// *MESH_TFACE 324	188	908	909

			int	dummy;
			int	a, b, c;
			sscanf(parms.asArray(), "%d %d %d %d", &dummy, &a, &b, &c);
			m.tfaces += a;
			m.tfaces += b;
			m.tfaces += c;
		}
		else if (!keyword.ncCompare("LIGHTOBJECT"))
		{
			// Start a new Light...

			TRACE(" Processing LIGHTOBJECT #%d...\n", lights().size() + 1);

			sAseLight	l;
			memset(&l, 0, sizeof(l));
			l.intensity = 1.0f;
			lights() += l;

			if (findMatchingBrace(contents, sindex) < 0) return "Mismatched brace for LIGHTOBJECT";

			// Just go recursive...

			fstl::string err = parse(contents, sindex);
			if (err.length()) return err;

			// When we finish, copy the node information into the light

			sAseLight &	ll = lights()[lights().size() - 1];
			ll.pos = cnode().pos;

			// Wipe out the node for the next guy...

			memset(&cnode(), 0, sizeof(cnode()));
		}
		else if (!keyword.ncCompare("NODE_TM"))
		{
			TRACE("  Processing NODE_TM...\n");
			if (findMatchingBrace(contents, sindex) < 0) return "Mismatched brace for NODE_TM";

			// Clear the current node...

			memset(&cnode(), 0, sizeof(cnode()));

			// Just go recursive...

			fstl::string err = parse(contents, sindex);
			if (err.length()) return err;
		}
		else if (!keyword.ncCompare("LIGHT_USELIGHT"))
		{
			// Get current light...

			if (!lights().size()) return "Uselight appears outside of a light object";
			sAseLight &	l = lights()[lights().size()-1];

			// Parse the USELIGHT
			//
			// *LIGHT_USELIGHT 1

			int	temp;
			sscanf(parms.asArray(), "%d", &temp);
			l.enabled = temp != 0;

		}
		else if (!keyword.ncCompare("TM_POS"))
		{
			// Parse the pos...
			//
			// *TM_POS 187.3466 200.4976 82.6469

			sscanf(parms.asArray(), "%f %f %f", &cnode().pos.x(), &cnode().pos.y(), &cnode().pos.z());
		}
		else if (!keyword.ncCompare("LIGHT_SETTINGS"))
		{
			TRACE("  Processing LIGHT_SETTINGS...\n");
			if (findMatchingBrace(contents, sindex) < 0) return "Mismatched brace for LIGHT_SETTINGS";

			// Just go recursive...

			fstl::string err = parse(contents, sindex);
			if (err.length()) return err;
		}
		else if (!keyword.ncCompare("LIGHT_COLOR"))
		{
			// Get current light...

			if (!lights().size()) return "Uselight appears outside of a light object";
			sAseLight &	l = lights()[lights().size()-1];

			// Parse the light's color
			//
			// *LIGHT_COLOR 1.0000 1.0000 1.0000

			sscanf(parms.asArray(), "%f %f %f", &l.color.r(), &l.color.g(), &l.color.b());
		}
		else if (!keyword.ncCompare("LIGHT_INTENS"))
		{
			// Get current light...

			if (!lights().size()) return "Uselight appears outside of a light object";
			sAseLight &	l = lights()[lights().size()-1];

			// Parse the light intensity
			//
			// *LIGHT_INTENS 1.0000

			sscanf(parms.asArray(), "%f", &l.intensity);
		}

		// Skip unknown keywords

		else
		{
			// If there is a bracket, skip to the matching bracket

			if (bracket)
			{
				int	skipCount = findMatchingBrace(contents, sindex);
				if (skipCount < 0) return "Mismatched braces";

				sindex += skipCount;
			}
		}
	}

	return "";
}

// ---------------------------------------------------------------------------------------------------------------------------------
// When this is called, sindex should point to the character just after the brace being matched
// ---------------------------------------------------------------------------------------------------------------------------------

int	AseFile::findMatchingBrace(const fstl::string & contents, const unsigned int sindex)
{
	int	bcount = 1;
	int	cur = 0;

	while(bcount && sindex < contents.length())
	{
		int	idx = contents.findFirstOf("{}", sindex + cur);
		if (idx == -1) break;
		idx -= sindex + cur;

		if (contents[sindex+cur+idx] == '{')	++bcount;
		else					--bcount;

		cur += idx + 1;
	}

	if (bcount) -1;

	return cur;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// AseFile.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
