#pragma once

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

typedef enum {
    CAM_BEHIND,
    CAM_COCKPIT,
    CAM_GOD,
} CameraMode;

typedef struct {
    float distance;
    float height;
    float tilt;
} CameraTransform;

typedef struct {
    CameraMode mode;
    vec3 forward;
    vec3 up;
    vec3 pos;
    vec3 lookat;
    CameraTransform behindTransform;
    CameraTransform cockpitTransform;
} Camera;

#include "Car.h"

Camera createCamera();
void rotateGodCamera(Camera* cam, int x, int y);
void moveGodCamera(Camera* camera, char key);
void updateCamera(Camera* cam, Car* car);
