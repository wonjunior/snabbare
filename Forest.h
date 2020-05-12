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
} Forest;

GLfloat* generateStakes(int nbStakes, float xmax);
Forest* loadForest(const Terrain* terrain, char* fileTexture, GLuint shader, int nbStakes);
void drawForest(Forest* forest, mat4 worldToView);

