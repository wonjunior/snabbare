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



typedef struct {
    vec3 forward;
    vec3 up;
    vec3 pos;
} Camera;

Skybox* skybox;
Terrain* terrain;

// camera setup init
vec3 forward = { 10,-5,10 };
vec3 up = { 0,1,0 };
vec3 cam = { 20, 20, 0 };
int oldMouseX = 0, oldMouseY = 0;

bool showNormals;

vec3 bunnyPos = { 20, 0, 0 };
vec3 bunnyUp = { 0, 1, 0 };


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
    {1.0f, 1.0f, 1.0f}, 		// White light
    {1.0f, 1.0f, 1.0f}, 		// White light
    {1.0f, 1.0f, 1.0f}, 		// White light
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

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void mouseHandler(int x, int y)
{
    int dx = oldMouseX - x;
    int dy = y - oldMouseY;

    vec3 left = Normalize(CrossProduct(up, forward));
    float step = 0.002;

    // calculate camera rotation
    mat4 camRotationUp = Ry(dx * step);
    mat4 camRotationLeft = ArbRotate(left, dy * step);
    mat4 camRotation = Mult(camRotationUp, camRotationLeft);

    // update forward and up vectors
    forward = MultVec3(camRotation, forward);
    up = MultVec3(camRotation, up);

    // update (x,y) buffer
    oldMouseX = 800 / 2;
    oldMouseY = 600 / 2;
    glutWarpPointer(800 / 2, 600 / 2);
}

void keyHandler(unsigned char key, int x, int y)
{
    float step = 1;
    vec3 left = Normalize(CrossProduct(up, forward)); // basis (up, forward, left)
    float turningSensibility = 0.1;

    switch (key) {
    case 'z':
        cam = VectorAdd(cam, ScalarMult(forward, step));
        break;
    case 's':
        cam = VectorSub(cam, ScalarMult(forward, step));
        break;
    case 'q':
        cam = VectorAdd(cam, ScalarMult(left, step));
        break;
    case 'd':
        cam = VectorSub(cam, ScalarMult(left, step));
        break;

    case 'a':
        cam.y -= step;
        break;
    case 32: // SPACEBAR
        cam.y += step;
        break;

        // car control
    case 'i':
        subaru->pos = VectorAdd(subaru->pos, subaru->speed);
        break;
    case 'k':
        subaru->pos = VectorSub(subaru->pos, subaru->speed);
        break;
    case 'j':
        subaru->speed = MultVec3(Ry(turningSensibility), subaru->speed);
        subaru->front = MultVec3(Ry(turningSensibility), subaru->front);
        subaru->rotation = Mult(subaru->rotation, Ry(turningSensibility));
        break;
    case 'l':
        subaru->speed = MultVec3(Ry(-turningSensibility), subaru->speed);
        subaru->front = MultVec3(Ry(-turningSensibility), subaru->front);
        subaru->rotation = Mult(subaru->rotation, Ry(-turningSensibility));
        break;


        // terrain nromals
    case 'n':
        showNormals = !showNormals;
        break;
    }

}


void init(void)
{

    dumpInfo();
    forward = Normalize(forward);
    showNormals = false;

    // GL inits
    glClearColor(0.15, 0.42, 0.70, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glutHideCursor();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // Load and compile shader
    program = loadShaders("lab4-6.vert", "lab4-6.frag");
    glUseProgram(program);

    // ------------------- textures loading
    LoadTGATextureSimple("textures/maskros512.tga", &grassTexture);
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0); 		// Texture unit 0
    glActiveTexture(GL_TEXTURE0);

    // ------------------- Load skybox
    skybox = CreateSkybox("models/skybox.obj", "textures/SkyBox512.tga", program);
 

    // ------------------- Load models
    subaru = loadCar(program, "models/fiat.obj", "textures/orange.tga");



    //ProjectionMatrix = frustum(-0.5, 0.5, -0.5, 0.5, 1.0, 30.0);
    // ------------------- Global variable
    // Projection
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    // Lighting
    glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[0]);
    glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

    
    terrain = GenerateTerrain(program, "textures/fft-terrain.tga");
}

GLfloat a, b = 0.0;


void display(void)
{

    setCarHeight(subaru, terrain);
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform1ui(glGetUniformLocation(program, "enableTransparency"), false);

    // night node activated
    glUniform1ui(glGetUniformLocation(program, "NIGHT_MODE"), NIGHT_MODE);

    // update modelToView matrix
    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    float a = t / 1000;
    glUniform1f(glGetUniformLocation(program, "time"), a);

    mat4 modelToWorld = Mult(T(0, 0, 0), Ry(0));
    mat4 worldToView = lookAtv(cam, VectorAdd(cam, forward), up);

    // -------- Draw skybox
    DrawSkybox(skybox, worldToView);
    // -------- Draw terrain
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassTexture);

    DrawTerrain(terrain, modelToWorld, worldToView);

    if (showNormals)
        DrawNormals(terrain);


    drawCar(subaru);


    glutSwapBuffers();
}

int prevx = 0, prevy = 0;

void mouseUpDown(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        prevx = x;
        prevy = y;
    }
}

void mouseDragged(int x, int y)
{
    vec3 p;
    mat4 m;
    mat3 wv3;

    // This is a simple and IMHO really nice trackball system:
    // You must have two things, the worldToViewMatrix and the modelToWorldMatrix
    // (and the latter is modified).

    // Use the movement direction to create an orthogonal rotation axis
    p.y = x - prevx;
    p.x = -(prevy - y);
    p.z = 0;

    // Transform the view coordinate rotation axis to world coordinates!
    wv3 = mat4tomat3(worldToViewMatrix);
    p = MultMat3Vec3(InvertMat3(wv3), p);

    // Create a rotation around this axis and premultiply it on the model-to-world matrix
    m = ArbRotate(p, sqrt(p.x * p.x + p.y * p.y) / 50.0);
    modelToWorldMatrix = Mult(m, modelToWorldMatrix);

    prevx = x;
    prevy = y;

    glutPostRedisplay();
}

void mouse(int x, int y)
{
    b = x * 1.0;
    a = y * 1.0;
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
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
    glutRepeatingTimer(20);
   

    glutMainLoop();
    exit(0);
}