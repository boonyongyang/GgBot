# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

GgBot is an interactive 3D robot renderer with animations and weapon modes. The project exists in two forms:

- **`Main.cpp`** — the original single-file Windows-only implementation using Win32 + legacy OpenGL (1.x/2.x fixed-function pipeline, `opengl32.lib`/`glu32.lib`). ~3,400 lines. No longer the active development target.
- **`src/`** — the current multi-file port targeting **macOS Apple Silicon** (and any platform with OpenGL 3.3 Core Profile support), built with CMake, GLFW 3.4, GLAD, and GLM.

## Building (macOS / cross-platform port)

```bash
cmake -B build
cmake --build build
```

The executable and its required assets (`shaders/`, `textures/`) are automatically copied next to the binary by a CMake `POST_BUILD` step. Run from the build directory:

```bash
./build/GgBot
```

There is no test suite and no linter configuration.

**Dependencies** — fetched automatically by CMake via `FetchContent`:
- GLFW 3.4 (windowing + input)
- GLAD v0.1.36 (GL 3.3 Core loader)
- GLM 0.9.9.8 (header-only math)
- `vendor/stb/stb_image.h` (texture loading, committed to repo)

## Architecture of the `src/` Port

The rendering pipeline flows: `main.cpp` → `display()` in `scene.cpp` → `summonGgBot()` → individual `draw*()` functions in `robot.cpp`. Every frame:

1. `display()` clears buffers, calls `scene1()`.
2. `scene1()` calls `computeProjection()` + `setLighting()`, then calls `setFrameContext()` to store the per-frame `(prog, uniforms, view, proj)` as statics in `mesh.cpp`.
3. `summonGgBot()` applies global robot position/yaw, splits the robot into an **upper body** (rotates for attack360) and **lower body** (legs only), then calls all `draw*()` functions.
4. Each `draw*()` function constructs a `DrawCtx` (which calls `getFrameContext()` to recover the stored state) and builds its geometry using `render*()` helpers from `mesh.cpp`.

### Module responsibilities

| File | Responsibility |
|---|---|
| `state.h/cpp` | All mutable global state — animation angles, mode flags, camera, lighting params, texture handles. Every other module reads/writes these globals directly. |
| `shader.h/cpp` | Loads GLSL shaders from disk, caches all `GLint` uniform locations in `struct Uniforms`, and provides `setDrawUniforms()` (sets texture/color per draw call) and `setModelUniforms()` (pushes MVP matrices). |
| `texture.h/cpp` | `loadTexture()` uses `stb_image` with vertical flip. `loadAllTextures()` fills `textureArrOuter[0..4]`, `textureArrInner[0..2]`, and the named handles `texEye`, `texMetal`, `texOcean`, `texSky`. Called once at startup. |
| `mesh.h/cpp` | Procedural geometry generators (`getCubeMesh`, `getSphereMesh`, `getCylinderMesh`, etc.). Each generator is lazily computed and cached in a `std::map` keyed by its parameters — the first call for a given set of parameters uploads to GPU; subsequent calls return the cached `MeshData&`. High-level `render*()` wrappers call `setModelUniforms` + `setDrawUniforms` + `drawMesh` in one call. Also owns the per-frame context statics (`setFrameContext`/`getFrameContext`). |
| `robot.h/cpp` | All 34 `draw*()` functions. Each takes a `glm::mat4 model` parameter passed down from its parent. Animation state mutations (advancing angle accumulators) happen inside the draw functions themselves (e.g. `drawLeftArm` advances `leftArmRup`, `leftArmRup1`, etc.). |
| `scene.h/cpp` | `computeProjection()`, `setLighting()`, `summonGgBot()`, `scene1()`, `display()`. Bridges state → shaders → robot. |
| `animation.h/cpp` | `walkFront()` and `attack360()` — pure state mutations with no GL calls. Triggered by key events in `input.cpp`. |
| `input.h/cpp` | GLFW key/mouse/scroll callbacks. All user input mutates globals from `state.h` here. `walkFront()` / `attack360()` are called directly from the key callback on Q/E. |
| `shaders/ggbot.vert/frag` | Single shader pair for the entire scene. Vertex layout: `location=0` pos (vec3), `location=1` normal (vec3), `location=2` texcoord (vec2) — stride 8 floats. Fragment shader implements Phong with three logical lights matching the original `GL_LIGHT0` (ambient), `GL_LIGHT1` (diffuse), `GL_LIGHT2` (specular). `uUseTexture > 0.5` enables texture sampling multiplied by `uColor`. |

### Key patterns in `robot.cpp`

**DrawCtx** — constructed at the top of every `draw*()` function. Calls `getFrameContext()` to get `prog` and `u`, and resolves the current outer/inner texture from `textureArrOuter[outerTextureNo]` / `textureArrInner[innerTextureNo]`.

**Transform shorthands** — three file-local helpers keep draw code terse:
```cpp
static glm::mat4 T(float x, float y, float z);          // translate
static glm::mat4 R(float deg, float x, float y, float z); // rotate (degrees)
static glm::mat4 Sc(float x, float y, float z);           // scale
```

**Pivot-based animation** — every animated joint follows the same pattern:
```cpp
glm::mat4 joint = parent * T(pivot) * R(angle, axis) * T(-pivot);
// then draw children relative to `joint`
```

**`renderTrapezoid` vs `renderTrapezoidGLU`** — two distinct shapes with similar names:
- `renderTrapezoid(…, top, bot1, bot2, hy, hz)` — manual quad mesh (was `renderTrapezoidWithoutGLU` in `Main.cpp`). Used in body armor, legs, and foot.
- `renderTrapezoidGLU(…, baseL, topL, h)` — 4-sided frustum cone with an internal 45° rotation (was `renderTrapezoid` in `Main.cpp`). Used in arms, neck, and head structure.

**Mesh caching** — `getCylinderMesh(baseR, topR, h, slices)` with `topR == 0` generates a cone (no separate cone generator). `getPrismMesh(l, h, sides)` delegates to `getCylinderMesh`. Mesh keys are `std::tuple` instances, so every unique combination of float parameters produces a separate GPU buffer.

**Texture slots:**
- `textureArrOuter[0..4]`: metal2, camoTexture, armorMetal, armorPattern3, complexTexture
- `textureArrInner[0..2]`: darksteel32, armorPattern, armorPattern2
- Named: `texEye` (eyetest), `texMetal` (metal2), `texOcean` (ocean), `texSky` (sky)

**Weapon / mode flags** — `boolWeapon` (F2 dual cannon), `boolSword` (F3 sword + cannon), `boolHI` (F4 HI pose) are mutually exclusive modes that change which geometry is drawn in `drawLeftArm`/`drawRightArm`.

**Upper vs lower body split** in `summonGgBot()` — the upper body matrix has `rBody` applied (for attack360 spinning), while `drawLeftLeg`/`drawRightLeg` receive the base `model` matrix without that rotation. This means leg animations (`legLeftUpperAngle`, `leftRightUpperAngle`, etc.) are independent of the upper body spin.

## Porting fidelity notes

The `draw*()` functions copy the original `Main.cpp` transform sequences **verbatim** (each legacy `glTranslatef/glRotatef/glScalef` becomes one `T()/R()/Sc()` factor, same order). Two consequences future contributors must respect:

- **`getCylinderMesh` is oriented along +Z**, matching `gluCylinder` (base ring at `z=0`, top at `z=h`, cross-section in the XY plane). This is load-bearing: every cylinder/cone/prism/`renderTrapezoidGLU` call inherits the original's rotations, which assume a +Z tube axis. Do **not** "simplify" it to a +Y axis — that silently rotates every such part 90°. `getDiskMesh` likewise matches `gluDisk` (XY plane, +Z normal).
- **Persistent GL state is resolved explicitly.** The original relied on the last-bound `glBindTexture` and last-set `glColor3f` persisting across draws. The port passes texture/color per call. Where a small internal detail piece (spine-joint sub-spheres, `drawScale`, `drawLegInnerNerve`, `drawCoreDetail1`) is drawn untextured (`texID 0`) and the wireframe foot sphere is drawn solid, that is a deliberate simplification, not a bug.

## Controls Reference

| Key | Action |
|---|---|
| P | Toggle ortho ↔ perspective |
| WASD | Face direction (mutates `faceAngle`) |
| Q | Step walking animation |
| E | Toggle 360° attack spin |
| F | Toggle upper body rotation |
| B/N | Turn head left/right |
| T/Y | Right arm up/down |
| U/I | Left arm up/down |
| Z/X | Fingers fist/open |
| F1/F2/F3/F4 | Normal / dual cannon / sword+cannon / HI pose |
| C | Shoot cannon (weapon modes) |
| V | Sword attack (F3 mode) |
| K/M | Cycle outer/inner texture |
| L/G/H/J | Toggle light / ambient / diffuse / specular |
| SPACE | Full state reset |
| Mouse drag | Orbit camera |
| Scroll | Zoom |

## Original Windows Build (`Main.cpp`)

Open `GgBot.sln` in Visual Studio and build (Debug/Release, Win32/x64). No external dependencies beyond Windows SDK. The `Main.cpp` is kept as a reference but is not actively maintained; the `src/` port is the working codebase.
