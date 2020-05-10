#pragma once

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"


typedef struct {
    Model* cockpit;
    Model* steeringWheel;
    Model* frame;
    GLuint texture;
    GLuint shader;
    vec3 pos;
    vec3 direction;
    vec3 front;
    vec3 up;
    vec3 left;
    vec3 speedVec;
    mat4 rotation;
    float steering;
    float speed;
    float gas;
    float halfLength;
} Car;


enum { CTRL_GAS, CTRL_BRAKE, CTRL_LEFT, CTRL_RIGHT };


#include "Terrain.h"
#include "Camera.h"


void updateCar(Car* subaru, const char* controls, Terrain* terrain);

Car* loadCar(GLuint shader, char* cockpitModel, char* frameModel, char* texture);
void drawCar(Car* car, CameraMode cameraMode);
void setCarHeight(Car* car, Terrain* terrain);
void setCarUp(Car* car, Terrain* terrain);
void updateCar(Car* subaru);

mat4 RotateTowards(vec3 src, vec3 dest);
void handleCollisions(Car* subaru, const Terrain* terrain);