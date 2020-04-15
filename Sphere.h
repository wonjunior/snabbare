#pragma once

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

typedef struct {
    vec3 center;
    double radius;
} Sphere;

Sphere* CollisionSphere(Model* model);

bool areColliding(Sphere* a, Sphere* b);