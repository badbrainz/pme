#include "WaterDatabase.h"
#include "../Kernel/Gateway.h"
#include "../Factories/Water.h"

#include "../Nodes/TransformGroup.h"
#include "../Appearance/Appearance.h"

#include "../Geometry/Model.h"
#include "../Geometry/Geometry.h"
#include "../Geometry/Edge.h"

#include "../Renderer/Renderer.h"

WaterDatabase::WaterDatabase()
{
	rootGroup = new TransformGroup();
}

bool WaterDatabase::initialize(const char* path)
{
	if (const char* inp = MediaPathManager::lookUpMediaPath("water_2_1.col"))
	{
		ifstream in(inp, ios_base::binary);
		
		if (in.is_open())
		{
			watertexture.create2DShell("watertexture", 256, 256);
			unsigned char* colsbuf = (unsigned char*) Gateway::aquireGeometryMemory(256 * 256 * 4);
			in.read((char*)colsbuf, 256 * 256 * 4);
			in.close();
			unsigned int availableID;
			glGenTextures(1, &availableID);
			glBindTexture(GL_TEXTURE_2D, availableID);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, 4, 256, 256, 0, GL_BGRA, GL_UNSIGNED_BYTE, colsbuf);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 4, 256, 256, GL_BGRA, GL_UNSIGNED_BYTE, colsbuf);
			Gateway::releaseGeometryMemory(colsbuf);
			
			Tuple4i viewport;
			glGetIntegerv(GL_VIEWPORT, viewport);
			glViewport(0, 0, 256, 256);
			Renderer::enter2DMode();
			glEnable(GL_TEXTURE_2D);
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(0.0f, 1.0f);
			glVertex2i(0, 0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2i(0, 256);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2i(256, 0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2i(256, 256);
			glEnd();
			watertexture.copyCurrentBuffer();
			glDisable(GL_TEXTURE_2D);
			Renderer::exit2DMode();
			glViewport(0, 0, viewport.z, viewport.w);
		}
	}
	
	return true;
}

void WaterDatabase::addWaterModel(Water* water)
{
	int						 *indextable;
	unsigned short *newtriangles;
	Tuple3f				 *vertices;
	Tuple3i				 *triangles;
	
	TransformGroup *newgroup;
	Appearance		 *appearance;
	Geometry			 *newgeometry;
	Model					 *newmodel;

	String	 watername;
	Tuple4f	 watercolor;
	Tuple4ub ucwatercolor;
	unsigned int counter, vertcount, tricount, i, j;
	
	Array <Edge>		 outeredges(water->getTriangleCount()*3);
	Array <Tuple2f>  newtxcoords(water->getVertexCount());
	Array <Tuple3f>  newvertices(water->getVertexCount());
	Array <Tuple4ub> newcolors(water->getVertexCount());
	
	tricount	 = water->getTriangleCount();
	vertcount  = water->getVertexCount();
	vertices   = water->getVertices();
	triangles	 = water->getTriangles();
	watercolor = water->getColor();
	
	ucwatercolor.x = char(watercolor.x * 255);
	ucwatercolor.y = char(watercolor.y * 255);
	ucwatercolor.z = char(watercolor.z * 255);
	ucwatercolor.w = char(watercolor.w * 255);
	
	newtriangles = new unsigned short[tricount*3];
	indextable	 = new int[vertcount];
	memset(indextable, -1, vertcount * 2);
	memset(newtriangles, 0, vertcount * 2);
	
	for (i = 0; i < tricount; i++)
	{
		indextable[triangles[i].x] = 0;
		indextable[triangles[i].y] = 0;
		indextable[triangles[i].z] = 0;
	}
	
	counter = 0;
	
	for (i = 0; i < vertcount; i++)
		if (indextable[i] != -1)
		{
			indextable[i] = counter++;
			newvertices.append(vertices[i]);
			newcolors.append(ucwatercolor);
		}
		
	for (i = 0, j = 0; i < tricount; i++, j += 3)
	{
		newtriangles[j+0] = indextable[triangles[i].z];
		newtriangles[j+1] = indextable[triangles[i].y];
		newtriangles[j+2] = indextable[triangles[i].x];
		
		if (!outeredges.remove(Edge(newtriangles[j], newtriangles[j+1])))
			outeredges.append(Edge(newtriangles[j], newtriangles[j+1]));
			
		if (!outeredges.remove(Edge(newtriangles[j+1], newtriangles[j+2])))
			outeredges.append(Edge(newtriangles[j+1], newtriangles[j+2]));
			
		if (!outeredges.remove(Edge(newtriangles[j+2], newtriangles[j])))
			outeredges.append(Edge(newtriangles[j+2], newtriangles[j]));
	}
	
	for (i = 0; i < outeredges.length(); i++)
	{
		newcolors(outeredges(i).e.x).w = 0;
		newcolors(outeredges(i).e.y).w = 0;
	}
	
	for (i = 0; i < newvertices.length(); i++)
	{
		newtxcoords(i).x = newvertices(i).x * 0.25f;
		newtxcoords(i).y = newvertices(i).z * 0.25f;
	}
	
	watername = String("H2O_") + int(watersList.length());
	newgeometry = new Geometry(watername, tricount * 3, newvertices.length());
	newgeometry->setIndices(newtriangles, false);
	newgeometry->setColors(newcolors.data(), false);
	newgeometry->setVertices(newvertices.data(), false);
	newgeometry->setTextureElements(newtxcoords.data(), 2, false);
	newgeometry->computeBounds();
	
	deleteArray(indextable);
	deleteArray(newtriangles);
	
	appearance = new Appearance();
	appearance->setBlendAttributes(BlendAttributes(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	appearance->setTexture(0, watertexture);
	//appearance->setBlendAttributes(BlendAttributes(GL_DST_COLOR, GL_SRC_ALPHA));
	
	newmodel = new Model();
	newmodel->setAppearance(appearance);
	newmodel->setGeometry(newgeometry);
	
	newgroup = new TransformGroup();
	newgroup->addChild(newmodel);
	newgroup->updateBoundsDescriptor();
	
	rootGroup->addChild(newgroup);
	
	watersList.append(water);
}

Texture& WaterDatabase::getWaterTexture()
{
	return watertexture;
}

void WaterDatabase::addWaterModel(Model* model)
{
	TransformGroup* newgroup = new TransformGroup();
	newgroup->addChild(model);
	newgroup->updateBoundsDescriptor();
	rootGroup->addChild(newgroup);
}

void WaterDatabase::addWaterModel(TransformGroup* group)
{
	rootGroup->addChild(group);
}

unsigned int WaterDatabase::getWatersCount()
{
	return watersList.length();
}

TransformGroup* WaterDatabase::getRootGroup()
{
	return rootGroup;
}

void WaterDatabase::reset()
{
	rootGroup->destroy();
	watersList.clearAndDestroy();
}

WaterDatabase::~WaterDatabase()
{
	deleteObject(rootGroup);
	watersList.clearAndDestroy();
}