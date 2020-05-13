#pragma once

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

#include "Camera.h"

typedef struct {
    Model* mapBillboard;
    Model* carBillboard;
    Model* checkpointBillboard;
    GLuint mapTexture;
    GLuint carTexture;
    GLuint checkpointTexture;
    float size;
    float offset;
    float terrainSize;
} Minimap;

typedef struct {
    Minimap minimap;
    GLuint shader;
    float screenResolution;
} HUD;

HUD* loadHUD(GLuint shader, float terrainWidth);
void drawHUD(HUD* hud, mat4 worldToView, Camera cam, vec3 carPos);

mat4 RotateTowards(vec3 src, vec3 dest);