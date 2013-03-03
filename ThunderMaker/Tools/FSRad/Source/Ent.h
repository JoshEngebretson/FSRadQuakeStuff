// ---------------------------------------------------------------------------------------------------------------------------------
//  ______       _       _     
// |  ____|     | |     | |    
// | |__   _ __ | |_    | |__  
// |  __| | '_ \| __|   | '_ \ 
// | |____| | | | |_  _ | | | |
// |______|_| |_|\__|(_)|_| |_|
//                             
//                             
//
// Description:
//
//   ENT file format
//
// Notes:
//
//   Best viewed with 8-character tabs and (at least) 132 columns
//
// History:
//
//   08/14/2001 by Paul Nettle: Original creation
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

#ifndef	_H_ENT
#define _H_ENT

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------

class	GeomDB;
class	RadPoly;

// ---------------------------------------------------------------------------------------------------------------------------------

typedef	struct	tag_ent_color
{
	float	b, g, r;
} entColor;
typedef	fstl::array<entColor>		entColorArray;

// ---------------------------------------------------------------------------------------------------------------------------------

typedef	struct	tag_ent_material
{
	entColor	illuminationColor;
	float		illuminationMultiplier;
	entColor	reflectanceColor;
	float		reflectanceMultiplier;
} entMaterial;
typedef	fstl::array<entMaterial>	entMaterialArray;

// ---------------------------------------------------------------------------------------------------------------------------------

typedef	struct	tag_ent_poly
{
	unsigned int		verts[3];
	geom::Point2		tex[3];
} entPoly;
typedef	fstl::array<entPoly>		entPolyArray;

// ---------------------------------------------------------------------------------------------------------------------------------

typedef	struct	tag_ent_object
{
	geom::Point3Array	verts;
	entPolyArray		polys;
	unsigned int		materialID;
} entObject;
typedef	fstl::array<entObject>		entObjectArray;

// ---------------------------------------------------------------------------------------------------------------------------------

class	EntFile
{
public:
	enum		{MAX_FILENAME_LEN = 255};
	enum		{ENT_SIGNATURE_LEN = 128};
	enum		{STACK_SIZE = 256};
	enum		{ENT_TYPE_STRING  = 0x01};
	enum		{ENT_TYPE_BOOLEAN = 0x02};
	enum		{ENT_TYPE_INTEGER = 0x03};
	enum		{ENT_TYPE_FLOAT32 = 0x04};
	enum		{ENT_TYPE_FLOAT64 = 0x05};
	enum		{ENT_TYPE_XY      = 0x06};
	enum		{ENT_TYPE_XYZ     = 0x07};
	enum		{ENT_TYPE_MATRIX  = 0x08};
	enum		{ENT_TYPE_RGB     = 0x09};

	// File tags

	enum		{  ETAG_TERMINATOR					= 0x00000000			};
	enum		{  ETAG_IDENTIFIER					= 0xBAAADF00			};
	enum		{   ETAG_FILEINFO					= 0x10000000			};
	enum		{   ETAG_PARAMETERS					= 0x20000000			};
	enum		{   ETAG_GEOMETRY					= 0x30000000			};
	enum		{    ETAG_GEOMETRY_OBJECT_COUNT				= 0x31000000			};
	enum		{     ETAG_GEOMETRY_OBJECT_VERTEX_COUNT			= 0x31030000			};
	enum		{      ETAG_GEOMETRY_OBJECT_VERTEX_LOCATION		= 0x31030300 + ENT_TYPE_XYZ	};
	enum		{     ETAG_GEOMETRY_OBJECT_POLY_COUNT			= 0x31040000			};
	enum		{      ETAG_GEOMETRY_OBJECT_POLY_MATERIAL		= 0x31040200 + ENT_TYPE_INTEGER	};
	enum		{      ETAG_GEOMETRY_OBJECT_POLY_VERTEX_COUNT		= 0x31040300			};
	enum		{       ETAG_GEOMETRY_OBJECT_POLY_VERTEX_INDEX		= 0x31040400 + ENT_TYPE_INTEGER	};
	enum		{   ETAG_MATERIAL	      				= 0x40000000			};
	enum		{    ETAG_MATERIAL_COUNT      				= 0x41000000			};
	enum		{     ETAG_MATERIAL_ILLUMINATION_COLOR			= 0x41040000 + ENT_TYPE_RGB	};
	enum		{     ETAG_MATERIAL_ILLUMINATION_MULTIPLIER		= 0x41050000 + ENT_TYPE_FLOAT32 };
	enum		{     ETAG_MATERIAL_REFLECTANCE_COLOR			= 0x41060000 + ENT_TYPE_RGB	};
	enum		{     ETAG_MATERIAL_REFLECTANCE_MULTIPLIER		= 0x41070000 + ENT_TYPE_FLOAT32	};

private:
	char		fName[MAX_FILENAME_LEN + 1];
	FILE *		pFile;
	unsigned int	locationStack[STACK_SIZE], locationStackPointer;
	unsigned int	nextMaterialID;

	// Reading values

	void		readValue(char *value) const;
	void		readValue(unsigned int &value) const;
	void		readValue(float &value) const;
	void		readValue(geom::Point3 &value) const;
	void		readValue(entColor &value) const;

	// Format tags

	void		readTag(unsigned int &tag) const;
	void		skipTag(const unsigned int tag) const;
	void		ignoreSkipLength(const unsigned int tag) const;

	// Headers

	void		readSignature() const;
	bool		verifyTerminator() const;

	// Read data

	void		readGeometry(entObjectArray & objects);
	void		readObjects(entObjectArray & objects);
	void		readVertices(entObject &object);
	void		readPolygons(entObject &object);
	void		readPolygonVerts(entPoly &poly);
	void		readMaterials(entMaterialArray &materials);

public:
			EntFile(const char *filename);
virtual			~EntFile();
virtual	void		read();

	entMaterialArray	materials;
	entObjectArray		objects;

};

#endif // _H_ENT
// ---------------------------------------------------------------------------------------------------------------------------------
// Ent.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
