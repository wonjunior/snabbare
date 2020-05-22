#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec2 vert_textCoord;
out vec4 vert_positionInWorld;
out vec3 vert_normalInWorld;
out vec3 vert_normalInView;
out vec3 vert_viewDirection;

uniform float time;
uniform mat4 modelToWorld;
uniform mat4 worldToView;
uniform mat4 projectionMatrix;

void main(void)
{
    // -------- Vertex position
    vec4 positionInWorld = modelToWorld * vec4(in_Position, 1.0);
    gl_Position = projectionMatrix * worldToView * positionInWorld;

    mat4 modelToView = modelToWorld * worldToView;

    vert_positionInWorld = positionInWorld;
    vert_normalInWorld = inverse(transpose(mat3(modelToWorld))) * in_Normal;
    vert_normalInView = inverse(transpose(mat3(modelToView))) * in_Normal;
    vert_viewDirection = vec3(modelToView * vec4(in_Position, 1.0));

    // -------- texture
    vert_textCoord = in_TexCoord;
}
