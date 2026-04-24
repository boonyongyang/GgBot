# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

GgBot is a Windows OpenGL application that renders and animates an interactive 3D robot. It uses the Win32 API with legacy OpenGL (1.x/2.x fixed-function pipeline via `opengl32.lib` and `glu32.lib`). The entire program is a single file: `Main.cpp` (~3400 lines).

## Building

This is a Visual Studio project targeting Windows. Build using:

- **Visual Studio (recommended):** Open `GgBot.sln` and build via the IDE (Debug/Release, Win32/x64).
- **MSBuild CLI:**
  ```
  msbuild GgBot.sln /p:Configuration=Debug /p:Platform=x64
  ```

The project has no external dependencies beyond the Windows SDK and OpenGL system libraries (`opengl32.lib`, `glu32.lib`). There is no package manager, no test suite, and no linter configuration.

Textures must be present in a `textures/` subdirectory relative to the working directory when the executable runs (the project loads them from relative paths like `"textures/sky.bmp"`).

## Architecture

All code lives in `Main.cpp` and is organized into these logical sections (marked with comment banners):

1. **Global state** (top of file) — All animation state, camera, lighting, and texture handles are plain global variables. There are no classes or structs. Animation is driven by boolean flags (e.g. `leftArmUpBool`, `moveLeftLeg`) and float angle accumulators (e.g. `leftArmRup`, `legLeftUpperAngle`).

2. **`WindowProcedure`** — Win32 message handler. Handles `WM_KEYDOWN`, mouse events (`WM_MOUSEMOVE`, `WM_LBUTTONDOWN`, `WM_MOUSEWHEEL`), and `WM_DESTROY`. All user input mutates global state here.

3. **`projection()`** — Switches between orthographic (`glOrtho`) and perspective (`gluPerspective` + `glFrustum`) based on `isOrtho`. Camera panning and rotation are applied directly to the projection matrix.

4. **`lighting()`** — Configures three OpenGL lights (GL_LIGHT0 ambient, GL_LIGHT1 diffuse, GL_LIGHT2 specular) using global color and position arrays.

5. **`loadTexture()`** — Loads `.bmp` textures from disk using `LoadImage` / `GetObject` and uploads them via `glTexImage2D`. Called once at startup inside `WinMain`.

6. **Primitive renderers** — `renderSphere`, `renderCylinder`, `renderCone`, `renderPrism`, `renderCuboid`, `renderTrapezoid`, `renderCubeWithoutGLU`, `renderTrapezoidWithoutGLU` etc. These are the building blocks for all robot parts. GLU-based helpers use `gluNewQuadric`; the `WithoutGLU` variants use `glBegin/GL_QUADS`.

7. **Robot draw functions** — Each body part has its own `draw*` function (e.g. `drawHead`, `drawBody`, `drawSpine`, `drawLeftArm`, `drawRightArm`, `drawLeftLeg`, `drawRightLeg`). They use nested `glPushMatrix/glPopMatrix` for hierarchical transforms. Animations work by rotating/translating around a pivot point: translate to pivot → apply rotation → translate back.

8. **`walkFront()` / `attack360()`** — Called from `WinMain`'s render loop each frame. They advance angle accumulators based on the direction booleans and move `tZ` to translate the robot forward.

9. **`WinMain`** — Entry point. Creates the Win32 window, sets up the OpenGL pixel format and rendering context (`PIXELFORMATDESCRIPTOR`, `wglCreateContext`), loads textures into `textureArrOuter[7]` and `textureArrInner[3]`, then runs the message/render loop calling `display()` each frame.

10. **`display()`** — Main render function called every frame. Calls `projection()`, `lighting()`, clears buffers, applies global transforms (mouse rotation, zoom, WASD facing angle), then calls all robot draw functions.

## Key Conventions

- **Pivot-based animation:** To rotate a limb around its joint, the pattern is always: `glTranslatef(pivot)` → `glRotatef(angle, ...)` → `glTranslatef(-pivot)` → draw child.
- **Texture switching:** `textureArrOuter` (5 slots used) and `textureArrInner` (3 slots) are cycled by `outerTextureNo`/`innerTextureNo`. `glBindTexture` is called per-part before drawing.
- **Toggle pattern:** Repeating the same key when a boolean action is active turns it off (via the `temp` char variable tracking the last active key).
- **No depth test in perspective:** `glEnable(GL_DEPTH_TEST)` is commented out, which causes z-fighting in perspective mode — this is a known issue in the existing code.
- **Weapon modes:** `boolWeapon` (dual cannon, F2), `boolSword` (sword + cannon, F3), normal (F1) affect which arm geometry is drawn and which attack animations are available.
