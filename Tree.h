#pragma once


#pragma once

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"


typedef struct {
    Model* model;
    GLuint texture;
    GLuint shader;
} Tree;

Tree* loadTree(float xmax, char* fileTexture);
void drawTree(Tree* forest, mat4 worldToView);
