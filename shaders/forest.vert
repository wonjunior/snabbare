#version 150

in  vec3 inPosition;
//in  vec3 inColor;
//in  vec3 inNormal;
in vec2 inTexCoord;

//out vec3 exColor; // Gouraud
//out vec3 exNormal; // Phong
out vec2 texCoord;

uniform mat4 modelviewMatrix;
uniform mat4 projectionMatrix;
uniform bool ACTIVE_HUD;

void main(void)
{
    if (ACTIVE_HUD)
        gl_Position = modelviewMatrix * vec4(inPosition, 1.0);
    else
        gl_Position = projectionMatrix * modelviewMatrix * vec4(inPosition, 1.0);

	texCoord = inTexCoord;
}
