#pragma once

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"


#include "Car.h"



typedef struct {
    Model* starter;
    Car* ghost;
    float checkPointsRadius;
    GLfloat* checkPoints;
    int nbCheckPoints;
    int nextCheckPoint;
    int lap;
} RaceController;

void updateController(RaceController* controller, const Car* car);
RaceController createController();
float square(float a);
