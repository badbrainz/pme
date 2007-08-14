#include "../Nodes/TransformGroup.h"
#include "../Math/MathUtils.h"

#ifndef FACTORY3DS_H
#define FACTORY3DS_H

#pragma pack(push)
#pragma pack(1)

typedef struct
{
	unsigned short ID;
	unsigned long Length;
} Chunk3DS_t;

typedef struct
{
	char  Name[64];
	Tuple4f Ambient,
	        Diffuse,
          Specular,
          Emission;
	float Shininess;
	char  Texture[32];
} Material3DS_t;

typedef struct
{
	char Name[64];
	int  Material,
	     Start,
	     Size;
	unsigned short *Face;

} Group3DS;

typedef struct
{
  Tuple3f *binormalVecs,
		      *tangentVecs,
	        *normalVecs,
          *vertices;

  Tuple2f *texCoords;

  char     Name[64];

  int      NumUV,
           NumFace,
           NumGroup,
           NumVertex,
          *smoothGroup;

  unsigned short *Face;
  Group3DS *group;
} Mesh3D;

typedef struct
{
	int NumMaterial;
	Material3DS_t *Material;

	int NumMesh;
	Mesh3D *Mesh;
} Model3DS_t;

#pragma pack(pop)

bool load3DS(TransformGroup *group, bool tbn, char *strFileName);

#endif
