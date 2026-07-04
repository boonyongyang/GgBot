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

// render style: true = legacy fixed-function look, false = modern Phong (O key)
uniform bool  uLegacyLook;

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

// Legacy fixed-function look: matches the original Windows build. GL_COLOR_MATERIAL
// is off, so per-part glColor3f is ignored — textured surfaces show the texture only
// (GL_MODULATE) and uColor tints just the untextured detail pieces. No normals were
// ever set, so shading is flat; LIGHT0's white ambient saturates it to fullbright.
vec4 legacy() {
    vec4 baseColor = (uUseTexture > 0.5) ? texture(uTexture, vTexCoord)
                                         : vec4(uColor, 1.0);
    if (!uLightOn) return baseColor;

    vec3 result = vec3(0.0);
    if (uAmbientOn)  result += uAmbientColor * uMaterialAmbient;        // ~white
    if (uDiffuseOn)  result += uMaterialDiffuse * uDiffuseColor * 0.5;  // faint flat blue
    if (uSpecularOn) result += uMaterialDiffuse * uDiffuseColor * 0.3;
    return vec4(clamp(result, 0.0, 1.0), 1.0) * baseColor;
}

// Modern look: per-fragment Phong using real geometry normals, with the per-part
// colors kept as a tint on the texture.
vec4 modern() {
    vec4 baseColor = (uUseTexture > 0.5) ? texture(uTexture, vTexCoord) * vec4(uColor, 1.0)
                                         : vec4(uColor, 1.0);
    if (!uLightOn) return baseColor;

    vec3 norm   = normalize(vNormal);
    vec3 result = vec3(0.0);
    if (uAmbientOn)  result += uAmbientColor * uMaterialAmbient;
    if (uDiffuseOn)  result += phong(uDiffusePos, uDiffuseColor, norm, vFragPos, 32.0) * uMaterialDiffuse;
    if (uSpecularOn) {
        float shine = (uMaterialFv == 2.0) ? 64.0 : 32.0;
        result += phong(uDiffusePos, uDiffuseColor, norm, vFragPos, shine);
    }
    result = max(result, vec3(0.05));
    return vec4(result, 1.0) * baseColor;
}

void main() {
    FragColor = uLegacyLook ? legacy() : modern();
}
