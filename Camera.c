#include "Camera.h"

static int oldMouseX = 0, oldMouseY = 0;

Camera createCamera() {

    Camera camera = 
    { 
        CAM_BEHIND,
        { 10, -5, 10 },
        { 0, 1, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 20, 7 , 3 },
        { 0.5, 3.6, 3.6 },

    };
    camera.forward = Normalize(camera.forward);
    return camera;
}

void rotateGodCamera(Camera* camera, int x, int y)
{
    if (camera->mode == CAM_GOD)
    {
        int dx = oldMouseX - x;
        int dy = y - oldMouseY;

        vec3 left = Normalize(CrossProduct(camera->up, camera->forward));
        float step = 0.002;

        // calculate camera rotation
        mat4 camRotationUp = Ry(dx * step);
        mat4 camRotationLeft = ArbRotate(left, dy * step);
        mat4 camRotation = Mult(camRotationUp, camRotationLeft);

        // update forward and up vectors
        camera->forward = MultVec3(camRotation, camera->forward);
        camera->up = MultVec3(camRotation, camera->up);

        // update (x,y) buffer
        oldMouseX = 800 / 2;
        oldMouseY = 600 / 2;
        glutWarpPointer(800 / 2, 600 / 2);
    }
}

void moveGodCamera(Camera* camera, char key) {
    float step = 3;
    vec3 left = Normalize(CrossProduct(camera->up, camera->forward)); // basis (up, forward, left)

    switch (key) {
    case 'i':
        camera->pos = VectorAdd(camera->pos, ScalarMult(camera->forward, step));
        break;
    case 'k':
        camera->pos = VectorSub(camera->pos, ScalarMult(camera->forward, step));
        break;
    case 'j':
        camera->pos = VectorAdd(camera->pos, ScalarMult(left, step));
        break;
    case 'l':
        camera->pos = VectorSub(camera->pos, ScalarMult(left, step));
        break;
    case 'u':
        camera->pos.y -= step;
        break;
    case 32: // SPACEBAR
        camera->pos.y += step;
        break;

    case 'c':
        if (camera->mode == CAM_BEHIND)
            camera->mode = CAM_COCKPIT;
        else if (camera->mode == CAM_COCKPIT)
            camera->mode = CAM_GOD;
        else
        {
            camera->mode = CAM_BEHIND;
            camera->up.x = 0;
            camera->up.y = 1;
            camera->up.z = 0;
        }
        break;
    }
}

void updateCamera(Camera* cam, Car* car, char controls[]) {

    if (cam->mode == CAM_BEHIND)
    {

        float conv = 0.05;
        car->front = car->direction;//for now ...
        cam->forward = VectorAdd(cam->forward, ScalarMult(VectorSub(car->front, cam->forward), conv));

        cam->pos = VectorSub(car->pos, ScalarMult(Normalize(cam->forward), cam->behindTransform.distance));
        cam->pos = VectorAdd(cam->pos, ScalarMult(Normalize(cam->up), cam->behindTransform.height));
        cam->lookat = VectorAdd(car->pos, ScalarMult(Normalize(cam->up), cam->behindTransform.tilt));

    }
    else if (cam->mode == CAM_COCKPIT)
    {
        cam->pos = VectorSub(car->pos, ScalarMult(Normalize(car->front), cam->cockpitTransform.distance));
        cam->pos = VectorAdd(cam->pos, ScalarMult(Normalize(cam->up), cam->cockpitTransform.height));
        cam->lookat = VectorAdd(car->pos, ScalarMult(Normalize(cam->up), cam->cockpitTransform.tilt));

        cam->pos = VectorAdd(cam->pos, ScalarMult(car->left, 1.1));
        cam->lookat = VectorAdd(cam->lookat, ScalarMult(car->left, 1.1));
    }
    else if (cam->mode == CAM_GOD)
        cam->lookat = VectorAdd(cam->pos, cam->forward);
}