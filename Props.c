#include "Props.h"

Props* LoadProps(GLuint shader) {
    // cadnav.com

    Props* props = malloc(sizeof(Props));
    props->shader = shader;

    // Checkpoint
    props->checkpoint = LoadModelPlus("models/checkpoint.obj");
    LoadTGATextureSimple("textures/checkpoint.tga", &(props->checkpointTexture));

    // Rabit
    props->rabbit = LoadModelPlus("models/rabbit.obj");
    LoadTGATextureSimple("textures/rabbit.tga", &(props->rabbitTexture));

    return props;
}

void drawProps(Props* props) {

    // draw checkpoint
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, props->checkpointTexture);
    
    mat4 startingPosition = T(89.0, 10.0, 602.0);
    glUniformMatrix4fv(glGetUniformLocation(props->shader, "modelToWorld"), 1, GL_TRUE, startingPosition.m);
    DrawModel(props->checkpoint, props->shader, "in_Position", "in_Normal", "in_TexCoord");

    // draw rabbits
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, props->rabbitTexture);

    mat4 rabbitPosition = Mult(Mult(T(89.0 + 14.0, 10.2, 602.0), Ry(3.14 * 3.0 / 2.0)), S(0.5, 0.5, 0.5));
    glUniformMatrix4fv(glGetUniformLocation(props->shader, "modelToWorld"), 1, GL_TRUE, rabbitPosition.m);
    DrawModel(props->rabbit, props->shader, "in_Position", "in_Normal", "in_TexCoord");

    rabbitPosition = Mult(Mult(T(89.0 - 14.0, 10.1, 602.0), Ry(3.14 / 2.0)), S(0.5, 0.5, 0.5));
    glUniformMatrix4fv(glGetUniformLocation(props->shader, "modelToWorld"), 1, GL_TRUE, rabbitPosition.m);
    DrawModel(props->rabbit, props->shader, "in_Position", "in_Normal", "in_TexCoord");
}