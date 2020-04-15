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


    return car;
}

void drawCar(Car* car) {

    //texture
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, car->texture);
    //glUniform1i(glGetUniformLocation(car->shader, "texUnit"), 0);

    mat4 modelToWorld = T(car->pos.x, car->pos.y, car->pos.z);


    glUniformMatrix4fv(glGetUniformLocation(car->shader, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);
    DrawModel(car->model, car->shader, "in_Position", "in_Normal", "in_TexCoord");
}