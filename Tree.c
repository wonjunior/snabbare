#include "Tree.h"

Tree* loadTrees(char* fileTexture, const char* mapFile, Terrain* terrain, GLuint shader) {

    Tree* tree = malloc(sizeof(Tree));
    if (tree == NULL)
        printf("failed to allocate forest struct");

    tree->shader = shader;


    LoadTGATextureSimple(fileTexture, &tree->texture);
    //    float texture_size = 100
    float repetitions = 2;
    float h = 10.8;
    float width = 6.9;

    GLfloat vertices[] =
    {   -width/2, 0.0, 0.0,
        -width/2, h, 0.0,
        width/2, 0.0, 0.0,
        width/2, h, 0.0,
    };

    GLuint indices[] = {
        3, 0, 2,
        1, 0, 3,
    };

    GLfloat texCoords[] = {
        0.0, 1.0,
        0.0, 0.0,
        1.0, 1.0,
        1.0, 0.0,
    };

    tree->model = LoadDataToModel(vertices, NULL, texCoords, NULL, indices, 4, 3 * 2);

    loadTreesPositions(tree, terrain, mapFile);

    return tree;
}

void drawTrees(Tree* tree, mat4 worldToView) {

    mat4 modelToWorld = IdentityMatrix();
    mat4 modelToView = Mult(modelToWorld, worldToView);

    glUseProgram(tree->shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tree->texture);

    for (int i = 0; i < tree->nbTrees; i++) {
        modelToWorld = T(tree->pos[i].x, tree->pos[i].y, tree->pos[i].z);
        modelToView = Mult(worldToView, modelToWorld);
        glUniformMatrix4fv(glGetUniformLocation(tree->shader, "modelviewMatrix"), 1, GL_TRUE, modelToView.m);
        DrawModel(tree->model, tree->shader, "inPosition", NULL, "inTexCoord");
    }
}



void loadTreesPositions(Tree* tree, const Terrain* terrain, char* mapFile)
{
    TextureData treeMap;
    LoadTGATextureData(mapFile, &treeMap);
    int x = 0, z = 0, i =0;
    int terrainWidth = sqrt(terrain->model->numVertices);
    tree->nbTrees = 0;

   
    
    //first count nb trees
    for (x = 0; x < treeMap.width; x++)
    {
        for (z = 0; z < treeMap.height; z++)
        {
            if (treeMap.imageData[(x + z * treeMap.width) * (treeMap.bpp / 8)] == 0.0) {
                tree->nbTrees++;
            }
        }
    }

    tree->pos = malloc(tree->nbTrees * sizeof(vec3));
  

    for (x = 0; x < treeMap.width; x++)
    {
        for (z = 0; z < treeMap.height; z++)
        {
            if (treeMap.imageData[(x + z * treeMap.width) * (treeMap.bpp / 8)] == 0.0) {
                tree->pos[i].x = (float) x;
                tree->pos[i].z = (float) z;
                tree->pos[i].y = (float) terrain->model->vertexArray[(int)(x + z * terrainWidth) * 3 + 1];
                i++;
            }
        }
    }

}