#include "mesh.h"
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <map>
#include <tuple>
#include <cmath>
#include <array>

// ── GPU upload / draw ────────────────────────────────────────────────────────

void uploadMesh(MeshData& m) {
    glGenVertexArrays(1, &m.vao);
    glGenBuffers(1, &m.vbo);
    glGenBuffers(1, &m.ebo);

    glBindVertexArray(m.vao);

    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m.vertices.size() * sizeof(float),
                 m.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 m.indices.size() * sizeof(unsigned int),
                 m.indices.data(), GL_STATIC_DRAW);

    // layout(location=0) pos, (1) normal, (2) texcoord — stride = 8 floats
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);
}

void drawMesh(const MeshData& m) {
    glBindVertexArray(m.vao);
    glDrawElements(GL_TRIANGLES,
                   static_cast<GLsizei>(m.indices.size()),
                   GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

// ── Vertex helper ─────────────────────────────────────────────────────────────
static void pushVertex(MeshData& m,
                       glm::vec3 pos, glm::vec3 norm, glm::vec2 uv) {
    m.vertices.insert(m.vertices.end(),
        { pos.x, pos.y, pos.z, norm.x, norm.y, norm.z, uv.x, uv.y });
}

static void pushQuad(MeshData& m,
                     unsigned int a, unsigned int b,
                     unsigned int c, unsigned int d) {
    // quad (a,b,c,d) → two CCW triangles
    m.indices.insert(m.indices.end(), { a, b, c, a, c, d });
}

// ── Box (replaces renderCubeWithoutGLU) ──────────────────────────────────────
static MeshData buildCubeMesh(float x, float y, float z) {
    MeshData m;
    // 6 faces, each a quad → 4 vertices × 8 floats, face normal = constant
    struct Face { glm::vec3 n; glm::vec3 v[4]; };
    Face faces[6] = {
        // bottom  (-Y)
        {{ 0,-1, 0},{{ -x,-y,-z},{-x,-y, z},{ x,-y, z},{ x,-y,-z}}},
        // top     (+Y)
        {{ 0, 1, 0},{{ -x, y,-z},{ x, y,-z},{ x, y, z},{-x, y, z}}},
        // front   (+Z)
        {{ 0, 0, 1},{{ -x,-y, z},{ x,-y, z},{ x, y, z},{-x, y, z}}},
        // back    (-Z)
        {{ 0, 0,-1},{{  x,-y,-z},{-x,-y,-z},{-x, y,-z},{ x, y,-z}}},
        // left    (-X)
        {{-1, 0, 0},{{ -x,-y,-z},{-x, y,-z},{-x, y, z},{-x,-y, z}}},
        // right   (+X)
        {{ 1, 0, 0},{{  x,-y, z},{ x, y, z},{ x, y,-z},{ x,-y,-z}}},
    };
    glm::vec2 uvs[4] = {{0,1},{1,1},{1,0},{0,0}};
    for (auto& f : faces) {
        unsigned int base = m.vertices.size() / 8;
        for (int i = 0; i < 4; ++i)
            pushVertex(m, f.v[i], f.n, uvs[i]);
        pushQuad(m, base, base+1, base+2, base+3);
    }
    uploadMesh(m);
    return m;
}

MeshData& getCubeMesh(float x, float y, float z) {
    using Key = std::tuple<float,float,float>;
    static std::map<Key, MeshData> cache;
    Key key{x,y,z};
    auto it = cache.find(key);
    if (it == cache.end())
        it = cache.emplace(key, buildCubeMesh(x,y,z)).first;
    return it->second;
}

// ── Trapezoid (replaces renderTrapezoidWithoutGLU) ───────────────────────────
// Parameters match original: top width, bot1/bot2 (x extents), y height, z depth
static MeshData buildTrapezoidMesh(float top, float bot1, float bot2,
                                    float hy, float hz) {
    MeshData m;
    // 6 faces, using the vertex positions from the original glBegin(GL_QUADS)
    // translated so that the trapezoid is centred (glTranslatef(-top/2,-hy/2,-hz/2))
    float ox = -top * 0.5f, oy = -hy * 0.5f, oz = -hz * 0.5f;

    glm::vec3 v[8] = {
        {ox+bot1, oy,      oz     }, // 0
        {ox+bot1, oy,      oz+hz  }, // 1
        {ox+bot2, oy,      oz+hz  }, // 2
        {ox+bot2, oy,      oz     }, // 3
        {ox,      oy+hy,   oz     }, // 4
        {ox+top,  oy+hy,   oz     }, // 5
        {ox,      oy+hy,   oz+hz  }, // 6
        {ox+top,  oy+hy,   oz+hz  }, // 7
    };

    // face: indices into v[], normal computed per face
    struct Face { int idx[4]; };
    Face faces[6] = {
        {{0,1,2,3}},   // bottom
        {{0,3,5,4}},   // front
        {{0,4,6,1}},   // left
        {{4,5,7,6}},   // top
        {{5,3,2,7}},   // right
        {{2,1,6,7}},   // back
    };
    glm::vec2 uvs[4] = {{0,1},{1,1},{1,0},{0,0}};
    for (auto& f : faces) {
        glm::vec3 a = v[f.idx[0]], b = v[f.idx[1]],
                  c = v[f.idx[2]], d = v[f.idx[3]];
        glm::vec3 n = glm::normalize(glm::cross(b-a, d-a));
        unsigned int base = m.vertices.size() / 8;
        pushVertex(m, a, n, uvs[0]);
        pushVertex(m, b, n, uvs[1]);
        pushVertex(m, c, n, uvs[2]);
        pushVertex(m, d, n, uvs[3]);
        pushQuad(m, base, base+1, base+2, base+3);
    }
    uploadMesh(m);
    return m;
}

MeshData& getTrapezoidMesh(float top, float bot1, float bot2, float y, float z) {
    using Key = std::tuple<float,float,float,float,float>;
    static std::map<Key, MeshData> cache;
    Key key{top,bot1,bot2,y,z};
    auto it = cache.find(key);
    if (it == cache.end())
        it = cache.emplace(key, buildTrapezoidMesh(top,bot1,bot2,y,z)).first;
    return it->second;
}

// ── UV Sphere (replaces gluSphere) ───────────────────────────────────────────
static MeshData buildSphereMesh(float r, int slices, int stacks) {
    MeshData m;
    const float pi = glm::pi<float>();
    for (int i = 0; i <= stacks; ++i) {
        float phi   = pi * i / stacks;
        float y     = r * std::cos(phi);
        float sinPhi = std::sin(phi);
        for (int j = 0; j <= slices; ++j) {
            float theta = 2.f * pi * j / slices;
            float x = r * sinPhi * std::cos(theta);
            float z = r * sinPhi * std::sin(theta);
            glm::vec3 pos{x, y, z};
            glm::vec3 norm = glm::normalize(pos);
            glm::vec2 uv{(float)j/slices, 1.f - (float)i/stacks};
            pushVertex(m, pos, norm, uv);
        }
    }
    for (int i = 0; i < stacks; ++i)
        for (int j = 0; j < slices; ++j) {
            unsigned int a = i*(slices+1)+j,   b = a+1,
                         c = (i+1)*(slices+1)+j, d = c+1;
            pushQuad(m, a, b, d, c);
        }
    uploadMesh(m);
    return m;
}

MeshData& getSphereMesh(float r, int slices, int stacks) {
    using Key = std::tuple<float,int,int>;
    static std::map<Key, MeshData> cache;
    Key key{r,slices,stacks};
    auto it = cache.find(key);
    if (it == cache.end())
        it = cache.emplace(key, buildSphereMesh(r,slices,stacks)).first;
    return it->second;
}

// ── Cylinder / cone (replaces gluCylinder + gluDisk caps) ───────────────────
static MeshData buildCylinderMesh(float baseR, float topR, float h, int slices) {
    MeshData m;
    const float pi = glm::pi<float>();
    // Oriented along +Z to match gluCylinder: base ring at z=0, top ring at z=h,
    // circular cross-section in the XY plane. Every draw* call copies the
    // original's rotations verbatim, so the base axis MUST be +Z (not +Y).
    // side
    for (int i = 0; i <= slices; ++i) {
        float t = (float)i / slices;
        float a = 2.f * pi * t;
        float cx = std::cos(a), cy = std::sin(a);
        glm::vec3 n{cx, cy, 0};
        pushVertex(m, {baseR*cx, baseR*cy, 0}, n, {t, 0});
        pushVertex(m, {topR*cx,  topR*cy,  h}, n, {t, 1});
    }
    for (int i = 0; i < slices; ++i) {
        unsigned int b = i*2, t2 = b+1;
        pushQuad(m, b, b+2, t2+2, t2);
    }
    // bottom cap (fan) at z=0, facing -Z
    if (baseR > 0.f) {
        unsigned int center = m.vertices.size() / 8;
        pushVertex(m, {0,0,0}, {0,0,-1}, {0.5f,0.5f});
        for (int i = 0; i <= slices; ++i) {
            float a = 2.f * pi * i / slices;
            float cx = std::cos(a), cy = std::sin(a);
            pushVertex(m, {baseR*cx,baseR*cy,0}, {0,0,-1},
                       {cx*0.5f+0.5f, cy*0.5f+0.5f});
        }
        for (int i = 0; i < slices; ++i)
            m.indices.insert(m.indices.end(),
                             {center, center+i+2, center+i+1});
    }
    // top cap at z=h, facing +Z
    if (topR > 0.f) {
        unsigned int center = m.vertices.size() / 8;
        pushVertex(m, {0,0,h}, {0,0,1}, {0.5f,0.5f});
        for (int i = 0; i <= slices; ++i) {
            float a = 2.f * pi * i / slices;
            float cx = std::cos(a), cy = std::sin(a);
            pushVertex(m, {topR*cx,topR*cy,h}, {0,0,1},
                       {cx*0.5f+0.5f, cy*0.5f+0.5f});
        }
        for (int i = 0; i < slices; ++i)
            m.indices.insert(m.indices.end(),
                             {center, center+i+1, center+i+2});
    }
    uploadMesh(m);
    return m;
}

MeshData& getCylinderMesh(float baseR, float topR, float h, int slices) {
    using Key = std::tuple<float,float,float,int>;
    static std::map<Key, MeshData> cache;
    Key key{baseR,topR,h,slices};
    auto it = cache.find(key);
    if (it == cache.end())
        it = cache.emplace(key, buildCylinderMesh(baseR,topR,h,slices)).first;
    return it->second;
}

// ── Disk / annulus (replaces gluDisk) ────────────────────────────────────────
static MeshData buildDiskMesh(float inR, float outR, int slices, int /*loops*/) {
    MeshData m;
    const float pi = glm::pi<float>();
    for (int i = 0; i <= slices; ++i) {
        float t = (float)i / slices;
        float a = 2.f * pi * t;
        float cx = std::cos(a), cz = std::sin(a);
        glm::vec3 n{0,0,1};
        pushVertex(m, {inR *cx, inR *cz, 0}, n, {cx*0.5f+0.5f, cz*0.5f+0.5f});
        pushVertex(m, {outR*cx, outR*cz, 0}, n, {cx*0.5f+0.5f, cz*0.5f+0.5f});
    }
    for (int i = 0; i < slices; ++i) {
        unsigned int b = i*2;
        pushQuad(m, b, b+2, b+3, b+1);
    }
    uploadMesh(m);
    return m;
}

MeshData& getDiskMesh(float inR, float outR, int slices, int loops) {
    using Key = std::tuple<float,float,int,int>;
    static std::map<Key, MeshData> cache;
    Key key{inR,outR,slices,loops};
    auto it = cache.find(key);
    if (it == cache.end())
        it = cache.emplace(key, buildDiskMesh(inR,outR,slices,loops)).first;
    return it->second;
}

// ── Prism / n-gon prism (replaces gluCylinder with n slices) ─────────────────
MeshData& getPrismMesh(float l, float h, int sides) {
    // A prism is just a cylinder with `sides` slices
    return getCylinderMesh(l, l, h, sides);
}

// ── Wireframe sphere (foot decoration) ───────────────────────────────────────
static MeshData buildWireSphereMesh(float r, int slices, int stacks) {
    // Approximated as a solid sphere rendered without texture — same visual mass
    return buildSphereMesh(r, slices, stacks);
}

MeshData& getWireSphereMesh(float r, int slices, int stacks) {
    return getSphereMesh(r, slices, stacks);
}

// ── Sword tip (4 hardcoded triangles from Main.cpp lines 2131-2148) ──────────
static MeshData buildSwordTipMesh() {
    MeshData m;
    // Original vertices (absolute positions in arm-local space)
    // We store them verbatim; the model matrix will place them correctly.
    glm::vec3 A{-1.75f,-1.9f,-5.6f};
    glm::vec3 B{-1.75f,-2.2f,-6.0f};
    glm::vec3 C{-1.6f, -2.2f,-5.6f};
    glm::vec3 D{-1.9f, -2.2f,-5.6f};
    glm::vec3 E{-1.75f,-2.5f,-5.6f};

    auto addTri = [&](glm::vec3 p0, glm::vec3 p1, glm::vec3 p2){
        glm::vec3 n = glm::normalize(glm::cross(p1-p0, p2-p0));
        unsigned int base = m.vertices.size() / 8;
        pushVertex(m, p0, n, {0,0});
        pushVertex(m, p1, n, {1,0});
        pushVertex(m, p2, n, {0,1});
        m.indices.insert(m.indices.end(), {base, base+1, base+2});
    };

    addTri(A, B, C);
    addTri(A, B, D);
    addTri(E, B, C);
    addTri(E, B, D);
    uploadMesh(m);
    return m;
}

MeshData& getSwordTipMesh() {
    static MeshData m = buildSwordTipMesh();
    return m;
}

// ── High-level render helpers ─────────────────────────────────────────────────

static void draw(GLuint prog, const Uniforms& u,
                 const glm::mat4& model, const glm::mat4& view,
                 const glm::mat4& proj,
                 GLuint texID, glm::vec3 color, MeshData& mesh) {
    setModelUniforms(prog, u, model, view, proj);
    setDrawUniforms(prog, u, texID, color);
    drawMesh(mesh);
}

// The render* helpers below accept model/view/proj from a thread-local set in
// scene.cpp/robot.cpp; for simplicity we pass view and proj via globals set
// once per frame in scene.cpp.

// ── Per-frame view/proj provided by scene.cpp ─────────────────────────────────
static glm::mat4 s_view{1.f}, s_proj{1.f};
static GLuint    s_prog = 0;
static Uniforms  s_u{};

void setFrameContext(GLuint prog, const Uniforms& u,
                     const glm::mat4& view, const glm::mat4& proj) {
    s_prog = prog; s_u = u; s_view = view; s_proj = proj;
}

// Expose frame context for robot.cpp
void getFrameContext(GLuint& prog, Uniforms& u,
                     glm::mat4& view, glm::mat4& proj) {
    prog = s_prog; u = s_u; view = s_view; proj = s_proj;
}

void renderCube(GLuint prog, const Uniforms& u, glm::mat4 model,
                GLuint texID, glm::vec3 color, float x, float y, float z) {
    draw(prog, u, model, s_view, s_proj, texID, color, getCubeMesh(x,y,z));
}

void renderTrapezoid(GLuint prog, const Uniforms& u, glm::mat4 model,
                     GLuint texID, glm::vec3 color,
                     float top, float bot1, float bot2, float hy, float hz) {
    draw(prog, u, model, s_view, s_proj, texID, color,
         getTrapezoidMesh(top,bot1,bot2,hy,hz));
}

void renderSphere(GLuint prog, const Uniforms& u, glm::mat4 model,
                  GLuint texID, glm::vec3 color, float r) {
    draw(prog, u, model, s_view, s_proj, texID, color, getSphereMesh(r));
}

void renderCylinder(GLuint prog, const Uniforms& u, glm::mat4 model,
                    GLuint texID, glm::vec3 color,
                    float baseR, float topR, float h) {
    draw(prog, u, model, s_view, s_proj, texID, color,
         getCylinderMesh(baseR,topR,h));
}

void renderDisk(GLuint prog, const Uniforms& u, glm::mat4 model,
                GLuint texID, glm::vec3 color,
                float inR, float outR, int slices, int loops) {
    draw(prog, u, model, s_view, s_proj, texID, color,
         getDiskMesh(inR,outR,slices,loops));
}

void renderPrism(GLuint prog, const Uniforms& u, glm::mat4 model,
                 GLuint texID, glm::vec3 color, float l, float h, int sides) {
    draw(prog, u, model, s_view, s_proj, texID, color, getPrismMesh(l,h,sides));
}

void renderCone(GLuint prog, const Uniforms& u, glm::mat4 model,
                GLuint texID, glm::vec3 color, float topR, float h) {
    draw(prog, u, model, s_view, s_proj, texID, color,
         getCylinderMesh(0.f, topR, h));
}

// GLU-based trapezoid (original renderTrapezoid): 4-sided frustum rotated 45°
void renderTrapezoidGLU(GLuint prog, const Uniforms& u, glm::mat4 model,
                        GLuint texID, glm::vec3 color,
                        float baseL, float topL, float h) {
    glm::mat4 m2 = model *
        glm::rotate(glm::mat4(1.f), glm::radians(45.f), glm::vec3(0,0,1));
    draw(prog, u, m2, s_view, s_proj, texID, color,
         getCylinderMesh(baseL, topL, h, 4));
}

// Unused, mirroring Main.cpp's renderCuboid (commented there as "no use dao?").
// Kept for parity with the original; safe to drop if the reference is retired.
void renderCuboidGLU(GLuint prog, const Uniforms& u, glm::mat4 model,
                     GLuint texID, glm::vec3 color, float l, float h) {
    glm::mat4 m2 = model *
        glm::rotate(glm::mat4(1.f), glm::radians(45.f), glm::vec3(0,0,1));
    draw(prog, u, m2, s_view, s_proj, texID, color,
         getCylinderMesh(l, l, h, 4));
}

void renderWireSphere(GLuint prog, const Uniforms& u, glm::mat4 model,
                      GLuint texID, glm::vec3 color, float r) {
    // Original drew the foot decoration as a GL_LINE_STRIP wireframe; reproduce
    // that by rasterizing the sphere in line mode, then restore fill.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    draw(prog, u, model, s_view, s_proj, texID, color, getWireSphereMesh(r));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void renderSwordTip(GLuint prog, const Uniforms& u, glm::mat4 model,
                    GLuint texID, glm::vec3 color) {
    draw(prog, u, model, s_view, s_proj, texID, color, getSwordTipMesh());
}
