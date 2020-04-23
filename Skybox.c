#include "Skybox.h"

Skybox* CreateSkybox(const char* modelFileName, char* textureFileName, GLuint shader)
{
    Skybox* skybox = malloc(sizeof(Skybox));

    skybox->shader = shader;
    skybox->model = LoadModelPlus(modelFileName);
    LoadTGATextureSimple(textureFileName, &skybox->texture);

    return skybox;
}


void DrawSkybox(Skybox* skybox, mat4 worldToView)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skybox->texture);
    glUniform1ui(glGetUniformLocation(skybox->shader, "enableLight"), false);


    mat4 modelToWorldSkyBox = Mult(T(0, -10, 0), S(SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE));
    mat4 worldToViewSkyBox = worldToView;

    worldToViewSkyBox.m[3] = 0.0;
    worldToViewSkyBox.m[7] = 0.0;
    worldToViewSkyBox.m[11] = 0.0;
    LoadMatrixToUniform("modelToWorld", modelToWorldSkyBox);
    LoadMatrixToUniform("worldToView", worldToViewSkyBox);

    DrawModel(skybox->model, skybox->shader, "in_Position", "in_Normal", "in_TexCoord");
    glUniform1ui(glGetUniformLocation(skybox->shader, "enableLight"), true);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // reset back to default worldToView
    LoadMatrixToUniform("worldToView", worldToView);
}
