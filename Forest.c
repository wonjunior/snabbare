
#include "Forest.h"

Forest* loadForest(float xmax, char* fileTexture) {

    Forest* forest = malloc(sizeof(Forest));
    if (forest == NULL)
        printf("forest mal alloue");


    glUseProgram(forest->shader);

    

    forest->shader = loadShaders("shaders/forest.vert", "shaders/forest.frag");

    LoadTGATextureSimple(fileTexture, &forest->texture);
//    float texture_size = 100
    float repetitions = 13;
    float h = 40;

    GLfloat vertices[] =
    {   0.0, 0.0, 0.0,
        0.0, h, 0.0,
        xmax, 0.0, 0.0,
        xmax, h, 0.0 };

    GLuint indices[] = {
        3, 0, 2,
        1, 0, 3,
    };

    GLfloat texCoords[] = {
        0.0, 1.0,
        0.0, 0.0,
        repetitions, 1.0,
        repetitions, 0.0
    };

    forest->model = LoadDataToModel(vertices, NULL, texCoords, NULL, indices, 4, 6);

    const GLfloat projectionMatrix[] =
    {
        2.0f * NEAR / (RIGHT - LEFT), 0.0f, (RIGHT + LEFT) / (RIGHT - LEFT), 0.0f,
        0.0f, 2.0f * NEAR / (TOP - BOTTOM), (TOP + BOTTOM) / (TOP - BOTTOM), 0.0f,
        0.0f, 0.0f, -(FAR + NEAR) / (FAR - NEAR), -2 * FAR * NEAR / (FAR - NEAR),
        0.0f, 0.0f, -1.0f, 0.0f
    };

    
    glUniformMatrix4fv(glGetUniformLocation(forest->shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    glUniform1i(glGetUniformLocation(forest->shader, "texUnit"), 0);
   

    return forest;
}

void drawForest(Forest* forest, mat4 worldToView) {
    mat4 m2w = IdentityMatrix();
    mat4 modelToView = Mult(m2w, worldToView);
    glUseProgram(forest->shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, forest->texture);
    glUniformMatrix4fv(glGetUniformLocation(forest->shader, "modelviewMatrix"), 1, GL_TRUE, modelToView.m);
    DrawModel(forest->model, forest->shader, "inPosition", NULL, "inTexCoord");
}