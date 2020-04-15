#pragma once

#include "MicroGlut.h"
// uses framework Cocoa
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

#define SKYBOX_SIZE 100

typedef struct {
    Model* model;
    GLuint texture;
    GLuint shader;
} Skybox;


Skybox* CreateSkybox(const char* modelFileName, char* textureFileName, GLuint shader);
void DrawSkybox(Skybox* skybox, mat4 worldToView);