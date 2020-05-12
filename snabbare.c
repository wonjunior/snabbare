// Ingemar's psychedelic teapot 2+
// Rewritten for modern OpenGL
// Teapot model generated with the OpenGL Utilities for Geometry Generation
// (also written by Ingemar) based on the original teapot data.

// Here with comfortable mouse dragging interface!

// gcc psychteapot2.c ../common/*.c ../common/Linux/*.c -lGL -o psychteapot2 -I../common -I../common/Linux -DGL_GLEXT_PROTOTYPES  -lXt -lX11 -lm

#include "MicroGlut.h"
// uses framework Cocoa
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

#include <math.h>
#include "Terrain.h"
#include "Car.h"
#include "Skybox.h"
#include "Camera.h"
#include "Forest.h"
#include "Tree.h"
#include "HUD.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// Globals
#define PI 3.1415
#define WORLD_SIZE 100.0
#define NIGHT_MODE false
#define TERRAIN_REPEAT 100.0

#define NEAR 1.0
#define FAR 5000.0
#define RIGHT 0.5
#define LEFT -0.5
#define TOP 0.5
#define BOTTOM -0.5




char controls[4] = { 0, 0, 0, 0 };

Skybox* skybox;
Terrain* terrain;
Forest* forest;
Tree* tree;

Camera camera;
HUD* hud;

bool showNormals;
bool showTerrain;
bool showForest;

GLuint grassTexture;

Car* subaru;

GLfloat projectionMatrix[] =
{
    2.0f * NEAR / (RIGHT - LEFT), 0.0f, (RIGHT + LEFT) / (RIGHT - LEFT), 0.0f,
    0.0f, 2.0f * NEAR / (TOP - BOTTOM), (TOP + BOTTOM) / (TOP - BOTTOM), 0.0f,
    0.0f, 0.0f, -(FAR + NEAR) / (FAR - NEAR), -2 * FAR * NEAR / (FAR - NEAR),
    0.0f, 0.0f, -1.0f, 0.0f
};

mat4 worldToViewMatrix, modelToWorldMatrix;

// Reference to shader program
GLuint program;



// Lightning 
vec3 lightSourcesDirectionsPositions[] =
{
    {0.0f, 100.0f, 0.0f}, 		// positional
    {256.0f, 100.0f, 0.0f}, 		// positional
    {256.0f, 100.0f, 256.0f},	 	// along X (towards +X)
    {0.0f, 100.0f, 256.0f}, 	// along Z (towards +Z)
};

vec3 lightSourcesColorsArr[] =
{
    {1.0f, 1.0f, 1.0f}, 		// White light
    {0.2f, 0.2f, 0.2f}, 		// White light
    {0.2f, 0.2f, 0.2f}, 		// White light
    {0.2f, 0.2f, 0.2f}, 		// White light
};

GLint isDirectional[] = { 0,0,0,0 };
GLfloat specularExponent[] = { 100.0, 200.0, 60.0, 50.0, 300.0, 150.0 }; // should be 1 per object


mat4 RotateTowards(vec3 src, vec3 dest)
{
    vec3 v = CrossProduct(src, dest);
    float cos = DotProduct(Normalize(src), Normalize(dest));
    return ArbRotate(v, acos(cos));
}

void LoadMatrixToUniform(const char* variableName, mat4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(program, variableName), 1, GL_TRUE, matrix.m);
}



void mouseHandler(int x, int y)
{
    rotateGodCamera(&camera, x, y);
}

void keyUpHandler(unsigned char key, int x, int y) {
    switch (key)
    {
            // car control
        case 'z':
            controls[CTRL_GAS] = 0;
            break;
        case 's':
            controls[CTRL_BRAKE] = 0;
            break;
        case 'q':
            controls[CTRL_LEFT] = 0;
            break;
        case 'd':
            controls[CTRL_RIGHT] = 0;
            break;
    }
}

void keyHandler(unsigned char key, int x, int y)
{
    moveGodCamera(&camera, key);

    switch (key) {

        // car control
    case 'z':
        controls[CTRL_GAS] = 1;
        break;
    case 's':
        controls[CTRL_BRAKE] = 1;
        break;
    case 'q':
        controls[CTRL_LEFT] = 1;
        break;
    case 'd':
        controls[CTRL_RIGHT] = 1;
        break;

    // dev
    case 'n':
        showNormals = !showNormals;
        break;
    case 'b':
        showTerrain = !showTerrain;
        break;
    case 'v':
        showForest = !showForest;
        break;
    }
}


void loadShaderParams(GLuint shader) 
{
    // Projection

    glUniformMatrix4fv(glGetUniformLocation(shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    // Lighting
    glUniform3fv(glGetUniformLocation(shader, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(shader, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1f(glGetUniformLocation(shader, "specularExponent"), specularExponent[0]);
    glUniform1iv(glGetUniformLocation(shader, "isDirectional"), 4, isDirectional);
    glUniform1ui(glGetUniformLocation(shader, "enableLight"), true);
}

void initBillboardShader(GLuint shader)
{
    glUniform1ui(glGetUniformLocation(shader, "ACTIVE_HUD"), 0);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 0);
}

void init(void)
{
    dumpInfo();
    camera = createCamera();
    showNormals = false;
    showTerrain = true;
    showForest = true;

    // GL inits
    glClearColor(0.15, 0.42, 0.70, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glutHideCursor();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    

    // Load and compile shader
    program = loadShaders("lab4-6.vert", "lab4-6.frag");
    glUseProgram(program); // default shader

    // ------------------- textures loading
    LoadTGATextureSimple("textures/concrete.tga", &grassTexture);
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0); 		// Texture unit 0
    glActiveTexture(GL_TEXTURE0);

    // ------------------- Load skybox
    skybox = CreateSkybox("models/skybox.obj", "textures/SkyBox512.tga", program);
 
    // ------------------- Load models

    subaru = loadCar(program, "models/cockpit.obj", "models/steering_wheel.obj", "models/frame.obj", "models/tire.obj", "textures/orange.tga");
    loadShaderParams(program);


    GLuint terrainShader = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");

    terrain = GenerateTerrain(terrainShader, "textures/heightmap.tga", "textures/terrain_multitex.tga");

    loadShaderParams(terrain->shader);

    GLuint billboardShader = loadShaders("shaders/forest.vert", "shaders/forest.frag");
    forest = loadForest(terrain, "textures/forest_3.tga", billboardShader, 401);
    initBillboardShader(billboardShader);

    char* treeFiles[] = {"textures/tree3.tga", "textures/tree1.tga", "textures/tree2.tga"};
    tree = loadTrees(treeFiles, 3, "textures/tree_map.tga", terrain, billboardShader);
    
    // ------------------- Load HUD
    hud = loadHUD(billboardShader, terrain->w);

    glUseProgram(program);
}

GLfloat a, b = 0.0;


void display(void)
{
   
    
    //setCarUp(subaru, terrain);
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform1ui(glGetUniformLocation(program, "enableTransparency"), false);

    // night node activated
    glUniform1ui(glGetUniformLocation(program, "NIGHT_MODE"), 0);

    // update modelToView matrix
    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    float a = t / 1000;
    glUniform1f(glGetUniformLocation(program, "time"), a);

    mat4 modelToWorld = Mult(T(0, 0, 0), Ry(0));
    mat4 worldToView;

    updateCamera(&camera, subaru, controls);

    worldToView = lookAtv(camera.pos, camera.lookat, camera.up);

    // -------- Draw skybox
    DrawSkybox(skybox, worldToView);
    


    // -------- Draw terrain
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, grassTexture);

    glUniformMatrix4fv(glGetUniformLocation(terrain->shader, "worldToView"), 1, GL_TRUE, worldToView.m);

    vec3 carLightPos = { 0,0,0 };
    //light updtae
    
    lightSourcesDirectionsPositions[0].x = camera.pos.x;
    lightSourcesDirectionsPositions[0].y = camera.pos.y;
    lightSourcesDirectionsPositions[0].z = camera.pos.z;
    glUseProgram(terrain->shader);
    glUniform3fv(glGetUniformLocation(terrain->shader, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUseProgram(program);
    glUniform3fv(glGetUniformLocation(terrain->shader, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    if (showTerrain)
        DrawTerrain(terrain, modelToWorld);

    if (showTerrain && showNormals)
        DrawNormals(terrain);
        

    glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE, worldToView.m);
    glUseProgram(program); // temporary default shader
    updateCar(subaru, controls, terrain);
    setCarHeight(subaru, terrain);
    drawCar(subaru, camera.mode);

    if (showForest)
        drawForest(forest, worldToView);
    
    drawTrees(tree, worldToView, camera);
   
    drawHUD(hud, worldToView, camera, subaru->pos);

    glUseProgram(program);
    glutSwapBuffers();
}


int main(int argc, char* argv[])
{
    //PlaySound("c:\\cb.wav", NULL, SND_ASYNC);
    //delay(2000);
    printf("Snabbare");
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(1200, 1200);
    glutCreateWindow("Snabbare");
#ifdef WIN32
    glewInit();
#endif
    init();
    glutDisplayFunc(display);
    glutPassiveMotionFunc(mouseHandler);
    glutKeyboardFunc(keyHandler);
    glutKeyboardUpFunc(keyUpHandler);
    glutRepeatingTimer(20);
    //glutTimerFunc(20, &OnTimer, 0);

    glutMainLoop();
    exit(0);
}