#pragma once

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

#include <math.h>
#include "Car.h"

typedef struct {
    unsigned int time;
    GLfloat pos[2];
} Step;


typedef struct {
    int lap;
    float checkPointsRadius;
    GLfloat* checkPoints;
    int nbCheckPoints;
    int nextCheckPoint;
    Step* carPath;
    Step* ghostPath;
    int lastGhostStep;
    int nbGhostSteps;
    int lastCarStep;
    float distanceBetweenSteps;
    unsigned int carLoopCounter;
    unsigned int ghostLoopCounter;
} RaceController;

void updateController(RaceController* controller, Car* car, Car* ghost);
RaceController createController(const Car*);
float square(float a);
float angleBetween(float x1, float z1, float x2, float z2);
