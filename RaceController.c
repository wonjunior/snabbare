#include "RaceController.h"

float square(float a) {
    return a * a;
}

RaceController createController() {
    GLfloat points[] = {
        133, 410,
        69, 866,
        554, 923,
        770, 853,
        894, 717,
        928, 405,
        510, 171,
    };
    RaceController controller = {
        NULL,
        NULL,
        70,
        NULL,
        7,
        1,
        1,
    };
    controller.checkPoints = malloc(2 * controller.nbCheckPoints * sizeof(GLfloat));
    for (int i = 0;i < 2*controller.nbCheckPoints;i++) {
        controller.checkPoints[i] = points[i];
    }

    return controller;
}

void updateController(RaceController* controller, const Car* car) {
    
    float checkpoint_x = controller->checkPoints[2 * controller->nextCheckPoint];
    float checkpoint_z = controller->checkPoints[2 * controller->nextCheckPoint + 1];
    if (square(car->pos.x - checkpoint_x) + square(car->pos.z - checkpoint_z) < square(controller->checkPointsRadius)) {
        controller->nextCheckPoint++;
        controller->nextCheckPoint %= controller->nbCheckPoints;
        
        if (controller->nextCheckPoint == 1) {
            controller->lap++;
            exit(0);
        }
    }
}
