#include "Tree.h"


Tree* loadTrees(char** textureFiles, int nbTextures, const char* mapFile, Terrain* terrain, GLuint shader) {

    Tree* tree = malloc(sizeof(Tree));
    if (tree == NULL)
        printf("failed to allocate forest struct");

    tree->shader = shader;
    tree->nbTextures = nbTextures;
    tree->textures = malloc(nbTextures * sizeof(GLuint));

    for (int i = 0;i < nbTextures; i++) {
        LoadTGATextureSimple(textureFiles[i], &tree->textures[i]);
    }



    //    float texture_size = 100
    float repetitions = 2;
    float h = 20;
    float width = 14;

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

void drawTrees(Tree* tree, mat4 worldToView, const Camera camera) {

    mat4 modelToWorld, rot, modelToView;
    vec3 old_normal = { 0, 0, 1 }, new_normal, up = { 0, 1, 0 };
    float cos_angle, sin_angle, angle;

    glUseProgram(tree->shader);
    glActiveTexture(GL_TEXTURE0);
    

    for (int i = 0; i < tree->nbTrees; i++) {

        glBindTexture(GL_TEXTURE_2D, tree->textures[i % tree->nbTextures]);
        new_normal = VectorSub(camera.pos, tree->pos[i]);
        new_normal.y = 0;
        new_normal = Normalize(new_normal);
        cos_angle = DotProduct(new_normal, old_normal);
        sin_angle = DotProduct(CrossProduct(new_normal, old_normal), up);
        angle = acos(cos_angle);
        if (sin_angle > 0)
            angle *= -1;
        rot = Ry(angle);

       // rot = RotateTowards(old_normal, new_normal);

        modelToWorld = Mult(T(tree->pos[i].x, tree->pos[i].y, tree->pos[i].z), rot);
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