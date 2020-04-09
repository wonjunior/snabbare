#pragma once


#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"


typedef struct {
    Model* model;
    unsigned int arrayNormalArrow;
    unsigned int bufferNormalArrow;
    TextureData terrainTextureMap;
    GLuint shader;
} Terrain;

Terrain* GenerateTerrain(GLuint shader, char* textureFileName);

void DrawNormals(Terrain* terrain);
void DrawTerrain(Terrain* terrain, mat4 modelToWorld, mat4 worldToView);