// ---------------------------------------------------------------------------------------------------------------------------------
//  ______       _                        
// |  ____|     | |                       
// | |__   _ __ | |_      ___ _ __  _ __  
// |  __| | '_ \| __|    / __| '_ \| '_ \ 
// | |____| | | | |_  _ | (__| |_) | |_) |
// |______|_| |_|\__|(_) \___| .__/| .__/ 
//                           | |   | |    
//                           |_|   |_|    
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
//   12/14/2001 by Paul Nettle: Original creation
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
#include "RadPrim.h"
#include "GeomDB.h"
#include "Ent.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

	EntFile::EntFile(const char *filename)
{
	memset(fName, 0, sizeof(fName));
	strncpy(fName, filename, MAX_FILENAME_LEN);

	memset(locationStack, 0, sizeof(locationStack));
	locationStackPointer = 0;

	// Open the output file

	pFile = fopen(fName, "rb");
	if (!pFile) throw "Unable to open file for read";
}

// ---------------------------------------------------------------------------------------------------------------------------------

	EntFile::~EntFile()
{
	if (pFile) fclose(pFile);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// VALUES
// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::readValue(char *value) const
{
	unsigned int	length = 0;
	readValue(length);
	if (fread(value, length, 1, pFile) != 1) throw "Unable to read string characters";
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::readValue(unsigned int &value) const
{
	if (fread(&value, sizeof(value), 1, pFile) != 1) throw "Unable to read integer";
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::readValue(float &value) const
{
	if (fread(&value, sizeof(value), 1, pFile) != 1) throw "Unable to read 32-bit float";
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::readValue(geom::Point3 &value) const
{
	double	x, y, z;
	if (fread(&x, sizeof(x), 1, pFile) != 1) throw "Unable to read xyz.x";
	if (fread(&y, sizeof(y), 1, pFile) != 1) throw "Unable to read xyz.y";
	if (fread(&z, sizeof(z), 1, pFile) != 1) throw "Unable to read xyz.z";
	value.x() = static_cast<float>(x);
	value.y() = static_cast<float>(y);
	value.z() = static_cast<float>(z);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::readValue(entColor &value) const
{
	if (fread(&value, sizeof(value), 1, pFile) != 1) throw "Unable to read color";
}

// ---------------------------------------------------------------------------------------------------------------------------------
// TAGS
// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::readTag(unsigned int &tag) const
{
	readValue(tag);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::skipTag(const unsigned int tag) const
{
	// First, determine if we have a specific length to skip

	if ((tag & 0xff) == 0)
	{
		unsigned int	length = 0;
		readValue(length);
		fseek(pFile, length, SEEK_CUR);
		return;
	}

	// No length, use the tagID to determine how much to skip

	switch(tag & 0xff)
	{
		case ENT_TYPE_STRING:
		{
			char	str[256];
			readValue(str);
			break;
		}

		case ENT_TYPE_BOOLEAN:
			fseek(pFile, sizeof(char), SEEK_CUR);
			break;

		case ENT_TYPE_INTEGER:
			fseek(pFile, sizeof(int), SEEK_CUR);
			break;

		case ENT_TYPE_FLOAT32:
			fseek(pFile, sizeof(float), SEEK_CUR);
			break;

		case ENT_TYPE_FLOAT64:
			fseek(pFile, sizeof(double), SEEK_CUR);
			break;

		case ENT_TYPE_XY:
			fseek(pFile, sizeof(double) * 2, SEEK_CUR);
			break;

		case ENT_TYPE_XYZ:
			fseek(pFile, sizeof(double) * 3, SEEK_CUR);
			break;

		case ENT_TYPE_MATRIX:
			fseek(pFile, sizeof(double) * 9, SEEK_CUR);
			break;

		case ENT_TYPE_RGB:
			fseek(pFile, sizeof(double) * 3, SEEK_CUR);
			break;

		default:
			throw "Unknown tag data type -- cannot skip";
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::ignoreSkipLength(const unsigned int tag) const
{
	// If there is a length, then read it and throw it away

	if ((tag & 0xff) == 0)
	{
		unsigned int	length = 0;
		readValue(length);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------
// HEADERS
// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::readSignature() const
{
	char	temp[ENT_SIGNATURE_LEN];
	if (fread(temp, ENT_SIGNATURE_LEN, 1, pFile) != 1) throw "Unable to read signature";
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	EntFile::verifyTerminator() const
{
	unsigned int	tag;
	readTag(tag);
	return tag == ETAG_TERMINATOR;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// READ
// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::readGeometry(entObjectArray & objects)
{
	bool	done = false;

	while(!done)
	{
		unsigned int	tag;
		readTag(tag);

		switch(tag)
		{
			case ETAG_GEOMETRY_OBJECT_COUNT:
				ignoreSkipLength(tag);
				readObjects(objects);
				break;

			case ETAG_TERMINATOR:
				done = true;
				break;

			default:
				skipTag(tag);
				break;
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::readObjects(entObjectArray & objects)
{
	unsigned int	count = 0;
	readValue(count);

	for (unsigned int i = 0; i < count; i++)
	{
		bool	done = false;

		entObject	obj;

		while(!done)
		{
			unsigned int	tag;
			readTag(tag);

			switch(tag)
			{
				case ETAG_GEOMETRY_OBJECT_VERTEX_COUNT:
					ignoreSkipLength(tag);
					readVertices(obj);
					break;

				case ETAG_GEOMETRY_OBJECT_POLY_COUNT:
					ignoreSkipLength(tag);
					readPolygons(obj);
					break;

				case ETAG_TERMINATOR:
					done = true;
					break;

				default:
					skipTag(tag);
					break;
			}
		}

		objects += obj;
	}

	if (!verifyTerminator()) throw "Invalid termination";
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::readVertices(entObject &object)
{
	unsigned int	count = 0;
	readValue(count);

	for (unsigned int i = 0; i < count; i++)
	{
		bool	done = false;

		while(!done)
		{
			unsigned int	tag;
			readTag(tag);

			switch(tag)
			{
				case ETAG_GEOMETRY_OBJECT_VERTEX_LOCATION:
				{
					ignoreSkipLength(tag);
					geom::Point3	xyz;
					readValue(xyz);
					object.verts += xyz;
					break;
				}

				case ETAG_TERMINATOR:
					done = true;
					break;

				default:
					skipTag(tag);
					break;
			}
		}
	}

	if (!verifyTerminator()) throw "Invalid termination";
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::readPolygons(entObject &object)
{
	unsigned int	count = 0;
	readValue(count);

	unsigned int	temp = 0;

	for (unsigned int i = 0; i < count; i++)
	{
		bool	done = false;

		entPoly	poly;

		while(!done)
		{
			unsigned int	tag;
			readTag(tag);

			switch(tag)
			{
				case ETAG_GEOMETRY_OBJECT_POLY_MATERIAL:
					ignoreSkipLength(tag);
					readValue(temp);

					// Set the material to the proper indexing

					object.materialID = temp + nextMaterialID;
					break;

				case ETAG_GEOMETRY_OBJECT_POLY_VERTEX_COUNT:
					ignoreSkipLength(tag);
					readPolygonVerts(poly);
					break;

				case ETAG_TERMINATOR:
					done = true;
					break;

				default:
					skipTag(tag);
					break;
			}
		}

		// Add the poly

		object.polys += poly;
	}

	if (!verifyTerminator()) throw "Invalid termination";
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::readPolygonVerts(entPoly &poly)
{
	unsigned int	count = 0;
	readValue(count);

	if (count != 3) throw "Polygon not 3-sided";

	for (unsigned int i = 0; i < count; i++)
	{
		bool	done = false;

		while(!done)
		{
			unsigned int	tag;
			readTag(tag);

			switch(tag)
			{
				case ETAG_GEOMETRY_OBJECT_POLY_VERTEX_INDEX:
					ignoreSkipLength(tag);
					readValue(poly.verts[i]);
					break;

				case ETAG_TERMINATOR:
					done = true;
					break;

				default:
					skipTag(tag);
					break;
			}
		}
	}

	if (!verifyTerminator()) throw "Invalid termination";
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::readMaterials(entMaterialArray & materials)
{
	unsigned int	tag;
	readTag(tag);
	ignoreSkipLength(tag);
	if (tag != ETAG_MATERIAL_COUNT) throw "Unable to read materials";

	unsigned int	count = 0;
	readValue(count);

	for (unsigned int i = 0; i < count; i++)
	{
		// Default material

		entMaterial	mat;
		mat.illuminationColor.r = 0;
		mat.illuminationColor.g = 0;
		mat.illuminationColor.b = 0;
		mat.reflectanceColor.r = 0;
		mat.reflectanceColor.g = 0;
		mat.reflectanceColor.b = 0;
		mat.illuminationMultiplier = 1;
		mat.reflectanceMultiplier = 1;

		bool	done = false;

		while(!done)
		{
			unsigned int	tag;
			readTag(tag);

			switch(tag)
			{
				case ETAG_MATERIAL_ILLUMINATION_COLOR:
					ignoreSkipLength(tag);
					readValue(mat.illuminationColor);
					break;

				case ETAG_MATERIAL_ILLUMINATION_MULTIPLIER:
					ignoreSkipLength(tag);
					readValue(mat.illuminationMultiplier);
					break;

				case ETAG_MATERIAL_REFLECTANCE_COLOR:
					ignoreSkipLength(tag);
					readValue(mat.reflectanceColor);
					break;

				case ETAG_MATERIAL_REFLECTANCE_MULTIPLIER:
					ignoreSkipLength(tag);
					readValue(mat.reflectanceMultiplier);
					break;

				case ETAG_TERMINATOR:
					done = true;
					break;

				default:
					skipTag(tag);
					break;
			}
		}

		// Add it

		materials += mat;
	}

	if (!verifyTerminator()) throw "Invalid termination";
}

// ---------------------------------------------------------------------------------------------------------------------------------
// FILE I/O
// ---------------------------------------------------------------------------------------------------------------------------------

void	EntFile::read()
{
	if (!pFile) throw "file pointer null";

	readSignature();

	// Get the material index

	nextMaterialID = 0;

	// Verify the file type

	unsigned int	tag;
	readTag(tag);
	if (tag != ETAG_IDENTIFIER) throw "Invalid format";
	
	// Skip the file length & version

	unsigned int	unused;
	readValue(unused);
	readValue(unused);

	// Parse the file tags

	bool	done = false;

	while(!done)
	{
		readTag(tag);

		switch(tag)
		{
			case ETAG_FILEINFO:
				skipTag(tag);
				break;

			case ETAG_PARAMETERS:
				skipTag(tag);
				break;

			case ETAG_GEOMETRY:
				ignoreSkipLength(tag);
				readGeometry(objects);
				break;

			case ETAG_MATERIAL:
				ignoreSkipLength(tag);
				readMaterials(materials);
				break;

			case ETAG_TERMINATOR:
				done = true;
				break;

			default:
				skipTag(tag);
				break;
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Ent.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

