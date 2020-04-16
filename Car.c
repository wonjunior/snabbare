#include "Car.h"
/*
typedef struct {
    Model* model;
    TextureData textureMap;
    GLuint shader;
    vec3 pos;
    vec3 speed;
    vec3 front;
    vec3 up;
} Car;
*/

Car* loadCar(GLuint shader, char* modelFile, char textureFile[])
{
    Car* car = malloc(sizeof(Car));
    if (car == NULL) {
        printf("Failed to allocate memory for Car struct (loadCar)");
    }

    car->model = LoadModelPlus(modelFile);
    if (car->model == NULL) {
        printf("Failed to allocate memory for Model struct (loadCar)");
    }

    car->shader = shader;

    LoadTGATextureSimple(textureFile, &(car->texture));

    car->pos.x = 50;
    car->pos.y = 10;
    car->pos.z = 50;
    car->front.x = 0;
    car->front.y = 0;
    car->front.z = 1;
    car->speed.x = 0;
    car->speed.y = 0;
    car->speed.z = 0.5;
    car->up.x = 0;
    car->up.y = 1;
    car->up.z = 0;
    car->rotation = IdentityMatrix();

    return car;
}

void drawCar(Car* car) {

    //texture
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, car->texture);
    //glUniform1i(glGetUniformLocation(car->shader, "texUnit"), 0);

    mat4 modelToWorld = Mult(T(car->pos.x, car->pos.y, car->pos.z), car->rotation);


    glUniformMatrix4fv(glGetUniformLocation(car->shader, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);
    DrawModel(car->model, car->shader, "in_Position", "in_Normal", "in_TexCoord");
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
    printf("%f\n", car->pos.x - a.x);

    car->pos.y = weightA * a.y + weightB * b.y + weightC * c.y;
}