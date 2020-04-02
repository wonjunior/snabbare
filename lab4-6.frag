#version 150

in vec2 vert_textCoord;
in vec4 vert_positionInWorld;
in vec3 vert_normalInView;
in vec3 vert_normalInWorld;
in vec3 vert_viewDirection;

out vec4 out_Color;

uniform sampler2D texUnit;

uniform bool enableLight;
uniform bool enableTransparency;
uniform bool nightMode;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];

uniform mat4 modelToWorld;
uniform mat4 worldToView;

void main(void)
{
    // -------- Lightning
    float diffuse, specular;
    vec3 shade = vec3(0);

    for (int i = 0 ; i < 4 ; i++)
    {
        // Diffuse lighting
        vec3 lightDir;
        if (isDirectional[i])
            lightDir = -lightSourcesDirPosArr[i];
        else
            lightDir = lightSourcesDirPosArr[i] - vec3(vert_positionInWorld);

        diffuse = dot(normalize(vert_normalInWorld), normalize(lightDir));
        diffuse = max(0.0, diffuse);

        // Specular lighting
        vec3 r = normalize(reflect(-mat3(worldToView) * lightDir, normalize(vert_normalInView)));
        vec3 v = normalize(-vert_viewDirection);
        specular = dot(r, v);
        specular = (specular > 0.0) ? 1.0 * pow(specular, 150.0) : 0.0;

        shade += lightSourcesColorArr[i] * (0.7*diffuse + 0.3*specular);
   }

	// -------- Texture mapping
	vec4 texColor = texture(texUnit, vert_textCoord);

    if (enableLight)
        out_Color = vec4(shade, (enableTransparency ? 0.5 : 1.0)) * texColor * (nightMode ? 0.15 : 1) + 0.1;
    else
        out_Color = texColor * (nightMode ? 0.15 : 1);
}
