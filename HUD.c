#include "HUD.h"

#define GLUT_WINDOW_WIDTH   102
#define GLUT_WINDOW_HEIGHT  103

HUD* loadHUD(GLuint shader, float terrainSize)
{
    HUD* hud = malloc(sizeof(HUD));

    hud->minimap.terrainSize = terrainSize;
    hud->shader = shader;
    hud->screenResolution = 1.0;

    LoadTGATextureSimple("textures/minimap.tga", &(hud->minimap.mapTexture));
    LoadTGATextureSimple("textures/checkpoint.tga", &(hud->minimap.checkpointTexture));
    LoadTGATextureSimple("textures/car.tga", &(hud->minimap.carTexture));

    hud->minimap.size = 0.5;
    hud->minimap.offset = 0.5;

    float a = hud->minimap.offset;
    float screenResolution = hud->screenResolution;

    float size = hud->minimap.size;
    GLfloat mapVertices[] = {
        (a-size/2)/screenResolution,(-a-size/2),0.0f,
        (a-size/2)/screenResolution,(-a+size/2),0.0f,
        (a+size/2)/screenResolution,(-a-size/2),0.0f,
        (a+size/2)/screenResolution,(-a+size/2),0.0f
    };
    
    size = hud->minimap.size / 50.0;
    GLfloat carVertices[] = {
        (a - size / 2) / screenResolution, (-a - size / 2), 0.0f,
        (a - size / 2) / screenResolution, (-a + size / 2), 0.0f,
        (a + size / 2) / screenResolution, (-a - size / 2), 0.0f,
        (a + size / 2) / screenResolution, (-a + size / 2), 0.0f
    };

    float width = hud->minimap.size / 20.0;
    float height = width / 3;
    GLfloat checkpointVertices[] = {
        (a - width) / screenResolution,(-a - height),0.0f,
        (a - width) / screenResolution,(-a + height),0.0f,
        (a + width) / screenResolution,(-a - height),0.0f,
        (a + width) / screenResolution,(-a + height),0.0f
    };

    GLuint indices[] = {
        0, 3, 1,
        0, 2, 3
    };

    GLfloat texCoords[] = {
        1.0, 0.0,
        1.0, 1.0,
        0.0, 0.0,
        0.0, 1.0
    };

    GLfloat checkpointTexCoords[] = {
        3.0, 0.0,
        3.0, 1.0,
        0.0, 0.0,
        0.0, 1.0
    };

    hud->minimap.mapBillboard = LoadDataToModel(mapVertices, NULL, texCoords, NULL, indices, 4, 3 * 2);
    hud->minimap.carBillboard = LoadDataToModel(carVertices, NULL, texCoords, NULL, indices, 4, 3 * 2);
    hud->minimap.checkpointBillboard = LoadDataToModel(checkpointVertices, NULL, checkpointTexCoords, NULL, indices, 4, 3 * 2);

    return hud;
}

void drawHUD(HUD* hud, mat4 worldToView, Camera cam, vec3 carPos)
{
    glUseProgram(hud->shader);
    glActiveTexture(GL_TEXTURE0);

    glDisable(GL_DEPTH_TEST);
    glUniform1ui(glGetUniformLocation(hud->shader, "ACTIVE_HUD"), 1);

    // ---------- load map billboard
    glUniformMatrix4fv(glGetUniformLocation(hud->shader, "modelviewMatrix"), 1, GL_TRUE, IdentityMatrix().m);
    glBindTexture(GL_TEXTURE_2D, hud->minimap.mapTexture);
    DrawModel(hud->minimap.mapBillboard, hud->shader, "inPosition", NULL, "inTexCoord");

    // ---------- load map checkpoint
    float x = 90.0 * hud->minimap.size / hud->minimap.terrainSize - hud->minimap.size / 2;
    float y = 600.0 * hud->minimap.size / hud->minimap.terrainSize - hud->minimap.size / 2;
    mat4 translation = T(-x, y, 0.0);

    glUniformMatrix4fv(glGetUniformLocation(hud->shader, "modelviewMatrix"), 1, GL_TRUE, translation.m);
    glBindTexture(GL_TEXTURE_2D, hud->minimap.checkpointTexture);
    DrawModel(hud->minimap.checkpointBillboard, hud->shader, "inPosition", NULL, "inTexCoord");

    // ---------- load car billboard
    x = carPos.x * hud->minimap.size / hud->minimap.terrainSize - hud->minimap.size / 2;
    y = carPos.z * hud->minimap.size / hud->minimap.terrainSize - hud->minimap.size / 2;
    translation = T(-x, y, 0.0);

    glUniformMatrix4fv(glGetUniformLocation(hud->shader, "modelviewMatrix"), 1, GL_TRUE, translation.m);
    glBindTexture(GL_TEXTURE_2D, hud->minimap.carTexture);
    DrawModel(hud->minimap.carBillboard, hud->shader, "inPosition", NULL, "inTexCoord");


    glEnable(GL_DEPTH_TEST);
    glUniform1ui(glGetUniformLocation(hud->shader, "ACTIVE_HUD"), 0);
}