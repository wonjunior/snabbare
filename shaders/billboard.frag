#version 150

in vec2 texCoord;

out vec4 outColor;

uniform sampler2D texUnit;

void main(void)
{
	vec4 t = texture(texUnit, texCoord);
   if (t.a < 0.01) discard;
   else
       outColor = t;
}
