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
    // The original renders with GL_COLOR_MATERIAL OFF, so per-part glColor3f is
    // ignored under lighting: textured surfaces show the texture only (GL_MODULATE),
    // and the material drives lighting. We mirror that — uColor tints only
    // untextured detail pieces (which relied on a leftover binding in the original).
    vec4 baseColor = (uUseTexture > 0.5) ? texture(uTexture, vTexCoord)
                                         : vec4(uColor, 1.0);

    if (!uLightOn) {
        FragColor = baseColor;
        return;
    }

    // The original sets NO normals anywhere, so every vertex keeps the default
    // (0,0,1): lighting is flat (no per-face shading). With LIGHT0's white ambient
    // and a white material ambient the ambient term saturates to white, making the
    // model effectively fullbright — the "flat and bright" legacy look.
    vec3 result = vec3(0.0);
    if (uAmbientOn)  result += uAmbientColor * uMaterialAmbient;        // ~white
    if (uDiffuseOn)  result += uMaterialDiffuse * uDiffuseColor * 0.5;  // faint flat blue
    if (uSpecularOn) result += uMaterialDiffuse * uDiffuseColor * 0.3;
    result = clamp(result, 0.0, 1.0);

    FragColor = vec4(result, 1.0) * baseColor;
}
