
#include "Terrain.h"


Terrain* GenerateTerrain(GLuint shader, char* textureFileName, char* terrainTextureMapFileName)
{

    TextureData tex;
    LoadTGATextureData(textureFileName, &tex);

    int vertexCount = tex.width * tex.height;
    int triangleCount = (tex.width - 1) * (tex.height - 1) * 2;
    int x, z;

    GLfloat* vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat* normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat* texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
    GLfloat* normalArrowVertices = malloc(sizeof(GLfloat) * 3 * 2 * vertexCount);
    GLuint* indexArray = malloc(sizeof(GLuint) * triangleCount * 3);

    printf("bpp %d\n", tex.bpp);
    printf("width ; height => %d %d\n", tex.width, tex.height);

    for (x = 0; x < tex.width; x++)
    {
        for (z = 0; z < tex.height; z++)
        {
            // Vertex array. You need to scale this properly
            vertexArray[(x + z * tex.width) * 3 + 0] = x / 1.0;
            vertexArray[(x + z * tex.width) * 3 + 1] = tex.imageData[(x + z * tex.width) * (tex.bpp / 8)] / (5.0 * 2.0);
            vertexArray[(x + z * tex.width) * 3 + 2] = z / 1.0;
        }
    }

    for (x = 0; x < tex.width - 1; x++)
    {
        for (z = 0; z < tex.height - 1; z++)
        {
            // Triangle 1
            indexArray[(x + z * (tex.width - 1)) * 6 + 0] = x + z * tex.width;
            indexArray[(x + z * (tex.width - 1)) * 6 + 1] = x + (z + 1) * tex.width;
            indexArray[(x + z * (tex.width - 1)) * 6 + 2] = x + 1 + z * tex.width;

            // Triangle 2
            indexArray[(x + z * (tex.width - 1)) * 6 + 3] = x + 1 + z * tex.width;
            indexArray[(x + z * (tex.width - 1)) * 6 + 4] = x + (z + 1) * tex.width;
            indexArray[(x + z * (tex.width - 1)) * 6 + 5] = x + 1 + (z + 1) * tex.width;
        }
    }

    for (x = 0; x < tex.width; x++)
    {
        for (z = 0; z < tex.height; z++)
        {
            vec3 temp;

            // Normal vectors. You need to calculate these.
            if (x > 1 && z > 1 && x + 1 < tex.width && z + 1 < tex.height)
            {
                temp.x = -(vertexArray[(x + 1 + z * tex.width) * 3 + 1] - vertexArray[(x - 1 + z * tex.width) * 3 + 1]);
                temp.y = 2.0;
                temp.z = -(vertexArray[(x + (z + 1) * tex.width) * 3 + 1] - vertexArray[(x + (z - 1) * tex.width) * 3 + 1]);
            }
            else
            {
                temp.x = 0.0;
                temp.y = 1.0;
                temp.z = 0.0;
            }

            temp = Normalize(temp);
            normalArray[(x + z * tex.width) * 3 + 0] = temp.x;
            normalArray[(x + z * tex.width) * 3 + 1] = temp.y;
            normalArray[(x + z * tex.width) * 3 + 2] = temp.z;

            // 2 points per normal-arrow
            normalArrowVertices[(x + z * tex.width) * 6 + 0] = vertexArray[(x + z * tex.width) * 3 + 0];
            normalArrowVertices[(x + z * tex.width) * 6 + 1] = vertexArray[(x + z * tex.width) * 3 + 1];
            normalArrowVertices[(x + z * tex.width) * 6 + 2] = vertexArray[(x + z * tex.width) * 3 + 2];
            normalArrowVertices[(x + z * tex.width) * 6 + 3] = vertexArray[(x + z * tex.width) * 3 + 0] + 2 * temp.x;
            normalArrowVertices[(x + z * tex.width) * 6 + 4] = vertexArray[(x + z * tex.width) * 3 + 1] + 2 * temp.y;
            normalArrowVertices[(x + z * tex.width) * 6 + 5] = vertexArray[(x + z * tex.width) * 3 + 2] + 2 * temp.z;

            // Texture coordinates. You may want to scale them.
            texCoordArray[(x + z * tex.width) * 2 + 0] = (float)x / tex.width;
            texCoordArray[(x + z * tex.width) * 2 + 1] = (float)z / tex.height;
        }
    }

    Terrain* terrain = malloc(sizeof(Terrain));

    terrain->shader = shader;

    // Create model
    terrain->model = LoadDataToModel(
        vertexArray,
        normalArray,
        texCoordArray,
        NULL,
        indexArray,
        vertexCount,
        triangleCount * 3
    );

    // Save normals
    glGenVertexArrays(1, &terrain->arrayNormalArrow);
    glBindVertexArray(terrain->arrayNormalArrow);
    glGenBuffers(1, &terrain->bufferNormalArrow);

    glBindBuffer(GL_ARRAY_BUFFER, terrain->bufferNormalArrow);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * 3 * sizeof(GLfloat), normalArrowVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(shader, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(shader, "in_Position"));


    // load terrain textures
    LoadTGATextureSimple(terrainTextureMapFileName, &terrain->terrainTextureMap);
    LoadTGATextureSimple("textures/concrete.tga", &terrain->textures[0]);
    LoadTGATextureSimple("textures/sand.tga", &terrain->textures[1]);
    LoadTGATextureSimple("textures/grass.tga", &terrain->textures[2]);
    LoadTGATextureSimple("textures/snow.tga", &terrain->textures[3]);
    return terrain;
}

void DrawNormals(Terrain* terrain)
{
    glBindVertexArray(terrain->arrayNormalArrow);
    glDrawArrays(GL_LINES, 0, 2 * terrain->model->numVertices);
}

void DrawTerrain(Terrain* terrain, mat4 modelToWorld)
{
    glUseProgram(terrain->shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, terrain->terrainTextureMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, terrain->textures[0]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, terrain->textures[1]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, terrain->textures[2]);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, terrain->textures[3]);

    glUniform1i(glGetUniformLocation(terrain->shader, "textureMapUnit"), 0);
    glUniform1i(glGetUniformLocation(terrain->shader, "concreteTexUnit"), 1);
    glUniform1i(glGetUniformLocation(terrain->shader, "sandTexUnit"), 2);
    glUniform1i(glGetUniformLocation(terrain->shader, "grassTexUnit"), 3);
    glUniform1i(glGetUniformLocation(terrain->shader, "snowTexUnit"), 4);

    glUniformMatrix4fv(glGetUniformLocation(terrain->shader, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);
    glUniform1ui(glGetUniformLocation(terrain->shader, "enableLight"), true);
    DrawModel(terrain->model, terrain->shader, "in_Position", "in_Normal", "in_TexCoord");
}