#pragma once

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

typedef struct {
    Model* checkpoint;
    Model* rabbit;
    GLuint checkpointTexture;
    GLuint rabbitTexture;
    GLuint shader;
} Props;

Props* LoadProps(GLuint shader);
void drawProps(Props* props);