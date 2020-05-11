#pragma once

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

#include "Camera.h"

typedef struct {
    Model* model;
} Minimap;

typedef struct {
    Minimap minimap;
    GLuint shader;
    GLuint texture;
} HUD;

HUD* loadHUD(GLuint shader);
void drawHUD(HUD* hud, Camera cam, mat4 worldToView);

mat4 RotateTowards(vec3 src, vec3 dest);