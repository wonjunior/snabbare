#include "Car.h"
#include <math.h>

Car* loadCar(GLuint shader, char* cockpitModel, char* steeringWheelModel, char* frameModel, char* tireModel, char* textureFile, char* tireTexFile)
{
    Car* car = malloc(sizeof(Car));
    if (car == NULL) {
        printf("Failed to allocate memory for Car struct (loadCar)");
    }

    car->cockpit = LoadModelPlus(cockpitModel);
    car->steeringWheel = LoadModelPlus(steeringWheelModel);
    car->frame = LoadModelPlus(frameModel);
    car->tire = LoadModelPlus(tireModel);
    if (car->cockpit == NULL) {
        printf("Failed to allocate memory for Model struct (loadCar)");
    }

    car->shader = shader;

    LoadTGATextureSimple(textureFile, &(car->texture));
    LoadTGATextureSimple(tireTexFile, &(car->tireTexture));

    car->pos.x = 90;
    car->pos.y = 10;
    car->pos.z = 600;
    car->front.x = 0;
    car->front.y = 0;
    car->front.z = 1;
    car->direction.x = 0;
    car->direction.y = 0;
    car->direction.z = 0.5;
    car->up.x = 0;
    car->up.y = 1;
    car->up.z = 0;
    car->left.x = 1;
    car->left.y = 0;
    car->left.z = 0;
    car->rotation = IdentityMatrix();
    car->speed = 0.05;
    car->gas = 0.0;
    car->steering = 0;
    car->tireRotationAngle = 0;
    return car;
}

void drawCar(Car* car, CameraMode cameraMode) {

    //texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, car->texture);
    //glUniform1i(glGetUniformLocation(car->shader, "texUnit"), 0);

    car->front = car->direction;
    // vec3 halfCarSize = { 0, 0, 5 };
    // mat4 midToBack = T(halfCarSize.x, halfCarSize.y, halfCarSize.z);
    // mat4 modelToWorld = Mult(car->rotation, midToBack);
    mat4 modelToWorld = Mult(T(car->pos.x, car->pos.y, car->pos.z), car->rotation);

    glUniformMatrix4fv(glGetUniformLocation(car->shader, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);

    if (cameraMode == CAM_COCKPIT) {
        DrawModel(car->cockpit, car->shader, "in_Position", "in_Normal", "in_TexCoord");

        float angle = car->steering > 0 ? (car->steering < 0.2 ? 0 : car->steering) : (-car->steering < 0.2 ? 0 : car->steering);
        mat4 steeringWheelToCar = Mult(T(1.1, 2.7, 1.3), Mult(Rx(0.4), Rz(0.6*angle)));
        modelToWorld = Mult(T(car->pos.x, car->pos.y, car->pos.z), Mult(car->rotation, steeringWheelToCar));
        glUniformMatrix4fv(glGetUniformLocation(car->shader, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);
        DrawModel(car->steeringWheel, car->shader, "in_Position", "in_Normal", "in_TexCoord");
    }
    else {
        DrawModel(car->frame, car->shader, "in_Position", "in_Normal", "in_TexCoord");

        glBindTexture(GL_TEXTURE_2D, car->tireTexture);

        // front-left wheel
        mat4 tireWheelToCar = Mult(T(2.5, 0.9, 4.2), Ry(-0.3 * car->steering));
        modelToWorld = Mult(Mult(car->rotation, tireWheelToCar), Rx(car->tireRotationAngle));
        modelToWorld = Mult(T(car->pos.x, car->pos.y, car->pos.z), modelToWorld);
        glUniformMatrix4fv(glGetUniformLocation(car->shader, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);
        DrawModel(car->tire, car->shader, "in_Position", "in_Normal", "in_TexCoord");

        // front-right wheel
        tireWheelToCar = Mult(T(-2.5, 0.9, 4.2), Ry(3.14 - 0.3 * car->steering));
        modelToWorld = Mult(Mult(car->rotation, tireWheelToCar), Rx(-car->tireRotationAngle));
        modelToWorld = Mult(T(car->pos.x, car->pos.y, car->pos.z), modelToWorld);
        glUniformMatrix4fv(glGetUniformLocation(car->shader, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);
        DrawModel(car->tire, car->shader, "in_Position", "in_Normal", "in_TexCoord");

        // back-left wheel
        tireWheelToCar = T(2.5, 0.9, -4.2);
        modelToWorld = Mult(Mult(car->rotation, tireWheelToCar), Rx(car->tireRotationAngle));
        modelToWorld = Mult(T(car->pos.x, car->pos.y, car->pos.z), modelToWorld);
        glUniformMatrix4fv(glGetUniformLocation(car->shader, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);
        DrawModel(car->tire, car->shader, "in_Position", "in_Normal", "in_TexCoord");

        // back-right wheel
        tireWheelToCar = Mult(T(-2.5, 0.9, -4.2), Ry(3.14));
        modelToWorld = Mult(Mult(car->rotation, tireWheelToCar), Rx(-car->tireRotationAngle));
        modelToWorld = Mult(T(car->pos.x, car->pos.y, car->pos.z), modelToWorld);
        glUniformMatrix4fv(glGetUniformLocation(car->shader, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);
        DrawModel(car->tire, car->shader, "in_Position", "in_Normal", "in_TexCoord");
    }
}

void setCarHeight(Car* car, Terrain* terrain)
{
    //set car position
    int xleft = floor(car->pos.x);
    int xright = xleft + 1;
    int zleft = floor(car->pos.z);
    int zright = zleft + 1;

    vec3 a, b, c;
    if (car->pos.x - xleft + car->pos.z - zleft < 1) // can be either triangle 1 or triangle 2
    {
        a = SetVector(xleft, 0, zleft);
        b = SetVector(xright, 0, zleft);
        c = SetVector(xleft, 0, zright);
    }
    else
    {
        a = SetVector(xright, 0, zright);
        b = SetVector(xleft, 0, zright);
        c = SetVector(xright, 0, zleft);
    }

    int terrainWidth = sqrt(terrain->model->numVertices);
    a.y = terrain->model->vertexArray[(int)(a.x + a.z * terrainWidth) * 3 + 1];
    b.y = terrain->model->vertexArray[(int)(b.x + b.z * terrainWidth) * 3 + 1];
    c.y = terrain->model->vertexArray[(int)(c.x + c.z * terrainWidth) * 3 + 1];

    float weightB = (car->pos.x - a.x) * (car->pos.x > a.x ? +1 : -1);
    float weightC = (car->pos.z - a.z) * (car->pos.z > a.z ? +1 : -1);
    float weightA = 1 - weightB - weightC;

    car->pos.y = weightA * a.y + weightB * b.y + weightC * c.y;
}

void setCarUp(Car* car, Terrain* terrain)
{
    //set car position
    int xleft = floor(car->pos.x);
    int xright = xleft + 1;
    int zleft = floor(car->pos.z);
    int zright = zleft + 1;

    vec3 a, b, c;
    if (car->pos.x - xleft + car->pos.z - zleft < 1) // can be either triangle 1 or triangle 2
    {
        a = SetVector(xleft, 0, zleft);
        b = SetVector(xright, 0, zleft);
        c = SetVector(xleft, 0, zright);
    }
    else
    {
        a = SetVector(xright, 0, zright);
        b = SetVector(xleft, 0, zright);
        c = SetVector(xright, 0, zleft);
    }

    int terrainWidth = sqrt(terrain->model->numVertices);
    vec3 normalA, normalB, normalC;

    normalA.x = terrain->model->vertexArray[(int)(a.x + a.z * terrainWidth) * 3 + 0];
    normalA.y = terrain->model->vertexArray[(int)(a.x + a.z * terrainWidth) * 3 + 1];
    normalA.z = terrain->model->vertexArray[(int)(a.x + a.z * terrainWidth) * 3 + 2];

    normalB.x = terrain->model->vertexArray[(int)(b.x + b.z * terrainWidth) * 3 + 0];
    normalB.y = terrain->model->vertexArray[(int)(b.x + b.z * terrainWidth) * 3 + 1];
    normalB.z = terrain->model->vertexArray[(int)(b.x + b.z * terrainWidth) * 3 + 2];

    normalC.x = terrain->model->vertexArray[(int)(c.x + c.z * terrainWidth) * 3 + 0];
    normalC.y = terrain->model->vertexArray[(int)(c.x + c.z * terrainWidth) * 3 + 1];
    normalC.z = terrain->model->vertexArray[(int)(c.x + c.z * terrainWidth) * 3 + 2];


    float weightB = (car->pos.x - a.x) * (car->pos.x > a.x ? +1 : -1);
    float weightC = (car->pos.z - a.z) * (car->pos.z > a.z ? +1 : -1);
    float weightA = 1 - weightB - weightC;
    printf("%f\n", car->pos.x - a.x);

   /* vec3 newUp = Normalize(VectorAdd(ScalarMult(normalA, weightA), VectorAdd(ScalarMult(normalB, weightB), ScalarMult(normalC, weightC))));
    car->rotation = Mult(car->rotation, RotateTowards(Normalize(car->up), newUp));
    car->up = newUp;*/
}

void updateCar(Car* subaru, const char* controls, Terrain* terrain)
{

    float turningSensibility = 0.07;
    float friction = 0.002;
    float v_max = 1;
    float steering_return = turningSensibility * 2;
    float max_steering = 2.0;
    float r_min = 70;

    if (controls[CTRL_GAS]) {
        if (subaru->speed < v_max)
            subaru->speed += 0.008;
        else
            subaru->speed = v_max;
        /**
        if (subaru->gas < 1) {
            subaru->gas += 0.001;
            float t = subaru->gas;
            float a_max = 0.0005;
            //float a = -4 * a_max * t * (t - 1);
            subaru->speed += a;
        }*/
    }
    else {
        subaru->gas = 0;
        if (subaru->speed > 0) {
            subaru->speed -= 0.002 + friction * subaru->speed * subaru->speed;
        }
        else
            subaru->speed = 0;
    }

    if (controls[CTRL_BRAKE]) {
        subaru->speed -= 0.005;
        if (subaru->speed < 0)
            subaru->speed = 0;
    }
    if (subaru->speed > 0) {
        if (controls[CTRL_LEFT]) {
            subaru->steering -= turningSensibility;
        }
        else if (controls[CTRL_RIGHT]) {
            subaru->steering += turningSensibility;
        }
        else {

            if (subaru->steering < 0)
                subaru->steering += steering_return;
            else if (subaru->steering > 0)
                subaru->steering -= steering_return;

        }


        if (subaru->steering > max_steering)
            subaru->steering = max_steering;
        else if (subaru->steering < -max_steering)
            subaru->steering = -max_steering;

        if (subaru->steering != 0) {
            float r = r_min / subaru->steering;
            float radial_speed = subaru->speed / r;
            subaru->direction = MultVec3(Ry(-radial_speed), subaru->direction);
            subaru->front = MultVec3(Ry(-radial_speed), subaru->front);
            subaru->rotation = Mult(subaru->rotation, Ry(-radial_speed));
        }
    }

    subaru->tireRotationAngle += subaru->speed / 2.0;
    subaru->pos = VectorAdd(subaru->pos, ScalarMult(Normalize(subaru->direction), subaru->speed));
    subaru->left = Normalize(CrossProduct(subaru->up, subaru->front));

    handleCollisions(subaru, terrain);

}


void handleCollisions(Car* subaru, const Terrain* terrain) {
    int const border = 5;
    if (subaru->pos.x > terrain->w - border) {
        subaru->pos.x = terrain->w - border;
        //subaru->direction.x = 0;
    }
    if (subaru->pos.x < border) {
        subaru->pos.x = border;
        //subaru->direction.x = 0;
    }
    if (subaru->pos.z > terrain->w - border) {
        subaru->pos.z = terrain->w - border;
        //subaru->direction.z = 0;
    }
    if (subaru->pos.z < border) {
        subaru->pos.z = border;
        //subaru->direction.z = 0;
    }
    //subaru->speed *= sqrt(DotProduct(subaru->direction, subaru->direction));
   // subaru->direction = Normalize(subaru->direction);
}
