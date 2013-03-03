// ---------------------------------------------------------------------------------------------------------------------------------
//                    ______ _ _          _     
//     /\            |  ____(_) |        | |    
//    /  \   ___  ___| |__   _| | ___    | |__  
//   / /\ \ / __|/ _ \  __| | | |/ _ \   | '_ \ 
//  / ____ \\__ \  __/ |    | | |  __/ _ | | | |
// /_/    \_\___/\___|_|    |_|_|\___|(_)|_| |_|
//                                              
//                                              
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

#ifndef	_H_ASEFILE
#define _H_ASEFILE

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------

typedef	struct	asemesh
{
	geom::Point3Array	verts;
	fstl::uintArray		faces;

	geom::Point3Array	tverts;
	fstl::uintArray		tfaces;

	fstl::uintArray		matids;
} sAseMesh;

typedef	fstl::array<sAseMesh>	AseMeshArray;
typedef	fstl::list<sAseMesh>	AseMeshList;

// ---------------------------------------------------------------------------------------------------------------------------------

typedef	struct	aselight
{
	bool			enabled;
	float			intensity;
	geom::Color3		color;
	geom::Point3		pos;
} sAseLight;

typedef	fstl::array<sAseLight>	AseLightArray;
typedef	fstl::list<sAseLight>	AseLightList;

// ---------------------------------------------------------------------------------------------------------------------------------

typedef	struct	asesubmat
{
	geom::Color3		diffuseColor;
	fstl::string		bitmap;
} sAseSubMat;

typedef	fstl::array<sAseSubMat>	AseSubMatArray;
typedef	fstl::list<sAseSubMat>	AseSubMatList;

// ---------------------------------------------------------------------------------------------------------------------------------

typedef	struct	asemat
{
	AseSubMatArray		submats;
	sAseSubMat		props;
} sAseMat;

typedef	fstl::array<sAseMat>	AseMatArray;
typedef	fstl::list<sAseMat>	AseMatList;

// ---------------------------------------------------------------------------------------------------------------------------------

typedef	struct	asenode
{
	geom::Point3		pos;
} sAseNode;

// ---------------------------------------------------------------------------------------------------------------------------------

class	AseFile
{
public:
	// Construction/Destruction

				AseFile();
virtual				~AseFile();

	// Operators

	// Implementation

virtual		bool		load(const fstl::string & filename);
virtual		fstl::string	parse(const fstl::string & contents, unsigned int & sindex);
virtual		int		findMatchingBrace(const fstl::string & contents, const unsigned int sindex);

	// Accessors

inline		AseMeshArray &	meshes()		{return _meshes;}
inline	const	AseMeshArray &	meshes() const		{return _meshes;}
inline		AseLightArray &	lights()		{return _lights;}
inline	const	AseLightArray &	lights() const		{return _lights;}
inline		AseMatArray &	materials()		{return _materials;}
inline	const	AseMatArray &	materials() const	{return _materials;}
inline		sAseNode &	cnode()			{return _cnode;}
inline	const	sAseNode &	cnode() const		{return _cnode;}

private:
	// Data members

		AseMeshArray	_meshes;
		AseLightArray	_lights;
		AseMatArray	_materials;
		sAseNode	_cnode;
};

#endif // _H_ASEFILE
// ---------------------------------------------------------------------------------------------------------------------------------
// AseFile.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
