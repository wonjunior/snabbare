#include "HUD.h"

HUD* loadHUD(GLuint shader, float terrainSize)
{
    HUD* hud = malloc(sizeof(HUD));

    hud->minimap.terrainSize = terrainSize;
    hud->shader = shader;

    LoadTGATextureSimple("textures/minimap2.tga", &(hud->minimap.mapTexture));
    LoadTGATextureSimple("textures/car.tga", &(hud->minimap.carTexture));

    hud->minimap.size = 1.0;
    GLfloat mapVertices[] =
    {
        -hud->minimap.size / 2, 0.0, 0.0,
        hud->minimap.size / 2, 0.0, 0.0,
        hud->minimap.size / 2, hud->minimap.size, 0.0,
        -hud->minimap.size / 2, hud->minimap.size, 0.0,
    };

    float carSize = hud->minimap.size / 50.0;
    GLfloat carVertices[] =
    {
        -carSize / 2, 0.0, 0.0,
        carSize / 2, 0.0, 0.0,
        carSize / 2, carSize, 0.0,
        -carSize / 2, carSize, 0.0
    };

    GLuint indices[] = {
        0, 2, 1,
        0, 3, 2,
    };

    GLfloat texCoords[] = {
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0
    };

    hud->minimap.mapBillboard = LoadDataToModel(mapVertices, NULL, texCoords, NULL, indices, 4, 3 * 2);
    hud->minimap.carBillboard = LoadDataToModel(carVertices, NULL, texCoords, NULL, indices, 4, 3 * 2);

    return hud;
}

void drawHUD(HUD* hud, mat4 worldToView, Camera cam, vec3 carPos)
{
    vec3 HUDnormal = { 0.0, 0.0, 1.0 };
    vec3 HUDup = { 0.0, 1.0, 0.0 };

    mat4 rotationToNormal = RotateTowards(HUDnormal, cam.forward);
    vec3 newHUDup = MultVec3(rotationToNormal, HUDup);
    mat4 rotationToUp = RotateTowards(newHUDup, cam.up);
    mat4 rotation = Mult(rotationToUp, rotationToNormal);

    glUseProgram(hud->shader);
    glActiveTexture(GL_TEXTURE0);

    glDisable(GL_DEPTH_TEST);

    // ---------- load map billboard
    mat4 modelToWorld = Mult(T(cam.pos.x, cam.pos.y, cam.pos.z), Mult(rotation, T(0.0, 0.0, 4.0)));
    mat4 modelToView = Mult(worldToView, modelToWorld);

    glUniformMatrix4fv(glGetUniformLocation(hud->shader, "modelviewMatrix"), 1, GL_TRUE, modelToView.m);
    glBindTexture(GL_TEXTURE_2D, hud->minimap.mapTexture);
    DrawModel(hud->minimap.mapBillboard, hud->shader, "inPosition", NULL, "inTexCoord");

    // ---------- load car billboard
    float x = carPos.x * hud->minimap.size / hud->minimap.terrainSize - hud->minimap.size / 2;
    float y = carPos.z * hud->minimap.size / hud->minimap.terrainSize;
    modelToWorld = Mult(T(cam.pos.x, cam.pos.y, cam.pos.z), Mult(rotation, T(x, y, 4.0)));
    modelToView = Mult(worldToView, modelToWorld);

    glUniformMatrix4fv(glGetUniformLocation(hud->shader, "modelviewMatrix"), 1, GL_TRUE, modelToView.m);
    glBindTexture(GL_TEXTURE_2D, hud->minimap.carTexture);
    DrawModel(hud->minimap.carBillboard, hud->shader, "inPosition", NULL, "inTexCoord");

    glEnable(GL_DEPTH_TEST);
}