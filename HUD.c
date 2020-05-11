#include "HUD.h"

HUD* loadHUD(GLuint shader)
{
    HUD* hud = malloc(sizeof(HUD));

    hud->shader = shader;

    LoadTGATextureSimple("textures/orange.tga", &(hud->texture));

    float a = 0.5;
    GLfloat vertices[] =
    { 
        -a, 0.0, 0.0,
        a, 0.0, 0.0,
        a, a, 0.0,
        -a, a, 0.0
    };

    GLuint indices[] = {
        0, 2, 1,
        0, 3, 2
    };

    GLfloat texCoords[] = {
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0
    };

    hud->minimap.model = LoadDataToModel(vertices, NULL, texCoords, NULL, indices, 4, 3 * 2);

    return hud;
}

void drawHUD(HUD* hud, Camera cam, mat4 worldToView)
{
    vec3 HUDnormal = { 0.0, 0.0, 1.0 };
    vec3 HUDup = { 0.0, 1.0, 0.0 };

    mat4 rotationToNormal = RotateTowards(HUDnormal, cam.forward);
    vec3 newHUDup = MultVec3(rotationToNormal, HUDup);
    mat4 rotationToUp = RotateTowards(newHUDup, cam.up);
    mat4 rotation = Mult(rotationToUp, rotationToNormal);

    mat4 modelToWorld = Mult(T(cam.pos.x, cam.pos.y, cam.pos.z), Mult(rotation, T(0.0, 0.0, 5.0)));
    mat4 modelToView = Mult(worldToView, modelToWorld);

    glUseProgram(hud->shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hud->texture);
    glUniform1i(glGetUniformLocation(hud->shader, "texUnit"), 0);

    glUniformMatrix4fv(glGetUniformLocation(hud->shader, "modelviewMatrix"), 1, GL_TRUE, modelToView.m);
    DrawModel(hud->minimap.model, hud->shader, "inPosition", NULL, "inTexCoord");
}