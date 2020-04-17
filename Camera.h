#pragma once

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

#include "Car.h"

typedef enum {
    CAM_BEHIND,
    CAM_FRONT,
    CAM_GOD,
} CameraMode;

typedef struct {
    CameraMode mode;
    vec3 forward;
    vec3 up;
    vec3 pos;
    vec3 lookat;
    float distance;
    float height;
    float tilt;
} Camera;

enum { CTRL_GAS, CTRL_BRAKE, CTRL_LEFT, CTRL_RIGHT };


Camera createCamera();
void rotateGodCamera(Camera* cam, int x, int y);
void moveGodCamera(Camera* camera, char key);
void updateCamera(Camera* cam, Car* car);