#pragma once

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

#define NEAR 1.0
#define FAR 5000.0
#define RIGHT 0.5
#define LEFT -0.5
#define TOP 0.5
#define BOTTOM -0.5

typedef struct {
    Model* model;
    GLuint texture;
    GLuint shader;
} Forest;

Forest* loadForest(float xmax, char* fileTexture);
void drawForest(Forest* forest, mat4 worldToView);

