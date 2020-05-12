
#include "Forest.h"


GLfloat* generateStakes(int nbStakes, float xmax) {
    int stepsPerFace = (nbStakes-1) / 4;
    int step = xmax / (float) stepsPerFace;
    GLfloat* stakes = malloc(2*nbStakes*sizeof(GLfloat));
    int i,j=0;
    for (i = 0; i < stepsPerFace;i++,j++) {
        stakes[2 * j + 0] = i * step;
        stakes[2 * j + 1] = 0;
    }
    for (i = 0; i < stepsPerFace;i++,j++) {
        stakes[2 * j + 0] = xmax;
        stakes[2 * j + 1] = i * step;
    }
    for (i = 0; i < stepsPerFace;i++,j++) {
        stakes[2 * j + 0] = xmax - i * step;
        stakes[2 * j + 1] = xmax;
    }
    for (i = 0; i < stepsPerFace;i++,j++) {
        stakes[2 * j + 0] = 0;
        stakes[2 * j + 1] = xmax - i * step;;
    }
    stakes[2*j] = 0.0;
    stakes[2*j+1] = 0.0;
    return stakes;
}

Forest* loadForest(const Terrain* terrain, char* fileTexture, GLuint shader, int nbStakes)
{
    Forest* forest = malloc(sizeof(Forest));
    if (forest == NULL)
        printf("failed to allocate forest struct");

    forest->shader = shader;
    GLfloat* stakes = generateStakes(nbStakes, terrain->w - 2);

    LoadTGATextureSimple(fileTexture, &forest->texture);

    float repetitions = 0.07;
    float h = 400;
    float h0 = 0;
    float margin = 1;
   
    GLfloat* vertices = malloc((nbStakes) * 3 * 2 * sizeof(GLfloat));
    GLuint* indices = malloc(nbStakes*2*3*sizeof(GLfloat));
    GLfloat* texCoords = malloc(nbStakes * 2 * 2 * sizeof(GLfloat));
    int terrainWidth = sqrt(terrain->model->numVertices);

    for (int i = 0;i < nbStakes;i++) 
    {

        h0 = terrain->model->vertexArray[(int)(stakes[2 * i] + stakes[2 * i+1] * terrainWidth) * 3 + 1];
        
        vertices[6*i + 0] = stakes[2*i];
        vertices[6*i + 1] = h0 - margin;
        vertices[6*i + 2] = stakes[2*i + 1];

        vertices[6 * i + 3] = stakes[2 * i];
        vertices[6 * i + 4] = h0 + h - margin;
        vertices[6 * i + 5] = stakes[2 * i + 1];

        indices[6 * i + 0] = 2 * i + 1;
        indices[6 * i + 1] = 2 * i;
        indices[6 * i + 2] = 2 * i + 3;

        indices[6 * i + 3] = 2 * i;
        indices[6 * i + 4] = 2 * i + 2;
        indices[6 * i + 5] = 2 * i + 3;

        texCoords[4 * i + 0] = (float) i * repetitions;
        texCoords[4 * i + 1] = 1.0;

        texCoords[4 * i + 2] = (float) i * repetitions;
        texCoords[4 * i + 3] = 0.0;

    }
   
    forest->model = LoadDataToModel(vertices, NULL, texCoords, NULL, indices, 2*nbStakes, 3*2*nbStakes);
    free(vertices);
    free(indices);
    free(texCoords);
    free(stakes);
   
    return forest;
}

void drawForest(Forest* forest, mat4 worldToView) {
    mat4 modelToWorld = IdentityMatrix();
    mat4 modelToView = Mult(modelToWorld, worldToView);
    glUseProgram(forest->shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, forest->texture);
    glUniformMatrix4fv(glGetUniformLocation(forest->shader, "modelviewMatrix"), 1, GL_TRUE, modelToView.m);
    DrawModel(forest->model, forest->shader, "inPosition", NULL, "inTexCoord");
}