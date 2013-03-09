// ---------------------------------------------------------------------------------------------------------------------------------
//   _____                      _____  ____                       
//  / ____|                    |  __ \|  _ \                      
// | |  __  ___  ___  _ __ ___ | |  | | |_) |     ___ _ __  _ __  
// | | |_ |/ _ \/ _ \| '_ ` _ \| |  | |  _ <     / __| '_ \| '_ \ 
// | |__| |  __/ (_) | | | | | | |__| | |_) | _ | (__| |_) | |_) |
//  \_____|\___|\___/|_| |_| |_|_____/|____/ (_) \___| .__/| .__/ 
//                                                   | |   | |    
//                                                   |_|   |_|    
//
// Description:
//
//   Geometry database manager
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
#include "GeomDB.h"
//#include "ProgressDlg.h"
#include "Ent.h"
#include "AseFile.h"

#include "jansson.h"
#include "sys/stat.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

	GeomDB::GeomDB()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

	GeomDB::~GeomDB()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool GeomDB::readJSON(const fstl::string & filename, const geom::Color3 & defaultReflectivity)
{

	FILE *	fp = NULL;
	char *	buf = NULL;
	
	// Check the file length

	printf("HEY!\n");

	unsigned int	fileLength;
	struct stat st;
	if(stat(filename.asArray(), &st)) fileLength = 0;
	fileLength = st.st_size;
	if (!fileLength) 
	{
		assert(0);
	}

	// Allocate RAM

	buf = new char[fileLength+1];
	if (!buf)
	{
		assert(0);
	}

	memset(buf, 0, fileLength+1);

	// Open up the file

	fp = fopen(filename.asArray(), "rb");
	if (!fp) 
	{
		assert(0);
	}

	// Read it in...

	if (fread(buf, fileLength, 1, fp) != 1)
	{
		assert(0);
	}

	// Done...

	fclose(fp);
	fp = NULL;

	json_error_t jerror;
	json_t *jworld = json_loads(buf, 0, &jerror);	

	printf("%s\n", jerror.text);

	unsigned int	polyID = 0;
	json_t* jvertices = json_object_get(jworld, "vertices");
	size_t numverts = json_array_size(jvertices);

	unsigned int	totalPolys = numverts / 9;

	printf("Reading %i %i polygons\n", numverts, totalPolys);

	// Reserve for speed

	lightmaps().reserve(totalPolys);
	polys().reserve(totalPolys);
	RadLMap	emptyLightmap(128, 128, static_cast<int>(polyID));

	double scale = 4;

	int k = 0;
	for (unsigned int j = 0; j < totalPolys; ++j, ++polyID)
	{

		RadPrim		poly;

		geom::Point3 p0;
		geom::Point3 p1;
		geom::Point3 p2;

		
		p0.x() = (float) json_real_value(json_array_get(jvertices, k++)) * scale;
		p0.z() = (float) json_real_value(json_array_get(jvertices, k++)) * scale;
		p0.y() = (float) json_real_value(json_array_get(jvertices, k++)) * scale;	

		p1.x() = (float) json_real_value(json_array_get(jvertices, k++)) * scale;
		p1.z() = (float) json_real_value(json_array_get(jvertices, k++)) * scale;
		p1.y() = (float) json_real_value(json_array_get(jvertices, k++)) * scale;	

		p2.x() = (float) json_real_value(json_array_get(jvertices, k++)) * scale;
		p2.z() = (float) json_real_value(json_array_get(jvertices, k++)) * scale;
		p2.y() = (float) json_real_value(json_array_get(jvertices, k++)) * scale;	

		//printf("%f %f %f ", p0.x(),  p0.y(), p0.z());
		//printf("%f %f %f ", p1.x(),  p1.y(), p1.z());
		//printf("%f %f %f \n", p2.x(),  p2.y(), p2.z());

		poly.xyz() += p0;
		poly.xyz() += p1;
		poly.xyz() += p2;
		poly.uv() += geom::Point2(0,0);
		poly.uv() += geom::Point2(0,0);
		poly.uv() += geom::Point2(0,0);
		poly.textureID() = polyID;
		poly.calcPlane(false);

		poly.illuminationColor() = geom::Color3(0,0,0);
		poly.reflectanceColor() = defaultReflectivity;

		float f = poly.plane().D();
		if (f != f)// NAN
			printf("WARNING: Degenerate poly!\n");					
		else
			polys() += poly;

	}

	json_t* jlights = json_object_get(jworld, "lights");
	

	unsigned int totalLights = json_array_size(jlights) / 4;

	printf("Reading %i lights\n", totalLights);

	k = 0;
	for (int j = 0; j < totalLights; j++)
	{
		float x = json_integer_value(json_array_get(jlights, k++));
		float z = json_integer_value(json_array_get(jlights, k++));
		float y = json_integer_value(json_array_get(jlights, k++));
		float light = json_integer_value(json_array_get(jlights, k++));

		x *= scale;
		y *= scale;
		z *= scale;

		RadPatch	patch1;
		patch1.area() = 0;
		patch1.origin() = geom::Point3(x, y, z);
		patch1.energy() = geom::Color3(1, 1, 1) * static_cast<float>(light * 1000);
		patch1.plane() = geom::Plane3(patch1.origin(), geom::Vector3(0, 1, 0));
		lights() += patch1;

		RadPatch	patch2;
		patch2.area() = 0;
		patch2.origin() = geom::Point3(x, y, z);
		patch2.energy() = geom::Color3(1, 1, 1) * static_cast<float>(light * 1000);
		patch2.plane() = geom::Plane3(patch2.origin(), geom::Vector3(0, -1, 0));
		lights() += patch2;

	}

	//delete [] buf;


	return true;
}

bool	GeomDB::readENT(const fstl::string & filename, const geom::Color3 & defaultReflectivity)
{
	try
	{
		//prog.setCurrentStatus("Converting geometry");

		EntFile	ef(filename.asArray());
		ef.read();

		// Convert the data over

		unsigned int	polyID = 0;
		unsigned int	totalPolys = 0;

		// Pre-count stuff
		{
			for (unsigned int i = 0; i < ef.objects.size(); ++i)
			{
				totalPolys += ef.objects[i].polys.size();
			}
		}

		// Reserve for speed

		lightmaps().reserve(totalPolys);
		polys().reserve(totalPolys);
		RadLMap	emptyLightmap(128, 128, static_cast<int>(polyID));

		for (unsigned int i = 0; i < ef.objects.size(); ++i)
		{
			entObject &	obj = ef.objects[i];

			for (unsigned int j = 0; j < obj.polys.size(); ++j, ++polyID)
			{
				if (!(polyID&0x3))
				{
					//prog.setCurrentPercent(static_cast<float>(polyID) / static_cast<float>(totalPolys) * 100.0f);
					//if (prog.stopRequested()) throw "";
				}

				entPoly &	p = obj.polys[j];
				RadPrim		poly;

				geom::Point3	p0 = obj.verts[p.verts[0]];
				geom::Point3	p1 = obj.verts[p.verts[1]];
				geom::Point3	p2 = obj.verts[p.verts[2]];

				geom::Point2	t0 = p.tex[0];
				geom::Point2	t1 = p.tex[1];
				geom::Point2	t2 = p.tex[2];

				poly.xyz() += p0;
				poly.xyz() += p1;
				poly.xyz() += p2;
				poly.uv() += geom::Point2(0,0);
				poly.uv() += geom::Point2(0,0);
				poly.uv() += geom::Point2(0,0);
				poly.textureID() = polyID;
				poly.calcPlane(false);

				if (obj.materialID != 0xffffffff)
				{
					poly.illuminationColor().r() = ef.materials[obj.materialID].illuminationColor.r * ef.materials[obj.materialID].illuminationMultiplier;
					poly.illuminationColor().g() = ef.materials[obj.materialID].illuminationColor.g * ef.materials[obj.materialID].illuminationMultiplier;
					poly.illuminationColor().b() = ef.materials[obj.materialID].illuminationColor.b * ef.materials[obj.materialID].illuminationMultiplier;

					poly.reflectanceColor().r() = ef.materials[obj.materialID].reflectanceColor.r * ef.materials[obj.materialID].reflectanceMultiplier;
					poly.reflectanceColor().g() = ef.materials[obj.materialID].reflectanceColor.g * ef.materials[obj.materialID].reflectanceMultiplier;
					poly.reflectanceColor().b() = ef.materials[obj.materialID].reflectanceColor.b * ef.materials[obj.materialID].reflectanceMultiplier;
				}
				else
				{
					poly.illuminationColor() = geom::Color3(0,0,0);
					poly.reflectanceColor() = defaultReflectivity;
				}

				polys() += poly;
			}
		}
	}
	catch (const char * err)
	{
		if (err && *err)
		{
			//AfxMessageBox(err);
			assert(0);
			return false;
		}
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	GeomDB::readASE(const fstl::string & filename, const geom::Color3 & defaultReflectivity)
{
	try
	{
		//prog.setCurrentStatus("Loading geometry");

		AseFile	ase;
		if (!ase.load(filename)) throw "Unable to load ASE file";

		//prog.setCurrentStatus("Converting geometry");

		// Convert the data over

		unsigned int	polyID = 0;
		unsigned int	totalPolys = 0;

		// Pre-count stuff
		{
			for (unsigned int i = 0; i < ase.meshes().size(); ++i)
			{
				totalPolys += ase.meshes()[i].faces.size() / 3;
			}
		}

		// Reserve for speed

		lightmaps().reserve(totalPolys);
		polys().reserve(totalPolys);
		RadLMap	emptyLightmap(128, 128, static_cast<int>(polyID));

		for (unsigned int i = 0; i < ase.meshes().size(); ++i)
		{
			sAseMesh &	obj = ase.meshes()[i];

			for (unsigned int j = 0; j < obj.faces.size(); j += 3, ++polyID)
			{
				if (!(polyID&0x3))
				{
					//prog.setCurrentPercent(static_cast<float>(polyID) / static_cast<float>(totalPolys) * 100.0f);
					//if (prog.stopRequested()) throw "";
				}

				// Vertex indices

				unsigned int	a = j+0;
				unsigned int	b = j+1;
				unsigned int	c = j+2;

				RadPrim		poly;

				// Vertices

				geom::Point3	p0 = obj.verts[obj.faces[a]];
				geom::Point3	p1 = obj.verts[obj.faces[b]];
				geom::Point3	p2 = obj.verts[obj.faces[c]];

				// Textured vertices

				geom::Point2	t0(0, 0);
				geom::Point2	t1(0, 0);
				geom::Point2	t2(0, 0);

				// If there are tverts & tfaces, use them

				if (obj.tfaces.size() == obj.faces.size() && obj.tverts.size())
				{
					unsigned int	ta = obj.tfaces[j+0];
					unsigned int	tb = obj.tfaces[j+1];
					unsigned int	tc = obj.tfaces[j+2];
					t0 = geom::Point2(obj.tverts[ta].x(), 1-obj.tverts[ta].y()) * geom::Point2(128.0f, 128.0f);
					t1 = geom::Point2(obj.tverts[tb].x(), 1-obj.tverts[tb].y()) * geom::Point2(128.0f, 128.0f);
					t2 = geom::Point2(obj.tverts[tc].x(), 1-obj.tverts[tc].y()) * geom::Point2(128.0f, 128.0f);
				}

				// Swap orientation because max uses non-left-handed coordinates

				{
					// Get the normal

					geom::Vector3	v = (p0-p1)%(p2-p1);

					// Swap y/z

					fstl::swap(p0.y(), p0.z());
					fstl::swap(p1.y(), p1.z());
					fstl::swap(p2.y(), p2.z());

					// If the normal swapped, re-order

					geom::Vector3	nv = (p0-p1)%(p2-p1);
					if (nv != v)
					{
						fstl::swap(p0, p1);
						fstl::swap(t0, t1);
					}
				}

				poly.xyz() += p0;
				poly.xyz() += p1;
				poly.xyz() += p2;
				poly.uv() += geom::Point2(0,0);
				poly.uv() += geom::Point2(0,0);
				poly.uv() += geom::Point2(0,0);
				poly.texuv() += t0;
				poly.texuv() += t1;
				poly.texuv() += t2;
				poly.textureID() = polyID;
				poly.polyID() = obj.matids[j/3];

				if (poly.polyID() > ase.materials()[0].submats.size()) poly.polyID() = ase.materials()[0].submats.size();

				poly.calcPlane(false);

				// No illumination color comes from MAX...

				poly.illuminationColor() = geom::Color3(0,0,0);

				// Is there a material? If so, use it for the reflective color

				geom::Color3	reflectivity = defaultReflectivity;

				if (ase.materials().size())
				{
					// Default to the material's color

					reflectivity = ase.materials()[0].props.diffuseColor;

					// If there are submaterials, grab the color from the appropriate one...

					if (ase.materials()[0].submats.size())
					{
						reflectivity = ase.materials()[0].submats[obj.matids[j/3]].diffuseColor;
					}
				}

				poly.reflectanceColor() = reflectivity;
				polys() += poly;
			}
		}

		// Save off the textures (all submaterials of the first material only)

		if (ase.materials().size())
		{
			//prog.setCurrentStatus("Storing textures");
			
			sAseMat &	m = ase.materials()[0];

			for (unsigned int j = 0; j < m.submats.size(); j++)
			{
				sOctTexture	ot;
				ot.id = j;
				memset(ot.name, 0, sizeof(ot.name));

				// Strip the path and extension off the file

				fstl::string	outname = m.submats[j].bitmap;
				int	idx = outname.rfind("\\");
				if (idx >= 0) outname.erase(0, idx+1);
				idx = outname.rfind(".");
				if (idx > 0) outname.erase(idx);

				strcpy(ot.name, outname.asArray());

				// Before we add the texture, look for a dup

				octTextures() += ot;
			}
		}

		// Locate and remove duplicate materials

		sOctTextureArray	noDups = octTextures();
		for (unsigned int j = 0; j < noDups.size(); ++j)
		{
			for (unsigned int k = j+1; k < noDups.size(); ++k)
			{
				if (!strcmp(noDups[j].name, noDups[k].name))
				{
					noDups.erase(k,1);
					k--;
				}
			}
		}

		// Fixup the nodups IDs

		for (unsigned int j = 0; j < noDups.size(); ++j)
		{
			noDups[j].id = j;
		}

		// Now re-map all the polygons to the cleaned-up textures

		for (RadPrimList::node * j = polys().head(); j; j = j->next())
		{
			RadPrim &	rp = j->data();

			// Find the original texture

			for (unsigned int k = 0; k < octTextures().size(); ++k)
			{
				sOctTexture &	originalTexture = octTextures()[k];

				if (originalTexture.id == rp.polyID())
				{
					// Now find the new texture in the dup-removed list

					for (unsigned int l = 0; l < noDups.size(); ++l)
					{
						sOctTexture &	newTexture = noDups[l];

						if (!strcmp(newTexture.name, originalTexture.name))
						{
							rp.polyID() = newTexture.id;
							break;
						}
					}
					break;
				}
			}
		}

		// Swap lists

		octTextures() = noDups;

		// Convert the lights to patches

		{
			//prog.setCurrentStatus("Converting lights");

			// We reserve for speed

			lights().reserve(ase.lights().size());

			for (unsigned int i = 0; i < ase.lights().size(); i++)
			{
				//prog.setCurrentPercent(static_cast<float>(i) / static_cast<float>(ase.lights().size()) * 100.0f);
				//if (prog.stopRequested()) throw false;
				sAseLight &	curLight = ase.lights()[i];

				// Swap orientation because max uses non-left-handed coordinates

				geom::Point3	pos = curLight.pos;
				fstl::swap(pos.y(), pos.z());

				RadPatch	patch1;
				patch1.area() = 0;
				patch1.origin() = geom::Point3(pos.x(), pos.y(), pos.z());
				patch1.energy() = curLight.color * static_cast<float>(curLight.intensity);
				fstl::swap(patch1.energy().r(), patch1.energy().b());
				patch1.plane() = geom::Plane3(patch1.origin(), geom::Vector3(0, 1, 0));
				lights() += patch1;

				RadPatch	patch2;
				patch2.area() = 0;
				patch2.origin() = geom::Point3(pos.x(), pos.y(), pos.z());
				patch2.energy() = curLight.color * static_cast<float>(curLight.intensity);
				fstl::swap(patch1.energy().r(), patch2.energy().b());
				patch2.plane() = geom::Plane3(patch2.origin(), geom::Vector3(0, -1, 0));
				lights() += patch2;
			}
		}
	}
	catch (const char * err)
	{
		if (err && *err)
		{
			//AfxMessageBox(err);
			assert(0);
			return false;
		}
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	GeomDB::readOCT(const fstl::string & filename, const geom::Color3 & defaultReflectivity)
{
	// Open the file

	FILE *	fp = fopen(filename.asArray(), "rb");
	if (!fp) return false;

	// Types used by the importer

	#pragma pack(1)
	typedef float		octVect2[2];
	typedef float		octVect3[3];
	typedef float		octPlane[4];			// 4th float is distance

	struct octHeader
	{
		int		numVerts;
		int		numFaces;
		int		numTextures;
		int		numLightmaps;
		int		numLights;
	};

	struct octVert
	{
		octVect2	tv;				// texture coordinates
		octVect2	lv;				// lightmap coordinates
		octVect3	pos;				// vertex position
	};

	struct octFace
	{
		int		start;				// first face vert in vertex array
		int		num;				// number of verts in the face
		int		id;				// texture index into the texture array
		int		lid;				// lightmap index into the lightmap array
		octPlane	p;
	};

	struct octTexture
	{
		unsigned int	id;				// texture id
		char		name[64];			// texture name
	};

	struct octLightmap
	{
		unsigned int	id;				// lightmaps id
		unsigned char	map[49152];			// 128 x 128 raw RGB data
	};

	struct octLight
	{
		octVect3	pos;				// Position
		octVect3	color;				// Color (RGB)
		int		intensity;			// Intensity
	};
	#pragma pack()

	// Get the header
	
	octHeader	header;
	if (fread(&header, sizeof(header), 1, fp) != 1) return false;

	// Allocate some RAM

	octVert	*	verts = new octVert[header.numVerts];
	octFace	*	faces = new octFace[header.numFaces];
	octTexture *	texts = new octTexture[header.numTextures];
	octLightmap *	lmaps = new octLightmap[header.numLightmaps];
	octLight *	ligts = new octLight[header.numLights];
	octVect3	playerPos;

	// Load the data

	try
	{
		// Make sure we got the memory we asked for...

		if (!verts || !faces || !texts || !lmaps || !ligts) throw false;

		// Read the data

		//prog.setCurrentStatus("Loading geometry");

		if (header.numVerts     && fread(verts,      sizeof(octVert)     * header.numVerts,     1, fp) != 1) throw false;
		if (header.numFaces     && fread(faces,      sizeof(octFace)     * header.numFaces,     1, fp) != 1) throw false;
		if (header.numTextures  && fread(texts,      sizeof(octTexture)  * header.numTextures,  1, fp) != 1) throw false;
		if (header.numLightmaps && fread(lmaps,      sizeof(octLightmap) * header.numLightmaps, 1, fp) != 1) throw false;
		if (header.numLights    && fread(ligts,      sizeof(octLight)    * header.numLights,    1, fp) != 1) throw false;
		if (                       fread(&playerPos, sizeof(playerPos),                         1, fp) != 1) throw false;

		// Convert the polys
		{
			//prog.setCurrentStatus("Converting geometry");

			polys().reserve(header.numFaces);

			for (int i = 0; i < header.numFaces; i++)
			{
				if (!(i&0xf))
				{
					//prog.setCurrentPercent(static_cast<float>(i) / static_cast<float>(header.numFaces) * 100.0f);
					//if (prog.stopRequested()) throw false;
				}

				octFace &	curFace = faces[i];
				RadPrim		poly;

				for (int j = curFace.start; j < curFace.start + curFace.num; j++)
				{
					poly.xyz() += geom::Point3(verts[j].pos[0], verts[j].pos[1], verts[j].pos[2]);
					poly.uv() += geom::Point2(verts[j].lv[0], verts[j].lv[1]) * 128.0;
					poly.texuv() += geom::Point2(verts[j].tv[0], verts[j].tv[1]) * 128.0;
				}

				poly.textureID() = curFace.lid;
				poly.polyID() = curFace.id;
				poly.calcPlane(false);

				// These oct files can mysteriously get garbage polygons... so to fix them we'll un-fan any
				// polygons that are not valid...
				//
				// What denotes an invalid polygon? First, if the polygon's plane's normal is not unit length.
				// Second, if it's not planar, and third, if it's not convex.

				float	normalLength = poly.plane().normal().length();
				bool	regenerate = normalLength < 0.9f || normalLength > 1.1f;

				if (!regenerate && poly.plane().vector() == geom::Vector3::zero()) regenerate = true;
				if (!regenerate && !poly.isPlanar()) regenerate = true;
				if (!regenerate && !poly.isConvex()) regenerate = true;

				RadPrimArray	convexPieces;
				if (regenerate)
				{
					// Fan the polygon out

					for (unsigned int j = 1; j < poly.xyz().size() - 1; ++j)
					{
						RadPrim	p = poly;
						p.xyz().erase();
						p.uv().erase();
						p.texuv().erase();

						p.xyz() += poly.xyz()[0];
						p.xyz() += poly.xyz()[j];
						p.xyz() += poly.xyz()[j+1];

						p.uv() += poly.uv()[0];
						p.uv() += poly.uv()[j];
						p.uv() += poly.uv()[j+1];

						p.texuv() += poly.texuv()[0];
						p.texuv() += poly.texuv()[j];
						p.texuv() += poly.texuv()[j+1];

						p.calcPlane(false);

						// Make sure the triangle is valid

						if (p.plane().normal().length() > 0.9f && p.plane().normal().length() < 1.1f)
						{
							convexPieces += p;
						}
					}
				}
				else
				{
					convexPieces += poly;
				}

				// Now add in all the convex pieces

				for (unsigned int k = 0; k < convexPieces.size(); ++k)
				{
					RadPrim &	p = convexPieces[k];

					// Throw away polygons with invalid planes

					if (p.plane().vector() == geom::Vector3(0,0,0)) continue;

					p.reflectanceColor() = defaultReflectivity;

					polys() += p;
				}
			}
		}

		// Save off the textures
		{
			//prog.setCurrentStatus("Storing textures");
			
			for (int i = 0; i < header.numTextures; i++)
			{
				sOctTexture	ot;
				ot.id = texts[i].id;
				memcpy(ot.name, texts[i].name, sizeof(ot.name));
				octTextures() += ot;
			}
		}

		// Convert the lights to patches
		{
			//prog.setCurrentStatus("Converting lights");

			// We reserve for speed

			lights().reserve(header.numLights);

			for (int i = 0; i < header.numLights; i++)
			{
				//prog.setCurrentPercent(static_cast<float>(i) / static_cast<float>(header.numLights) * 100.0f);
				//if (prog.stopRequested()) throw false;
				octLight &	curLight = ligts[i];

				fstl::swap(curLight.pos[1], curLight.pos[2]);
				fstl::swap(curLight.color[0], curLight.color[2]);

				RadPatch	patch1;
				patch1.area() = 0;
				patch1.origin() = geom::Point3(curLight.pos[0], curLight.pos[1], curLight.pos[2]);
				patch1.energy() = geom::Color3(curLight.color[0], curLight.color[1], curLight.color[2]) * static_cast<float>(curLight.intensity);
				patch1.plane() = geom::Plane3(patch1.origin(), geom::Vector3(0, 1, 0));
				lights() += patch1;

				RadPatch	patch2;
				patch2.area() = 0;
				patch2.origin() = geom::Point3(curLight.pos[0], curLight.pos[1], curLight.pos[2]);
				patch2.energy() = geom::Color3(curLight.color[0], curLight.color[1], curLight.color[2]) * static_cast<float>(curLight.intensity);
				patch2.plane() = geom::Plane3(patch2.origin(), geom::Vector3(0, -1, 0));
				lights() += patch2;
			}
		}
	}
	catch(const bool err)
	{
		delete[] verts;
		delete[] faces;
		delete[] texts;
		delete[] lmaps;
		delete[] ligts;
		fclose(fp);
		if (!err) return err;
	}

	delete[] verts;
	delete[] faces;
	delete[] texts;
	delete[] lmaps;
	delete[] ligts;
	fclose(fp);
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	GeomDB::writeOCT(const fstl::string & filename)
{
	// Open the file

	FILE *	fp = fopen(filename.asArray(), "wb");
	if (!fp) return false;

	// Types used by the exporter

	#pragma pack(1)
	typedef float		octVect2[2];
	typedef float		octVect3[3];
	typedef float		octPlane[4];			// 4th float is distance

	struct octHeader
	{
		int		numVerts;
		int		numFaces;
		int		numTextures;
		int		numLightmaps;
		int		numLights;
	};

	struct octVert
	{
		octVect2	tv;				// texture coordinates
		octVect2	lv;				// lightmap coordinates
		octVect3	pos;				// vertex position
	};

	struct octFace
	{
		int		start;				// first face vert in vertex array
		int		num;				// number of verts in the face
		int		id;				// texture index into the texture array
		int		lid;				// lightmap index into the lightmap array
		octPlane	p;
	};

	struct octTexture
	{
		unsigned int	id;				// texture id
		char		name[64];			// texture name
	};

	struct octLightmap
	{
		unsigned int	id;				// lightmaps id
		unsigned char	map[49152];			// 128 x 128 raw RGB data
	};

	struct octLight
	{
		octVect3	pos;				// Position
		octVect3	color;				// Color (RGB)
		int		intensity;			// Intensity
	};

	#pragma pack()

	// Create the header
	
	octHeader	header;

	// Setup the header info

	header.numFaces = polys().size();
	header.numTextures = octTextures().size();
	header.numLightmaps = lightmaps().size();
	header.numLights = lights().size();

	// Vertex count for the header

	{
		header.numVerts = 0;
		for (RadPrimList::node *i = polys().head(); i; i = i->next())
		{
			header.numVerts += i->data().xyz().size();
		}
	}

	// Write the header

	if (fwrite(&header, sizeof(header), 1, fp) != 1) return false;

	// Allocate some RAM

	octVert	*	verts = new octVert[header.numVerts];
	octFace	*	faces = new octFace[header.numFaces];
	octTexture *	texts = new octTexture[header.numTextures];
	octLightmap *	lmaps = new octLightmap[header.numLightmaps];
	octLight *	lites = new octLight[header.numLights];
	octVect3	playerPos;
	memset(faces, 0, sizeof(octFace) * header.numFaces);
	memset(playerPos, 0, sizeof(playerPos));

	// Save the data

	try
	{
		// Make sure we got the memory we asked for...

		if (!verts || !faces || !texts || !lmaps || !lites) throw false;

		// Convert the polys & vertices
		{
			//prog.setCurrentStatus("Converting geometry");

			unsigned int	pIndex = 0;
			unsigned int	vIndex = 0;

			for (RadPrimList::node *i = polys().head(); i; i = i->next(), ++pIndex)
			{
				if (!(pIndex&0xf))
				{
					//prog.setCurrentPercent(static_cast<float>(pIndex) / static_cast<float>(polys().size()) * 100.0f);
					//if (prog.stopRequested()) throw false;
				}

				RadPrim &	p = i->data();
				octFace &	curFace = faces[pIndex];

				// Populate the face

				curFace.start = vIndex;
				curFace.num = p.xyz().size();
				curFace.id = p.polyID();
				curFace.lid = p.textureID();

				// Populate the verts

				for (unsigned int j = 0; j < p.xyz().size(); ++j, ++vIndex)
				{
					octVert &	curVert = verts[vIndex];
					curVert.tv[0] = p.texuv()[j].u() / 128.0f;
					curVert.tv[1] = p.texuv()[j].v() / 128.0f;
					curVert.lv[0] = p.uv()[j].u() / 128.0f;
					curVert.lv[1] = p.uv()[j].v() / 128.0f;
					curVert.pos[0] = p.xyz()[j].x();
					curVert.pos[1] = p.xyz()[j].y();
					curVert.pos[2] = p.xyz()[j].z();
				}
			}
		}

		// Convert the textures
		{
			//prog.setCurrentStatus("Converting textures");

			for (int i = 0; i < header.numTextures; ++i)
			{
				if (!(i&0xf))
				{
					//prog.setCurrentPercent(static_cast<float>(i) / static_cast<float>(header.numTextures) * 100.0f);
					//if (prog.stopRequested()) throw false;
				}

				sOctTexture &	ot = octTextures()[i];
				texts[i].id = ot.id;
				memcpy(texts[i].name, ot.name, sizeof(ot.name));
			}
		}

		// Convert the lights
		{
			//prog.setCurrentStatus("Converting lights");

			unsigned int	idx = 0;
			for (RadPatchList::node * i = lights().head(); i; i = i->next(), ++idx)
			{
				if (!(idx&0xf))
				{
					//prog.setCurrentPercent(static_cast<float>(idx) / static_cast<float>(header.numLights) * 100.0f);
					//if (prog.stopRequested()) throw false;
				}

				RadPatch &	patch = i->data();
				lites[idx].pos[0] = patch.origin().x();
				lites[idx].pos[1] = patch.origin().y();
				lites[idx].pos[2] = patch.origin().z();
				lites[idx].color[0] = patch.energy().r(); // these are stored incorrectly... bummer.
				lites[idx].color[1] = patch.energy().g();
				lites[idx].color[2] = patch.energy().b();
				lites[idx].intensity = 1;
			}
		}

		// Convert the lightmaps
		{
			//prog.setCurrentStatus("Converting lightmaps");

			for (int i = 0; i < header.numLightmaps; ++i)
			{
				if (!(i&0xf))
				{
					//prog.setCurrentPercent(static_cast<float>(i) / static_cast<float>(header.numLightmaps) * 100.0f);
					//if (prog.stopRequested()) throw false;
				}

				RadLMap & lm = lightmaps()[i];
				lmaps[i].id = i;

				// Convert the lightmap data
				{
					geom::Color3 *	src = &lm.data()[0];
					unsigned char *	dst = lmaps[i].map;
					unsigned int	size = 128*128;
					for (unsigned int i = 0; i < size; ++i, ++src, dst += 3)
					{
						dst[0] = static_cast<unsigned char>(src->r());
						dst[1] = static_cast<unsigned char>(src->g());
						dst[2] = static_cast<unsigned char>(src->b());
					}
				}
			}
		}

		// Write the data

		//prog.setCurrentStatus("Saving geometry");

		if (header.numVerts     && fwrite(verts,      sizeof(octVert)     * header.numVerts,     1, fp) != 1) throw false;
		if (header.numFaces     && fwrite(faces,      sizeof(octFace)     * header.numFaces,     1, fp) != 1) throw false;
		if (header.numTextures  && fwrite(texts,      sizeof(octTexture)  * header.numTextures,  1, fp) != 1) throw false;
		if (header.numLightmaps && fwrite(lmaps,      sizeof(octLightmap) * header.numLightmaps, 1, fp) != 1) throw false;
		if (header.numLights    && fwrite(lites,      sizeof(octLight)    * header.numLights,    1, fp) != 1) throw false;
		if (                       fwrite(&playerPos, sizeof(playerPos),                         1, fp) != 1) throw false;

		// Oki doki!

		throw true;
	}
	catch(const bool err)
	{
		delete[] verts;
		delete[] faces;
		delete[] texts;
		delete[] lmaps;
		delete[] lites;
		fclose(fp);
		if (!err) return err;
	}

	fclose(fp);
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// GeomDB.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
