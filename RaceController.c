#include "RaceController.h"

float square(float a) {
    return a * a;
}

RaceController createController(const Car* car) {
    GLfloat points[] = {
        133, 410,
        69, 866,
        554, 923,
        770, 853,
        894, 717,
        928, 405,
        510, 171,
    };
    RaceController controller;
    controller.checkPointsRadius = 70;
    controller.nbCheckPoints = 7;
    controller.nextCheckPoint = 1;
    controller.lastGhostStep = 0;
    controller.nbGhostSteps = 0;
    controller.lastCarStep = 0;
    controller.distanceBetweenSteps = 50;
    controller.carLoopCounter = 0;
    controller.ghostLoopCounter = 0;

    controller.carPath = malloc(100 * sizeof(Step));
    controller.ghostPath = malloc(100 * sizeof(Step));

    controller.carPath[0].pos[0] = car->pos.x;
    controller.carPath[0].pos[1] = car->pos.z;
    controller.carPath[0].time = 0;

    controller.checkPoints = malloc(2 * controller.nbCheckPoints * sizeof(GLfloat));
    for (int i = 0;i < 2*controller.nbCheckPoints;i++) {
        controller.checkPoints[i] = points[i];
    }

    return controller;
}

void updateController(RaceController* controller, Car* car, Car* ghost) {
    controller->carLoopCounter++;

    //Car path recording
    float laststep_x = controller->carPath[controller->lastCarStep].pos[0];
    float laststep_z = controller->carPath[controller->lastCarStep].pos[1];
    if (square(car->pos.x - laststep_x) + square(car->pos.z - laststep_z) >= square(controller->distanceBetweenSteps))
    {
        int current = ++controller->lastCarStep;
        controller->carPath[current].time = controller->carLoopCounter;
        controller->carPath[current].pos[0] = car->pos.x;
        controller->carPath[current].pos[1] = car->pos.z;
        controller->carLoopCounter = 0;
    }

    //Checkpoints
    float checkpoint_x = controller->checkPoints[2 * controller->nextCheckPoint];
    float checkpoint_z = controller->checkPoints[2 * controller->nextCheckPoint + 1];
    if (square(car->pos.x - checkpoint_x) + square(car->pos.z - checkpoint_z) < square(controller->checkPointsRadius)) {
        controller->nextCheckPoint++;
        controller->nextCheckPoint %= controller->nbCheckPoints;
        
        //new lap
        if (controller->nextCheckPoint == 1) {
            controller->nbGhostSteps = controller->lastCarStep + 1;

            Step* newGhostPath = controller->carPath;
            //ghostPath useless content will be overwritten
            controller->carPath = controller->ghostPath;
            controller->ghostPath = newGhostPath;

            controller->carLoopCounter = 0;
            controller->ghostLoopCounter = 0;
            controller->lastCarStep = 0;
            controller->carPath[0].time = 0;
            controller->carPath[0].pos[0] = car->pos.x;
            controller->carPath[0].pos[1] = car->pos.z;


            float x1 = controller->ghostPath[0].pos[0];
            float z1 = controller->ghostPath[0].pos[1];
            float x2 = controller->ghostPath[1].pos[0];
            float z2 = controller->ghostPath[1].pos[1];
            ghost->pos.x = x1;
            ghost->pos.z = z1;
            ghost->direction.x = x2 - x1;
            ghost->direction.z = z2 - z1;
            ghost->direction.y = 0;
            ghost->direction = Normalize(ghost->direction);
            ghost->front = ghost->direction;
            ghost->nextDirection = ghost->direction;
            ghost->speed = sqrt(square(x2 - x1) + square(z2 - z1)) / (float) controller->ghostPath[1].time;
            exit(0);
            controller->lastGhostStep = 0;
        }
    }

    //Ghost path
    if (controller->nbGhostSteps != 0) {
        controller->ghostLoopCounter++;
        int last = controller->lastGhostStep;
        int current = last + 1;
        int next = current + 1;
        if (controller->ghostLoopCounter >= controller->ghostPath[current].time){
            if (next < controller->nbGhostSteps){
                controller->ghostLoopCounter = 0;
                controller->lastGhostStep = current;
                float x1 = controller->ghostPath[current].pos[0];
                float z1 = controller->ghostPath[current].pos[1];
                float x2 = controller->ghostPath[next].pos[0];
                float z2 = controller->ghostPath[next].pos[1];
                ghost->pos.x = x1;
                ghost->pos.z = z1;
                ghost->direction.x = x2 - x1;
                ghost->direction.z = z2 - z1;
                ghost->direction.y = 0;
                ghost->direction = Normalize(ghost->direction);
                ghost->speed = sqrt(square(x2 - x1) + square(z2 - z1)) / (float)controller->ghostPath[next].time; 
            }
            else {
                controller->nbGhostSteps = 0;
   
            }
        }
            
    }
}
