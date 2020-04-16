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
    vec3 pos;
    vec3 speed;
    vec3 front;
    vec3 up;
} Car;

Car* loadCar(GLuint shader, char* model, char* texture);
void freeCar(Car* car);
void drawCar(Car* car);
void setCarHeight(Car* car, Terrain* terrain);