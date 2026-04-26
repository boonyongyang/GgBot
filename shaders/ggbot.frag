#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTexCoord;

uniform vec3  uColor;
uniform float uUseTexture;   // 0=color only, 1=texture
uniform sampler2D uTexture;

// lighting toggles
uniform bool  uLightOn;
uniform bool  uAmbientOn;
uniform bool  uDiffuseOn;
uniform bool  uSpecularOn;

// light parameters (matching original 3-light setup)
uniform vec3  uAmbientColor;
uniform vec3  uAmbientPos;
uniform vec3  uDiffuseColor;
uniform vec3  uDiffusePos;

// material
uniform vec3  uMaterialAmbient;
uniform vec3  uMaterialDiffuse;
uniform float uMaterialFv;   // 1=white scheme, 2=blue scheme

out vec4 FragColor;

vec3 phong(vec3 lightPos, vec3 lightColor, vec3 norm, vec3 fragPos, float shininess) {
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = diff * lightColor;

    vec3  viewDir   = normalize(-fragPos);
    vec3  reflectDir = reflect(-lightDir, norm);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3  specular  = spec * lightColor;

    return diffuse + specular;
}

void main() {
    vec4 baseColor;
    if (uUseTexture > 0.5) {
        baseColor = texture(uTexture, vTexCoord) * vec4(uColor, 1.0);
    } else {
        baseColor = vec4(uColor, 1.0);
    }

    if (!uLightOn) {
        FragColor = baseColor;
        return;
    }

    vec3 norm    = normalize(vNormal);
    vec3 result  = vec3(0.0);

    // GL_LIGHT0: ambient
    if (uAmbientOn) {
        result += uAmbientColor * uMaterialAmbient;
    }

    // GL_LIGHT1: diffuse
    if (uDiffuseOn) {
        result += phong(uDiffusePos, uDiffuseColor, norm, vFragPos, 32.0) * uMaterialDiffuse;
    }

    // GL_LIGHT2: specular (reuses dif color/pos like original)
    if (uSpecularOn) {
        float shine = (uMaterialFv == 2.0) ? 64.0 : 32.0;
        result += phong(uDiffusePos, uDiffuseColor, norm, vFragPos, shine);
    }

    // ensure minimum visibility when lights give zero contribution
    result = max(result, vec3(0.05));

    FragColor = vec4(result, 1.0) * baseColor;
}
