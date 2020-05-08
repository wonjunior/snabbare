#pragma once

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

#include "Terrain.h"
#include "Camera.h"


typedef struct {
    Model* model;
    GLuint* textures;
    GLuint shader;
    vec3* pos;
    int nbTrees;
    int nbTextures;
} Tree;

Tree* loadTrees(const char** textureFiles, int nbTextures, const char* mapFile, Terrain* terrain, GLuint shader);
void drawTrees(Tree* tree, mat4 worldToView, const Camera cam);
void loadTreesPositions(Tree* tree, const Terrain* terrain, char* mapFile);
