#pragma once

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

#include "Terrain.h"


typedef struct {
    Model* model;
    GLuint texture;
    GLuint shader;
    vec3* pos;
    int nbTrees;
} Tree;

Tree* loadTrees(char* fileTexture, const char* mapFile, Terrain* terrain, GLuint shader);
void drawTrees(Tree* tree, mat4 worldToView);
void loadTreesPositions(Tree* tree, const Terrain* terrain, char* mapFile);
