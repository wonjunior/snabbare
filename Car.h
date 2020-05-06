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
    vec3 direction;
    vec3 front;
    vec3 up;
    mat4 rotation;
    float speed;
    float gas;
} Car;

enum { CTRL_GAS, CTRL_BRAKE, CTRL_LEFT, CTRL_RIGHT };

void updateCar(Car* subaru, const char* controls);

Car* loadCar(GLuint shader, char* model, char* texture);
void freeCar(Car* car);
void drawCar(Car* car);
void setCarHeight(Car* car, Terrain* terrain);
void setCarUp(Car* car, Terrain* terrain);
void updateCar(Car* subaru);

mat4 RotateTowards(vec3 src, vec3 dest);